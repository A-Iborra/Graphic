/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "utils.h"

#include "ViewSet.h"


   HRESULT ViewSet::BeforeAllPropertyPages() {
   return E_NOTIMPL;
   }


   HRESULT ViewSet::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {
   return E_NOTIMPL;
   }


   HRESULT ViewSet::CreatePropertyPage(long pageNumber,HWND hwndPP,RECT* pRect,BOOL fModal,HWND* pHwnd) {
   return E_NOTIMPL;
   }

   HRESULT ViewSet::Apply() {
   return E_NOTIMPL;
   }


   HRESULT ViewSet::IsPageDirty(long hwndPage,BOOL* isDirty) {
   return E_NOTIMPL;
   }


   HRESULT ViewSet::Help(BSTR bstrHelpDir) {
   return E_NOTIMPL;
   }


   HRESULT ViewSet::TranslateAccelerator(long,long* pResult) {
   return E_NOTIMPL;
   }


   HRESULT ViewSet::AfterAllPropertyPages(BOOL userCanceled) {
   return E_NOTIMPL;
   }



   HRESULT ViewSet::DestroyPropertyPage(long index) {
   return E_NOTIMPL;
   }


   HRESULT ViewSet::GetPropertySheetHeader(void *pv) {
   return E_NOTIMPL;
#if 0
   if ( ! pv )
      return E_POINTER;

   PROPSHEETHEADER *pHeader = reinterpret_cast<PROPSHEETHEADER *>(pv);

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;
   pHeader -> hInstance = hModule;
   pHeader -> pszIcon = NULL;
   pHeader -> pszCaption = " Properties";
   pHeader -> pfnCallback = NULL;

   return S_OK;
#endif
   }


   HRESULT ViewSet::get_PropertyPageCount(long *pCount) {
   return E_NOTIMPL;
#if 0
   if ( ! pCount )
      return E_POINTER;
   *pCount = 1;
   return S_OK;
#endif
   }


   HRESULT ViewSet::GetPropertySheets(void *pPages) {
   return E_NOTIMPL;
#if 0
   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDD_PAD_PROPERTIES);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)SignaturePad::settingsHandler;
   pPropSheetPages[0].pszTitle = pParent -> signatureDeviceProductName;
   pPropSheetPages[0].lParam = (long)pParent;
   pPropSheetPages[0].pfnCallback = NULL;

   return S_OK;
#endif
   }

