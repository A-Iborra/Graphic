// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ObjectInOffice.h"

   STDMETHODIMP ObjectInOffice::Draw(DWORD dvAspect,long index,void *pvAspect,DVTARGETDEVICE *ptd,
                          HDC hicTarget,HDC hdcDraw,
                          const struct _RECTL *rectlClient,
                          const struct _RECTL *rectlMetaFile,
                          int (__stdcall *cancelFunction)(DWORD),DWORD cancelParameter) {
 
   if ( dvAspect != DVASPECT_CONTENT ) return S_OK;

   if ( rectlMetaFile ) {

      HDC hdcSource = CreateCompatibleDC(NULL);
      HANDLE hBitmap = LoadImage(hModule,MAKEINTRESOURCE(IDCONTROLBITMAP),IMAGE_BITMAP,0,0,0L);
      HANDLE oldObject = SelectObject(hdcSource,hBitmap);
      StretchBlt(hdcDraw,rectlClient -> left,rectlClient -> top,rectlClient -> right - rectlClient -> left,rectlClient -> bottom - rectlClient -> top,hdcSource,0,0,241,150,SRCCOPY);
      DeleteObject(hBitmap);
      DeleteDC(hdcSource);

   } else {

      SetWindowPos(hwndMain,HWND_TOP,
         rectlClient -> left,rectlClient -> top,rectlClient -> right - rectlClient -> left, rectlClient -> bottom - rectlClient -> top,SWP_NOZORDER);
   }

   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetColorSet(DWORD,long,void *,DVTARGETDEVICE *,HDC,LOGPALETTE **) {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::Freeze(DWORD,long,void *,DWORD *) {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::Unfreeze(DWORD) {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::SetAdvise(DWORD dwAspect,DWORD advf,IAdviseSink *pAdvise) {
   if ( ! pAdvise ) {
      if ( pAdviseSink ) pAdviseSink -> Release();
      pAdviseSink = NULL;
      return S_OK;
   }
   pAdviseSink = pAdvise;
   adviseSink_dwAspect = dwAspect;
   adviseSink_advf = advf;
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetAdvise(DWORD *pdwAspect,DWORD *padvf,IAdviseSink **ppAdvise) {
   if ( ppAdvise ) {
      *ppAdvise = pAdviseSink;
      pAdviseSink -> AddRef();
   }      
   if ( pdwAspect ) 
      *pdwAspect = adviseSink_dwAspect;
   if ( padvf ) 
      *padvf = adviseSink_advf;
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetExtent(unsigned long,long,DVTARGETDEVICE *,struct tagSIZE *s) {
   RECT rc;
   GetWindowRect(hwndMain,&rc);
   s -> cx = rc.right - rc.left;
   s -> cy = rc.bottom - rc.top;
   pixelsToHiMetric(s,s);
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::GetRect(DWORD dwAspect,RECTL *){
   return S_OK; 
   }
 
   STDMETHODIMP ObjectInOffice::GetViewStatus(DWORD *pdwStatus){ 
   *pdwStatus = VIEWSTATUS_OPAQUE;
   return S_OK; 
   }
 
   STDMETHODIMP ObjectInOffice::QueryHitPoint(DWORD dwAspect,const struct tagRECT *pRectBounds,POINT ptlHit,long lCloseHint,DWORD *pdwHitResult) {
   *pdwHitResult = HITRESULT_OUTSIDE;
   return S_OK; 
   }
 
   STDMETHODIMP ObjectInOffice::QueryHitRect(DWORD dwAspect,const struct tagRECT *pRectBounds,const struct tagRECT *rctHit,long lCloseHint,DWORD *dwHitResult){  
   return S_OK; 
   }
 
   STDMETHODIMP ObjectInOffice::GetNaturalExtent(DWORD dwExtent,LONG lIndex,DVTARGETDEVICE *ptd,HDC hicTargetDev,DVEXTENTINFO *extentInfo,SIZEL *){ 
   return S_OK;
   }


