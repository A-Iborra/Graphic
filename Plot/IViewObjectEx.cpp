// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Plot.h"


  STDMETHODIMP Plot::Draw(DWORD dvAspect,long index,void *pvAspect,DVTARGETDEVICE *ptd,
                         HDC hicTarget,HDC hdcDraw,
                         const struct _RECTL *rectlClient,
                         const struct _RECTL *rectlMetaFile,
                         int (__stdcall *cancelFunction)(ULONG_PTR),ULONG_PTR cancelParameter) {
  return S_OK;
  }


  STDMETHODIMP Plot::GetColorSet(DWORD,long,void *,DVTARGETDEVICE *,HDC,LOGPALETTE **) {
  return S_OK;
  }


  STDMETHODIMP Plot::Freeze(DWORD,long,void *,DWORD *) {
  return S_OK;
  }


  STDMETHODIMP Plot::Unfreeze(DWORD) {
  return S_OK;
  }


  STDMETHODIMP Plot::SetAdvise(DWORD dwAspect,DWORD advf,IAdviseSink *pAdvise) {
  if ( ! pAdvise ) {
     if ( pIViewObjectEx_IAdviseSink ) pIViewObjectEx_IAdviseSink -> Release();
     pIViewObjectEx_IAdviseSink = NULL;
     return S_OK;
  }
  pIViewObjectEx_IAdviseSink = pAdvise;
  adviseSink_dwAspect = dwAspect;
  adviseSink_advf = advf;
  return S_OK;
  }


  STDMETHODIMP Plot::GetAdvise(DWORD *pdwAspect,DWORD *padvf,IAdviseSink **ppAdvise) {
  if ( ppAdvise ) *ppAdvise = pIViewObjectEx_IAdviseSink;
  if ( pdwAspect ) *pdwAspect = adviseSink_dwAspect;
  if ( padvf ) *padvf = adviseSink_advf;
  return S_OK;
  }


  STDMETHODIMP Plot::GetExtent(unsigned long,long,DVTARGETDEVICE *,struct tagSIZE *s) {
  return E_NOTIMPL;
  }


  STDMETHODIMP Plot::GetRect(DWORD dwAspect,RECTL *){
  return E_NOTIMPL; 
  }

  STDMETHODIMP Plot::GetViewStatus(DWORD *){ 
  return E_NOTIMPL; 
  }

  STDMETHODIMP Plot::QueryHitPoint(DWORD dwAspect,const struct tagRECT *pRectBounds,POINT ptlHit,long lCloseHint,DWORD *dwHitResult){
  return S_OK; 
  }

  STDMETHODIMP Plot::QueryHitRect(DWORD dwAspect,const struct tagRECT *pRectBounds,const struct tagRECT *rctHit,long lCloseHint,DWORD *dwHitResult){  
  return S_OK; 
  }

  STDMETHODIMP Plot::GetNaturalExtent(DWORD dwExtent,LONG lIndex,DVTARGETDEVICE *ptd,HDC hicTargetDev,DVEXTENTINFO *extentInfo,SIZEL *){ 
  return S_OK;
  }


