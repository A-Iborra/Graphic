// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Axis.h"

#include "utils.h"
#include "Graphic_resource.h"


   extern HMODULE hModule;

   HRESULT Axis::BeforeAllPropertyPages() {
   return E_NOTIMPL;
   }


   HRESULT Axis::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {
   return E_NOTIMPL;
   }


   HRESULT Axis::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {
   return E_NOTIMPL;
   }


   HRESULT Axis::Apply() {
   return E_NOTIMPL;
   }


   HRESULT Axis::IsPageDirty(long hwndPage,BOOL* isDirty) {
   return E_NOTIMPL;
   }


   HRESULT Axis::Help(BSTR bstrHelpDir) {
   return E_NOTIMPL;
   }


   HRESULT Axis::TranslateAccelerator(long,long* pResult) {
   return E_NOTIMPL;
   }


   HRESULT Axis::AfterAllPropertyPages(BOOL userCanceled) {
   return S_OK;
   }



   HRESULT Axis::DestroyPropertyPage(long pageNumber) {
   return E_NOTIMPL;
   }


   HRESULT Axis::GetPropertySheetHeader(void *pv) {
   
   if ( ! pv )
      return E_POINTER;

   PROPSHEETHEADER *pHeader = reinterpret_cast<PROPSHEETHEADER *>(pv);

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;// | PSH_RESIZABLE | PSH_AEROWIZARD;
   pHeader -> hInstance = hModule;
   pHeader -> pszIcon = NULL;

   switch ( type ) {
   case 'X':
      pHeader -> pszCaption = " X-Axis Properties";
      break;
   case 'Y':
      pHeader -> pszCaption = " Y-Axis Properties";
      break;
   case 'Z':
      pHeader -> pszCaption = " Z-Axis Properties";
      break;
   }

   pHeader -> pfnCallback = NULL;

   return S_OK;
   }


   HRESULT Axis::get_PropertyPageCount(long *pCount) {

   if ( ! pCount )
      return E_POINTER;

   *pCount = 4;

   IGPropertyPageClient *pIGPropertyPageClient = NULL;
   
   pRepresentativeText -> QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void **>(&pIGPropertyPageClient));

   long countAdditional = 0;

   pIGPropertyPageClient -> get_PropertyPageCount(&countAdditional);

   countAdditional -= 2;

   *pCount += countAdditional;

   pIGPropertyPageClient -> Release();

   return S_OK;
   }


   HRESULT Axis::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_AXIS_STYLE);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)styleHandler;
   pPropSheetPages[0].pszTitle = "Style";
   pPropSheetPages[0].lParam = (LPARAM)this;
   pPropSheetPages[0].pfnCallback = NULL;

   pPropSheetPages[1].dwFlags = PSP_USETITLE;
   pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[1].hInstance = hModule;
   pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_AXIS_POSITION);
   pPropSheetPages[1].pfnDlgProc = (DLGPROC)positionHandler;
   pPropSheetPages[1].pszTitle = "Position";
   pPropSheetPages[1].lParam = (LPARAM)this;
   pPropSheetPages[1].pfnCallback = NULL;

   pPropSheetPages[2].dwFlags = PSP_USETITLE;
   pPropSheetPages[2].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[2].hInstance = hModule;
   pPropSheetPages[2].pszTemplate = MAKEINTRESOURCE(IDDIALOG_AXIS_TEXT);
   pPropSheetPages[2].pfnDlgProc = (DLGPROC)textHandler;
   pPropSheetPages[2].pszTitle = "Text";
   pPropSheetPages[2].lParam = (LPARAM)this;
   pPropSheetPages[2].pfnCallback = NULL;

   IGPropertyPageClient *pIGPropertyPageClient = NULL;
   
   pRepresentativeText -> QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void **>(&pIGPropertyPageClient));

   pRepresentativeText -> put_ShowContentPropertyPage(VARIANT_FALSE);
   pRepresentativeText -> put_ShowStylePropertyPage(VARIANT_FALSE);
   pRepresentativeText -> put_ShowOrientationPropertyPage(VARIANT_TRUE);

   long countAdditional = 0;

   pIGPropertyPageClient -> get_PropertyPageCount(&countAdditional);

   countAdditional -= 2;

   pIGPropertyPageClient -> GetPropertySheets((void *)&pPropSheetPages[3]);

   for ( int k = 0; k < countAdditional; k++ ) {
      if ( 0 == strcmp(pPropSheetPages[3 + k].pszTitle,"Orientation") ) {
         pPropSheetPages[3 + k].pszTitle = "Text Orientation";
         break;
      }
   }

   pIGPropertyPageClient -> Release();

   pPropSheetPages[3 + countAdditional].dwFlags = PSP_USETITLE;
   pPropSheetPages[3 + countAdditional].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[3 + countAdditional].hInstance = hModule;
   pPropSheetPages[3 + countAdditional].pszTemplate = MAKEINTRESOURCE(IDDIALOG_AXIS_COLOR);
   pPropSheetPages[3 + countAdditional].pfnDlgProc = (DLGPROC)colorHandler;
   pPropSheetPages[3 + countAdditional].pszTitle = "Color";
   pPropSheetPages[3 + countAdditional].lParam = (LPARAM)this;
   pPropSheetPages[3 + countAdditional].pfnCallback = NULL;

   return S_OK;
   }

