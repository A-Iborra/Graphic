/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>

#include "utils.h"
#include "Graphic.h"
#include "Graphic_resource.h"

#include "List.cpp"


   HRESULT G::_IGPropertyPageClient::BeforeAllPropertyPages() {

   pParent -> pIGProperties -> Push();
   pParent -> pIGProperties -> Push();

   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {

   if ( ! pCntPages ) return E_POINTER;
   if ( ! thePageNames ) return E_POINTER;
   if ( ! theHelpDirs ) return E_POINTER;
   if ( ! pSize ) return E_POINTER;

   *pCntPages = 8;

   SAFEARRAYBOUND rgsabound[1];
   long index = 0;

   rgsabound[0].cElements = *pCntPages;
   rgsabound[0].lLbound = 1;
   SafeArrayRedim(*thePageNames,rgsabound);
   SafeArrayRedim(*theHelpDirs,rgsabound);
   rgsabound[0].cElements = 2 * *pCntPages;
   SafeArrayRedim(*pSize,rgsabound);

   index = 0;
   index++; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Pos-Size"));
   index++; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Style"));
   index++; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Background"));
   index++; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Text"));
   index++; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Lighting"));
   index++; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Axis"));
   index++; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Plot"));
   index++; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Functions"));

   index = 0;
   index++; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index++; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index++; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index++; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index++; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index++; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index++; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index++; SafeArrayPutElement(*theHelpDirs,&index,NULL);

   index = 0;
   long sizeW = 350,sizeH = 300;
   index++; SafeArrayPutElement(*pSize,&index,&sizeW);
   index++; SafeArrayPutElement(*pSize,&index,&sizeH);
   index++; SafeArrayPutElement(*pSize,&index,&sizeW);
   index++; SafeArrayPutElement(*pSize,&index,&sizeH);
   index++; SafeArrayPutElement(*pSize,&index,&sizeW);
   index++; SafeArrayPutElement(*pSize,&index,&sizeH);
   index++; SafeArrayPutElement(*pSize,&index,&sizeW);
   index++; SafeArrayPutElement(*pSize,&index,&sizeH);
   index++; SafeArrayPutElement(*pSize,&index,&sizeW);
   index++; SafeArrayPutElement(*pSize,&index,&sizeH);
   index++; SafeArrayPutElement(*pSize,&index,&sizeW);
   index++; SafeArrayPutElement(*pSize,&index,&sizeH);
   index++; SafeArrayPutElement(*pSize,&index,&sizeW);
   index++; SafeArrayPutElement(*pSize,&index,&sizeH);
   index++; SafeArrayPutElement(*pSize,&index,&sizeW);
   index++; SafeArrayPutElement(*pSize,&index,&sizeH);

   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {

   if ( ! pHwnd ) return E_POINTER;

   if ( ! hwndParent ) return E_POINTER;

   PROPSHEETPAGE propSheetPage = {0};

   propSheetPage.lParam = (LPARAM)pParent;

   switch ( pageNumber ) {
   case 0: {

      pParent -> hwndAppearanceSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_SIZEPOS),(HWND)hwndParent,(DLGPROC)G::posSizeHandler,(LPARAM)&propSheetPage);

      *pHwnd = pParent -> hwndAppearanceSettings;

      }
      break;

   case 1: {

      pParent -> hwndStyleSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_STYLE),(HWND)hwndParent,(DLGPROC)G::styleHandler,(LPARAM)&propSheetPage);

      *pHwnd = pParent -> hwndStyleSettings;

      }
      break;

   case 2: {

      pParent -> hwndBackgroundSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_BACKGROUND),(HWND)hwndParent,(DLGPROC)G::backgroundHandler,(LPARAM)&propSheetPage); 

      *pHwnd = pParent -> hwndBackgroundSettings;

      }
      break;

   case 3: {

      pParent -> hwndTextSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_TEXT_SETTINGS),(HWND)hwndParent,(DLGPROC)G::textHandler,(LPARAM)&propSheetPage);

      *pHwnd = pParent -> hwndTextSettings;

      }
      break;

   case 4: {

      pParent -> hwndLightingSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_LIGHTING),(HWND)hwndParent,(DLGPROC)lightingHandler,(LPARAM)&propSheetPage);
 
      *pHwnd = pParent -> hwndLightingSettings;

      }
      break;

   case 5: {

      pParent -> hwndAxisSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_AXIIS),(HWND)hwndParent,(DLGPROC)G::axisHandler,(LPARAM)&propSheetPage);

      *pHwnd = pParent -> hwndAxisSettings;

      }
      break;

   case 6: {

      pParent -> hwndPlotSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_PLOTS),(HWND)hwndParent,(DLGPROC)G::plotHandler,(LPARAM)&propSheetPage);
 
      *pHwnd = pParent -> hwndPlotSettings;

      }
      break;

   case 7: {

      pParent -> hwndFunctionSettings = CreateDialogParam(hModule,MAKEINTRESOURCE(IDDIALOG_GRAPHIC_FUNCTIONS),(HWND)hwndParent,(DLGPROC)G::functionHandler,(LPARAM)&propSheetPage);

      *pHwnd = pParent -> hwndFunctionSettings;

      }
      break;

   }

   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::Apply() {

   pParent -> pIGProperties -> Discard();
   pParent -> pIGProperties -> Push();

   RECT rect;  
   GetWindowRect(pParent -> hwndFrame,&rect);
   SendMessage(pParent -> hwndFrame,WM_SIZE,(WPARAM)SIZE_RESTORED,MAKELPARAM(rect.right - rect.left,rect.bottom - rect.top));

   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::IsPageDirty(long pageNumber,BOOL* isDirty) {
   pParent -> pIGProperties -> Compare((VARIANT_BOOL*)isDirty);
   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::Help(BSTR bstrHelpDir) {
   return  S_OK;
   }


   HRESULT G::_IGPropertyPageClient::TranslateAccelerator(long,long* pResult) {
   *pResult = S_FALSE;
   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::AfterAllPropertyPages(BOOL userCanceled) {

   if ( userCanceled ) {
      pParent -> pIGProperties -> Pop();
      pParent -> pIGProperties -> Pop();
   } else {
      pParent -> pIGProperties -> Discard();
      pParent -> pIGProperties -> Discard();
   }

   pParent -> render(0);

   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::DestroyPropertyPage(long index) {

   switch ( index ) {
   case 0:
      if ( pParent -> hwndAppearanceSettings ) 
         DestroyWindow(pParent -> hwndAppearanceSettings);
      pParent -> hwndAppearanceSettings = NULL;
      break;
   case 1:
      if ( pParent -> hwndStyleSettings ) 
         DestroyWindow(pParent -> hwndStyleSettings);
      pParent -> hwndStyleSettings = NULL;
      break;
   case 2:
      if ( pParent -> hwndBackgroundSettings ) 
         DestroyWindow(pParent -> hwndBackgroundSettings);
      pParent -> hwndBackgroundSettings = NULL;
      break;
   case 3:
      if ( pParent -> hwndTextSettings ) 
         DestroyWindow(pParent -> hwndTextSettings);
      pParent -> hwndTextSettings = NULL;
      break;
   case 4:
      if ( pParent -> hwndLightingSettings ) 
         DestroyWindow(pParent -> hwndLightingSettings);
      pParent -> hwndLightingSettings = NULL;
      break;
   case 5:
      if ( pParent -> hwndAxisSettings ) 
         DestroyWindow(pParent -> hwndAxisSettings);
      pParent -> hwndAxisSettings = NULL;
      break;
   case 6:
      if ( pParent -> hwndPlotSettings ) 
         DestroyWindow(pParent -> hwndPlotSettings);
      pParent -> hwndPlotSettings = NULL;
      break;
   case 7:
      if ( pParent -> hwndFunctionSettings ) 
         DestroyWindow(pParent -> hwndFunctionSettings);
      pParent -> hwndFunctionSettings = NULL;
      break;
   }

   return S_OK;
   }


   HRESULT G::_IGPropertyPageClient::GetPropertySheetHeader(void *pv) {
   
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


   HRESULT G::_IGPropertyPageClient::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 8;
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
   pPropSheetPages[1].pszTitle = "Style";
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
   pPropSheetPages[3].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_TEXT_SETTINGS);
   pPropSheetPages[3].pfnDlgProc = (DLGPROC)G::textHandler;
   pPropSheetPages[3].pszTitle = "Text";
   pPropSheetPages[3].lParam = (LPARAM)pParent;
   pPropSheetPages[3].pfnCallback = NULL;

   pPropSheetPages[4].dwFlags = PSP_USETITLE;
   pPropSheetPages[4].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[4].hInstance = hModule;
   pPropSheetPages[4].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_LIGHTING);
   pPropSheetPages[4].pfnDlgProc = (DLGPROC)G::lightingHandler;
   pPropSheetPages[4].pszTitle = "Lighting";
   pPropSheetPages[4].lParam = (LPARAM)pParent;
   pPropSheetPages[4].pfnCallback = NULL;

   pPropSheetPages[5].dwFlags = PSP_USETITLE;
   pPropSheetPages[5].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[5].hInstance = hModule;
   pPropSheetPages[5].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_AXIIS);
   pPropSheetPages[5].pfnDlgProc = (DLGPROC)G::axisHandler;
   pPropSheetPages[5].pszTitle = "Axiis";
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
   pPropSheetPages[7].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_FUNCTIONS);
   pPropSheetPages[7].pfnDlgProc = (DLGPROC)G::functionHandler;
   pPropSheetPages[7].pszTitle = "Functions";
   pPropSheetPages[7].lParam = (LPARAM)pParent;
   pPropSheetPages[7].pfnCallback = NULL;

   return S_OK;
   }


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
