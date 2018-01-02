
#include "DataSet.h"

   long DataSet::loadExcelWorkbook(HWND hwndSheetList,HWND hwndRangeList,HWND hwndErrorReport,char *pszWorkbookName,char *pszRelevantSheetName,char *pszRelevantRangeName) {

   bool wasOpen = false;

   if ( hwndSheetList )
      SendMessage(hwndSheetList,CB_RESETCONTENT,0L,0L);

   if ( hwndRangeList )
      SendMessage(hwndRangeList,CB_RESETCONTENT,0L,0L);

   Excel::_Workbook *pIWorkbook = NULL;
   Excel::_Worksheet *pIWorksheet = NULL;

   VARIANT vtIndex;
   vtIndex.vt = VT_I4;
   vtIndex.lVal = 0L;
    
   try {

   pIWorkbook = openExcelWorkbook(pszWorkbookName,&wasOpen);

   if ( NULL == pIWorkbook ) {

      if ( hwndErrorReport ) {
         char szMessage[1024];
         sprintf_s(szMessage,1024,"The Excel workbook %s could not be found or opening it failed.",pszWorkbookName);
         SetWindowText(hwndErrorReport,szMessage);
      }

      if ( pIApplication )
         pIApplication -> Release();

      pIApplication = NULL;

      return 0;
   }

   if ( hwndSheetList ) {

      long count;

      Excel::Sheets *pIWorksheets = NULL;

      HRESULT hr = pIWorkbook -> get_Worksheets(&pIWorksheets);

      pIWorksheets -> get_Count(&count);

      char szSheetName[64];

      for ( long k = 0; k < count; k++ ) {
   
         vtIndex.lVal = k + 1;

         IDispatch *pIDispatch = NULL;

         hr = pIWorksheets -> get_Item(vtIndex,&pIDispatch);

         Excel::_Worksheet *pIWorksheetTemp;

         pIDispatch -> QueryInterface(__uuidof(Excel::_Worksheet),reinterpret_cast<void **>(&pIWorksheetTemp));

         BSTR bstr = SysAllocString(pIWorksheetTemp -> GetName());
         
         WideCharToMultiByte(CP_ACP,0,bstr,-1,szSheetName,64,0,0);

         SendMessage(hwndSheetList,CB_ADDSTRING,(WPARAM)0,(LPARAM)szSheetName);

         if ( pszRelevantSheetName && pszRelevantSheetName[0] && 0 == strcmp(pszRelevantSheetName,szSheetName) )
            SendMessage(hwndSheetList,CB_SETCURSEL,(WPARAM)(SendMessage(hwndSheetList,CB_GETCOUNT,0L,0L) - 1),(LPARAM)0);

         pIDispatch -> Release();

         pIWorksheetTemp -> Release();

         pIWorksheetTemp = NULL;

      }

      pIWorksheets -> Release();

   }

   if ( hwndRangeList ) {

      Excel::NamesPtr pRanges = NULL;

      HRESULT hr = pIWorkbook -> get_Names(&pRanges);

      long count = pRanges -> GetCount();

      for ( long k = 0; k < count; k++ ) {

         vtIndex.lVal = k + 1;

         Excel::NamePtr pName = pRanges -> Item(vtIndex);

         _bstr_t theName = pName -> GetName();

         char szName[128];

         WideCharToMultiByte(CP_ACP,0,theName,-1,szName,128,0,0);

         SendMessage(hwndRangeList,CB_ADDSTRING,0L,(LPARAM)szName);

         if ( pszRelevantRangeName && pszRelevantRangeName[0] && 0 == strcmp(pszRelevantRangeName,szName) )
            SendMessage(hwndRangeList,CB_SETCURSEL,(WPARAM)(SendMessage(hwndRangeList,CB_GETCOUNT,0L,0L) - 1),(LPARAM)0);

      }

      pRanges -> Release();

   }

   if ( pszRelevantSheetName && pszRelevantSheetName[0] )
      pIWorksheet = getExcelWorksheet(pIWorkbook,pszRelevantSheetName);
  
   if ( pszRelevantSheetName && pszRelevantSheetName[0] && ! pIWorksheet ) {

      int mbResponse = IDYES;

      if ( strcmp("<new>",pszRelevantSheetName) ) {
         char szMessage[1024];
         sprintf(szMessage,"The workbook %s does not contain the worksheet %s.\n\nWould you like to create this worksheet in the workbook ?",szDataSource,pszRelevantSheetName);
         mbResponse = MessageBox(NULL,szMessage,"Error!",MB_YESNO | MB_DEFBUTTON1 | MB_TOPMOST);
      }

      if ( IDNO == mbResponse ) {

         if ( ! wasOpen )
            pIWorkbook -> Close(VARIANT_FALSE);

         pIWorkbook -> Release();

         pIApplication -> Release();

         pIApplication = NULL;

         return 0;

      } 

      long count;

      std::list<BSTR> existingSheets;

      Excel::Sheets *pIWorksheets = NULL;

      HRESULT hr = pIWorkbook -> get_Worksheets(&pIWorksheets);

      pIWorksheets -> get_Count(&count);

      for ( long k = 0; k < count; k++ ) {
   
         vtIndex.lVal = k + 1;

         IDispatch *pIDispatch = NULL;

         hr = pIWorksheets -> get_Item(vtIndex,&pIDispatch);

         Excel::_Worksheet *pIWorksheetTemp;

         pIDispatch -> QueryInterface(__uuidof(Excel::_Worksheet),reinterpret_cast<void **>(&pIWorksheetTemp));

         existingSheets.push_back(SysAllocString(pIWorksheetTemp -> GetName()));

         pIDispatch -> Release();

         pIWorksheetTemp -> Release();

         pIWorksheetTemp = NULL;

      }

      pIWorksheets -> Add();

      pIWorksheets -> get_Count(&count);

      char szNewSheetName[64];

      for ( long k = 0; k < count; k++ ) {
   
         vtIndex.lVal = k + 1;

         IDispatch *pIDispatch = NULL;

         HRESULT hr = pIWorksheets -> get_Item(vtIndex,&pIDispatch);

         Excel::_Worksheet *pIWorksheetTemp = NULL;

         pIDispatch -> QueryInterface(__uuidof(Excel::_Worksheet),reinterpret_cast<void **>(&pIWorksheetTemp));

         bstr_t theName = pIWorksheetTemp -> GetName();

         pIDispatch -> Release();
         
         bool wasFound = false;

         for ( BSTR bstr : existingSheets ) {
            if ( 0 == wcscmp(bstr,theName.GetBSTR()) ) {
               wasFound = true;
               break;
            }
         }
   
         pIWorksheetTemp -> Release();

         if ( wasFound )
            continue;

         WideCharToMultiByte(CP_ACP,0,theName.GetBSTR(),-1,szNewSheetName,64,0,0);

         break;

      }

      for ( BSTR bstr : existingSheets )
         SysFreeString(bstr);

      if ( pIWorksheets )
         pIWorksheets -> Release();

      if ( pIWorkbook && ! wasOpen ) 
         pIWorkbook -> Close(VARIANT_TRUE);

      if ( pIWorkbook )
         pIWorkbook -> Release();

      if ( pIApplication )
         pIApplication -> Release();

      pIApplication = NULL;

      return loadExcelWorkbook(hwndSheetList,hwndRangeList,hwndErrorReport,pszWorkbookName,szNewSheetName,pszRelevantRangeName);

   }

   } catch ( _com_error e ) {

      char szMessage[1024];

      sprintf(szMessage,"There was an error working with the workbook %s in worksheet %s.\n\nThe Excel subsystem returned the error %s (%ld).\n\n"
                           "Please check your Workbook or try using a different one.",
                  szDataSource,pszRelevantSheetName ? pszRelevantSheetName : "??" ,e.ErrorMessage(),e.Error());

      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);

   }

   if ( pIWorksheet )
      pIWorksheet -> Release();

   if ( pIWorkbook && ! wasOpen ) 
      pIWorkbook -> Close(VARIANT_TRUE);

   if ( pIWorkbook )
      pIWorkbook -> Release();

   if ( pIApplication )
      pIApplication -> Release();

   pIApplication = NULL;

   return 0;
   }
