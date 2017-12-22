
#include "DataSet.h"

#include <time.h>

   Excel::_Workbook *DataSet::openExcelWorkbook(bool *pWasOpen) {

   Excel::Workbooks *pIWorkbooks = NULL;
   Excel::_Workbook *pIWorkbook = NULL;

   VARIANT vtIndex;
   vtIndex.vt = VT_I4;
   vtIndex.lVal = 0L;

   HRESULT hr = CoCreateInstance(CLSID_excel,NULL,CLSCTX_LOCAL_SERVER,__uuidof(Excel::_Application),reinterpret_cast<void**>(&pIApplication));

   hr = pIApplication -> get_Workbooks(&pIWorkbooks);

   BSTR bstrFile = SysAllocStringLen(NULL,MAX_PATH);

   MultiByteToWideChar(CP_ACP,0,szDataSource,-1,bstrFile,MAX_PATH);

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
      pIWorkbooks -> Open(bstrFile);
      pIWorkbooks -> get_Item(vtIndex,&pIWorkbook);
   }

   pIWorkbooks -> Release();

   SysFreeString(bstrFile);

   return pIWorkbook;
   }


   long DataSet::loadExcelWorkbook() {

   bool wasOpen = false;

   if ( hwndExcelSettings ) {
      SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_RESETCONTENT,0L,0L);
      SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_RESETCONTENT,0L,0L);
   }

   VARIANT vtFalse;
   vtFalse.vt = VT_BOOL;
   vtFalse.boolVal = false;
   
   VARIANT vtIndex;
   vtIndex.vt = VT_I4;
   vtIndex.lVal = 0L;

   Excel::_Workbook *pIWorkbook = NULL;
   Excel::Sheets *pIWorksheets = NULL;
   Excel::_Worksheet *pIWorksheet = NULL;
   Excel::_Worksheet *pIWorksheet_Selected = NULL;

   try {

   pIWorkbook = openExcelWorkbook(&wasOpen);

   HRESULT hr = pIWorkbook -> get_Worksheets(&pIWorksheets);

   long count = 0L;

   pIWorksheets -> get_Count(&count);

   char szThisSheet[64];

   std::list<char *> existingSheets;

   for ( long k = 0; k < count; k++ ) {

      vtIndex.lVal = k + 1;

      IDispatch *pIDispatch = NULL;

      hr = pIWorksheets -> get_Item(vtIndex,&pIDispatch);

      pIDispatch -> QueryInterface(__uuidof(Excel::_Worksheet),reinterpret_cast<void **>(&pIWorksheet));

      bstr_t theName = pIWorksheet -> GetName();

      pIDispatch -> Release();

      WideCharToMultiByte(CP_ACP,0,theName.GetBSTR(),-1,szThisSheet,64,0,0);

      char *p = new char[strlen(szThisSheet) + 1];

      strcpy(p,szThisSheet);

      existingSheets.insert(existingSheets.end(),p);

      if ( hwndExcelSettings )
         SendMessage(GetDlgItem(hwndExcelSettings,IDDI_DATASET_EXCEL_SPREADSHEET_LIST),CB_ADDSTRING,0L,(LPARAM)szThisSheet);

      if ( szSpreadsheetName[0] && 0 == strcmp(szThisSheet,szSpreadsheetName) ) {
         pIWorksheet_Selected = pIWorksheet;
         pIWorksheet_Selected -> AddRef();
         if ( hwndExcelSettings )
            SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_SETCURSEL,
                           (WPARAM)(SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCOUNT,0L,0L) - 1),(LPARAM)0L);

      }

      pIWorksheet -> Release();

      pIWorksheet = NULL;

   }

   if ( szSpreadsheetName[0] && ! pIWorksheet_Selected ) {

      char szMessage[1024];
      sprintf(szMessage,"The workbook %s does not contain the worksheet %s.\n\nWould you like to create this worksheet in the workbook ?",szDataSource,szSpreadsheetName);

      if ( IDNO == MessageBox(NULL,szMessage,"Error!",MB_YESNO | MB_DEFBUTTON1 | MB_TOPMOST) ) {

         if ( ! wasOpen )
            pIWorkbook -> Close(vtFalse);

         pIWorksheets -> Release();
         pIWorkbook -> Release();

         for ( std::list<char *>::iterator it = existingSheets.begin(); it != existingSheets.end(); it++ )
            delete [] (*it);

         existingSheets.clear();

      } else {
      
         pIWorksheets -> Add();

         pIWorksheets -> get_Count(&count);

         for ( long k = 0; k < count; k++ ) {
   
            vtIndex.lVal = k + 1;

            IDispatch *pIDispatch = NULL;

            hr = pIWorksheets -> get_Item(vtIndex,&pIDispatch);

            pIDispatch -> QueryInterface(__uuidof(Excel::_Worksheet),reinterpret_cast<void **>(&pIWorksheet));

            bstr_t theName = pIWorksheet -> GetName();

            pIDispatch -> Release();

            WideCharToMultiByte(CP_ACP,0,theName.GetBSTR(),-1,szThisSheet,64,0,0);
         
            bool wasFound = false;
            for ( std::list<char *>::iterator it = existingSheets.begin(); it != existingSheets.end(); it++ ) {
               if ( 0 == strcmp((*it),szThisSheet) ) {
                  wasFound = true;
                  break;
               }
            }
   
            if ( ! wasFound )
               break;

            pIWorksheet -> Release();

            pIWorksheet = NULL;

         }

         BSTR theSheetName = SysAllocStringLen(NULL,64);

         MultiByteToWideChar(CP_ACP,0,szSpreadsheetName,-1,theSheetName,64);

         pIWorksheet -> PutName(theSheetName);

         SysFreeString(theSheetName);

      }

   }

   for ( std::list<char *>::iterator it = existingSheets.begin(); it != existingSheets.end(); it++ )
      delete [] (*it);

   existingSheets.clear();

   if ( hwndExcelSettings ) {

      Excel::NamesPtr pRanges = NULL;

      hr = pIWorkbook -> get_Names(&pRanges);

      count = pRanges -> GetCount();

      for ( long k = 0; k < count; k++ ) {

         vtIndex.lVal = k + 1;

         Excel::NamePtr pName = pRanges -> Item(vtIndex);

         _bstr_t theName = pName -> GetName();

         char szName[128];

         WideCharToMultiByte(CP_ACP,0,theName,-1,szName,128,0,0);

         SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_ADDSTRING,0L,(LPARAM)szName);

         if ( szNamedRange[0] && 0 == strcmp(szNamedRange,szName) )
            SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_SETCURSEL,(WPARAM)(SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCOUNT,0L,0L) - 1),(LPARAM)szName);

      }

      pRanges -> Release();

   }

   } catch ( _com_error e ) {

      char szMessage[1024];

      sprintf(szMessage,"There was an error working with the workbook %s in worksheet %s.\n\nThe Excel subsystem returned the error %s (%ld).\n\n"
                           "Please check your Workbook or try using a different one.",
                  szDataSource,szSpreadsheetName,e.ErrorMessage(),e.Error());

      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);

   }

   if ( pIWorksheet_Selected )
      pIWorksheet_Selected -> Release();

   if ( pIWorksheet )
      pIWorksheet -> Release();

   if ( pIWorksheets )
      pIWorksheets -> Release();

   if ( pIWorkbook && ! wasOpen ) 
      pIWorkbook -> Close(VARIANT_TRUE);

   if ( pIWorkbook )
      pIWorkbook -> Release();

   if ( pIApplication )
      pIApplication -> Release();

   return 0;
   }
