#include "DataSet.h"

#include <time.h>

   long DataSet::loadExcelWorksheet() {

   bool wasOpen = false;

   //VARIANT vtFalse;
   //vtFalse.vt = VT_BOOL;
   //vtFalse.boolVal = false;
   //
   VARIANT vtIndex;
   vtIndex.vt = VT_I4;
   vtIndex.lVal = 0L;

   Excel::_Workbook *pIWorkbook = NULL;
   Excel::Sheets *pIWorksheets = NULL;
   Excel::_Worksheet *pIWorksheet = NULL;
   Excel::_Worksheet *pIWorksheet_Selected = NULL;

   if ( hwndExcelSettings ) 
      SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_NAMEDRANGES_LIST,CB_RESETCONTENT,0L,0L);

   try {

   pIWorkbook = openExcelWorkbook(&wasOpen);

   HRESULT hr = pIWorkbook -> get_Worksheets(&pIWorksheets);

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

      if ( szSpreadsheetName[0] && 0 == strcmp(szThisSheet,szSpreadsheetName) ) {
         pIWorksheet_Selected = pIWorksheet;
         pIWorksheet_Selected -> AddRef();
         pIWorksheet -> Release();
         break;
      }

      pIWorksheet -> Release();

      pIWorksheet = NULL;

   }

   pIWorksheets -> Release();

   Excel::NamesPtr pRanges = NULL;

   hr = pIWorksheet_Selected -> get_Names(&pRanges);

   count = pRanges -> GetCount();

   for ( long k = 0; k < count; k++ ) {

      vtIndex.lVal = k + 1;

      Excel::NamePtr pName = pRanges -> Item(vtIndex);

      _bstr_t theName = pName -> GetName();

      char szName[128];

      WideCharToMultiByte(CP_ACP,0,theName,-1,szName,128,0,0);

      if ( hwndExcelSettings ) 
         SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_NAMEDRANGES_LIST,CB_ADDSTRING,0L,(LPARAM)szName);

   }

   pRanges -> Release();

   pIWorksheet_Selected -> Release();

   } catch ( _com_error e ) {

      char szMessage[1024];

      sprintf(szMessage,"There was an error working with the workbook %s in worksheet %s.\n\nThe Excel subsystem returned the error %s (%ld).\n\n"
                           "Please check your Workbook or try using a different one.",
                  szDataSource,szSpreadsheetName,e.ErrorMessage(),e.Error());

      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);

   }

   if ( pIWorkbook && ! wasOpen ) 
      pIWorkbook -> Close(VARIANT_TRUE);

   if ( pIWorkbook )
      pIWorkbook -> Release();

   if ( pIApplication )
      pIApplication -> Release();

   return 0;
   }