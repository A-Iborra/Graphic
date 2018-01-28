// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include "Graphic_resource.h"

   long __stdcall G::_IGPropertyPageClient::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
 
   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown*>(this); 
   else

   if ( riid == IID_IDispatch )
      *ppv = this;

   if ( riid == IID_IGPropertyPageClient )
      *ppv = static_cast<IGPropertyPageClient*>(this);
   else
 
      return pParent -> QueryInterface(riid,ppv);
 
   static_cast<IUnknown*>(*ppv) -> AddRef();
  
   return S_OK; 
   }
 
   unsigned long __stdcall G::_IGPropertyPageClient::AddRef() {
   return 1;
   }
 
   unsigned long __stdcall G::_IGPropertyPageClient::Release() {
   return 1;
   }

   HRESULT G::_IGPropertyPageClient::BeforeAllPropertyPages() {
   return E_NOTIMPL;
   }


   HRESULT G::_IGPropertyPageClient::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {
   return E_NOTIMPL;
   }


   HRESULT G::_IGPropertyPageClient::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {
   return E_NOTIMPL;
   }


   HRESULT G::_IGPropertyPageClient::Apply() {
   return E_NOTIMPL;
   }


   HRESULT G::_IGPropertyPageClient::IsPageDirty(long pageNumber,BOOL* isDirty) {
   return E_NOTIMPL;
   }


   HRESULT G::_IGPropertyPageClient::Help(BSTR bstrHelpDir) {
   return  S_OK;
   }


   HRESULT G::_IGPropertyPageClient::TranslateAccelerator(long,long* pResult) {
   *pResult = S_FALSE;
   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::AfterAllPropertyPages(BOOL userCanceled) {
   return E_NOTIMPL;
   }


   HRESULT G::_IGPropertyPageClient::DestroyPropertyPage(long index) {
   return E_NOTIMPL;
   }


   HRESULT G::_IGPropertyPageClient::GetPropertySheetHeader(void *pv) {
   
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


   HRESULT G::_IGPropertyPageClient::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 9;
   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_SIZEPOS);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)G::posSizeHandler;
   pPropSheetPages[0].pszTitle = "Size and position";
   pPropSheetPages[0].lParam = (LPARAM)pParent;
   pPropSheetPages[0].pfnCallback = NULL;

   pPropSheetPages[1].dwFlags = PSP_USETITLE;
   pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[1].hInstance = hModule;
   pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_STYLE);
   pPropSheetPages[1].pfnDlgProc = (DLGPROC)G::styleHandler;
   pPropSheetPages[1].pszTitle = "View";
   pPropSheetPages[1].lParam = (LPARAM)pParent;
   pPropSheetPages[1].pfnCallback = NULL;

   pPropSheetPages[2].dwFlags = PSP_USETITLE;
   pPropSheetPages[2].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[2].hInstance = hModule;
   pPropSheetPages[2].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_BACKGROUND);
   pPropSheetPages[2].pfnDlgProc = (DLGPROC)G::backgroundHandler;
   pPropSheetPages[2].pszTitle = "Background";
   pPropSheetPages[2].lParam = (LPARAM)pParent;
   pPropSheetPages[2].pfnCallback = NULL;

   pPropSheetPages[3].dwFlags = PSP_USETITLE;
   pPropSheetPages[3].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[3].hInstance = hModule;
   pPropSheetPages[3].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_LIGHTING);
   pPropSheetPages[3].pfnDlgProc = (DLGPROC)G::lightingHandler;
   pPropSheetPages[3].pszTitle = "Lighting";
   pPropSheetPages[3].lParam = (LPARAM)pParent;
   pPropSheetPages[3].pfnCallback = NULL;

   pPropSheetPages[4].dwFlags = PSP_USETITLE;
   pPropSheetPages[4].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[4].hInstance = hModule;
   pPropSheetPages[4].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_AXIIS);
   pPropSheetPages[4].pfnDlgProc = (DLGPROC)G::axisHandler;
   pPropSheetPages[4].pszTitle = "Axiis";
   pPropSheetPages[4].lParam = (LPARAM)pParent;
   pPropSheetPages[4].pfnCallback = NULL;

   pPropSheetPages[5].dwFlags = PSP_USETITLE;
   pPropSheetPages[5].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[5].hInstance = hModule;
   pPropSheetPages[5].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_TEXT_SETTINGS);
   pPropSheetPages[5].pfnDlgProc = (DLGPROC)G::textHandler;
   pPropSheetPages[5].pszTitle = "Text";
   pPropSheetPages[5].lParam = (LPARAM)pParent;
   pPropSheetPages[5].pfnCallback = NULL;

   pPropSheetPages[6].dwFlags = PSP_USETITLE;
   pPropSheetPages[6].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[6].hInstance = hModule;
   pPropSheetPages[6].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_PLOTS);
   pPropSheetPages[6].pfnDlgProc = (DLGPROC)G::plotHandler;
   pPropSheetPages[6].pszTitle = "Plots";
   pPropSheetPages[6].lParam = (LPARAM)pParent;
   pPropSheetPages[6].pfnCallback = NULL;

   pPropSheetPages[7].dwFlags = PSP_USETITLE;
   pPropSheetPages[7].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[7].hInstance = hModule;
   pPropSheetPages[7].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_DATASETS);
   pPropSheetPages[7].pfnDlgProc = (DLGPROC)G::dataSetHandler;
   pPropSheetPages[7].pszTitle = "DataSets";
   pPropSheetPages[7].lParam = (LPARAM)pParent;
   pPropSheetPages[7].pfnCallback = NULL;

   pPropSheetPages[8].dwFlags = PSP_USETITLE;
   pPropSheetPages[8].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[8].hInstance = hModule;
   pPropSheetPages[8].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_FUNCTIONS);
   pPropSheetPages[8].pfnDlgProc = (DLGPROC)G::functionHandler;
   pPropSheetPages[8].pszTitle = "Functions";
   pPropSheetPages[8].lParam = (LPARAM)pParent;
   pPropSheetPages[8].pfnCallback = NULL;

   hwndSampleGraphic = CreateWindowEx(WS_EX_CLIENTEDGE,"G-plotSettingsGraphic","",WS_CHILD,0,0,0,0,pParent -> Canvas(),NULL,hModule,(void *)pParent);

   return S_OK;
   }

   long __stdcall G::_IGPropertyPageClient::GetTypeInfoCount(UINT *i) { 
   *i = 0; 
   return S_OK; 
   } 
  
 
   long __stdcall G::_IGPropertyPageClient::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   return S_OK; 
   } 
  
 
   long __stdcall G::_IGPropertyPageClient::GetIDsOfNames(REFIID riid,OLECHAR **rgszNames,UINT cNames,LCID lcid,DISPID *rgdispid) { 
   return E_NOTIMPL;
   } 
 
       
   long __stdcall G::_IGPropertyPageClient::Invoke(DISPID dispidMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pdispparams,VARIANT *pvarResult,EXCEPINFO *pexcepinfo,UINT *puArgErr) { 
   return E_NOTIMPL;
   } 
