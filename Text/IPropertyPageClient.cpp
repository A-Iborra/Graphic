// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"
#include "Graphic_resource.h"

   HRESULT Text::BeforeAllPropertyPages() {
   return E_NOTIMPL;
   }


   HRESULT Text::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {
   return E_NOTIMPL;
   }


   HRESULT Text::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {
   return E_NOTIMPL;
   }


   HRESULT Text::Apply() {
   return E_NOTIMPL;
   }


   HRESULT Text::IsPageDirty(long hwndPage,BOOL* isDirty) {
   return E_NOTIMPL;
   }


   HRESULT Text::Help(BSTR bstrHelpDir) {
   return  E_NOTIMPL;
   }


   HRESULT Text::TranslateAccelerator(long,long* pResult) {
   return E_NOTIMPL;
   }


   HRESULT Text::AfterAllPropertyPages(BOOL userCanceled) {
   return E_NOTIMPL;
   }



   HRESULT Text::DestroyPropertyPage(long pageNumber) {
   return E_NOTIMPL;
   }


   HRESULT Text::GetPropertySheetHeader(void *pv) {
   
   if ( ! pv )
      return E_POINTER;

   PROPSHEETHEADER *pHeader = reinterpret_cast<PROPSHEETHEADER *>(pv);

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;// | PSH_RESIZABLE | PSH_AEROWIZARD;
   pHeader -> hInstance = hModule;
   pHeader -> pszIcon = NULL;
   pHeader -> pszCaption = " Properties";
   pHeader -> pfnCallback = NULL;

   return S_OK;
   }


   HRESULT Text::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 3;

   if ( ! showContentPropertyPage )
      *pCount--;

   if ( ! showStylePropertyPage )
      *pCount--;

   if ( ! showOrientationPropertyPage )
      *pCount--;

   return S_OK;
   }


   HRESULT Text::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = (PROPSHEETPAGE *)pPages;

   long index = 0;

   if ( showContentPropertyPage ) {
      pPropSheetPages[index].dwFlags = PSP_USETITLE;
      pPropSheetPages[index].dwSize = sizeof(PROPSHEETPAGE);
      pPropSheetPages[index].hInstance = hModule;
      pPropSheetPages[index].pszTemplate = MAKEINTRESOURCE(IDDIALOG_TEXT_CONTENT);
      pPropSheetPages[index].pfnDlgProc = (DLGPROC)Text::contentHandler;
      pPropSheetPages[index].pszTitle = "Content";
      pPropSheetPages[index].lParam = (LPARAM)this;
      pPropSheetPages[index].pfnCallback = NULL;
      index++;
   }

   if ( showStylePropertyPage ) {
      pPropSheetPages[index].dwFlags = PSP_USETITLE;
      pPropSheetPages[index].dwSize = sizeof(PROPSHEETPAGE);
      pPropSheetPages[index].hInstance = hModule;
      pPropSheetPages[index].pszTemplate = MAKEINTRESOURCE(IDDIALOG_TEXT_STYLE);
      pPropSheetPages[index].pfnDlgProc = (DLGPROC)Text::styleHandler;
      pPropSheetPages[index].pszTitle = "Style";
      pPropSheetPages[index].lParam = (LPARAM)this;
      pPropSheetPages[index].pfnCallback = NULL;
      index++;
   }

   if ( showOrientationPropertyPage ) {
      pPropSheetPages[index].dwFlags = PSP_USETITLE;
      pPropSheetPages[index].dwSize = sizeof(PROPSHEETPAGE);
      pPropSheetPages[index].hInstance = hModule;
      pPropSheetPages[index].pszTemplate = MAKEINTRESOURCE(IDDIALOG_TEXT_ORIENTATION);
      pPropSheetPages[index].pfnDlgProc = (DLGPROC)Text::orientationHandler;
      pPropSheetPages[index].pszTitle = "Orientation";
      pPropSheetPages[index].lParam = (LPARAM)this;
      pPropSheetPages[index].pfnCallback = NULL;
      index++;
   }

   return S_OK;
   }

