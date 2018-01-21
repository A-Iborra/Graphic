// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"

   long DataSet::launchExcel(char *pszWorkbookName) {

   Excel::_Application *pIApplication = NULL;
   Excel::Workbooks *pIWorkbooks = NULL;

   BSTR bstrFile = SysAllocStringLen(NULL,MAX_PATH);

   MultiByteToWideChar(CP_ACP,0,pszWorkbookName,-1,bstrFile,MAX_PATH);

   HRESULT hr = CoCreateInstance(CLSID_excel,NULL,CLSCTX_LOCAL_SERVER,__uuidof(Excel::_Application),reinterpret_cast<void**>(&pIApplication));

   pIApplication -> put_Visible(0,VARIANT_TRUE);

   hr = pIApplication -> get_Workbooks(&pIWorkbooks);

   pIWorkbooks -> Open(bstrFile);

   if ( pIWorkbooks )
      pIWorkbooks -> Release();

   if ( pIApplication )
      pIApplication -> Release();

   return 0;
   }
