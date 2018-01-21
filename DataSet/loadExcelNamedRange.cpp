// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"

   long DataSet::loadExcelNamedRange(HWND hwndDestinationList,HWND hwndErrorReport,char *pszWorkbookName,char *pszRangeName) {

   if ( ! hwndExcelSettings )
      return 0;

   bool wasOpen = false;

   Excel::_Workbook *pIWorkbook = NULL;
   Excel::_Worksheet *pIWorksheet = NULL;
   Excel::NamePtr pRange = NULL;
     
   if ( hwndDestinationList ) { 

      SendMessage(hwndDestinationList,LVM_DELETEALLITEMS,0L,0L);

      LVCOLUMN lvColumn = {0};

      lvColumn.mask = LVCF_WIDTH;

      while ( SendMessage(hwndDestinationList,LVM_GETCOLUMN,0,(LPARAM)&lvColumn) )
         SendMessage(hwndDestinationList,LVM_DELETECOLUMN,0L,0L);

   }

   try {

   pIWorkbook = openExcelWorkbook(pszWorkbookName,&wasOpen);

   if ( NULL == pIWorkbook ) {

      if ( hwndErrorReport ) {
         char szMessage[256];
         sprintf_s(szMessage,256,"The Excel import failed because the workbook (%s) could not be opened.",pszWorkbookName);
         SetWindowText(hwndErrorReport,szMessage);
      }

      if ( pIApplication )
         pIApplication -> Release();

      pIApplication = NULL;

      return 0;
   }

   pRange = findNamedRangeName(pIWorkbook,pszRangeName);

   if ( ! pRange ) {

      if ( hwndErrorReport ) {
         char szMessage[256];
         sprintf_s(szMessage,256,"The Excel import failed because the worksheet (%s) does not contain the range %s.",pszWorkbookName,pszRangeName);
         SetWindowText(hwndErrorReport,szMessage);
      }

      if ( pIWorkbook && ! wasOpen ) 
         pIWorkbook -> Close(VARIANT_TRUE);

      if ( pIWorkbook )
         pIWorkbook -> Release();

      if ( pIApplication )
         pIApplication -> Release();

      return 0;

   }

   _variant_t rangeCellReference = pRange -> GetRefersTo();

   _variant_t rangeRefersTo = pRange -> GetRefersToR1C1();

   WCHAR szwInfo[128];

   wcscpy(szwInfo,rangeRefersTo.bstrVal);

   WCHAR *p = wcschr(szwInfo,L'!');

   *p = '\0';

   WCHAR *pszwSheetName = &szwInfo[1];

   pIWorksheet = getExcelWorksheet(pIWorkbook,pszwSheetName);

   if ( NULL == pIWorksheet ) {

      if ( hwndErrorReport ) {
         char szMessage[256];
         char szSheet[128];
         WideCharToMultiByte(CP_ACP,0,pszwSheetName,-1,szSheet,128,0,0);
         sprintf_s(szMessage,256,"The Excel import failed because the worksheet (%s) in %s could not be opened.",szSheet,pszWorkbookName);
         SetWindowText(hwndErrorReport,szMessage);
      }

      pRange -> Release();

      if ( pIWorkbook && ! wasOpen ) 
         pIWorkbook -> Close(VARIANT_TRUE);

      if ( pIWorkbook )
         pIWorkbook -> Release();

      if ( pIApplication )
         pIApplication -> Release();

      return 0;

   }

   _variant_t vtSafeArray = pIWorksheet -> GetCells() -> GetRange(rangeCellReference) -> GetValue();

   populateData(hwndDestinationList,hwndErrorReport,true,vtSafeArray.parray);

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

   if ( pIWorkbook && ! wasOpen ) 
      pIWorkbook -> Close(VARIANT_TRUE);

   if ( pIWorkbook )
      pIWorkbook -> Release();

   if ( pIApplication )
      pIApplication -> Release();

   return 0;
   }