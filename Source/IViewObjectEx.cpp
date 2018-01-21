// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include "Graphic_resource.h"

#include "utils.h"


   STDMETHODIMP G::Draw(DWORD dvAspect,long index,void *pvAspect,DVTARGETDEVICE *ptd,
                          HDC hicTarget,HDC hdcDraw,
                          const struct _RECTL *rectlClient,
                          const struct _RECTL *rectlMetaFile,
                          int (__stdcall *cancelFunction)(DWORD),DWORD cancelParameter) {
 
   if ( dvAspect != DVASPECT_CONTENT ) return S_OK;

   if ( rectlMetaFile ) {

      HDC hdcSource = CreateCompatibleDC(NULL);
      HANDLE hBitmap = LoadImage(hModule,MAKEINTRESOURCE(IDCONTROLBITMAP_GRAPHIC),IMAGE_BITMAP,0,0,0L);
      HANDLE oldObject = SelectObject(hdcSource,hBitmap);
      StretchBlt(hdcDraw,rectlClient -> left,rectlClient -> top,rectlClient -> right - rectlClient -> left,rectlClient -> bottom - rectlClient -> top,hdcSource,0,0,241,150,SRCCOPY);
      DeleteObject(hBitmap);
      DeleteDC(hdcSource);

   } else {

      SetWindowPos(hwndFrame,HWND_TOP,
         rectlClient -> left,rectlClient -> top,rectlClient -> right - rectlClient -> left, rectlClient -> bottom - rectlClient -> top,SWP_NOZORDER);
   }

   return S_OK;
   }
 
 
   STDMETHODIMP G::GetColorSet(DWORD,long,void *,DVTARGETDEVICE *,HDC,LOGPALETTE **) {
   return S_OK;
   }
 
 
   STDMETHODIMP G::Freeze(DWORD,long,void *,DWORD *) {
   return S_OK;
   }
 
 
   STDMETHODIMP G::Unfreeze(DWORD) {
   return S_OK;
   }
 
 
   STDMETHODIMP G::SetAdvise(DWORD dwAspect,DWORD advf,IAdviseSink *pAdvise) {
   G *p = static_cast<G *>(this);
   if ( ! pAdvise ) {
      if ( pAdviseSink ) pAdviseSink -> Release();
      p -> pAdviseSink = NULL;
      return S_OK;
   }
   p -> pAdviseSink = pAdvise;
   p -> adviseSink_dwAspect = dwAspect;
   p -> adviseSink_advf = advf;
   return S_OK;
   }
 
 
   STDMETHODIMP G::GetAdvise(DWORD *pdwAspect,DWORD *padvf,IAdviseSink **ppAdvise) {
   G *p = static_cast<G *>(this);
   if ( ppAdvise ) {
      *ppAdvise = p -> pAdviseSink;
      p -> pAdviseSink -> AddRef();
   }      
   if ( pdwAspect ) *pdwAspect = p -> adviseSink_dwAspect;
   if ( padvf ) *padvf = p -> adviseSink_advf;
   return S_OK;
   }
 
 
   STDMETHODIMP G::GetExtent(unsigned long,long,DVTARGETDEVICE *,struct tagSIZE *s) {
   s -> cx = containerSize.cx;
   s -> cy = containerSize.cy;
   pixelsToHiMetric(s,s);
   return S_OK;
   }
 
 
   STDMETHODIMP G::GetRect(DWORD dwAspect,RECTL *){
   return S_OK; 
   }
 
   STDMETHODIMP G::GetViewStatus(DWORD *pdwStatus){ 
   *pdwStatus = VIEWSTATUS_OPAQUE;
   return S_OK; 
   }
 
   STDMETHODIMP G::QueryHitPoint(DWORD dwAspect,const struct tagRECT *pRectBounds,POINT ptlHit,long lCloseHint,DWORD *dwHitResult){
   return S_OK; 
   }
 
   STDMETHODIMP G::QueryHitRect(DWORD dwAspect,const struct tagRECT *pRectBounds,const struct tagRECT *rctHit,long lCloseHint,DWORD *dwHitResult){  
   return S_OK; 
   }
 
   STDMETHODIMP G::GetNaturalExtent(DWORD dwExtent,LONG lIndex,DVTARGETDEVICE *ptd,HDC hicTargetDev,DVEXTENTINFO *extentInfo,SIZEL *){ 
   return S_OK;
   }


