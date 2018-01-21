// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Function.h"

#include <olectl.h>

#include "Graphic_resource.h"

   STDMETHODIMP Function::Draw(DWORD dvAspect,long index,void *pvAspect,DVTARGETDEVICE *ptd,
                          HDC hicTarget,HDC hdcDraw,const struct _RECTL *rectlClient,
                          const struct _RECTL *rectlMetaFile,int (__stdcall *cancelFunction)(DWORD),DWORD cancelParameter) {
 
   if ( dvAspect != DVASPECT_CONTENT ) return S_OK;

   if ( ! pIOleClientSite ) return E_UNEXPECTED;
 
   if ( rectlMetaFile ) {

      HDC hdcSource = CreateCompatibleDC(NULL);
      HANDLE hBitmap = LoadImage(hModule,MAKEINTRESOURCE(IDCONTROLBITMAP_FUNCTION),IMAGE_BITMAP,0,0,0L);
      HANDLE oldObject = SelectObject(hdcSource,hBitmap);
      StretchBlt(hdcDraw,rectlClient -> left,rectlClient -> top,rectlClient -> right - rectlClient -> left,rectlClient -> bottom - rectlClient -> top,hdcSource,0,0,241,150,SRCCOPY);
      DeleteObject(hBitmap);
      DeleteDC(hdcSource);

   } else {

      IDispatch* pIDispatch;
      if ( SUCCEEDED(pIOleClientSite -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(&pIDispatch))) ) {

	      DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
         VARIANT var;
         VariantClear(&var);
         var.vt = VT_BOOL;
         pIDispatch -> Invoke(DISPID_AMBIENT_USERMODE,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&var,NULL,NULL);
         pIDispatch -> Release();
         userMode = var.bVal;

      }      

      SetWindowPos(hwndSpecDialog,HWND_TOP,
         rectlClient -> left,rectlClient -> top,rectlClient -> right - rectlClient -> left, rectlClient -> bottom - rectlClient -> top,SWP_NOZORDER);
   }

   return S_OK;
   }
 
 
   STDMETHODIMP Function::GetColorSet(DWORD,long,void *,DVTARGETDEVICE *,HDC,LOGPALETTE **) {
   return S_OK;
   }
 
 
   STDMETHODIMP Function::Freeze(DWORD,long,void *,DWORD *) {
   return S_OK;
   }
 
 
   STDMETHODIMP Function::Unfreeze(DWORD) {
   return S_OK;
   }
 
 
   STDMETHODIMP Function::SetAdvise(DWORD dwAspect,DWORD advf,IAdviseSink *pAdvise) {
   if ( ! pAdvise ) {
      if ( pAdviseSink ) pAdviseSink -> Release();
      pAdviseSink = NULL;
      return S_OK;
   }
   pAdviseSink = pAdvise;
   adviseSink_dwAspect = dwAspect;
   adviseSink_advf = advf;
   pAdvise -> AddRef();
   return S_OK;
   }
 
 
   STDMETHODIMP Function::GetAdvise(DWORD *pdwAspect,DWORD *padvf,IAdviseSink **ppAdvise) {
   if ( pAdviseSink == NULL ) {
      if ( padvf ) *padvf = 0;
      if ( pdwAspect ) *pdwAspect = 0;
      if ( ppAdvise ) *ppAdvise = NULL; 
      return S_OK;
   }
   if ( ppAdvise ) {
      *ppAdvise = pAdviseSink;
      pAdviseSink -> AddRef();
   } else {
      if ( pAdviseSink ) pAdviseSink -> Release();
      pAdviseSink = NULL;
   }
   if ( pdwAspect ) *pdwAspect = adviseSink_dwAspect;
   if ( padvf ) *padvf = adviseSink_advf;
   return S_OK;
   }
 
 
   STDMETHODIMP Function::GetExtent(unsigned long,long,DVTARGETDEVICE *,struct tagSIZE *s) {
   if ( ! hwndSpecDialog )
      initWindows();
   s -> cx = resultingWidth;
   s -> cy = resultingHeight;
   pixelsToHiMetric(s,s);
   return S_OK;
   }
 
 
   STDMETHODIMP Function::GetRect(DWORD dwAspect,RECTL *pRectl){
   return S_OK; 
   }
 
   STDMETHODIMP Function::GetViewStatus(DWORD *dwViewStatus){ 
   *dwViewStatus = VIEWSTATUS_OPAQUE;
   return S_OK; 
   }
 
   STDMETHODIMP Function::QueryHitPoint(DWORD dwAspect,const struct tagRECT *pRectBounds,POINT ptlHit,long lCloseHint,DWORD *dwHitResult){
   return S_OK; 
   }
 
   STDMETHODIMP Function::QueryHitRect(DWORD dwAspect,const struct tagRECT *pRectBounds,const struct tagRECT *rctHit,long lCloseHint,DWORD *dwHitResult){  
   return S_OK; 
   }
 
   STDMETHODIMP Function::GetNaturalExtent(DWORD dwExtent,LONG lIndex,DVTARGETDEVICE *ptd,HDC hicTargetDev,DVEXTENTINFO *extentInfo,SIZEL *){ 
   return S_OK;
   }


