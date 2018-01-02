
#include <oleacc.h>
#include <vector>

#include "DataSet.h"
#include "utilities.h"

   Excel::_Workbook *DataSet::openExcelWorkbook(char *pszWorkbookName,bool *pWasOpen) {

   Excel::Workbooks *pIWorkbooks = NULL;
   Excel::_Workbook *pIWorkbook = NULL;

   HWND hwndExcel = FindWindow("XLMain",NULL);

   if ( hwndExcel ) {
      std::function<bool(HWND)> f( [](HWND hwndTest) { 
         Excel::Window *pIWindow = NULL;
         AccessibleObjectFromWindow(hwndTest, OBJID_NATIVEOM,__uuidof(Excel::Window),(void**)&pIWindow);
         if ( pIWindow ) {
            pIWindow -> Release();
            return true;
         }
         return false;
         });
      hwndExcel = (HWND)FindWindowWithFunction(hwndExcel,&f);
   }

   HRESULT hr = S_OK;

   if ( hwndExcel ) {
      Excel::Window *pIWindow;
      hr = AccessibleObjectFromWindow(hwndExcel, OBJID_NATIVEOM,__uuidof(Excel::Window),(void**)&pIWindow);
      if ( ! pIWindow ) 
         hr = CoCreateInstance(CLSID_excel,NULL,CLSCTX_LOCAL_SERVER,__uuidof(Excel::_Application),reinterpret_cast<void**>(&pIApplication));
      else {
         Excel::_ApplicationPtr theApplication = pIWindow -> GetApplication();
         hr = theApplication.QueryInterface(__uuidof(Excel::_Application),&pIApplication);
         pIWindow -> Release();
      }
   } else
      hr = CoCreateInstance(CLSID_excel,NULL,CLSCTX_LOCAL_SERVER,__uuidof(Excel::_Application),reinterpret_cast<void**>(&pIApplication));

   hr = pIApplication -> get_Workbooks(&pIWorkbooks);

   BSTR bstrFile = SysAllocStringLen(NULL,MAX_PATH);

   MultiByteToWideChar(CP_ACP,0,pszWorkbookName,-1,bstrFile,MAX_PATH);

   VARIANT vtIndex;
   vtIndex.vt = VT_I4;
   vtIndex.lVal = 0L;

   long count = 0L;

   *pWasOpen = false;

   pIWorkbooks -> get_Count(&count);

   for ( long k = 0; k < count; k++ ) {

      vtIndex.lVal = k + 1;

      hr = pIWorkbooks -> get_Item(vtIndex,&pIWorkbook);

      bstr_t theName = pIWorkbook -> GetFullName();

      if ( 0 == wcscmp(theName,bstrFile) ) {
         *pWasOpen = true;
         break;
      }

      pIWorkbook -> Release();

      pIWorkbook = NULL;

   } 

   if ( ! *pWasOpen ) {

      vtIndex.lVal = count + 1;

      try {

      pIWorkbooks -> Open(bstrFile);
      pIWorkbooks -> get_Item(vtIndex,&pIWorkbook);

      } catch ( _com_error e ) {

         pIWorkbooks -> Release();

         pIApplication -> Release();

         pIApplication = NULL;

         return NULL;
      }
   }

   pIWorkbooks -> Release();

   SysFreeString(bstrFile);

   return pIWorkbook;
   }


   Excel::_Worksheet *DataSet::getExcelWorksheet(Excel::_Workbook *pIWorkbook,WCHAR *pszwSheetName) {
   char szSheetName[128];
   WideCharToMultiByte(CP_ACP,0,pszwSheetName,-1,szSheetName,128,0,0);
   return getExcelWorksheet(pIWorkbook,szSheetName);
   }

   
   Excel::_Worksheet *DataSet::getExcelWorksheet(Excel::_Workbook *pIWorkbook,char *pszSheetName) {

   Excel::Sheets * pIWorksheets = NULL;
   Excel::_Worksheet *pIWorksheet = NULL;

   HRESULT hr = pIWorkbook -> get_Worksheets(&pIWorksheets);

   VARIANT vtIndex;
   vtIndex.vt = VT_I4;
   vtIndex.lVal = 0L;

   long count = 0L;

   pIWorksheets -> get_Count(&count);

   char szThisSheet[64];

   for ( long k = 0; k < count; k++ ) {

      vtIndex.lVal = k + 1;

      IDispatch *pIDispatch = NULL;

      hr = pIWorksheets -> get_Item(vtIndex,&pIDispatch);

      pIDispatch -> QueryInterface(__uuidof(Excel::_Worksheet),reinterpret_cast<void **>(&pIWorksheet));

      bstr_t theName = pIWorksheet -> GetName();

      pIDispatch -> Release();

      WideCharToMultiByte(CP_ACP,0,theName.GetBSTR(),-1,szThisSheet,64,0,0);

      if ( 0 == strcmp(szThisSheet,pszSheetName) ) 
         break;

      pIWorksheet -> Release();

      pIWorksheet = NULL;

   }

   pIWorksheets -> Release();

   return pIWorksheet;
   }


   Excel::NamePtr DataSet::findNamedRangeName(Excel::_Workbook *pIWorkbook,char *pszRangeName) {

   Excel::NamesPtr pAllNames = NULL;
   Excel::NamePtr pName = NULL;

   VARIANT vtIndex;
   vtIndex.vt = VT_I4;
   vtIndex.lVal = 0L;

   VARIANT vRange;
   vRange.vt = VT_BSTR;

   vRange.bstrVal = SysAllocStringLen(NULL,32);
   
   MultiByteToWideChar(CP_ACP,0,pszRangeName,-1,vRange.bstrVal,32);

   HRESULT hr = pIWorkbook -> get_Names(&pAllNames);

   long count = pAllNames -> GetCount();

   for ( long k = 0; k < count; k++ ) {

      vtIndex.lVal = k + 1;

      pName = pAllNames -> Item(vtIndex);

      _bstr_t theName = pName -> GetName();

      char szName[128];

      WideCharToMultiByte(CP_ACP,0,theName,-1,szName,128,0,0);

      if ( 0 == strcmp(pszRangeName,szName) ) 
         break;

      pName -> Release();

      pName = NULL;

   }

   if ( pAllNames )
      pAllNames -> Release();

   return pName;
   }


   bool DataSet::peekExcelCells(char *pszWorkbookName,char *pszRangeName,char *pszWorksheetName,char *pszCellRange,bool *pContainsHeaderRow) {

   bool wasOpen = false;

   Excel::_Workbook *pIWorkbook = NULL;
   Excel::_Worksheet *pIWorksheet = NULL;
   Excel::NamePtr pName = NULL;
   Excel::RangePtr pRange = NULL;

   *pContainsHeaderRow = false;
   
   bool returnValue = false;

   try {

   pIWorkbook = openExcelWorkbook(pszWorkbookName,&wasOpen);

   if ( ! pIWorkbook ) {

      if ( pIApplication )
         pIApplication -> Release();

      pIApplication = NULL;

      return false;
   }

   _variant_t vtSafeArray;

   if ( pszRangeName ) {

      pName = findNamedRangeName(pIWorkbook,pszRangeName);

      if ( ! pName ) {

         if ( pIApplication )
            pIApplication -> Release();

         pIApplication = NULL;

         return false;

      }

      pRange = pName -> GetRefersToRange();

      vtSafeArray = pRange -> GetCells() -> GetValue();

   } else {

      pIWorksheet = getExcelWorksheet(pIWorkbook,pszWorksheetName);

      if ( ! pIWorksheet ) {

         if ( pIApplication )
            pIApplication -> Release();

         pIApplication = NULL;

         return false;

      }

      WCHAR szwRange[64];

      MultiByteToWideChar(CP_ACP,0,pszCellRange,-1,szwRange,64);

      vtSafeArray = pIWorksheet -> GetCells() -> GetRange(szwRange) -> GetValue();

   }

   if ( VT_ARRAY & vtSafeArray.vt ) {
      *pContainsHeaderRow = dataHasHeaderRow(vtSafeArray.parray);
      returnValue = true;
   } else
      returnValue = false;

   } catch ( ... ) {

      returnValue = false;

   }

   if ( pRange )
      pRange -> Release();

   if ( pName )
      pName -> Release();

   if ( pIWorksheet )
      pIWorksheet -> Release();

   if ( pIWorkbook && ! wasOpen ) 
      pIWorkbook -> Close(VARIANT_TRUE);

   if ( pIWorkbook )
      pIWorkbook -> Release();

   if ( pIApplication )
      pIApplication -> Release();

   pIApplication = NULL;


   return returnValue;
   }


   bool DataSet::dataHasHeaderRow(SAFEARRAY *pArray) {

   LONG lowerBound[2],upperBound[2];

   SafeArrayGetUBound(pArray,1,&upperBound[0]);
   SafeArrayGetUBound(pArray,2,&upperBound[1]);

   SafeArrayGetLBound(pArray,1,&lowerBound[0]);
   SafeArrayGetLBound(pArray,2,&lowerBound[1]);

   long countRows = upperBound[0] - lowerBound[0] + 1;

   if ( 2 > countRows )
      return false;

   long countColumns = upperBound[1] - lowerBound[1] + 1;

   VARIANT *pValues = NULL;

   SafeArrayAccessData(pArray,(void **)&pValues);

   std::vector<VARTYPE> firstRow;
   std::vector<VARTYPE> secondRow;

   VARIANT *pv = pValues;

   for ( long columnIndex = 0; columnIndex < countColumns; columnIndex++ )
      firstRow.push_back((pv + columnIndex * countRows) -> vt);

   pv = pValues + 1;

   for ( long columnIndex = 0; columnIndex < countColumns; columnIndex++ )
      secondRow.push_back((pv + columnIndex * countRows) -> vt);

   bool hasHeaderRow = false;

   for ( long k = 0; k < countColumns; k++ ) {
      if ( ! ( firstRow[k] == secondRow[k] ) ) {
         hasHeaderRow = true;
         break;
      }
   }

   SafeArrayUnaccessData(pArray);

   return hasHeaderRow;
   }
