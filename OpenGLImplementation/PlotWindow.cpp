// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"

#include <gl\glu.h>

#include "general.h"
#include "Graphic_resource.h"
#include "utils.h"

   int PlotWindow::pixelFormat = 0;
   HGLRC PlotWindow::renderingContext = NULL;

   static double piOver2 = 0.0;

   //static double piOver2 = 2.0 * atan(1.0);

   PIXELFORMATDESCRIPTOR pfd = {
         sizeof(PIXELFORMATDESCRIPTOR),
         1,
         PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,// | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER, 
         PFD_TYPE_RGBA,   
         24,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,PFD_MAIN_PLANE,0,0,0,0};

   HGLRC wglCreateContextAttribsARB(HDC hDC, HGLRC hshareContext, const int *attribList);

   void lookAtMatrix(GLfloat *eyePosition3D,GLfloat *center3D,GLfloat *upVector3D,GLfloat *pResult);

   PlotWindow::PlotWindow(HWND h,OpenGLImplementor *pp,IEvaluator *piev) :

       deviceContext(NULL),
       hwnd(h),
       pParent(pp),
       pIEvaluator(piev),
       initialized(false),
       isRendered(false),
       lineMode(false),
       polygonMode(false),

       pSaved_PlotView(NULL),
       pSaved_RotationTheta(NULL),
       pSaved_RotationPhi(NULL),
       pSaved_RotationSpin(NULL),
       pSaved_LeftMargin(NULL),
       pSaved_TopMargin(NULL),
       pSaved_RightMargin(NULL),
       pSaved_BottomMargin(NULL),
       pSaved_MarginUnits(NULL),
       pSaved_StretchToMargins(NULL),

       pSaved_BackgroundColor(NULL) {

   memset(&openGLLighting,0,sizeof(lightingParameters));

   memset(&openGLState,0,sizeof(openGLState));

   openGLState.extentsXMin = openGLState.extentsYMin = openGLState.extentsZMin = DBL_MAX;
   openGLState.extentsXMax = openGLState.extentsYMax = openGLState.extentsZMax = -DBL_MAX;
   openGLState.xScaleFactor = 1.0;
   openGLState.yScaleFactor = 1.0;
   openGLState.zScaleFactor = 1.0;

   openGLLighting.shinyness = 25;

   RECT rect;
   GetWindowRect(h,&rect);
   openGLState.windowCX = rect.right - rect.left;
   openGLState.windowCY = rect.bottom - rect.top;

   if ( pParent ) {

      PlotWindow *ppw = pParent -> GetPlotWindow();

      if ( ppw ) {

         ppw -> saveState();

         pSaved_PlotView = ppw -> pSaved_PlotView;

         pSaved_RotationTheta = ppw -> pSaved_RotationTheta;
         pSaved_RotationPhi = ppw -> pSaved_RotationPhi;
         pSaved_RotationSpin = ppw -> pSaved_RotationSpin;

         pSaved_LeftMargin = ppw -> pSaved_LeftMargin;
         pSaved_TopMargin = ppw -> pSaved_TopMargin;
         pSaved_RightMargin = ppw -> pSaved_RightMargin;
         pSaved_BottomMargin = ppw -> pSaved_BottomMargin;

         pSaved_MarginUnits = ppw -> pSaved_MarginUnits;

         pSaved_StretchToMargins = ppw -> pSaved_StretchToMargins;

         pSaved_BackgroundColor = ppw -> pSaved_BackgroundColor;

         restoreState(&ppw -> openGLState);

      }

   }

   createRenderingContext();

   return;
   };
 
 
   PlotWindow::~PlotWindow() {
   if ( IsWindow(hwnd) && deviceContext )
      ReleaseDC(hwnd,deviceContext);
   unsetViewProperties();
   return;
   };
 
 
   int PlotWindow::createRenderingContext() {

   isRendered = false;

   if ( renderingContext ) {
      ReleaseDC(hwnd,deviceContext);
      wglMakeCurrent(NULL,NULL);
      wglDeleteContext(renderingContext);
      renderingContext = NULL;
   }

   deviceContext = GetDC(hwnd);
  
   if ( 0 == pixelFormat ) {

      pixelFormat = ::ChoosePixelFormat(deviceContext,&pfd);

      if ( ! pixelFormat ) {
         pfd.dwFlags = pfd.dwFlags & ~ PFD_GENERIC_ACCELERATED;
         pixelFormat = ::ChoosePixelFormat(deviceContext,&pfd);
      }

   }

   SetPixelFormat(deviceContext,pixelFormat,&pfd);

   if ( 0 == openGLState.windowCX || 0 == openGLState.windowCY )
      return FALSE;

   renderingContext = wglCreateContext(deviceContext);

   wglMakeCurrent(deviceContext,renderingContext);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   glDrawBuffer(GL_BACK);

   return TRUE;
   }
 
 
   int PlotWindow::saveState() {

   glGetDoublev(GL_MODELVIEW_MATRIX,openGLState.modelMatrix);
   glGetDoublev(GL_PROJECTION_MATRIX,openGLState.projectionMatrix);
   glGetIntegerv(GL_VIEWPORT,openGLState.viewPort);

   openGLState.viewPortMargins[0] = openGLState.viewPort[0];
   openGLState.viewPortMargins[1] = openGLState.viewPort[1];
   openGLState.viewPortMargins[2] = openGLState.windowCX - openGLState.viewPort[2] - openGLState.viewPort[0];
   openGLState.viewPortMargins[3] = openGLState.windowCY - openGLState.viewPort[3] - openGLState.viewPort[1];

   return 0;
   }
 
 
   int PlotWindow::restoreState(transformationMatrixes* tm) {

   RECT rect;
   GetWindowRect(hwnd,&rect);
   openGLState.windowCX = rect.right - rect.left;
   openGLState.windowCY = rect.bottom - rect.top;

   if ( tm ) {

      openGLState.viewPortMargins[0] = (long)((double)openGLState.windowCX * (double)(tm -> viewPortMargins[0]) / (double)(tm -> windowCX));
      openGLState.viewPortMargins[1] = (long)((double)openGLState.windowCY * (double)(tm -> viewPortMargins[1]) / (double)(tm -> windowCY));
      openGLState.viewPortMargins[2] = (long)((double)openGLState.windowCX * (double)(tm -> viewPortMargins[2]) / (double)(tm -> windowCX));
      openGLState.viewPortMargins[3] = (long)((double)openGLState.windowCY * (double)(tm -> viewPortMargins[3]) / (double)(tm -> windowCY));

   }

   openGLState.viewPort[0] = openGLState.viewPortMargins[0];
   openGLState.viewPort[1] = openGLState.viewPortMargins[1];
   openGLState.viewPort[2] = openGLState.windowCX - openGLState.viewPortMargins[0] - openGLState.viewPortMargins[2];
   openGLState.viewPort[3] = openGLState.windowCY - openGLState.viewPortMargins[2] - openGLState.viewPortMargins[3];

   if ( tm ) {

      memcpy(openGLState.projectionMatrix,tm -> projectionMatrix,sizeof(tm -> projectionMatrix));
      memcpy(openGLState.modelMatrix,tm -> modelMatrix,sizeof(tm -> modelMatrix));

      openGLState.xScaleFactor = tm -> xScaleFactor;
      openGLState.yScaleFactor = tm -> yScaleFactor;
      openGLState.zScaleFactor = tm -> zScaleFactor;
      
      openGLState.extentsXMin = tm -> extentsXMin;
      openGLState.extentsYMin = tm -> extentsYMin;
      openGLState.extentsZMin = tm -> extentsZMin;
      openGLState.extentsXMax = tm -> extentsXMax;
      openGLState.extentsYMax = tm -> extentsYMax;
      openGLState.extentsZMax = tm -> extentsZMax;

   }

   setUp(NULL);

   return 0;
   }
 
 
   HRESULT PlotWindow::setViewProperties(
                               IGProperty *pPropPlotView,
                               IGProperty *pPropRotationTheta,
                               IGProperty *pPropRotationPhi,
                               IGProperty *pPropRotationSpin,
                               IGProperty *pPropLeftMargin,
                               IGProperty *pPropTopMargin,
                               IGProperty *pPropRightMargin,
                               IGProperty *pPropBottomMargin,
                               IGProperty *pPropMarginUnits,
                               IGProperty *pPropStretchToMargins) {
   unsetViewProperties();

   if ( pPropPlotView ) {
      pPropPlotView -> AddRef();
      pSaved_PlotView = pPropPlotView;
   }
   if ( pPropRotationTheta ) {
      pPropRotationTheta -> AddRef();
      pSaved_RotationTheta = pPropRotationTheta;
   }
   if ( pPropRotationPhi ) {
      pPropRotationPhi -> AddRef();
      pSaved_RotationPhi = pPropRotationPhi;
   }
   if ( pPropRotationSpin ) {
      pPropRotationSpin -> AddRef();
      pSaved_RotationSpin = pPropRotationSpin;
   }
   if ( pPropLeftMargin ) {
      pPropLeftMargin -> AddRef();
      pSaved_LeftMargin = pPropLeftMargin;
   }
   if ( pPropTopMargin ) {
      pPropTopMargin -> AddRef();
      pSaved_TopMargin = pPropTopMargin;
   }
   if ( pPropRightMargin ) {
      pPropRightMargin -> AddRef();
      pSaved_RightMargin = pPropRightMargin;
   }
   if ( pPropBottomMargin ) {
      pPropBottomMargin -> AddRef();
      pSaved_BottomMargin = pPropBottomMargin;
   }
   if ( pPropMarginUnits ) {
      pPropMarginUnits -> AddRef();
      pSaved_MarginUnits = pPropMarginUnits;
   }
   if ( pPropStretchToMargins ) {
      pPropStretchToMargins -> AddRef();
      pSaved_StretchToMargins = pPropStretchToMargins;
   }

   return S_OK;
   }


   HRESULT PlotWindow::unsetViewProperties() {
   if ( pSaved_PlotView ) {
      pSaved_PlotView -> Release();
      pSaved_PlotView = NULL;
   }
   if ( pSaved_RotationTheta ) {
      pSaved_RotationTheta -> Release();
      pSaved_RotationTheta = NULL;
   }
   if ( pSaved_RotationPhi ) {
      pSaved_RotationPhi -> Release();
      pSaved_RotationPhi = NULL;
   }
   if ( pSaved_RotationSpin ) {
      pSaved_RotationSpin -> Release();
      pSaved_RotationSpin = NULL;
   }
   if ( pSaved_LeftMargin ) {
      pSaved_LeftMargin -> Release();
      pSaved_LeftMargin = NULL;
   }
   if ( pSaved_TopMargin ) {
      pSaved_TopMargin -> Release();
      pSaved_TopMargin = NULL;
   }
   if ( pSaved_RightMargin ) {
      pSaved_RightMargin -> Release();
      pSaved_RightMargin = NULL;
   }
   if ( pSaved_BottomMargin ) {
      pSaved_BottomMargin -> Release();
      pSaved_BottomMargin = NULL;
   }
   if ( pSaved_MarginUnits ) {
      pSaved_MarginUnits -> Release();
      pSaved_MarginUnits = NULL;
   }
   if ( pSaved_StretchToMargins ) {
      pSaved_StretchToMargins -> Release();
      pSaved_StretchToMargins = NULL;
   }
   return S_OK;
   }


   HRESULT PlotWindow::set2D(IDataSet *masterDataSet,long margins,char directionRight,char directionUp) {

   openGLState.viewPort[0] = margins;
   openGLState.viewPort[1] = margins;
   openGLState.viewPort[2] = openGLState.windowCX - 2 * margins;
   openGLState.viewPort[3] = openGLState.windowCY - 2 * margins;

   glViewport(openGLState.viewPort[0],openGLState.viewPort[1],(GLsizei)openGLState.viewPort[2],(GLsizei)openGLState.viewPort[3]);
OPENGL_ERROR_CHECK

   DataPoint dp[2];
   masterDataSet -> GetDomain(&dp[0],&dp[1]);
   openGLState.extentsXMin = dp[0].x;
   openGLState.extentsYMin = dp[0].y;
   openGLState.extentsZMin = dp[0].z;
   openGLState.extentsXMax = dp[1].x;
   openGLState.extentsYMax = dp[1].y;
   openGLState.extentsZMax = dp[1].z;
 
   if ( openGLState.extentsZMin == openGLState.extentsZMax ) 
      openGLState.extentsZMax += 0.1 * openGLState.extentsZMin;
   if ( openGLState.extentsZMin == openGLState.extentsZMax ) 
      openGLState.extentsZMax = 1.0;

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

OPENGL_ERROR_CHECK

   switch ( directionRight ) {
   case 'X':
      if ( 'Z' == directionUp ) 
         glRotated(-90.0,1.0,0.0,0.0);
      break;
   case 'Y':
      glRotated(-90.0,0.0,0.0,1.0);
      if ( 'X' == directionUp ) 
         glRotated(180.0,0.0,1.0,0.0);
      else 
         glRotated(-90.0,0.0,1.0,0.0);
      break;
   case 'Z':
      glRotated(-90.0,1.0,0.0,0.0);
      glRotated(90.0,0.0,1.0,0.0);
      if ( 'X' == directionUp ) 
         glRotated(180.0,0.0,0.0,1.0);
      else 
         glRotated(90.0,0.0,0.0,1.0);
      break;
   }

   glGetDoublev(GL_MODELVIEW_MATRIX,openGLState.modelMatrix);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   openGLState.extentsZMin = -1.0;
   openGLState.extentsZMax = 1.0;

   glOrtho(openGLState.extentsXMin,openGLState.extentsXMax,openGLState.extentsYMin,openGLState.extentsYMax,openGLState.extentsZMin,openGLState.extentsZMax);

   glGetDoublev(GL_PROJECTION_MATRIX,openGLState.projectionMatrix);

OPENGL_ERROR_CHECK

   initialized = true;

   openGLState.xScaleFactor = 1.0;
   openGLState.yScaleFactor = 1.0;
   openGLState.zScaleFactor = 1.0;

   isRendered = false;

   return S_OK;
   }


   HRESULT PlotWindow::setUp(IDataSet *pIMasterDataSet,
                                IGProperty *pPropPlotView,
                                IGProperty *pPropRotationTheta,
                                IGProperty *pPropRotationPhi,
                                IGProperty *pPropRotationSpin,
                                IGProperty *pPropLeftMargin,
                                IGProperty *pPropTopMargin,
                                IGProperty *pPropRightMargin,
                                IGProperty *pPropBottomMargin,
                                IGProperty *pPropMarginUnits,
                                IGProperty *pPropStretchToMargins) {

   RECT rect;
   char *pszUnits = NULL;
   long plotView;
   short stretchToMargins;
   double phi,theta,spin;

   GetWindowRect(hwnd,&rect);

   if ( ! renderingContext || ! ( ( rect.right - rect.left ) == openGLState.windowCX ) || ! ( ( rect.bottom - rect.top ) == openGLState.windowCY) ) {
      openGLState.windowCX = rect.right - rect.left;
      openGLState.windowCY = rect.bottom - rect.top;
      createRenderingContext();
   }

   openGLState.windowCX = rect.right - rect.left;
   openGLState.windowCY = rect.bottom - rect.top;
 
   if ( openGLState.windowCX == 0 || openGLState.windowCY == 0 ) 
      return S_OK;
 
   if ( pPropMarginUnits ) {
      long n;
      pPropMarginUnits -> get_size(&n);
      pszUnits = new char[n];
      pPropMarginUnits -> get_szValue(pszUnits);
      if ( ! pSaved_MarginUnits ) {
         pSaved_MarginUnits = pPropMarginUnits;
         pSaved_MarginUnits -> AddRef();
      }
   } else {
      if ( pSaved_MarginUnits ) {
         long n;
         pSaved_MarginUnits -> get_size(&n);
         pszUnits = new char[n];
         pSaved_MarginUnits -> get_szValue(pszUnits);
      }
      else
         pszUnits = NULL;
   }

   if ( pPropPlotView ) {
      pPropPlotView -> get_longValue(&plotView);
      if ( ! pSaved_PlotView ) {
         pSaved_PlotView = pPropPlotView;
         pSaved_PlotView -> AddRef();
      }
   } else {
      if ( pSaved_PlotView ) {
         pSaved_PlotView -> get_longValue(&plotView);
      } else
         plotView = gcPlotView3D;
   }
   
   if ( pPropRotationTheta ) {
      pPropRotationTheta -> get_doubleValue(&theta);
      if ( ! pSaved_RotationTheta ) {
         pSaved_RotationTheta = pPropRotationTheta;
         pSaved_RotationTheta -> AddRef();
      }
   } else {
      if ( pSaved_RotationTheta ) {
         pSaved_RotationTheta -> get_doubleValue(&theta);         
      } else
         theta = 0.0;
   }

   if ( pPropRotationPhi ) {
      pPropRotationPhi -> get_doubleValue(&phi);
      if ( ! pSaved_RotationPhi ) {
         pSaved_RotationPhi = pPropRotationPhi;
         pSaved_RotationPhi -> AddRef();
      }
   } else {
      if ( pSaved_RotationPhi ) {
         pSaved_RotationPhi -> get_doubleValue(&phi);
      } else
         phi = 0.0;
   }

   if ( pPropRotationSpin ) {
      pPropRotationSpin -> get_doubleValue(&spin);
      if ( ! pSaved_RotationSpin ) {
         pSaved_RotationSpin = pPropRotationSpin;
         pSaved_RotationSpin -> AddRef();
      }
   } else {
      if ( pSaved_RotationSpin ) {
         pSaved_RotationSpin -> get_doubleValue(&spin);
      } else 
         spin = 0.0;
   }

   if ( pPropLeftMargin ) {
      pPropLeftMargin -> get_longValue(&openGLState.viewPortMargins[0]);
      if ( pszUnits )
         if ( strcmp(pszUnits,"percent") == 0 ) 
            openGLState.viewPortMargins[0] = (long)((double)openGLState.windowCX * (double)openGLState.viewPortMargins[0] / 100.0);
      if ( ! pSaved_LeftMargin ) {
         pSaved_LeftMargin = pPropLeftMargin;
         pSaved_LeftMargin -> AddRef();
      }
   } else {
      if ( pSaved_LeftMargin ) {
         pSaved_LeftMargin -> get_longValue(&openGLState.viewPortMargins[0]);
         if ( pszUnits )
            if ( strcmp(pszUnits,"percent") == 0 ) 
               openGLState.viewPortMargins[0] = (long)((double)openGLState.windowCX * (double)openGLState.viewPortMargins[0] / 100.0);         
      } else
         openGLState.viewPortMargins[0] = 8;
   }

   if ( pPropTopMargin ) {
      pPropTopMargin -> get_longValue(&openGLState.viewPortMargins[3]);
      if ( pszUnits )
         if ( strcmp(pszUnits,"percent") == 0 ) 
            openGLState.viewPortMargins[3] = (long)((double)openGLState.windowCY * (double)openGLState.viewPortMargins[3] / 100.0);
      if ( ! pSaved_TopMargin ) {
         pSaved_TopMargin = pPropTopMargin;
         pSaved_TopMargin -> AddRef();
      }
   } else {
      if ( pSaved_TopMargin ) {
         pSaved_TopMargin -> get_longValue(&openGLState.viewPortMargins[3]);
         if ( pszUnits )
            if ( strcmp(pszUnits,"percent") == 0 ) 
               openGLState.viewPortMargins[3] = (long)((double)openGLState.windowCY * (double)openGLState.viewPortMargins[3] / 100.0);
      } else 
         openGLState.viewPortMargins[3] = 8;
   }

   if ( pPropRightMargin ) {
      pPropRightMargin -> get_longValue(&openGLState.viewPortMargins[2]);
      if ( pszUnits )
         if ( strcmp(pszUnits,"percent") == 0 ) 
            openGLState.viewPortMargins[2] = (long)((double)openGLState.windowCX * (double)openGLState.viewPortMargins[2] / 100.0);
      if ( ! pSaved_RightMargin ) {
         pSaved_RightMargin = pPropRightMargin;
         pSaved_RightMargin -> AddRef();
      }
   } else {
      if ( pSaved_RightMargin ) {
         pSaved_RightMargin -> get_longValue(&openGLState.viewPortMargins[2]);
         if ( pszUnits )
            if ( strcmp(pszUnits,"percent") == 0 ) 
               openGLState.viewPortMargins[2] = (long)((double)openGLState.windowCX * (double)openGLState.viewPortMargins[2] / 100.0);
      } else 
         openGLState.viewPortMargins[2] = 8;
   }

   if ( pPropBottomMargin ) {
      pPropBottomMargin -> get_longValue(&openGLState.viewPortMargins[1]);
      if ( pszUnits )
         if ( strcmp(pszUnits,"percent") == 0 ) 
            openGLState.viewPortMargins[1] = (long)((double)openGLState.windowCY * (double)openGLState.viewPortMargins[1] / 100.0);
      if ( ! pSaved_BottomMargin ) {
         pSaved_BottomMargin = pPropBottomMargin;
         pSaved_BottomMargin -> AddRef();
      }
   } else {
      if ( pSaved_BottomMargin ) {
         pSaved_BottomMargin -> get_longValue(&openGLState.viewPortMargins[1]);
         if ( pszUnits )
            if ( strcmp(pszUnits,"percent") == 0 ) 
               openGLState.viewPortMargins[1] = (long)((double)openGLState.windowCY * (double)openGLState.viewPortMargins[1] / 100.0);
      } else 
         openGLState.viewPortMargins[1] = 8;
   }

   if ( pszUnits ) 
      delete [] pszUnits;

   if ( pPropStretchToMargins ) {
      pPropStretchToMargins -> get_boolValue(&stretchToMargins);
      if ( ! pSaved_StretchToMargins ) {
         pSaved_StretchToMargins = pPropStretchToMargins;
         pSaved_StretchToMargins -> AddRef();
      }
   } else {
      if ( pSaved_StretchToMargins ) {
         pSaved_StretchToMargins -> get_boolValue(&stretchToMargins);
      } else
         pPropStretchToMargins = FALSE;
   }

   if ( pIMasterDataSet ) {
      pIMasterDataSet -> get_minX(&openGLState.extentsXMin);
      pIMasterDataSet -> get_minY(&openGLState.extentsYMin);
      pIMasterDataSet -> get_minZ(&openGLState.extentsZMin);
      pIMasterDataSet -> get_maxX(&openGLState.extentsXMax);
      pIMasterDataSet -> get_maxY(&openGLState.extentsYMax);
      pIMasterDataSet -> get_maxZ(&openGLState.extentsZMax);
   }

   setOpenGLMatrices(plotView,phi,theta,spin);

   isRendered = false;

   return S_OK;
   }


   void PlotWindow::setOpenGLMatrices(long plotView,double phi,double theta,double spin) {

   openGLState.viewPort[0] = (GLint)openGLState.viewPortMargins[0];
   openGLState.viewPort[1] = (GLint)openGLState.viewPortMargins[1];
   openGLState.viewPort[2] = (GLint)(openGLState.windowCX - openGLState.viewPortMargins[0] - openGLState.viewPortMargins[2]);
   openGLState.viewPort[3] = (GLint)(openGLState.windowCY - openGLState.viewPortMargins[1] - openGLState.viewPortMargins[3]);

   if ( 0.0 == piOver2 )
      piOver2 = 2.0 * atan(1.0);

   openGLState.rotationTheta = theta * piOver2 / 90.0;
   openGLState.rotationPhi = phi * piOver2 / 90.0;
   openGLState.rotationSpin = spin * piOver2 / 90.0;

   openGLState.plotView = plotView;

   glViewport(openGLState.viewPort[0],openGLState.viewPort[1],(GLsizei)openGLState.viewPort[2],(GLsizei)openGLState.viewPort[3]);

   if ( openGLState.extentsXMin == DBL_MAX || openGLState.extentsXMax == -DBL_MAX || openGLState.extentsYMin == DBL_MAX || 
         openGLState.extentsYMax == -DBL_MAX || openGLState.extentsZMin == DBL_MAX || openGLState.extentsZMax == -DBL_MAX ) 
      return;

   if ( openGLState.extentsZMin == openGLState.extentsZMax ) 
      openGLState.extentsZMax += 0.1 * openGLState.extentsZMin;

   if ( openGLState.extentsZMin == openGLState.extentsZMax ) 
      openGLState.extentsZMax = 1.0;

   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity();

   if ( plotView == gcPlotView3D && ( phi != 90.0 || theta != 270.0 || spin != 0.0 ) ) {

      openGLState.xScaleFactor = 2.0 / (openGLState.extentsXMax - openGLState.extentsXMin);
      openGLState.yScaleFactor = 2.0 / (openGLState.extentsYMax - openGLState.extentsYMin);
      openGLState.zScaleFactor = 2.0 / (openGLState.extentsZMax - openGLState.extentsZMin);

#if 1
      glRotated(phi,1.0,0.0,0.0);
      glRotated(-theta,0.0,1.0,0.0);
      glRotated(-90.0,1.0,0.0,0.0);
      glRotated(-90.0,0.0,0.0,1.0);
      glRotated(spin,1.0,1.0,1.0);

      glScaled(openGLState.xScaleFactor,openGLState.yScaleFactor,openGLState.zScaleFactor);

      glTranslated(-openGLState.extentsXMin,-openGLState.extentsYMin,-openGLState.extentsZMin);
#else

      GLfloat matrix[16];
      GLfloat eyePos[3] = {(GLfloat)(cos(openGLState.rotationPhi) * cos(openGLState.rotationTheta) * openGLState.extentsXMax),
                                 (GLfloat)(cos(openGLState.rotationPhi) * sin(openGLState.rotationTheta) * openGLState.extentsYMax),
                                 (GLfloat)(sin(openGLState.rotationPhi) * openGLState.extentsZMax)};
      GLfloat upVector[] = {0.0,0.0,1.0};
      GLfloat center[] = {(GLfloat)openGLState.extentsXMin,(GLfloat)openGLState.extentsYMin,(GLfloat)openGLState.extentsZMin};

      lookAtMatrix(eyePos,center,upVector,matrix);

      glLoadMatrixf(matrix);

      if ( ! ( 0.0 == spin ) )
         glRotated(spin,1.0,1.0,1.0);

      glScaled(openGLState.xScaleFactor,openGLState.yScaleFactor,openGLState.zScaleFactor);

#endif

   } else {

      openGLState.xScaleFactor = 1.0;
      openGLState.yScaleFactor = 1.0;
      openGLState.zScaleFactor = 1.0;

      openGLState.rotationTheta = 0.0;
      openGLState.rotationPhi = 0.0;
      openGLState.rotationSpin = 0.0;

   }

   glGetDoublev(GL_MODELVIEW_MATRIX,openGLState.modelMatrix);

   glMatrixMode(GL_PROJECTION);

   glLoadIdentity();

   if ( plotView == gcPlotView3D ) 
      glOrtho(-2.0,2.0,-2.0,2.0,-4.0,4.0);
   else
      glOrtho(openGLState.extentsXMin,openGLState.extentsXMax,openGLState.extentsYMin,openGLState.extentsYMax,-1.0,1.0);

   glGetDoublev(GL_PROJECTION_MATRIX,openGLState.projectionMatrix);

   char szTemp[64];

   if ( DBL_MAX != openGLState.extentsXMin && -DBL_MAX != openGLState.extentsXMin/* && fabs(extentsXMin - 1.7e–308) > 1000.0 */ ) {
      sprintf(szTemp,"MinX = %lf",openGLState.extentsXMin);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != openGLState.extentsXMax && -DBL_MAX != openGLState.extentsXMax/* && fabs(extentsXMax - 1.7e–308) > 1000.0 */ ) {
      sprintf(szTemp,"MaxX = %lf",openGLState.extentsXMax);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != openGLState.extentsYMin && -DBL_MAX != openGLState.extentsYMin/* && fabs(extentsYMin - 1.7e–308) > 1000.0 */) {
      sprintf(szTemp,"MinY = %lf",openGLState.extentsYMin);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != openGLState.extentsYMax && -DBL_MAX != openGLState.extentsYMax/* && fabs(extentsYMax - 1.7e–308) > 1000.0 */) {
      sprintf(szTemp,"MaxY = %lf",openGLState.extentsYMax);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != openGLState.extentsZMin && -DBL_MAX != openGLState.extentsZMin/* && fabs(extentsZMin - 1.7e–308) > 1000.0 */) {
      sprintf(szTemp,"MinZ = %lf",openGLState.extentsZMin);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != openGLState.extentsZMax && -DBL_MAX != openGLState.extentsZMax/* && fabs(extentsZMax - 1.7e–308) > 1000.0 */) {
      sprintf(szTemp,"MaxZ = %lf",openGLState.extentsZMax);
      eval(pIEvaluator,szTemp);
   }

   initialized = true;

//glBegin(GL_LINE_STRIP);
//glVertex3d(openGLState.extentsXMin,openGLState.extentsYMin,openGLState.extentsZMin);
//glVertex3d(openGLState.extentsXMax,openGLState.extentsYMin,openGLState.extentsZMin);
//glVertex3d(openGLState.extentsXMax,openGLState.extentsYMax,openGLState.extentsZMin);
//glVertex3d(openGLState.extentsXMin,openGLState.extentsYMax,openGLState.extentsZMin);
//glVertex3d(openGLState.extentsXMin,openGLState.extentsYMin,openGLState.extentsZMin);
//glEnd();

   return;
   }
 
 
   HRESULT PlotWindow::translate(DataPoint *dp) {
   glMatrixMode(GL_MODELVIEW);
   glTranslated(dp -> x,dp -> y,dp -> z);
   glGetDoublev(GL_MODELVIEW_MATRIX,openGLState.modelMatrix);
   return S_OK;
   }


   HRESULT PlotWindow::rotate(char axis,double angle) {
   glMatrixMode(GL_PROJECTION);
   switch ( axis ) {
   case 'x':
   case 'X':
      glRotated(angle,1.0,0.0,0.0);
      break;
   case 'y':
   case 'Y':
      glRotated(angle,0.0,1.0,0.0);
      break;
   case 'z':
   case 'Z':
      glRotated(angle,0.0,0.0,1.0);
      break;
   default:
      return E_INVALIDARG;
   }
   glGetDoublev(GL_PROJECTION_MATRIX,openGLState.projectionMatrix);
   return S_OK;
   }


   HRESULT PlotWindow::scale(DataPoint *dp) {
   glMatrixMode(GL_MODELVIEW);
   glScaled(dp -> x,dp -> y,dp -> z);
/*
   Not saving scale factors (int xScaleFactor, etc.). Investigate later
*/
   glGetDoublev(GL_MODELVIEW_MATRIX,openGLState.modelMatrix);
   return S_OK;
   }


   HRESULT PlotWindow::setViewPort(int *vp) {
   openGLState.viewPort[0] = vp[0];
   openGLState.viewPort[1] = vp[1];
   openGLState.viewPort[2] = vp[2];
   openGLState.viewPort[3] = vp[3];
   glViewport(openGLState.viewPort[0],openGLState.viewPort[1],(GLsizei)openGLState.viewPort[2],(GLsizei)openGLState.viewPort[3]);
   glMatrixMode(GL_MODELVIEW);
   glGetDoublev(GL_MODELVIEW_MATRIX,openGLState.modelMatrix);
   glMatrixMode(GL_PROJECTION);
   glGetDoublev(GL_PROJECTION_MATRIX,openGLState.projectionMatrix);
   return S_OK;
   }


   void PlotWindow::disableLighting() {

   for ( int k = 0; k < SUPPORTED_LIGHT_COUNT; k++ )
      glDisable(GL_LIGHT0 + k);

   glDisable(GL_LIGHTING);

   return;
   }


   HRESULT PlotWindow::setLighting(
                IGProperty *pPropLightEnabled[],
                IGProperty *pPropAmbientLight[],
                IGProperty *pPropDiffuseLight[],
                IGProperty *pPropSpecularLight[],
                IGProperty *pPropLightPos[],
                IGProperty *pPropCountLights,
                IGProperty *pPropShinyness) {

   memset(&openGLLighting,0,sizeof(lightingParameters));

   if ( ! pPropLightEnabled ) {
      disableLighting();
      return S_OK;
   }

   if ( pPropCountLights ) 
      pPropCountLights -> get_longValue(&openGLLighting.lightCount);
   else {
      disableLighting();
      return S_OK;
   }

   if ( 0 == openGLLighting.lightCount ) {
      disableLighting();
      return S_OK;
   }

   short anyEnabled = false;

   for ( int k = 0; k < openGLLighting.lightCount && k < openGLLighting.lightCount; k ++ ) {

      if ( ! pPropLightEnabled[k] )
         continue;

      pPropLightEnabled[k] -> get_boolValue(&anyEnabled);

      if ( anyEnabled )
         break;

   }

   if ( ! anyEnabled ) {
      openGLLighting.lightCount = 0;
      disableLighting();
      return S_OK;
   }

   for ( int k = 0; k < openGLLighting.lightCount && k < openGLLighting.lightCount; k ++ ) {

      if ( ! pPropLightEnabled[k] ) 
         continue;

      short b;

      pPropLightEnabled[k] -> get_boolValue(&b);

      if ( ! b ) 
         continue;

      if ( pPropAmbientLight && pPropAmbientLight[k] ) {
         BYTE *pb = (BYTE *)&openGLLighting.fvAmbientLight[k][0];
         pPropAmbientLight[k] -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
openGLLighting.fvAmbientLight[k][3] = 1.0;
         openGLLighting.hasAmbient[k] = false;
         if ( 0.0 < openGLLighting.fvAmbientLight[k][0] ||
               0.0 < openGLLighting.fvAmbientLight[k][1] ||
               0.0 < openGLLighting.fvAmbientLight[k][2] )
            openGLLighting.hasAmbient[k] = true;
      }

      if ( pPropDiffuseLight && pPropDiffuseLight[k] ) {
         BYTE *pb = (BYTE *)&openGLLighting.fvDiffuseLight[k][0];
         pPropDiffuseLight[k] -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
openGLLighting.fvDiffuseLight[k][3] = 1.0;
         openGLLighting.hasDiffuse[k] = false;
         if ( 0.0 < openGLLighting.fvDiffuseLight[k][0] ||
               0.0 < openGLLighting.fvDiffuseLight[k][1] ||
               0.0 < openGLLighting.fvDiffuseLight[k][2] )
            openGLLighting.hasDiffuse[k] = true;
      }

      if ( pPropSpecularLight && pPropSpecularLight[k] ) {
         BYTE *pb = (BYTE *)&openGLLighting.fvSpecularLight[k][0];
         pPropSpecularLight[k] -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
openGLLighting.fvSpecularLight[k][3] = 1.0;
         openGLLighting.hasSpecular[k] = false;
         if ( 0.0 < openGLLighting.fvSpecularLight[k][0] ||
               0.0 < openGLLighting.fvSpecularLight[k][1] ||
               0.0 < openGLLighting.fvSpecularLight[k][2] )
            openGLLighting.hasSpecular[k] = true;

      }

      if ( pPropLightPos && pPropLightPos[k] ) {
         memset(&openGLLighting.lightPosition[k],0,4 * sizeof(float));
         long n;
         pPropLightPos[k] -> get_size(&n);
         char *pszValues = new char[n];
         pPropLightPos[k] -> get_szValue(pszValues);
         if ( pszValues ) 
            openGLLighting.lightPosition[k][0] = (float)evalConsume(pIEvaluator,pszValues);
         if ( pszValues ) 
            openGLLighting.lightPosition[k][1] = (float)evalConsume(pIEvaluator,pszValues);
         if ( pszValues ) 
            openGLLighting.lightPosition[k][2] = (float)evalConsume(pIEvaluator,pszValues);
openGLLighting.lightPosition[k][3] = 1.0;
         delete [] pszValues;
      }

   }

   if ( pPropShinyness ) {
      openGLLighting.hasShinyness = true;
      pPropShinyness -> get_longValue(&openGLLighting.shinyness);
   }

   return S_OK;
   }


   void PlotWindow::enableLighting() {

   glShadeModel(GL_SMOOTH);
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);

   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,1.0f);

   for ( int k = 0; k < openGLLighting.lightCount && k < openGLLighting.lightCount; k ++ ) {

      if ( ! openGLLighting.hasAmbient[k] && ! openGLLighting.hasDiffuse[k] && ! openGLLighting.hasSpecular[k] ) {
         glDisable(GL_LIGHT0 + k);
         continue;
      }

      glEnable(GL_LIGHT0 + k);

      if ( openGLLighting.hasAmbient[k] ) 
         glLightfv(GL_LIGHT0 + k,GL_AMBIENT,openGLLighting.fvAmbientLight[k]);

      if ( openGLLighting.hasDiffuse[k] ) 
         glLightfv(GL_LIGHT0 + k,GL_DIFFUSE,openGLLighting.fvDiffuseLight[k]);

      if ( openGLLighting.hasSpecular[k] ) {
         glLightfv(GL_LIGHT0 + k,GL_SPECULAR,openGLLighting.fvSpecularLight[k]);
         glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,openGLLighting.fvSpecularLight[k]);
      }

      glLightfv(GL_LIGHT0 + k,GL_POSITION,openGLLighting.lightPosition[k]);
      //glLightf(GL_LIGHT0 + k,GL_SPOT_CUTOFF,90.0f);

   }

   if ( openGLLighting.hasShinyness ) {
      glMateriali(GL_FRONT,GL_SHININESS,openGLLighting.shinyness);
      glMateriali(GL_BACK,GL_SHININESS,openGLLighting.shinyness);
   }

   return;
   }
 

   HRESULT PlotWindow::erase(IGProperty *pPropBackgroundColor) {

   float fv[4];
   if ( pPropBackgroundColor ) {
      BYTE *pb = (BYTE *)fv;
      pPropBackgroundColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
   } else {
      if ( pSaved_BackgroundColor ) {
         BYTE *pb = (BYTE *)fv;
         pSaved_BackgroundColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
      } else {
         fv[0] = 1.0; 
         fv[1] = 1.0; 
         fv[2] = 1.0; 
         fv[3] = 0.0;
      }
   }

   glClearColor(fv[0],fv[1],fv[2],0.0);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   if ( ! pSaved_BackgroundColor )
      pSaved_BackgroundColor = pPropBackgroundColor;

   BYTE vb[3];
   vb[0] = (BYTE)(255.0f*fv[0]);
   vb[1] = (BYTE)(255.0f*fv[1]);
   vb[2] = (BYTE)(255.0f*fv[2]);

   HBRUSH hb = CreateSolidBrush(RGB(vb[0],vb[1],vb[2]));

   RECT rc = {0};

   GetClientRect(hwnd,&rc);

   SelectObject(deviceContext,hb);

   FillRect(deviceContext,&rc,hb);

   return S_OK;
   }
 
 
   HRESULT PlotWindow::resetDepth() {
   glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();
   return S_OK;
   }


   HRESULT PlotWindow::getPickBoxHits(POINTL *ptl,long pickWindowWidth,long pickWindowHeight,unsigned int *pHitTable,long hitTableSize,long *pCallLists,unsigned int *pHitTableHits) {

   if ( lineMode || polygonMode ) 
      return S_FALSE;

   double x,y;
 
   x = (double)ptl -> x;

   y = (double)openGLState.windowCY - (double)ptl -> y;
 
   if ( ptl -> x < openGLState.viewPort[0] )
      return S_FALSE;

   if ( ptl -> x > (openGLState.viewPort[0] + openGLState.viewPort[2]) )
      return S_FALSE;

   if ( (long)y < openGLState.viewPort[1] )
      return S_FALSE;

   if ( (long)y > (openGLState.viewPort[1] + openGLState.viewPort[3]) )
      return S_FALSE;

   long plotView = (long)gcPlotView2D; 

   if ( pSaved_PlotView )
      pSaved_PlotView -> get_longValue(&plotView);

   memset(pHitTable,0,hitTableSize * sizeof(unsigned int));

   *pHitTableHits = 0;
 
   glMatrixMode(GL_PROJECTION);

   glPushMatrix();

   glSelectBuffer(hitTableSize,pHitTable);

   glRenderMode(GL_SELECT);
 
   glInitNames();

   glPushName((GLuint)-1);
 
   glLoadIdentity();

   gluPickMatrix(x,y,pickWindowWidth,pickWindowHeight,openGLState.viewPort);

   if ( gcPlotView3D == plotView )
      glOrtho(-2.0,2.0,-2.0,2.0,-4.0,4.0);
   else
      glOrtho(openGLState.extentsXMin,openGLState.extentsXMax,openGLState.extentsYMin,openGLState.extentsYMax,openGLState.extentsZMin,openGLState.extentsZMax);

   long *pList = pCallLists;

   while ( *pList ) {
      glLoadName(*pList);
      glCallList(*pList);
      pList++;
   }

   *pHitTableHits = glRenderMode(GL_RENDER);

   glMatrixMode(GL_PROJECTION);

   glPopMatrix();

   return S_OK;
   }


   HBITMAP PlotWindow::getMergedBackground(HWND hwndTarget,HDC hdc) {

   GLint bufferSize[4] = {0};

   glGetIntegerv(GL_SCISSOR_BOX,bufferSize);

   long cxBuffer = bufferSize[2];
   long cyBuffer = bufferSize[3];

   long cxWindow = cxBuffer;
   long cyWindow = cyBuffer;

   long pixelCount = cxBuffer * cyBuffer;

   float *pPixelsFloat = new float[4 * pixelCount];

   memset(pPixelsFloat,0,4 * pixelCount * sizeof(float));

   GetPixels(0,0,cxBuffer,cyBuffer,cyBuffer,(BYTE *)pPixelsFloat);

   float *pf = pPixelsFloat;

   BYTE *pPixels = new BYTE[4 * pixelCount];

   memset(pPixels,0,4 * pixelCount * sizeof(BYTE));

   BYTE *b = pPixels;

   for ( long j = 0; j < cyBuffer; j++ ) {
      for ( long k = 0; k < cxBuffer; k++ ) {
         b[2] = (BYTE)(255.0 * pf[0]);
         b[1] = (BYTE)(255.0 * pf[1]);
         b[0] = (BYTE)(255.0 * pf[2]);
         b[3] = (BYTE)(255.0 * pf[3]);
         pf += 4;
         b += 4;
      }
   }   

   BITMAP bitmap = {0};

   {

   HBITMAP hbmReference = CreateCompatibleBitmap(hdc,cxWindow,cyWindow);

   GetObject(hbmReference,sizeof(BITMAP),&bitmap);

   DeleteObject(hbmReference);

   }

   BITMAPINFO bitmapInfo = {0};

   bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

   bitmapInfo.bmiHeader.biBitCount = bitmap.bmBitsPixel;
   bitmapInfo.bmiHeader.biHeight = -bitmap.bmHeight;
   bitmapInfo.bmiHeader.biWidth = bitmap.bmWidth;
   bitmapInfo.bmiHeader.biPlanes = 1;
   bitmapInfo.bmiHeader.biCompression = BI_RGB;
   bitmapInfo.bmiHeader.biSizeImage = -bitmap.bmHeight * ((bitmap.bmWidth * bitmap.bmPlanes * bitmap.bmBitsPixel + 31) & ~31) / 8;
   
   HBITMAP hbmOpenGL = CreateCompatibleBitmap(hdc,cxBuffer,cyBuffer);

   SetDIBits(hdc,hbmOpenGL,0,cyBuffer,(void *)pPixels,&bitmapInfo,DIB_RGB_COLORS);

   HDC hdcOpenGL = CreateCompatibleDC(hdc);

   HGDIOBJ oldBitmapOpenGL = SelectObject(hdcOpenGL,hbmOpenGL);

   // hdcOpenGL now contains the openGL rendering, but not the GDI 

   HBITMAP hbmMerged = CreateCompatibleBitmap(hdc,cxWindow,cyWindow);

   {

   HDC hdcMerged = CreateCompatibleDC(hdc);

   HGDIOBJ oldBitmap = SelectObject(hdcMerged,hbmMerged);

   //BitBlt(hdcMerged,0,0,cxWindow,cyWindow,hdc,0,0,SRCCOPY);

   BitBlt(hdcMerged,0,0,cxBuffer,cyBuffer,hdcOpenGL,0,0,SRCCOPY);

   SelectObject(hdcMerged,oldBitmap);

   DeleteDC(hdcMerged);

   }

   delete [] pPixelsFloat;

   delete [] pPixels;

   SelectObject(hdcOpenGL,oldBitmapOpenGL);

   DeleteDC(hdcOpenGL);

   DeleteObject(hbmOpenGL);

   return hbmMerged;
   }


   HRESULT PlotWindow::GetPixels(long x1,long y1,long x2,long y2,long cyWindow,BYTE *pResult) {

   long cx = x2 - x1;
   long cy = y2 - y1;

   long rowSize = 4 * sizeof(GLfloat) * cx;

   BYTE *pSwap = new BYTE[cy * rowSize];

   memset(pSwap,0,cy * rowSize);

glGetError();
   glReadPixels(x1,cyWindow - y2,cx,cy,GL_RGBA,GL_FLOAT,pSwap);
OPENGL_ERROR_CHECK

   for ( int k = 0; k < cy; k++ ) 
      memcpy(pResult + k * rowSize,pSwap + (cy - k - 1) * rowSize,rowSize);

   delete [] pSwap;

   return S_OK;
   }


   void PlotWindow::finalize() {

   HBITMAP hbmMerged = getMergedBackground(hwnd,deviceContext);

   HDC hdcSource = CreateCompatibleDC(deviceContext);
   HGDIOBJ oldBitmap = SelectObject(hdcSource,hbmMerged);
   BitBlt(deviceContext,0,0,openGLState.windowCX,openGLState.windowCY,hdcSource,0,0,SRCCOPY);
   SelectObject(hdcSource,oldBitmap);
   DeleteDC(hdcSource);
   DeleteObject(hbmMerged);

   isRendered = true;

   return;
   }


   void lookAtMatrix(GLfloat *pEyePos,GLfloat *pCenter,GLfloat *pUpVector,GLfloat *pResult) {

   GLfloat forward[3],side[3],up[3];

   forward[0] = pCenter[0] - pEyePos[0];
   forward[1] = pCenter[1] - pEyePos[1];
   forward[2] = pCenter[2] - pEyePos[2];

   unitVector(forward,forward);

   // Side = forward x up

   VxV(forward,pUpVector,side);

   unitVector(side,side);

   // Recompute up as: up = side x forward

   VxV(side,forward,up);

   memset(pResult,0,16 * sizeof(GLfloat));

   pResult[0] = side[0];
   pResult[4] = side[1];
   pResult[8] = side[2];
   pResult[12] = 0.0;

   pResult[1] = up[0];
   pResult[5] = up[1];
   pResult[9] = up[2];
   pResult[13] = 0.0;

   pResult[2] = -forward[0];
   pResult[6] = -forward[1];
   pResult[10] = -forward[2];
   pResult[14] = 0.0;

   pResult[3] = pResult[7];
   pResult[11] = 0.0;
   pResult[15] = 1.0;

   return;
   }
