// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"

   long DataSet::loadExcelWorksheet(char *pszWorkbookName,char *pszWorksheetName) {

   bool wasOpen = false;

   Excel::_Workbook *pIWorkbook = NULL;
   Excel::_Worksheet *pIWorksheet = NULL;

   if ( hwndExcelSettings ) 
      SendDlgItemMessage(hwndExcelSettings,IDDI_DATASET_EXCEL_NAMEDRANGES_LIST,CB_RESETCONTENT,0L,0L);

   try {

   pIWorkbook = openExcelWorkbook(pszWorkbookName,&wasOpen);

   pIWorksheet = getExcelWorksheet(pIWorkbook,pszWorksheetName);

   VARIANT vtIndex;
   vtIndex.vt = VT_I4;
   vtIndex.lVal = 0L;

   Excel::NamesPtr pRanges = NULL;

   HRESULT hr = pIWorksheet -> get_Names(&pRanges);

   long count = pRanges -> GetCount();

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

   pIWorksheet -> Release();

   } catch ( _com_error e ) {

      char szMessage[1024];

      sprintf(szMessage,"There was an error working with the workbook %s in worksheet %s.\n\nThe Excel subsystem returned the error %s (%ld).\n\n"
                           "Please check your Workbook or try using a different one.",
                  szDataSource,pszWorksheetName,e.ErrorMessage(),e.Error());

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