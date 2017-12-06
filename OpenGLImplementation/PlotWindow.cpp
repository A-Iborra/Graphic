/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "general.h"
#include "Graphic_resource.h"
#include "utils.h"

#include "GraphicControl_i.h"
#include "OpenGLImplementor.h"

   PlotWindow::PlotWindow(HWND h,OpenGLImplementor *pp,IEvaluator *piev) :
       deviceContext(0),
       hwnd(h),
       pParent(pp),
       pIEvaluator(piev),
       initialized(false),
       lineMode(false),
       polygonMode(false),
       dxleft(0),dytop(0),dxright(0),dybottom(0),

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

       pSaved_BackgroundColor(NULL),

       xScaleFactor(1.0),yScaleFactor(1.0),zScaleFactor(1.0) {

   extentsXMin = extentsYMin = extentsZMin = DBL_MAX;
   extentsXMax = extentsYMax = extentsZMax = -DBL_MAX;

   memset(viewPort,0,sizeof(viewPort));
   memset(projectionMatrix,0,sizeof(projectionMatrix));
   memset(modelMatrix,0,sizeof(modelMatrix));
   memset(fvAmbientLight,0,sizeof(fvAmbientLight));
   memset(fvSpecularLight,0,sizeof(fvSpecularLight));
   memset(fvDiffuseLight,0,sizeof(fvDiffuseLight));

   shinyness = 25;

   RECT rect;
   GetWindowRect(h,&rect);
   windowCX = rect.right - rect.left;
   windowCY = rect.bottom - rect.top;

   if ( pParent ) {

      PlotWindow *ppw = pParent -> GetPlotWindow();

      if ( ppw ) {

         transformationMatrixes tm;

         ppw -> saveState(&tm);

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

         restoreState(&tm);

      }

   }

   setRenderingContext();

   return;
   };
 
 
   PlotWindow::~PlotWindow() {
   unsetViewProperties();
   return;
   };
 
 
   int PlotWindow::setRenderingContext() {

   int pixelFormat;

   PIXELFORMATDESCRIPTOR pfd = {
       sizeof(PIXELFORMATDESCRIPTOR),
       1,
       PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | 0 * PFD_DOUBLEBUFFER, 
       PFD_TYPE_RGBA,   
       16,   
       0, 0, 0,
       0, 0, 0,
       0,   0,   
       0,   0, 0, 0, 0,
       32,   
       0,   
       0,   
       PFD_MAIN_PLANE,   
       0,   
       0, 0, 0 };
 
   deviceContext = GetDC(hwnd);
 
   pixelFormat = ChoosePixelFormat(deviceContext,&pfd);

   if ( ! pixelFormat ) {
      pfd.dwFlags = pfd.dwFlags & ~ PFD_GENERIC_ACCELERATED;
      pixelFormat = ChoosePixelFormat(deviceContext,&pfd);
   }

/* 
   pixelFormat = 0;
   while ( pixelFormat < 48 ) {
      pixelFormat++;
      DescribePixelFormat(deviceContext,pixelFormat,sizeof(PIXELFORMATDESCRIPTOR),&pfd);
      if ( pfd.dwFlags & PFD_GENERIC_FORMAT && ( pfd.dwFlags & PFD_GENERIC_ACCELERATED ) ) break;
   }
*/ 
   SetPixelFormat(deviceContext,pixelFormat,&pfd);
/*   
   DescribePixelFormat(deviceContext,pixelFormat,sizeof(PIXELFORMATDESCRIPTOR),&pfd);
 
   if ( pfd.cColorBits < 16 ) {
      MessageBox(NULL,"Your display must be set in a higher color mode\r\nPlease re-configure your display for more colors","Note",MB_OK);
      renderingContext = 0;
      return FALSE;
   }
*/ 
   renderingContext = wglCreateContext(deviceContext);

   wglMakeCurrent(deviceContext,renderingContext);

   return TRUE;
   }
 
 
   int PlotWindow::saveState(transformationMatrixes* tm) {

glGetError();
   glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
   glGetDoublev(GL_PROJECTION_MATRIX,projectionMatrix);
   glGetIntegerv(GL_VIEWPORT,viewPort);
OPENGL_ERROR_CHECK

   if ( tm ) {
      memcpy(tm -> projectionMatrix,projectionMatrix,sizeof(tm -> projectionMatrix));
      memcpy(tm -> modelMatrix,modelMatrix,sizeof(tm -> modelMatrix));

      tm -> viewPortMargins[0] = viewPort[0];
      tm -> viewPortMargins[1] = viewPort[1];
      tm -> viewPortMargins[2] = windowCX - viewPort[2] - viewPort[0];
      tm -> viewPortMargins[3] = windowCY - viewPort[3] - viewPort[1];

      tm -> windowCX = windowCX;
      tm -> windowCY = windowCY;

      tm -> xScaleFactor = xScaleFactor;
      tm -> yScaleFactor = yScaleFactor;
      tm -> zScaleFactor = zScaleFactor;
      
      tm -> extentsXMin = extentsXMin;
      tm -> extentsYMin = extentsYMin;
      tm -> extentsZMin = extentsZMin;
      tm -> extentsXMax = extentsXMax;
      tm -> extentsYMax = extentsYMax;
      tm -> extentsZMax = extentsZMax;

   }

   return 0;
   }
 
 
   int PlotWindow::restoreState(transformationMatrixes* tm) {

glGetError();

   if ( tm ) {
      memcpy(projectionMatrix,tm -> projectionMatrix,sizeof(tm -> projectionMatrix));
      memcpy(modelMatrix,tm -> modelMatrix,sizeof(tm -> modelMatrix));

      viewPort[0] = (long)((double)windowCX * (double)(tm -> viewPortMargins[0]) / (double)(tm -> windowCX));
      viewPort[1] = (long)((double)windowCY * (double)(tm -> viewPortMargins[1]) / (double)(tm -> windowCY));

      RECT rect;
      GetWindowRect(hwnd,&rect);

      viewPort[2] = (rect.right - rect.left) - viewPort[0] - (long)((double)(rect.right - rect.left) * (double)(tm -> viewPortMargins[2]) / (double)(tm -> windowCX));
      viewPort[3] = (rect.bottom - rect.top) - viewPort[1] - (long)((double)(rect.bottom - rect.top) * (double)(tm -> viewPortMargins[3]) / (double)(tm -> windowCY));

      xScaleFactor = tm -> xScaleFactor;
      yScaleFactor = tm -> yScaleFactor;
      zScaleFactor = tm -> zScaleFactor;
      
      extentsXMin = tm -> extentsXMin;
      extentsYMin = tm -> extentsYMin;
      extentsZMin = tm -> extentsZMin;
      extentsXMax = tm -> extentsXMax;
      extentsYMax = tm -> extentsYMax;
      extentsZMax = tm -> extentsZMax;


   }

   glViewport((GLint)viewPort[0],(GLint)viewPort[1],(GLsizei)viewPort[2],(GLsizei)viewPort[3]);

   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixd(modelMatrix);

   glMatrixMode(GL_PROJECTION);
   glLoadMatrixd(projectionMatrix);

OPENGL_ERROR_CHECK

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

   viewPort[0] = margins;
   viewPort[1] = margins;
   viewPort[2] = windowCX - 2 * margins;
   viewPort[3] = windowCY - 2 * margins;

   glViewport(viewPort[0],viewPort[1],viewPort[2],viewPort[3]);
OPENGL_ERROR_CHECK

   DataPoint dp[2];
   masterDataSet -> GetDomain(&dp[0],&dp[1]);
   extentsXMin = dp[0].x;
   extentsYMin = dp[0].y;
   extentsZMin = dp[0].z;
   extentsXMax = dp[1].x;
   extentsYMax = dp[1].y;
   extentsZMax = dp[1].z;
 
   if ( extentsZMin == extentsZMax ) extentsZMax += 0.1 * extentsZMin;
   if ( extentsZMin == extentsZMax ) extentsZMax = 1.0;

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

   glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   extentsZMin = -1.0;
   extentsZMax = 1.0;

   glOrtho(extentsXMin,extentsXMax,extentsYMin,extentsYMax,extentsZMin,extentsZMax);

   glGetDoublev(GL_PROJECTION_MATRIX,projectionMatrix);
OPENGL_ERROR_CHECK

   initialized = true;

   xScaleFactor = 1.0;
   yScaleFactor = 1.0;
   zScaleFactor = 1.0;

   return S_OK;
   }


   HRESULT PlotWindow::setUp(IDataSet *masterDataSet,
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
glGetError();

   RECT rect;
   char *pszUnits;
   long plotView;
   short stretchToMargins;
   double phi,theta,spin;

   GetWindowRect(hwnd,&rect);
 
   windowCX = rect.right - rect.left;
   windowCY = rect.bottom - rect.top;
 
   if ( windowCX == 0 || windowCY == 0 ) return S_OK;
 
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
      pPropLeftMargin -> get_longValue(&dxleft);
      if ( pszUnits )
         if ( strcmp(pszUnits,"percent") == 0 ) 
            dxleft = (long)((double)windowCX * (double)dxleft / 100.0);
      if ( ! pSaved_LeftMargin ) {
         pSaved_LeftMargin = pPropLeftMargin;
         pSaved_LeftMargin -> AddRef();
      }
   } else {
      if ( pSaved_LeftMargin ) {
         pSaved_LeftMargin -> get_longValue(&dxleft);
         if ( pszUnits )
            if ( strcmp(pszUnits,"percent") == 0 ) 
               dxleft = (long)((double)windowCX * (double)dxleft / 100.0);         
      } else
         dxleft = 8;
   }

   if ( pPropTopMargin ) {
      pPropTopMargin -> get_longValue(&dytop);
      if ( pszUnits )
         if ( strcmp(pszUnits,"percent") == 0 ) 
            dytop = (long)((double)windowCY * (double)dytop / 100.0);
      if ( ! pSaved_TopMargin ) {
         pSaved_TopMargin = pPropTopMargin;
         pSaved_TopMargin -> AddRef();
      }
   } else {
      if ( pSaved_TopMargin ) {
         pSaved_TopMargin -> get_longValue(&dytop);
         if ( pszUnits )
            if ( strcmp(pszUnits,"percent") == 0 ) 
               dytop = (long)((double)windowCY * (double)dytop / 100.0);
      } else 
         dytop = 8;
   }

   if ( pPropRightMargin ) {
      pPropRightMargin -> get_longValue(&dxright);
      if ( pszUnits )
         if ( strcmp(pszUnits,"percent") == 0 ) 
            dxright = (long)((double)windowCX * (double)dxright / 100.0);
      if ( ! pSaved_RightMargin ) {
         pSaved_RightMargin = pPropRightMargin;
         pSaved_RightMargin -> AddRef();
      }
   } else {
      if ( pSaved_RightMargin ) {
         pSaved_RightMargin -> get_longValue(&dxright);
         if ( pszUnits )
            if ( strcmp(pszUnits,"percent") == 0 ) 
               dxright = (long)((double)windowCX * (double)dxright / 100.0);
      } else 
         dxright = 8;
   }

   if ( pPropBottomMargin ) {
      pPropBottomMargin -> get_longValue(&dybottom);
      if ( pszUnits )
         if ( strcmp(pszUnits,"percent") == 0 ) 
            dybottom = (long)((double)windowCY * (double)dybottom / 100.0);
      if ( ! pSaved_BottomMargin ) {
         pSaved_BottomMargin = pPropBottomMargin;
         pSaved_BottomMargin -> AddRef();
      }
   } else {
      if ( pSaved_BottomMargin ) {
         pSaved_BottomMargin -> get_longValue(&dybottom);
         if ( pszUnits )
            if ( strcmp(pszUnits,"percent") == 0 ) 
               dybottom = (long)((double)windowCY * (double)dybottom / 100.0);
      } else 
         dybottom = 8;
   }

   if ( pszUnits ) delete [] pszUnits;

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

   viewPort[0] = dxleft;
   viewPort[1] = dybottom;
   viewPort[2] = windowCX - dxleft - dxright;
   viewPort[3] = windowCY - dybottom - dytop;

   if ( viewPort[2] < 1 ) return S_OK;
   if ( viewPort[3] < 1 ) return S_OK;
 
   glViewport(viewPort[0],viewPort[1],viewPort[2],viewPort[3]);

   masterDataSet -> get_minX(&extentsXMin);
   masterDataSet -> get_minY(&extentsYMin);
   masterDataSet -> get_minZ(&extentsZMin);
   masterDataSet -> get_maxX(&extentsXMax);
   masterDataSet -> get_maxY(&extentsYMax);
   masterDataSet -> get_maxZ(&extentsZMax);
 
   if ( extentsXMin == DBL_MAX || extentsXMax == -DBL_MAX || extentsYMin == DBL_MAX || 
         extentsYMax == -DBL_MAX || extentsZMin == DBL_MAX || extentsZMax == -DBL_MAX ) return E_FAIL;

   if ( extentsZMin == extentsZMax ) extentsZMax += 0.1 * extentsZMin;
   if ( extentsZMin == extentsZMax ) extentsZMax = 1.0;

   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity();

OPENGL_ERROR_CHECK

   if ( plotView == gcPlotView3D && ( phi != 90.0 || theta != 270.0 || spin != 0.0 ) ) {

      xScaleFactor = 2.0 / (extentsXMax - extentsXMin);
      yScaleFactor = 2.0 / (extentsYMax - extentsYMin);
      zScaleFactor = 2.0 / (extentsZMax - extentsZMin);

      glRotated(phi,1.0,0.0,0.0);
      glRotated(-theta,0.0,1.0,0.0);
      glRotated(-90.0,1.0,0.0,0.0);
      glRotated(-90.0,0.0,0.0,1.0);
      glRotated(spin,1.0,1.0,1.0);

      glScaled(xScaleFactor,yScaleFactor,zScaleFactor);

      glTranslated(-extentsXMin,-extentsYMin,-extentsZMin);

   } else {

      xScaleFactor = 1.0;
      yScaleFactor = 1.0;
      zScaleFactor = 1.0;

   }

   glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);

   glMatrixMode(GL_PROJECTION);

   glLoadIdentity();

   if ( plotView == gcPlotView3D ) 
      glOrtho(-2.0,2.0,-2.0,2.0,-4.0,4.0);
   else
      glOrtho(extentsXMin,extentsXMax,extentsYMin,extentsYMax,-1.0,1.0);//extentsZMin,extentsZMax);//-10000.0,10000.0);//-200.0 * fabs(extentsZMin),200.0 * fabs(extentsZMax));

   glGetDoublev(GL_PROJECTION_MATRIX,projectionMatrix);

   char szTemp[64];

   if ( DBL_MAX != extentsXMin && -DBL_MAX != extentsXMin/* && fabs(extentsXMin - 1.7e–308) > 1000.0 */ ) {
      sprintf(szTemp,"MinX = %lf",extentsXMin);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != extentsXMax && -DBL_MAX != extentsXMax/* && fabs(extentsXMax - 1.7e–308) > 1000.0 */ ) {
      sprintf(szTemp,"MaxX = %lf",extentsXMax);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != extentsYMin && -DBL_MAX != extentsYMin/* && fabs(extentsYMin - 1.7e–308) > 1000.0 */) {
      sprintf(szTemp,"MinY = %lf",extentsYMin);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != extentsYMax && -DBL_MAX != extentsYMax/* && fabs(extentsYMax - 1.7e–308) > 1000.0 */) {
      sprintf(szTemp,"MaxY = %lf",extentsYMax);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != extentsZMin && -DBL_MAX != extentsZMin/* && fabs(extentsZMin - 1.7e–308) > 1000.0 */) {
      sprintf(szTemp,"MinZ = %lf",extentsZMin);
      eval(pIEvaluator,szTemp);
   }
   if ( DBL_MAX != extentsZMax && -DBL_MAX != extentsZMax/* && fabs(extentsZMax - 1.7e–308) > 1000.0 */) {
      sprintf(szTemp,"MaxZ = %lf",extentsZMax);
      eval(pIEvaluator,szTemp);
   }

   initialized = true;

   return S_OK;
   }
 
 
   HRESULT PlotWindow::translate(DataPoint *dp) {
   glMatrixMode(GL_MODELVIEW);
   glTranslated(dp -> x,dp -> y,dp -> z);
   glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
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
   glGetDoublev(GL_PROJECTION_MATRIX,projectionMatrix);
   return S_OK;
   }


   HRESULT PlotWindow::scale(DataPoint *dp) {
   glMatrixMode(GL_MODELVIEW);
   glScaled(dp -> x,dp -> y,dp -> z);
/*
   Not saving scale factors (int xScaleFactor, etc.). Investigate later
*/
   glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
   return S_OK;
   }


   HRESULT PlotWindow::setViewPort(int *vp) {
   viewPort[0] = vp[0];
   viewPort[1] = vp[1];
   viewPort[2] = vp[2];
   viewPort[3] = vp[3];
   glViewport(viewPort[0],viewPort[1],viewPort[2],viewPort[3]);
   glMatrixMode(GL_MODELVIEW);
   glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
   glMatrixMode(GL_PROJECTION);
   glGetDoublev(GL_PROJECTION_MATRIX,projectionMatrix);
   return S_OK;
   }


   HRESULT PlotWindow::setLighting(
                IGProperty *pPropLightEnabled[],
                IGProperty *pPropAmbientLight[],
                IGProperty *pPropDiffuseLight[],
                IGProperty *pPropSpecularLight[],
                IGProperty *pPropLightPos[],
                IGProperty *pPropCountLights,
                IGProperty *pPropShinyness){

   if ( ! pPropLightEnabled ) {
      for ( int k = 0; k < SUPPORTED_LIGHT_COUNT; k++ )
         glDisable(GL_LIGHT0 + k);
      return S_OK;
   }

   long lightCount;

   if ( pPropCountLights ) 
      pPropCountLights -> get_longValue(&lightCount);
   else {
      for ( int k = 0; k < SUPPORTED_LIGHT_COUNT; k++ )
         glDisable(GL_LIGHT0 + k);
      return S_OK;
   }

   if ( ! lightCount ) {
      for ( int k = 0; k < SUPPORTED_LIGHT_COUNT; k++ )
         glDisable(GL_LIGHT0 + k);
      return S_OK;
   }

   short b;

   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,0);
//   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
 
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,1.0f);
OPENGL_ERROR_CHECK

   for ( int k = 0; k < lightCount && k < lightCount; k ++ ) {

      if ( pPropLightEnabled[k] ) {

         pPropLightEnabled[k] -> get_boolValue(&b);

         if ( b ) {

            glEnable(GL_LIGHT0 + k);

            if ( pPropAmbientLight ) {
               if ( pPropAmbientLight[k] ) {
                  BYTE *pb = (BYTE *)&fvAmbientLight[k][0];
                  pPropAmbientLight[k] -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
                  glLightfv(GL_LIGHT0 + k,GL_AMBIENT,fvAmbientLight[k]);
               }
            }

            if ( pPropDiffuseLight ) {
               if ( pPropDiffuseLight[k] ) {
                  BYTE *pb = (BYTE *)&fvDiffuseLight[k][0];
                  pPropDiffuseLight[k] -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
                  glLightfv(GL_LIGHT0 + k,GL_DIFFUSE,fvDiffuseLight[k]);
               }
            }

            if ( pPropSpecularLight ) {
               if ( pPropSpecularLight[k] ) {
                  BYTE *pb = (BYTE *)&fvSpecularLight[k][0];
                  pPropSpecularLight[k] -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
                  glLightfv(GL_LIGHT0 + k,GL_SPECULAR,fvSpecularLight[k]);
               }
            }

            if ( pPropLightPos ) {
               if ( pPropLightPos[k] ) {
                  memset(&lightPosition[k],0,4 * sizeof(float));
                  long n;
                  pPropLightPos[k] -> get_size(&n);
                  char *pszValues = new char[n];
                  pPropLightPos[k] -> get_szValue(pszValues);
                  if ( pszValues ) 
                     lightPosition[k][0] = (float)evalConsume(pIEvaluator,pszValues);
                  if ( pszValues ) 
                     lightPosition[k][1] = (float)evalConsume(pIEvaluator,pszValues);
                  if ( pszValues ) 
                     lightPosition[k][2] = (float)evalConsume(pIEvaluator,pszValues);
                  delete [] pszValues;
                  glLightfv(GL_LIGHT0 + k,GL_POSITION,lightPosition[k]);
                  glLightf(GL_LIGHT0 + k,GL_SPOT_CUTOFF,90.0f);
               }
            }

         }

         else
            glDisable(GL_LIGHT0 + k);

      }

   }

   if ( pPropShinyness ) {
      pPropShinyness -> get_longValue(&shinyness);
      glMateriali(GL_FRONT,GL_SHININESS,shinyness);
   }

OPENGL_ERROR_CHECK
   return S_OK;

   }
 
 
   HRESULT PlotWindow::erase(IGProperty *pPropBackgroundColor) {

   float fv[4];
   if ( pPropBackgroundColor ) {
      BYTE *pb = (BYTE *)fv;
      pPropBackgroundColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
   }
   else {
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
   glFlush();

   if ( ! pSaved_BackgroundColor )
      pSaved_BackgroundColor = pPropBackgroundColor;

   return S_OK;
   }
 
 
   HRESULT PlotWindow::resetDepth() {
   glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();
   return S_OK;
   }


   HRESULT PlotWindow::getPickBoxHits(POINT *ptl,long pickWindowSize,unsigned int *hitTable,long hitTableSize,long *pCallLists,unsigned int *hitTableHits) {

   if ( lineMode || polygonMode ) return S_FALSE;

   double x,y;
 
   x = (double)ptl -> x;
   y = (double)windowCY - (double)ptl -> y;
 
   if ( (long)x < viewPort[0] ) return S_FALSE;
   if ( (long)x > (viewPort[0] + viewPort[2]) ) return S_FALSE;
   if ( (long)y < viewPort[1] ) return S_FALSE;
   if ( (long)y > (viewPort[1] + viewPort[3]) ) return S_FALSE;
 
   memset(hitTable,0,hitTableSize * sizeof(unsigned int));

   *hitTableHits = 0;
 
   glMatrixMode(GL_PROJECTION);

   glPushMatrix();

   glSelectBuffer(hitTableSize,hitTable);

   glRenderMode(GL_SELECT);
 
   glInitNames();

   glPushName((GLuint)-1);
 
   glLoadIdentity();

   gluPickMatrix(x,y,pickWindowSize,pickWindowSize,viewPort);

   glOrtho(extentsXMin,extentsXMax,extentsYMin,extentsYMax,extentsZMin,extentsZMax);

   long *pList = pCallLists;
   while ( *pList ) {
      glLoadName(*pList);
      glCallList(*pList++);
   }

   *hitTableHits = glRenderMode(GL_RENDER);

   glMatrixMode(GL_PROJECTION);

   glPopMatrix();

   return S_OK;
   }
