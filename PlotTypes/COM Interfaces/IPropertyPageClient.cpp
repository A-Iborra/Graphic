
#include "PlotTypes.h"

   PlotTypes::_IGPropertyPageClient::_IGPropertyPageClient(PlotTypes *pp,long id,WNDPROC handler,char *pszTitle) {
   pParent = pp;
   dialogID = id;
   pDialogProc = handler;
   strcpy(szDialogTitle,pszTitle);
   refCount = 1;
   return;
   }

   PlotTypes::_IGPropertyPageClient::~_IGPropertyPageClient() {}

   long __stdcall PlotTypes::_IGPropertyPageClient::QueryInterface(REFIID riid,void **ppv) {

   *ppv = NULL; 

   if ( riid == IID_IUnknown )
      *ppv = this; 
   else

   if ( riid == IID_IGPropertyPageClient ) 
      *ppv = static_cast<IGPropertyPageClient *>(this);
   else

      return pParent -> QueryInterface(riid,ppv);

   AddRef();

   return S_OK; 
   }

   unsigned long __stdcall PlotTypes::_IGPropertyPageClient::AddRef() {
   return ++refCount;
   }
   unsigned long __stdcall PlotTypes::_IGPropertyPageClient::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }

   HRESULT PlotTypes::_IGPropertyPageClient::BeforeAllPropertyPages() { return E_NOTIMPL; }
   HRESULT PlotTypes::_IGPropertyPageClient::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) { return E_NOTIMPL; }
   HRESULT PlotTypes::_IGPropertyPageClient::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) { return E_NOTIMPL; }
   HRESULT PlotTypes::_IGPropertyPageClient::Apply() { return E_NOTIMPL; }
   HRESULT PlotTypes::_IGPropertyPageClient::IsPageDirty(long hwndPage,BOOL* isDirty) { return E_NOTIMPL; }
   HRESULT PlotTypes::_IGPropertyPageClient::Help(BSTR bstrHelpDir) { return  E_NOTIMPL; }
   HRESULT PlotTypes::_IGPropertyPageClient::TranslateAccelerator(long,long* pResult) { return E_NOTIMPL; }
   HRESULT PlotTypes::_IGPropertyPageClient::AfterAllPropertyPages(BOOL userCanceled) { return E_NOTIMPL; }
   HRESULT PlotTypes::_IGPropertyPageClient::DestroyPropertyPage(long pageNumber) { return E_NOTIMPL; }


   HRESULT PlotTypes::_IGPropertyPageClient::GetPropertySheetHeader(void *pv) {

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


   HRESULT PlotTypes::_IGPropertyPageClient::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 1;
   return S_OK;
   }


   HRESULT PlotTypes::_IGPropertyPageClient::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(dialogID);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)pDialogProc;
   pPropSheetPages[0].pszTitle = szDialogTitle;
   pPropSheetPages[0].lParam = (LPARAM)pParent;
   pPropSheetPages[0].pfnCallback = NULL;

   return S_OK;
   }

