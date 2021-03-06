// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Function.h"
#include <olectl.h>

#include "general.h"
#include "Graphic_resource.h"
#include "gmessage.h"

#include "VList.h"

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

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;// | PSH_RESIZABLE | PSH_AEROWIZARD;
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

   //
   //NTC: 12-28-2017: I am taking control visibility settings out of the interface.
   // I don't really recall why there are even there, unless at design time, a developer wants to hide
   // the entire Function specifications dialog from the UI (perhaps to prevent the specification dialog(s)
   // from appearing at all, which would be easy to implement as one property.)
   //
#if 0
   if ( isDesignMode || allowUserPropertiesControls )
      *pCount = 2;
   else
#endif
      *pCount = 1;

   IDataSet *pIDataSet = NULL;

   pIPlot -> get_IDataSet(&pIDataSet);

   if ( pIDataSet ) {

      IGPropertyPageClient *pDataSetProperties = NULL;

      pIDataSet -> QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void **>(&pDataSetProperties));

      long dataSetPropertiesCount = 0L;

      pDataSetProperties -> get_PropertyPageCount(&dataSetPropertiesCount);

      *pCount += dataSetPropertiesCount;

      pDataSetProperties -> Release();

   }

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

   long nextIndex = 1;

#if 0
   if ( isDesignMode || allowUserPropertiesControls ) {
      pPropSheetPages[1].dwFlags = PSP_USETITLE;
      pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
      pPropSheetPages[1].hInstance = hModule;
      pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_FUNCTION_PROPERTIES_VISIBILITY);
      pPropSheetPages[1].pfnDlgProc = (DLGPROC)functionPropertiesVisibilityHandler;
      pPropSheetPages[1].pszTitle = "Visibility";
      pPropSheetPages[1].lParam = (LPARAM)this;
      pPropSheetPages[1].pfnCallback = NULL;
      nextIndex = 2;
   }
#endif

   IGPropertyPageClient *pDataSetProperties = NULL;

   pIDataSet -> QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void **>(&pDataSetProperties));

   long dataSetPropertiesCount = 0L;

   pIDataSet -> put_IsFunctionSource(VARIANT_TRUE);

   BSTR bstrExpression = NULL;
   get_Expression(&bstrExpression);
   pIDataSet -> put_DataSource(bstrExpression);
   SysFreeString(bstrExpression);

   IGSFunctioNater *pFunction = NULL;
   QueryInterface(IID_IGSFunctioNater,reinterpret_cast<void **>(&pFunction));
   pIDataSet -> put_IFunction(reinterpret_cast<void *>(pFunction));
   pFunction -> Release();

   pDataSetProperties -> get_PropertyPageCount(&dataSetPropertiesCount);

   pDataSetProperties -> GetPropertySheets((void *)(pPropSheetPages + nextIndex));

   static char szNewName3[8][128];

   for ( long k = 0; k < dataSetPropertiesCount; k++ ) {
      PROPSHEETPAGE *pPage = pPropSheetPages + nextIndex + k;
      if ( strstr((char *)pPage -> pszTitle,"Plot-") ) 
         continue;
      sprintf(szNewName3[k],"DataSet-%s",pPage -> pszTitle);
      pPage -> pszTitle = szNewName3[k];
   }

   pDataSetProperties -> Release();

   return S_OK;
   }