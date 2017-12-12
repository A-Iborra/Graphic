/*

                       Copyright (c) 1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>
#include <CommCtrl.h>
#include <olectl.h>

#include "general.h"
#include "Graphic_resource.h"
#include "gmessage.h"

#include "VList.h"

#include "Function.h"

   HRESULT Function::BeforeAllPropertyPages() {
   return E_NOTIMPL;
   }


   HRESULT Function::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {
   return E_NOTIMPL;
   }


   HRESULT Function::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {
   return E_NOTIMPL;
   }


   HRESULT Function::Apply() {
   return E_NOTIMPL;
   }


   HRESULT Function::IsPageDirty(long pageNumber,BOOL* isDirty) {
   return E_NOTIMPL;
   }


   HRESULT Function::Help(BSTR bstrHelpDir) {
   return  E_NOTIMPL;
   }


   HRESULT Function::TranslateAccelerator(long,long* pResult) {
   return E_NOTIMPL;
   }


   HRESULT Function::AfterAllPropertyPages(BOOL userCanceled) {
   return E_NOTIMPL;
   }


   HRESULT Function::DestroyPropertyPage(long index) {
   return E_NOTIMPL;
   }

   HRESULT Function::GetPropertySheetHeader(void *pv) {
   
   if ( ! pv )
      return E_POINTER;

   PROPSHEETHEADER *pHeader = reinterpret_cast<PROPSHEETHEADER *>(pv);

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;
   pHeader -> hInstance = hModule;
   pHeader -> pszIcon = NULL;
   pHeader -> pszCaption = " Function Properties";
   pHeader -> pfnCallback = NULL;

   return S_OK;
   }

   HRESULT Function::get_PropertyPageCount(long *pCount) {

   if ( ! pCount )
      return E_POINTER;

   IDispatch* pIDispatch = (IDispatch *)NULL;

   isDesignMode = true;

   if ( pIOleClientSite && SUCCEEDED(pIOleClientSite -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(&pIDispatch))) ) {

      VARIANT varUserMode;

      DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};

      varUserMode.vt = VT_BOOL;

      VariantClear(&varUserMode);

      pIDispatch -> Invoke(DISPID_AMBIENT_USERMODE,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varUserMode,NULL,NULL);

      pIDispatch -> Release();

      if ( varUserMode.bVal ) 
         isDesignMode = 0;

   }

   if ( isDesignMode || allowUserPropertiesControls )
      *pCount = 2;
   else
      *pCount = 1;

   return S_OK;
   }

   HRESULT Function::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_FUNCTION_PROPERTIES);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)functionPropertiesHandler;
   pPropSheetPages[0].pszTitle = "Function";
   pPropSheetPages[0].lParam = (LPARAM)this;
   pPropSheetPages[0].pfnCallback = NULL;

   if ( ! isDesignMode && ! allowUserPropertiesControls )
      return S_OK;

   pPropSheetPages[1].dwFlags = PSP_USETITLE;
   pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[1].hInstance = hModule;
   pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_FUNCTION_PROPERTIES_VISIBILITY);
   pPropSheetPages[1].pfnDlgProc = (DLGPROC)functionPropertiesVisibilityHandler;
   pPropSheetPages[1].pszTitle = "Visibility";
   pPropSheetPages[1].lParam = (LPARAM)this;
   pPropSheetPages[1].pfnCallback = NULL;

   return S_OK;
   }