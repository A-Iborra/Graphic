// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"

#include <gl\glu.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094

typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);

   PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
   PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;


   HRESULT PlotWindow::OpenGLChoosePixelFormat(int *pAttributes,int *pPixelFormat) {

   if ( NULL == renderingContext ) {
      renderingContext = wglCreateContext(deviceContext);
      wglMakeCurrent(deviceContext,renderingContext);
   }

   if ( NULL == wglChoosePixelFormatARB ) {

      wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
   
      long k = glGetError();

      if ( NULL == wglChoosePixelFormatARB )
         return E_NOTIMPL;

   }

   UINT formatCount;

   wglChoosePixelFormatARB(deviceContext,pAttributes,NULL,1,pPixelFormat,&formatCount);

   if ( 0 == *pPixelFormat )
      return E_FAIL;
   
   PIXELFORMATDESCRIPTOR pfd{0};

   DescribePixelFormat(deviceContext, *pPixelFormat, sizeof(pfd), &pfd);

   SetPixelFormat(deviceContext, *pPixelFormat, &pfd);

   return S_OK;
   }


   HRESULT PlotWindow::OpenGLCreateContext(int *pAttributes,UINT_PTR *pContext) {

   if ( NULL == wglCreateContextAttribsARB ) {

      wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
   
      long k = glGetError();

      if ( NULL == wglCreateContextAttribsARB )
         return E_NOTIMPL;

   }

   wglDeleteContext(renderingContext);

   renderingContext = 0;

   *pContext = NULL;

   int attemptedVersions[] = {4,5, 4,4, 4,3, 4,2, 4,1, 4,0, 3,3, 3,2, 3,1, 0,0 };

   int contextAttribs[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
      WGL_CONTEXT_MINOR_VERSION_ARB, 2,
      WGL_CONTEXT_FLAGS_ARB, 0,
      0 // End of attributes list
      };

   for ( int k = 0; attemptedVersions[k] && ! *pContext; k++ ) {
      contextAttribs[1] = attemptedVersions[2 * k];
      contextAttribs[3] = attemptedVersions[2 * k + 1];
      *pContext = (UINT_PTR)wglCreateContextAttribsARB(deviceContext, 0, contextAttribs);
   }

   if ( NULL == *pContext )
      return E_FAIL;

   renderingContext = (HGLRC)*pContext;

   return S_OK;
   }

   
   HRESULT PlotWindow::OpenGLMakeCurrent(UINT_PTR hdc,UINT_PTR context) {

   BOOL rc = wglMakeCurrent((HDC)hdc,(HGLRC)context);

   if ( ! rc )
      return E_FAIL;
   
   renderingContext = (HGLRC)context;
   deviceContext = (HDC)hdc;

   return S_OK;
   }
   