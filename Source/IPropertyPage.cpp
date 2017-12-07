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

   PROPSHEETPAGE propSheetPage = {0};

   propSheetPage.lParam = (LPARAM)pParent;

   if ( CLSID_GSystemGraphicPropertiesPosSize == theCLSID ) {
      if ( ! pParent -> hwndAppearanceSettings )
         pParent -> hwndAppearanceSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_SIZEPOS),(HWND)hwndParent,(DLGPROC)G::posSizeHandler,(LPARAM)&propSheetPage);
      SetWindowPos(pParent -> hwndAppearanceSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesStyle == theCLSID ) {
      if ( ! pParent -> hwndStyleSettings )
         pParent -> hwndStyleSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_STYLE),(HWND)hwndParent,(DLGPROC)G::styleHandler,(LPARAM)&propSheetPage);
      SetWindowPos(pParent -> hwndStyleSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesBackground == theCLSID ) {
      if ( ! pParent -> hwndBackgroundSettings ) 
         pParent -> hwndBackgroundSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_BACKGROUND),(HWND)hwndParent,(DLGPROC)G::backgroundHandler,(LPARAM)&propSheetPage); 
      SetWindowPos(pParent -> hwndBackgroundSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesText == theCLSID ) {
      if ( ! pParent -> hwndTextSettings )
         pParent -> hwndTextSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_TEXT_SETTINGS),(HWND)hwndParent,(DLGPROC)G::textHandler,(LPARAM)&propSheetPage);
      SetWindowPos(pParent -> hwndTextSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesLighting == theCLSID ) {
      if ( ! pParent -> hwndLightingSettings )
         pParent -> hwndLightingSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_LIGHTING),(HWND)hwndParent,(DLGPROC)G::lightingHandler,(LPARAM)&propSheetPage);
      SetWindowPos(pParent -> hwndLightingSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesAxis == theCLSID ) {
      if ( ! pParent -> hwndAxisSettings )
         pParent -> hwndAxisSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_AXIIS),(HWND)hwndParent,(DLGPROC)G::axisHandler,(LPARAM)&propSheetPage);
      SetWindowPos(pParent -> hwndAxisSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesPlot == theCLSID ) {
      if ( ! pParent -> hwndPlotSettings )
         pParent -> hwndPlotSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_PLOTS),(HWND)hwndParent,(DLGPROC)G::plotHandler,(LPARAM)&propSheetPage);
      SetWindowPos(pParent -> hwndPlotSettings,HWND_TOP,pRect -> left,pRect -> top,pRect -> right - pRect -> left,pRect -> bottom - pRect -> top,SWP_SHOWWINDOW);
      return S_OK;
   }

   if ( CLSID_GSystemGraphicPropertiesFunctions == theCLSID ) {
      if ( ! pParent -> hwndFunctionSettings )
         pParent -> hwndFunctionSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_FUNCTIONS),(HWND)hwndParent,(DLGPROC)G::functionHandler,(LPARAM)&propSheetPage);
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
   return S_OK;
   }


   HRESULT G::_IPropertyPage::IsPageDirty() {
   return S_OK;
   }


   HRESULT G::_IPropertyPage::Apply() {
   return S_OK;
   }


   HRESULT G::_IPropertyPage::Help(LPCOLESTR pszHelpDir) {
   return E_FAIL;
   }


   HRESULT G::_IPropertyPage::TranslateAccelerator(MSG* pMsg) {
   return S_FALSE;
   }
