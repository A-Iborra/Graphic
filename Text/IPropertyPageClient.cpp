/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Text.h"

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

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;
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
   return S_OK;
   }


   HRESULT Text::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = (PROPSHEETPAGE *)pPages;

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_TEXT_CONTENT);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)Text::contentHandler;
   pPropSheetPages[0].pszTitle = "Content";
   pPropSheetPages[0].lParam = (LPARAM)this;
   pPropSheetPages[0].pfnCallback = NULL;

   pPropSheetPages[1].dwFlags = PSP_USETITLE;
   pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[1].hInstance = hModule;
   pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_TEXT_STYLE);
   pPropSheetPages[1].pfnDlgProc = (DLGPROC)Text::styleHandler;
   pPropSheetPages[1].pszTitle = "Style";
   pPropSheetPages[1].lParam = (LPARAM)this;
   pPropSheetPages[1].pfnCallback = NULL;

   pPropSheetPages[2].dwFlags = PSP_USETITLE;
   pPropSheetPages[2].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[2].hInstance = hModule;
   pPropSheetPages[2].pszTemplate = MAKEINTRESOURCE(IDDIALOG_TEXT_ORIENTATION);
   pPropSheetPages[2].pfnDlgProc = (DLGPROC)Text::orientationHandler;
   pPropSheetPages[2].pszTitle = "Orientation";
   pPropSheetPages[2].lParam = (LPARAM)this;
   pPropSheetPages[2].pfnCallback = NULL;

   return S_OK;
   }

