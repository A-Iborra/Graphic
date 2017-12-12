/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "utils.h"
#include "Graphic_resource.h"
#include "Axis.h"


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

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;
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

   pPropSheetPages[3].dwFlags = PSP_USETITLE;
   pPropSheetPages[3].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[3].hInstance = hModule;
   pPropSheetPages[3].pszTemplate = MAKEINTRESOURCE(IDDIALOG_AXIS_COLOR);
   pPropSheetPages[3].pfnDlgProc = (DLGPROC)colorHandler;
   pPropSheetPages[3].pszTitle = "Color";
   pPropSheetPages[3].lParam = (LPARAM)this;
   pPropSheetPages[3].pfnCallback = NULL;

   return S_OK;
   }

