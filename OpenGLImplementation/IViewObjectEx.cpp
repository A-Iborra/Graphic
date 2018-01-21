// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"
#include "utils.h"

   STDMETHODIMP OpenGLImplementor::Draw(DWORD dvAspect,long index,void *pvAspect,DVTARGETDEVICE *ptd,
                          HDC hicTarget,HDC hdcDraw,
                          const struct _RECTL *rectlClient,
                          const struct _RECTL *rectlMetaFile,
                          int (__stdcall *cancelFunction)(DWORD),DWORD cancelParameter) {
   OpenGLImplementor *p = static_cast<OpenGLImplementor *>(this);

   if ( dvAspect != DVASPECT_CONTENT ) return S_OK;

   SetWindowPos(p -> hwndControl,HWND_TOP,
      rectlClient -> left,rectlClient -> top,rectlClient -> right - rectlClient -> left, rectlClient -> bottom - rectlClient -> top,SWP_NOZORDER);

   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetColorSet(DWORD,long,void *,DVTARGETDEVICE *,HDC,LOGPALETTE **) {
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::Freeze(DWORD,long,void *,DWORD *) {
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::Unfreeze(DWORD) {
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::SetAdvise(DWORD dwAspect,DWORD advf,IAdviseSink *pAdvise) {
   OpenGLImplementor *p = static_cast<OpenGLImplementor *>(this);
/*CHECKME
   if ( ! pAdvise ) {
      if ( pAdviseSink ) pAdviseSink -> Release();
      p -> pIAdviseSink = NULL;
      return S_OK;
   }
   p -> pIAdviseSink = pAdvise;
   p -> adviseSink_dwAspect = dwAspect;
   p -> adviseSink_advf = advf;
*/
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetAdvise(DWORD *pdwAspect,DWORD *padvf,IAdviseSink **ppAdvise) {
   OpenGLImplementor *p = static_cast<OpenGLImplementor *>(this);
   if ( ppAdvise ) {
      *ppAdvise = p -> pIAdviseSink;
      p -> pIAdviseSink -> AddRef();
   }
   if ( pdwAspect ) *pdwAspect = p -> adviseSink_dwAspect;
   if ( padvf ) *padvf = p -> adviseSink_advf;
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetExtent(unsigned long,long,DVTARGETDEVICE *,struct tagSIZE *s) {
   OpenGLImplementor *p = static_cast<OpenGLImplementor *>(this);
   SIZEL sizel = {100,100};
   p -> pIPropertySize -> get_binaryValue(sizeof(SIZEL),reinterpret_cast<unsigned char **>(&sizel));
   s -> cx = sizel.cx;
   s -> cy = sizel.cy;
   pixelsToHiMetric(s,s);
   return S_OK;
   }
 
 
   STDMETHODIMP OpenGLImplementor::GetRect(DWORD dwAspect,RECTL *){
   return S_OK; 
   }
 
   STDMETHODIMP OpenGLImplementor::GetViewStatus(DWORD *pdwStatus) { 
   *pdwStatus = VIEWSTATUS_OPAQUE;
   return S_OK; 
   }
 
   STDMETHODIMP OpenGLImplementor::QueryHitPoint(DWORD dwAspect,const struct tagRECT *pRectBounds,POINT ptlHit,long lCloseHint,DWORD *dwHitResult){
   return S_OK; 
   }
 
   STDMETHODIMP OpenGLImplementor::QueryHitRect(DWORD dwAspect,const struct tagRECT *pRectBounds,const struct tagRECT *rctHit,long lCloseHint,DWORD *dwHitResult){  
   return S_OK; 
   }
 
   STDMETHODIMP OpenGLImplementor::GetNaturalExtent(DWORD dwExtent,LONG lIndex,DVTARGETDEVICE *ptd,HDC hicTargetDev,DVEXTENTINFO *extentInfo,SIZEL *){ 
   return S_OK;
   }


