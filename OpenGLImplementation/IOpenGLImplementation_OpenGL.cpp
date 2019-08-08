// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// 
// These methods are the OpenGL Passthru methods. They are for clients that are already using OpenGL and are here to allow simple
// redirection in the client to this implementation.
// Ultimately, they will probably be put into their own interface.
//

#include "OpenGLImplementor.h"
#include <process.h>

   HRESULT OpenGLImplementor::OpenGLClearColor(float *pVRGBA) {
   if ( ! plotWindow ) 
      return E_FAIL;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_GL_CLEAR_COLOR,(WPARAM)pVRGBA)
   return S_OK;
   }
 
 
  HRESULT OpenGLImplementor::OpenGLClearDepth(double *pv) {
   if ( ! plotWindow ) 
      return E_FAIL;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_GL_CLEAR_DEPTH,(WPARAM)pv)
   return S_OK;
   }


   HRESULT OpenGLImplementor::ChoosePixelFormat(int *pAttributes,int *pResult) {
   strCall_ChoosePixelFormat choosePixelFormat{pAttributes,pResult,S_OK};
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_GL_CHOOSE_PIXEL_FORMAT,(WPARAM)&choosePixelFormat);
   return choosePixelFormat.result;
   }


   HRESULT OpenGLImplementor::OpenGLCreateContext(int *pAttributes,UINT_PTR *pResult) {
   strCall_CreateContext createContext{pAttributes,pResult,S_OK};
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_GL_CREATE_CONTEXT,(WPARAM)&createContext);
   return createContext.result;
   }

   
   HRESULT OpenGLImplementor::OpenGLMakeCurrent(UINT_PTR hdc,UINT_PTR context,BOOL *pResult) {
   strCall_MakeCurrent makeCurrent{hdc,context,pResult};
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_GL_MAKE_CURRENT,(WPARAM)&makeCurrent);
   if ( ! makeCurrent.pResult )
      return E_FAIL;
   return S_OK;
   }