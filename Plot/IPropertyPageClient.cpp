/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "utils.h"
#include "Graphic_resource.h"

#include "Plot.h"

   extern HMODULE hModule;


   HRESULT Plot::BeforeAllPropertyPages() {
   return E_NOTIMPL;
   }


   HRESULT Plot::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {
   return E_NOTIMPL;
   }


   HRESULT Plot::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {
   return E_NOTIMPL;
   }


   HRESULT Plot::Apply() {
   return E_NOTIMPL;
   }


   HRESULT Plot::IsPageDirty(long hwndPage,BOOL* isDirty) {
   return E_NOTIMPL;
   }


   HRESULT Plot::Help(BSTR bstrHelpDir) {
   return  E_NOTIMPL;
   }


   HRESULT Plot::TranslateAccelerator(long,long* pResult) {
   return E_NOTIMPL;
   }


   HRESULT Plot::AfterAllPropertyPages(BOOL userCanceled) {
   return E_NOTIMPL;
   }



   HRESULT Plot::DestroyPropertyPage(long pageNumber) {
   return E_NOTIMPL;
   }


   HRESULT Plot::GetPropertySheetHeader(void *pv) {

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


   HRESULT Plot::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 3;
   return S_OK;
   }


   HRESULT Plot::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_PLOT_DIMENSION);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)dimensionHandler;
   pPropSheetPages[0].pszTitle = "View";
   pPropSheetPages[0].lParam = (LPARAM)this;
   pPropSheetPages[0].pfnCallback = NULL;

   pPropSheetPages[1].dwFlags = PSP_USETITLE;
   pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[1].hInstance = hModule;
   pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_PLOT_TYPE);
   pPropSheetPages[1].pfnDlgProc = (DLGPROC)typeHandler;
   pPropSheetPages[1].pszTitle = "Type";
   pPropSheetPages[1].lParam = (LPARAM)this;
   pPropSheetPages[1].pfnCallback = NULL;

   pPropSheetPages[2].dwFlags = PSP_USETITLE;
   pPropSheetPages[2].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[2].hInstance = hModule;
   pPropSheetPages[2].pszTemplate = MAKEINTRESOURCE(IDDIALOG_PLOT_COLOR);
   pPropSheetPages[2].pfnDlgProc = (DLGPROC)colorHandler;
   pPropSheetPages[2].pszTitle = "Color";
   pPropSheetPages[2].lParam = (LPARAM)this;
   pPropSheetPages[2].pfnCallback = NULL;

   return S_OK;
   }

