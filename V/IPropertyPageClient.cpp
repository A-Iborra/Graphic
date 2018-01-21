// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "V.h"

#include "Graphic_resource.h"
#include "utils.h"

#include "Properties_i.h"
#include "Variable_i.h"

   HRESULT V::BeforeAllPropertyPages() {
   iProperties -> Push();
   iProperties -> Push();
   return S_OK;
   }


   HRESULT V::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** theSizes) {
#if 1
   return E_NOTIMPL;
#else
   if ( ! pCntPages ) return E_POINTER;
   if ( ! thePageNames ) return E_POINTER;
   if ( ! theHelpDirs ) return E_POINTER;
   if ( ! theSizes ) return E_POINTER;

   *pCntPages = 1;

   long index = 0;
   BSTR bstrPageName = SysAllocStringLen(NULL,64);
   char szTemp[64];
   sprintf(szTemp,"Variable - %s",name);
   MultiByteToWideChar(CP_ACP,0,szTemp,-1,bstrPageName,64);
   SafeArrayPutElement(*thePageNames,&index,bstrPageName);
   SysFreeString(bstrPageName);

   SafeArrayPutElement(*theHelpDirs,&index,L"");

   long data;

   data = 75;
   SafeArrayPutElement(*theSizes,&index,&data);
   index = 1;
   data = 75;
   SafeArrayPutElement(*theSizes,&index,&data);

   return S_OK;
#endif
   }


   HRESULT V::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {
   return E_NOTIMPL;
   }



   HRESULT V::IsPageDirty(long,BOOL* isDirty) {
   iProperties -> Compare((VARIANT_BOOL*)isDirty);
   return S_OK;
   }


   HRESULT V::Help(BSTR bstrHelpDir) {
   return E_NOTIMPL;
   }


   HRESULT V::TranslateAccelerator(long pMsg,long* pResult) {
   *pResult = S_FALSE;
   return S_OK;
   }


   HRESULT V::Apply() {

   iProperties -> Discard();
   iProperties -> Push();

   getDomain(hwndDialog);

   return S_OK;
   }


   HRESULT V::AfterAllPropertyPages(BOOL userCanceled) {

   if ( userCanceled ) {
      propertiesHaveChanged = false;
      iProperties -> Pop();
      iProperties -> Pop();
   } else {
      iProperties -> Compare((VARIANT_BOOL*)&propertiesHaveChanged);
      iProperties -> Discard();
      iProperties -> Discard();
   }

   getDomain(hwndDialog);

   return S_OK;
   }


   HRESULT V::DestroyPropertyPage(long pageNumber) {
   DestroyWindow(hwndProperties);
   hwndProperties = NULL;
   return S_OK;
   }


   HRESULT V::GetPropertySheetHeader(void *pv) {

   if ( ! pv )
      return E_POINTER;

   PROPSHEETHEADER *pHeader = reinterpret_cast<PROPSHEETHEADER *>(pv);

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;// | PSH_RESIZABLE | PSH_AEROWIZARD;
   pHeader -> hInstance = gsVariables_hModule;
   pHeader -> pszIcon = NULL;
   pHeader -> pszCaption = " Variable Properties";
   pHeader -> pfnCallback = NULL;

   return S_OK;
   }


   HRESULT V::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 1;
   return S_OK;
   }


   HRESULT V::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = gsVariables_hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_VARIABLE);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)handler;
   pPropSheetPages[0].pszTitle = name;
   pPropSheetPages[0].lParam = (LPARAM)this;
   pPropSheetPages[0].pfnCallback = NULL;

   return S_OK;

   }

