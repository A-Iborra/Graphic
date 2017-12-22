/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

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
#if 1
   *pCount = 8;
#else
   *pCount = 9;
#endif
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

#if 1
   int nextIndex = 3;
#else
   int nextIndex = 4;

   pPropSheetPages[3].dwFlags = PSP_USETITLE;
   pPropSheetPages[3].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[3].hInstance = hModule;
   pPropSheetPages[3].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_TEXT_SETTINGS);
   pPropSheetPages[3].pfnDlgProc = (DLGPROC)G::textHandler;
   pPropSheetPages[3].pszTitle = "Text";
   pPropSheetPages[3].lParam = (LPARAM)pParent;
   pPropSheetPages[3].pfnCallback = NULL;
#endif

   pPropSheetPages[nextIndex].dwFlags = PSP_USETITLE;
   pPropSheetPages[nextIndex].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[nextIndex].hInstance = hModule;
   pPropSheetPages[nextIndex].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_LIGHTING);
   pPropSheetPages[nextIndex].pfnDlgProc = (DLGPROC)G::lightingHandler;
   pPropSheetPages[nextIndex].pszTitle = "Lighting";
   pPropSheetPages[nextIndex].lParam = (LPARAM)pParent;
   pPropSheetPages[nextIndex].pfnCallback = NULL;

   pPropSheetPages[++nextIndex].dwFlags = PSP_USETITLE;
   pPropSheetPages[nextIndex].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[nextIndex].hInstance = hModule;
   pPropSheetPages[nextIndex].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_AXIIS);
   pPropSheetPages[nextIndex].pfnDlgProc = (DLGPROC)G::axisHandler;
   pPropSheetPages[nextIndex].pszTitle = "Axiis";
   pPropSheetPages[nextIndex].lParam = (LPARAM)pParent;
   pPropSheetPages[nextIndex].pfnCallback = NULL;

   pPropSheetPages[++nextIndex].dwFlags = PSP_USETITLE;
   pPropSheetPages[nextIndex].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[nextIndex].hInstance = hModule;
   pPropSheetPages[nextIndex].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_PLOTS);
   pPropSheetPages[nextIndex].pfnDlgProc = (DLGPROC)G::plotHandler;
   pPropSheetPages[nextIndex].pszTitle = "Plots";
   pPropSheetPages[nextIndex].lParam = (LPARAM)pParent;
   pPropSheetPages[nextIndex].pfnCallback = NULL;

   pPropSheetPages[++nextIndex].dwFlags = PSP_USETITLE;
   pPropSheetPages[nextIndex].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[nextIndex].hInstance = hModule;
   pPropSheetPages[nextIndex].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_DATASETS);
   pPropSheetPages[nextIndex].pfnDlgProc = (DLGPROC)G::dataSetHandler;
   pPropSheetPages[nextIndex].pszTitle = "DataSets";
   pPropSheetPages[nextIndex].lParam = (LPARAM)pParent;
   pPropSheetPages[nextIndex].pfnCallback = NULL;

   pPropSheetPages[++nextIndex].dwFlags = PSP_USETITLE;
   pPropSheetPages[nextIndex].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[nextIndex].hInstance = hModule;
   pPropSheetPages[nextIndex].pszTemplate = MAKEINTRESOURCE(IDDIALOG_GRAPHIC_FUNCTIONS);
   pPropSheetPages[nextIndex].pfnDlgProc = (DLGPROC)G::functionHandler;
   pPropSheetPages[nextIndex].pszTitle = "Functions";
   pPropSheetPages[nextIndex].lParam = (LPARAM)pParent;
   pPropSheetPages[nextIndex].pfnCallback = NULL;

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
