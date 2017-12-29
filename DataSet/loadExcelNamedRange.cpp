#include "DataSet.h"

#include <time.h>

   long DataSet::loadExcelNamedRange(HWND hwndDestinationList,char *pszWorkbookName,char *pszRangeName) {

   if ( ! hwndExcelSettings )
      return 0;

   bool wasOpen = false;

   VARIANT vtIndex;
   vtIndex.vt = VT_I4;
   vtIndex.lVal = 0L;

   Excel::_Workbook *pIWorkbook = NULL;
   Excel::Sheets *pIWorksheets = NULL;
   Excel::_Worksheet *pIWorksheet = NULL;
   Excel::NamesPtr pRanges = NULL;
   Excel::NamePtr pRange = NULL;

   //HWND hwndListView = GetDlgItem(hwndExcelSettings,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS);
     
   if ( hwndDestinationList ) { 

      SendMessage(hwndDestinationList,LVM_DELETEALLITEMS,0L,0L);

      LVCOLUMN lvColumn = {0};

      lvColumn.mask = LVCF_WIDTH;

      while ( SendMessage(hwndDestinationList,LVM_GETCOLUMN,0,(LPARAM)&lvColumn) )
         SendMessage(hwndDestinationList,LVM_DELETECOLUMN,0L,0L);

   }

   try {

   pIWorkbook = openExcelWorkbook(pszWorkbookName,&wasOpen);

   HRESULT hr = pIWorkbook -> get_Names(&pRanges);

   long count = pRanges -> GetCount();

   for ( long k = 0; k < count; k++ ) {

      vtIndex.lVal = k + 1;

      pRange = pRanges -> Item(vtIndex);

      _bstr_t theName = pRange -> GetName();

      char szName[128];

      WideCharToMultiByte(CP_ACP,0,theName,-1,szName,128,0,0);

      if ( 0 == strcmp(pszRangeName,szName) ) 
         break;

      pRange -> Release();

      pRange = NULL;

   }

   if ( ! pRange ) {

      if ( pRanges )
         pRanges -> Release();

      if ( pIWorkbook && ! wasOpen ) 
         pIWorkbook -> Close(VARIANT_TRUE);

      if ( pIWorkbook )
         pIWorkbook -> Release();

      if ( pIApplication )
         pIApplication -> Release();

      return 0;

   }

   _variant_t rangeRefersTo = pRange -> GetRefersToR1C1();

   WCHAR szwInfo[128];

   wcscpy(szwInfo,rangeRefersTo.bstrVal);

   WCHAR *p = wcschr(szwInfo,L'!');

   WCHAR *pszSheetName = &szwInfo[1];

   *p = '\0';

   p++;

   WCHAR szwRange[64];

   wcscpy(szwRange,p);

   while ( ! iswdigit(*p) ) p++;
   
   WCHAR *pszNumber = p;

   while ( iswdigit(*p) ) p++;

   *p = '\0';

   long row1 = _wtol(pszNumber);

   p++;

   while ( ! iswdigit(*p) ) p++;

   pszNumber = p;

   while ( iswdigit(*p) ) p++;

   *p = '\0';

   long column1 = _wtol(pszNumber);

   p++;

   while ( ! iswdigit(*p) ) p++;

   pszNumber = p;

   while ( iswdigit(*p) ) p++;

   *p = '\0';

   long row2 = _wtol(pszNumber);

   p++;

   while ( ! iswdigit(*p) ) p++;

   pszNumber = p;

   while ( iswdigit(*p) ) p++;

   *p = '\0';

   long column2 = _wtol(pszNumber);

   long columnCount = column2 - column1 + 1;
   long rowCount = row2 - row1 + 1;

   hr = pIWorkbook -> get_Worksheets(&pIWorksheets);

   count = 0L;

   pIWorksheets -> get_Count(&count);

   for ( long k = 0; k < count; k++ ) {

      vtIndex.lVal = k + 1;

      IDispatch *pIDispatch = NULL;

      hr = pIWorksheets -> get_Item(vtIndex,&pIDispatch);

      pIDispatch -> QueryInterface(__uuidof(Excel::_Worksheet),reinterpret_cast<void **>(&pIWorksheet));

      bstr_t theName = pIWorksheet -> GetName();

      pIDispatch -> Release();

      if ( 0 == wcscmp(theName,pszSheetName) ) 
         break;

      pIWorksheet -> Release();

      pIWorksheet = NULL;

   }

   pIWorksheets -> Release();   

   pIWorksheets = NULL;

   if ( NULL == pIWorksheet ) {

      pRange -> Release();

      if ( pRanges )
         pRanges -> Release();

      if ( pIWorkbook && ! wasOpen ) 
         pIWorkbook -> Close(VARIANT_TRUE);

      if ( pIWorkbook )
         pIWorkbook -> Release();

      if ( pIApplication )
         pIApplication -> Release();

      return 0;

   }

   VARIANT vtCell1,vtCell2;

   vtCell1.vt = VT_BSTR;
   vtCell1.bstrVal = NULL;

   vtCell2.vt = VT_BSTR;
   vtCell2.bstrVal = NULL;

   if ( hwndDestinationList ) {

      RECT rc;

      GetWindowRect(hwndDestinationList,&rc);

      long cxColumns = rc.right - rc.left;

      long cxOneColumn = max(32,(cxColumns - 4)/ columnCount);

      char szHeader[32];

      LVCOLUMN lvColumn;
      memset(&lvColumn,0,sizeof(LVCOLUMN));

      lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
      lvColumn.fmt = LVCFMT_LEFT;
      lvColumn.cx = cxOneColumn;
      lvColumn.pszText = szHeader;

      for ( long k = 0; k < columnCount; k++ ) {

         swprintf(szwRange,L"R%ld:C%ld",row1 - 1,column1 + k);

         if ( vtCell1.bstrVal )
            SysFreeString(vtCell1.bstrVal);

         vtCell1.bstrVal = SysAllocString(szwRange);

         swprintf(szwRange,L"R%ld:C%ld",row1 - 1,column1 + k);

         if ( vtCell2.bstrVal )
            SysFreeString(vtCell2.bstrVal);

         vtCell2.bstrVal = SysAllocString(szwRange);

         try {

         _variant_t vtSafeArray = pIWorksheet -> GetCells() -> GetRange(vtCell1,vtCell2) -> GetValue();

         LONG upperBound;

         SafeArrayGetUBound(vtSafeArray.parray,1,&upperBound);

         VARIANT *pValues = NULL;

         SafeArrayAccessData(vtSafeArray.parray,(void **)&pValues);

         pValues = pValues + upperBound * k;

         if ( VT_BSTR == pValues -> vt ) 
            WideCharToMultiByte(CP_ACP,0,pValues -> bstrVal,-1,szHeader,32,0,0);
         else
            sprintf(szHeader,"Col %ld",k + 1);

         SafeArrayUnaccessData(vtSafeArray.parray);

         } catch ( _com_error e ) {

            sprintf(szHeader,"Col %ld",k + 1);

         }

         SendMessage(hwndDestinationList,LVM_INSERTCOLUMN,k,(LPARAM)&lvColumn);

      }

   }

   swprintf(szwRange,L"R%ld:C%ld",row1,column1);

   if ( vtCell1.bstrVal )
      SysFreeString(vtCell1.bstrVal);

   if ( vtCell2.bstrVal )
      SysFreeString(vtCell2.bstrVal);

   vtCell1.bstrVal = SysAllocString(szwRange);

   swprintf(szwRange,L"R%ld:C%ld",row2,column2);

   vtCell2.bstrVal = SysAllocString(szwRange);

   _variant_t vtSafeArray = pIWorksheet -> GetCells() -> GetRange(vtCell1,vtCell2) -> GetValue();

#if 1

   populateData(hwndDestinationList,vtSafeArray.parray);

#else

   LONG lowerBound[2],upperBound[2];

   SafeArrayGetUBound(vtSafeArray.parray,1,&upperBound[0]);
   SafeArrayGetUBound(vtSafeArray.parray,2,&upperBound[1]);

   SafeArrayGetLBound(vtSafeArray.parray,1,&lowerBound[0]);
   SafeArrayGetLBound(vtSafeArray.parray,2,&lowerBound[1]);

   VARIANT *pValues = NULL;

   SafeArrayAccessData(vtSafeArray.parray,(void **)&pValues);

   VARIANT *pv = pValues;

   WCHAR szwValue[EXCEL_VALUE_SIZE];
   char szValue[EXCEL_VALUE_SIZE];

   LVITEM lvItem = {0};
   lvItem.mask = LVIF_TEXT;
   lvItem.pszText = szValue;

   for ( long j = 0; j <= upperBound[1] - lowerBound[1]; j++ ) {

      for ( long k = 0; k <= upperBound[0] - lowerBound[0]; k++ ) {

         VARIANT *pvt = pv++;

         try {

#if 1
         if ( ! ( S_OK == VariantChangeTypeEx(pvt,pvt,NULL,0,VT_BSTR) ) ) 
            swprintf(szwValue,L"#error");
         else
            swprintf(szwValue,L"%ls",pvt -> bstrVal);

#else

         switch ( pvt -> vt ) {
         case VT_INT:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%d",pvt -> intVal); break;
         case VT_I1:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%c",pvt -> cVal); break;
         case VT_I2:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%d",pvt -> iVal); break;
         case VT_I4:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%d",pvt -> lVal); break;
         case VT_UI1:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%c",pvt -> cVal); break;
         case VT_UI2:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%d",pvt -> iVal); break;
         case VT_UI4:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%ld",pvt -> uintVal); break;
         case VT_UINT:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%ld",pvt -> uintVal); break;
         case VT_R4:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%f",pvt -> fltVal); break;
         case VT_R8:  
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%lf",pvt -> dblVal); break;
         case VT_BSTR:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%ls",pvt -> bstrVal); break;
         case VT_DATE: {
            if ( ! ( S_OK == VariantChangeTypeEx(pvt,pvt,NULL,0,VT_BSTR) ) ) 
               swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"#error");
            else
               swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%ls",pvt -> bstrVal);
            }
            break;
         default:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"nan");
            break;
         }
#endif

         } catch ( ... ) {
OutputDebugString("\nhello world");

         }


         if ( hwndDestinationList ) {

         lvItem.iItem = k;

         lvItem.iSubItem = j;

         WideCharToMultiByte(CP_ACP,0,szwValue,-1,szValue,32,0,0);

         if ( 0 == j )
            SendMessageW(hwndDestinationList,LVM_INSERTITEM,0L,(LPARAM)&lvItem);
         else
            SendMessageW(hwndDestinationList,LVM_SETITEM,0L,(LPARAM)&lvItem);

         }

      }

   }
#endif

   SafeArrayUnaccessData(vtSafeArray.parray);

   } catch ( _com_error e ) {

      char szMessage[1024];



      sprintf(szMessage,"There was an error working with the workbook %s in named range %s.\n\nThe Excel subsystem returned the error %s (%ld).\n\n"
                           "Please check your Workbook or try using a different one.",
                  szDataSource,pszRangeName,e.ErrorMessage(),e.Error());

      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);

   }

   if ( pIWorksheet )
      pIWorksheet -> Release();

   if ( pRange )
      pRange -> Release();

   if ( pRanges ) 
      pRanges -> Release();

   if ( pIWorkbook && ! wasOpen ) 
      pIWorkbook -> Close(VARIANT_TRUE);

   if ( pIWorkbook )
      pIWorkbook -> Release();

   if ( pIApplication )
      pIApplication -> Release();

   return 0;
   }