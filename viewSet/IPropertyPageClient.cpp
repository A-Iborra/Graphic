/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "utils.h"

#include "ViewSet.h"


   HRESULT ViewSet::BeforeAllPropertyPages() {
   size();
   iProperties -> Push();
   iProperties -> Push();
   return S_OK;
   }


   HRESULT ViewSet::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {

   *pCntPages = 1;
   long index = 0;

   SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"View"));

   SafeArrayPutElement(*theHelpDirs,&index,NULL);

   long size = 250;
   SafeArrayPutElement(*pSize,&index,&size);
   index++; 
   SafeArrayPutElement(*pSize,&index,&size);

   return S_OK;
   }


   HRESULT ViewSet::CreatePropertyPage(long pageNumber,HWND hwndPP,RECT* pRect,BOOL fModal,HWND* pHwnd) {

   if ( ! pHwnd ) return E_POINTER;

   if ( ! hwndPP ) return E_POINTER;

   hwndParent = (HWND)hwndPP;

   initWindows();

   *pHwnd = hwndViewSet;

   SAFEARRAYBOUND rgsa;
   BSTR *pData;
   DataPoint dpEndPoint;

   rgsa.lLbound = 0;
   rgsa.cElements = 3;

   pIOpenGLImplementation -> SetTargetWindow(hwndGraphic);

   pIOpenGLImplementation -> SetUp(pIDataSet);

   IText *pText[] = {pIText_X,pIText_Y,pIText_Z};
   IAxis *pAxis[] = {pIAxis_X,pIAxis_Y,pIAxis_Z};

   for ( long k = 0; k < 3; k++ ) {

      SAFEARRAY *psaEndPoint = SafeArrayCreate(VT_BSTR,1,&rgsa);

      pAxis[k] -> PrepData();

      pAxis[k] -> get_EndPoint(&psaEndPoint);

      SafeArrayAccessData(psaEndPoint,reinterpret_cast<void**>(&pData));
   
      dpEndPoint.x = evalBSTR(pIEvaluator,*pData);
      pData++;
      dpEndPoint.y = evalBSTR(pIEvaluator,*pData);
      pData++;
      dpEndPoint.z = evalBSTR(pIEvaluator,*pData);

      SafeArrayUnaccessData(psaEndPoint);

      SafeArrayDestroy(psaEndPoint);

      pText[k] -> put_PositionX(dpEndPoint.x);
      pText[k] -> put_PositionY(dpEndPoint.y);
      pText[k] -> put_PositionZ(dpEndPoint.z);

      pText[k] -> PrepData();

   }

   pIOpenGLImplementation -> ResetTargetWindow();

   return S_OK;
   }

   HRESULT ViewSet::Apply() {
   iProperties -> Discard();
   iProperties -> Push();
   if ( hwndGraphicContainer ) 
      InvalidateRect(hwndGraphicContainer,NULL,TRUE);
   return S_OK;
   }


   HRESULT ViewSet::IsPageDirty(long hwndPage,BOOL* isDirty) {
   iProperties -> Compare((short*)isDirty);
   return S_OK;
   }


   HRESULT ViewSet::Help(BSTR bstrHelpDir) {
   return  S_OK;
   }


   HRESULT ViewSet::TranslateAccelerator(long,long* pResult) {
   *pResult = S_FALSE;
   return S_OK;
   }


   HRESULT ViewSet::AfterAllPropertyPages(BOOL userCanceled) {
   if ( userCanceled ) {
      iProperties -> Pop();
      iProperties -> Pop();
   } else {
      iProperties -> Discard();
      iProperties -> Discard();
   }
   if ( hwndGraphicContainer ) 
      InvalidateRect(hwndGraphicContainer,NULL,TRUE);
   return S_OK;
   }



   HRESULT ViewSet::DestroyPropertyPage(long index) {
   DestroyWindow(hwndViewSet);
   hwndViewSet = NULL;
   return S_OK;
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
   if ( ! pCount )
      return E_POINTER;
   *pCount = 1;
   return S_OK;
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

