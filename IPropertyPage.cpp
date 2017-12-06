// Copyright 2017 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include "Graphic_resource.h"

   static bool okayToSave = false;

   long __stdcall G::_IPropertyPage::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL;
   if ( riid == IID_IPropertyPage )
      *ppv = static_cast<IPropertyPage *>(this);
   else
      return pParent -> QueryInterface(riid,ppv);
   AddRef(); 
   return S_OK; 
   }
   unsigned long __stdcall G::_IPropertyPage::AddRef() {
   return pParent -> AddRef();
   }
   unsigned long __stdcall G::_IPropertyPage::Release() {
   return pParent -> Release();
   }
 
 
   HRESULT G::_IPropertyPage::SetPageSite(IPropertyPageSite *pPageSite) {
   return S_OK;
   }

   HRESULT G::_IPropertyPage::Activate(HWND hwndParent,const RECT *pRect,BOOL doModal) {

   HWND hwndCreated = NULL;

   if ( CLSID_GSystemGraphicPropertiesPosSize == theCLSID ) {
      if ( ! pParent -> hwndAppearanceSettings )
         pParent -> pIPropertyPageClient -> CreatePropertyPage(0,hwndParent,const_cast<RECT *>(pRect),doModal,&hwndCreated);
      SetWindowPos(pParent -> hwndAppearanceSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesStyle == theCLSID ) {
      if ( ! pParent -> hwndStyleSettings )
         pParent -> pIPropertyPageClient -> CreatePropertyPage(1,hwndParent,const_cast<RECT *>(pRect),doModal,&hwndCreated);
      SetWindowPos(pParent -> hwndStyleSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesBackground == theCLSID ) {
      if ( ! pParent -> hwndBackgroundSettings ) 
         pParent -> pIPropertyPageClient -> CreatePropertyPage(2,hwndParent,const_cast<RECT *>(pRect),doModal,&hwndCreated);
      SetWindowPos(pParent -> hwndBackgroundSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesText == theCLSID ) {
      if ( ! pParent -> hwndTextSettings )
         pParent -> pIPropertyPageClient -> CreatePropertyPage(3,hwndParent,const_cast<RECT *>(pRect),doModal,&hwndCreated);
      SetWindowPos(pParent -> hwndTextSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesLighting == theCLSID ) {
      if ( ! pParent -> hwndLightingSettings )
         pParent -> pIPropertyPageClient -> CreatePropertyPage(4,hwndParent,const_cast<RECT *>(pRect),doModal,&hwndCreated);
      SetWindowPos(pParent -> hwndLightingSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesAxis == theCLSID ) {
      if ( ! pParent -> hwndAxisSettings )
         pParent -> pIPropertyPageClient -> CreatePropertyPage(5,hwndParent,const_cast<RECT *>(pRect),doModal,&hwndCreated);
      SetWindowPos(pParent -> hwndAxisSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesPlot == theCLSID ) {
      if ( ! pParent -> hwndPlotSettings )
         pParent -> pIPropertyPageClient -> CreatePropertyPage(6,hwndParent,const_cast<RECT *>(pRect),doModal,&hwndCreated);
      SetWindowPos(pParent -> hwndPlotSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesFunctions == theCLSID ) {
      if ( ! pParent -> hwndFunctionSettings )
         pParent -> pIPropertyPageClient -> CreatePropertyPage(7,hwndParent,const_cast<RECT *>(pRect),doModal,&hwndCreated);
      SetWindowPos(pParent -> hwndFunctionSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   return S_OK;
   }


   HRESULT G::_IPropertyPage::Deactivate() {

   if ( CLSID_GSystemGraphicPropertiesPosSize == theCLSID ) {
      if ( pParent -> hwndAppearanceSettings )
         DestroyWindow(pParent -> hwndAppearanceSettings);
      pParent -> hwndAppearanceSettings = NULL;
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesStyle == theCLSID ) {
      if ( pParent -> hwndStyleSettings )
         DestroyWindow(pParent -> hwndStyleSettings);
      pParent -> hwndStyleSettings = NULL;
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesBackground == theCLSID ) {
      if ( pParent -> hwndBackgroundSettings )
         DestroyWindow(pParent -> hwndBackgroundSettings);
      pParent -> hwndBackgroundSettings = NULL;
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesText == theCLSID ) {
      if ( pParent -> hwndTextSettings )
         DestroyWindow(pParent -> hwndTextSettings);
      pParent -> hwndTextSettings = NULL;
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesLighting == theCLSID ) {
      if ( pParent -> hwndLightingSettings )
         DestroyWindow(pParent -> hwndLightingSettings);
      pParent -> hwndLightingSettings = NULL;
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesAxis == theCLSID ) {
      if ( pParent -> hwndAxisSettings )
         DestroyWindow(pParent -> hwndAxisSettings);
      pParent -> hwndAxisSettings = NULL;
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesPlot == theCLSID ) {
      if ( pParent -> hwndPlotSettings )
         DestroyWindow(pParent -> hwndPlotSettings);
      pParent -> hwndPlotSettings = NULL;
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesFunctions == theCLSID ) {
      if ( pParent -> hwndFunctionSettings )
         DestroyWindow(pParent -> hwndFunctionSettings);
      pParent -> hwndFunctionSettings = NULL;
      return S_OK;
   }

   return S_OK;
   }



   HRESULT G::_IPropertyPage::GetPageInfo(PROPPAGEINFO *pPageInfo) {

   memset(pPageInfo,0,sizeof(PROPPAGEINFO));

   pPageInfo -> cb = sizeof(PROPPAGEINFO);

   pPageInfo -> pszTitle = (BSTR)CoTaskMemAlloc(128);

   pPageInfo -> pszDocString = NULL;

   memset(pPageInfo -> pszTitle,0,128);

   pPageInfo -> size.cx = 512;
   pPageInfo -> size.cy = 400;

   if ( CLSID_GSystemGraphicPropertiesPosSize == theCLSID ) {
      MultiByteToWideChar(CP_ACP,0,"Position and Size",-1,(BSTR)pPageInfo -> pszTitle,128);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesStyle == theCLSID ) {
      MultiByteToWideChar(CP_ACP,0,"Style",-1,(BSTR)pPageInfo -> pszTitle,128);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesBackground == theCLSID ) {
      MultiByteToWideChar(CP_ACP,0,"Background",-1,(BSTR)pPageInfo -> pszTitle,128);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesText == theCLSID ) {
      MultiByteToWideChar(CP_ACP,0,"Text",-1,(BSTR)pPageInfo -> pszTitle,128);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesLighting == theCLSID ) {
      MultiByteToWideChar(CP_ACP,0,"Lighting",-1,(BSTR)pPageInfo -> pszTitle,128);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesAxis == theCLSID ) {
      MultiByteToWideChar(CP_ACP,0,"Axis",-1,(BSTR)pPageInfo -> pszTitle,128);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesPlot == theCLSID ) {
      MultiByteToWideChar(CP_ACP,0,"Plot",-1,(BSTR)pPageInfo -> pszTitle,128);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesFunctions == theCLSID ) {
      MultiByteToWideChar(CP_ACP,0,"Functions",-1,(BSTR)pPageInfo -> pszTitle,128);
      return S_OK;
   }

   return S_OK;
   }


   HRESULT G::_IPropertyPage::SetObjects(ULONG cObjects,IUnknown** pUnk) {
   return S_OK;
   }


   HRESULT G::_IPropertyPage::Show(UINT cmdShow) {

   if ( CLSID_GSystemGraphicPropertiesPosSize == theCLSID ) {
      ShowWindow(pParent -> hwndAppearanceSettings,cmdShow);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesStyle == theCLSID ) {
      ShowWindow(pParent -> hwndStyleSettings,cmdShow);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesBackground == theCLSID ) {
      ShowWindow(pParent -> hwndBackgroundSettings,cmdShow);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesText == theCLSID ) {
      ShowWindow(pParent -> hwndTextSettings,cmdShow);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesLighting == theCLSID ) {
      ShowWindow(pParent -> hwndLightingSettings,cmdShow);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesAxis == theCLSID ) {
      ShowWindow(pParent -> hwndAxisSettings,cmdShow);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesPlot == theCLSID ) {
      ShowWindow(pParent -> hwndPlotSettings,cmdShow);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesFunctions == theCLSID ) {
      ShowWindow(pParent -> hwndFunctionSettings,cmdShow);
      return S_OK;
   }

   return S_OK;
   }


   HRESULT G::_IPropertyPage::Move(const RECT *prc) {

#if 0
   if ( CLSID_PhabletSignaturePadPropertyPage == theCLSID )
      SetWindowPos(pParent -> hwndPropertyPage,HWND_TOP,prc -> left,prc -> top, prc -> right - prc -> left,prc -> bottom - prc -> top,0L);

   if ( CLSID_PhabletSignaturePadRuntimePropertyPage == theCLSID )
      SetWindowPos(pParent -> hwndRuntimePropertyPage,HWND_TOP,prc -> left,prc -> top, prc -> right - prc -> left,prc -> bottom - prc -> top,0L);

   if ( CLSID_PhabletSignaturePadDocumentationPropertyPage == theCLSID )
      SetWindowPos(pParent -> hwndDocumentationPropertyPage,HWND_TOP,prc -> left,prc -> top, prc -> right - prc -> left,prc -> bottom - prc -> top,0L);
#endif

   return S_OK;
   }


   HRESULT G::_IPropertyPage::IsPageDirty() {
   return S_OK;
   }


   HRESULT G::_IPropertyPage::Apply() {

#if 0
   pParent -> fire_PropertyChanged();

   if ( ! okayToSave )
      return S_OK;

   okayToSave = false;

   PSHNOTIFY notify = {0};

   notify.hdr.code = PSN_APPLY;
   notify.lParam = 1L;

//   if ( CLSID_PhabletSignaturePadPropertyPage == theCLSID ) 
      SendMessage(pParent -> hwndPropertyPage,WM_NOTIFY,0L,(LPARAM)&notify);

//   if ( CLSID_PhabletSignaturePadRuntimePropertyPage == theCLSID ) 
      SendMessage(pParent -> hwndRuntimePropertyPage,WM_NOTIFY,0L,(LPARAM)&notify);

//   if ( CLSID_PhabletSignaturePadDocumentationPropertyPage == theCLSID ) 
      SendMessage(pParent -> hwndDocumentationPropertyPage,WM_NOTIFY,0L,(LPARAM)&notify);
#endif

   return S_OK;
   }


   HRESULT G::_IPropertyPage::Help(LPCOLESTR pszHelpDir) {
   return E_FAIL;
   }


   HRESULT G::_IPropertyPage::TranslateAccelerator(MSG* pMsg) {
   return S_FALSE;
   }

#if 0

//
//NTC: 02-08-2012. This is a total bullshit hack that is necessary because the MS documentation
// cannot bother to say anywhere how on earth to tell if the user pressed OK, or Cancel - as if
// nobody would ever want to know this absolutely obvious thing to need to know !?!
//
   LRESULT CALLBACK G::propertySheetFrameHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   if ( msg == WM_COMMAND ) {
      switch ( LOWORD(wParam ) ) {
      case IDOK:
         okayToSave = true;
         break;
      case IDCANCEL:
         okayToSave = false;
         break;
      }
   }
   return CallWindowProc(nativePropertySheetFrameHandler,hwnd,msg,wParam,lParam);
   }
   
#endif
