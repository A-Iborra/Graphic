// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"

#include <gl\glu.h>

#include "Graphic_resource.h"

#include "utils.h"
#include "Properties_i.h"
#include "OpenGLImplementor.h"

   unsigned int __stdcall OpenGLImplementor::runImplementationMsgQueue(void *pt) {

   CoInitialize(NULL);

   OpenGLImplementor* p = reinterpret_cast<OpenGLImplementor*>(pt);

   WNDCLASS wClass;
   memset(&wClass,0,sizeof(WNDCLASS));
   wClass.style = 0L;
   wClass.lpfnWndProc = OpenGLImplementor::openglImplementationHandler;
   wClass.hInstance = (HINSTANCE)hModule;
   wClass.lpszClassName = "OpenGL Implementor";
   wClass.cbClsExtra = 32;
   wClass.cbWndExtra = 32;

   RegisterClass(&wClass);

   p -> hwndObjectWindow = CreateWindow("OpenGL Implementor","OpenGL Implementor",0L,0,0,0,0,NULL,NULL,(HINSTANCE)hModule,(void *)p);

   PostMessage(p -> hwndObjectWindow,WM_USER,0L,(LPARAM)p -> hCreationSemaphore);

#if 0
   p -> hBlockingMutex = CreateMutex(NULL,FALSE,NULL);
#endif

   MSG msg;
   while ( GetMessage(&msg,NULL,0L,0L) ) {
      if ( PeekMessage(&msg,NULL,WM_QUIT,WM_QUIT,PM_REMOVE) ) break;
      DispatchMessage(&msg);
   }

#if 0
   ReleaseSemaphore(p -> hBlockingSemaphore,1,NULL);
#endif

   DestroyWindow(p -> hwndObjectWindow);

   p -> hwndObjectWindow = NULL;

   CoUninitialize();

   return 0;
   }


   LRESULT OpenGLImplementor::openglImplementationHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   OpenGLImplementor* p = (OpenGLImplementor*)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_CREATE: {
      CREATESTRUCT *cp = (CREATESTRUCT *)lParam;
      p = (OpenGLImplementor *)cp -> lpCreateParams;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      ReleaseSemaphore(p -> hCreationSemaphore,1,NULL);
      }
      break;

   case WM_OPENGLIMPLEMENTATION_STOP:
      PostQuitMessage(0);
      break;

   case WM_OPENGLIMPLEMENTATION_SETBASEWINDOW: {
      p -> hwndBase = (HWND)wParam;
      PlotWindow *pw = NULL;
      while ( p -> plotWindowList.size() ) {
         pw = p -> plotWindowList.back();
         p -> plotWindowList.remove(pw);
         delete pw;
      }
      p -> plotWindowList.clear();
      p -> plotWindow = NULL; // <-- this is set so that the constructor for the new plotWindow won't try to copy from this object's data.
      p -> plotWindow = new PlotWindow(p -> hwndBase,p,p -> pIEvaluator);
      p -> plotWindowList.push_back(p -> plotWindow);//Add(p -> plotWindow,NULL,reinterpret_cast<long>(p -> hwndBase));
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETTARGETWINDOW: {

      HWND hwndNew = (HWND)wParam;

      PlotWindow *pwExisting = NULL;
      for ( PlotWindow *pw : p -> plotWindowList ) {
         if ( pw -> hwnd == hwndNew ) {
            pwExisting = pw;
            break;
         }
      }

      if ( ! pwExisting ) {
         pwExisting = new PlotWindow(hwndNew,p,p -> pIEvaluator);
         p -> plotWindowList.push_back(pwExisting);
      } else
         pwExisting -> createRenderingContext();

      p -> plotWindow = pwExisting;

      p -> plotWindow -> restoreState();

      std::list<PlotWindow *> toDelete;
      for ( PlotWindow *pw : p -> plotWindowList ) {
         if ( ! ( NULL == pw -> hwnd ) && IsWindow(pw -> hwnd) )
            continue;
         toDelete.push_back(pw);
      }

      for ( PlotWindow *pw : toDelete ) {
         p -> plotWindowList.remove(pw);
         delete pw;
      }

      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETVIEWPROPERTIES: {
      strCall_SetViewProperties *ps = (strCall_SetViewProperties *)wParam;
      p -> plotWindow -> setViewProperties(ps -> pPropPlotView,ps -> pPropRotationTheta,ps -> pPropRotationPhi,ps -> pPropRotationSpin,
                                 ps -> pPropLeftMargin,ps -> pPropTopMargin,ps -> pPropRightMargin,ps -> pPropBottomMargin,ps -> pPropMarginUnits,ps -> pPropStretchToMargins);
      }
      break;

   case WM_OPENGLIMPLEMENTATION_UNSETVIEWPROPERTIES: {
      p -> plotWindow -> unsetViewProperties();
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SET2D: {
      strCall_Set2D *ps = (strCall_Set2D *)wParam;
      p -> plotWindow -> set2D(ps -> iDataSet,ps -> margins,ps -> directionRight,ps -> directionUp);
      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETUP: {
      strCall_Setup *ps = (strCall_Setup *)wParam;
      p -> plotWindow -> setUp(ps -> masterDataSet,ps -> pPropPlotView,ps -> pPropRotationTheta,ps -> pPropRotationPhi,ps -> pPropRotationSpin,
                                 ps -> pPropLeftMargin,ps -> pPropTopMargin,ps -> pPropRightMargin,ps -> pPropBottomMargin,ps -> pPropMarginUnits,ps -> pPropStretchToMargins);
      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETVIEWPORT: {
      int *newViewPort = (int *)wParam;
      memcpy(p -> plotWindow -> openGLState.viewPort,newViewPort,sizeof(p -> plotWindow -> openGLState.viewPort));
      p -> plotWindow -> setViewPort(newViewPort);   
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETLIGHTING: {
      strCall_SetLighting *ps = reinterpret_cast<strCall_SetLighting*>(wParam);
      p -> plotWindow -> setLighting(ps -> pPropLightEnabled,ps -> pPropAmbientLight,ps -> pPropDiffuseLight,ps -> pPropSpecularLight,ps -> pPropLightPos,ps -> pPropCountLights,ps -> pPropShinyness);
      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETCOLOR: {
      GLfloat *pfv = (GLfloat *)wParam;
      glColor3fv(pfv);
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETLINEWEIGHT: {
      IGProperty *pLineWeight = (IGProperty *)wParam;
      long lw = 0;
      float lineWeight;
      if ( pLineWeight ) 
         pLineWeight -> get_longValue(&lw);
      lineWeight = (float)lw;
      glLineWidth(lineWeight);
      }
      break;


   case WM_OPENGLIMPLEMENTATION_ERASE: {
      p -> plotWindow -> erase((IGProperty *)wParam);
      }
      break;

   case WM_OPENGLIMPLEMENTATION_TRANSLATE: {
      DataPoint* dp = (DataPoint *)wParam;
      p -> plotWindow -> translate(dp);
      delete dp;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_ROTATE: {
      strCall_Rotate *ps = (strCall_Rotate *)wParam;
      p -> plotWindow -> rotate(ps -> axis,ps -> angle);
      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SCALE: {
      DataPoint* dp = (DataPoint *)wParam;
      p -> plotWindow -> scale(dp);
      delete dp;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_PUSH: {

      transformationMatrixes* pm = new transformationMatrixes();

      memcpy(pm,&p -> plotWindow -> openGLState,sizeof(transformationMatrixes));

      glGetIntegerv(GL_VIEWPORT,pm -> viewPort);
      glGetDoublev(GL_MODELVIEW_MATRIX,pm -> modelMatrix);
      glGetDoublev(GL_PROJECTION_MATRIX,pm -> projectionMatrix);

      p -> matrixList.push_back(pm);

      }
      break;

   case WM_OPENGLIMPLEMENTATION_POP: {

      if ( ! p -> matrixList.size() ) 
         break;

      transformationMatrixes* pm = p -> matrixList.back();

      p -> matrixList.remove(pm);

      glViewport(pm -> viewPort[0],pm -> viewPort[1],pm -> viewPort[2],pm -> viewPort[3]);

      glMatrixMode(GL_PROJECTION);
      glLoadMatrixd(pm -> projectionMatrix);

      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixd(pm -> modelMatrix);

      memcpy(&p -> plotWindow -> openGLState,pm,sizeof(transformationMatrixes));

      delete pm;

      }
      break;

   case WM_OPENGLIMPLEMENTATION_RESETDEPTH:
      p -> plotWindow -> resetDepth();
      break;

   case WM_OPENGLIMPLEMENTATION_FINALIZE:
      if ( p -> plotWindow )
         p -> plotWindow -> finalize();
      break;

   case WM_OPENGLIMPLEMENTATION_NEWLINE: {

      DataPoint *dp = reinterpret_cast<DataPoint*>(wParam);

      if ( p -> plotWindow -> lineMode ) {
         glEnd();
         glGetError();
//OPENGL_ERROR_CHECK
      }

      glBegin(GL_LINE_STRIP);
OPENGL_ERROR_CHECK
      glVertex3d(dp -> x,dp -> y,dp -> z);  
OPENGL_ERROR_CHECK

      delete dp;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_NEWPOLYGON: {
      DataPoint *dp = reinterpret_cast<DataPoint*>(wParam);
glGetError();
      if ( p -> plotWindow -> polygonMode )
         glEnd();
      p -> plotWindow -> polygonMode = true;
      glBegin(GL_POLYGON);
      glVertex3d(dp -> x,dp -> y,dp -> z);  
OPENGL_ERROR_CHECK
      delete dp;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_NEWSEGMENTS: {
      strCall_NewSegments *ps = reinterpret_cast<strCall_NewSegments*>(wParam);
      *ps -> firstInRange = glGenLists(ps -> numberToMake);
      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_OPENSEGMENT: {

      strCall_OpenSegment *ps = (strCall_OpenSegment *)wParam;
      
      float fvColor[] = {CLR_BLACK};

      if ( ps -> pPropColor ) {
         BYTE *pb = (BYTE *)fvColor;
         ps -> pPropColor -> get_binaryValue(sizeof(fvColor),(BYTE**)&pb);
      }

      double lw = 1.0;
      if ( ps -> pPropLineWeight )
         ps -> pPropLineWeight -> get_doubleValue(&lw);

      p -> plotWindow -> disableLighting();

      glDeleteLists(ps -> segmentID,1);

      glNewList(ps -> segmentID,GL_COMPILE);//_AND_EXECUTE);

      glColor3f(fvColor[0],fvColor[1],fvColor[2]);

      glLineWidth((float)lw);

      glEnable(GL_LINE_SMOOTH);

      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      
      glBegin(GL_LINE_STRIP);

      p -> plotWindow -> lineMode = true;

      delete ps;

      }
      break;

   case WM_OPENGLIMPLEMENTATION_STARTSEGMENT: {
      long *pSegmentID = (long *)wParam;
      glDeleteLists(*pSegmentID,1);
      glNewList(*pSegmentID,GL_COMPILE_AND_EXECUTE);
      glLoadName(*pSegmentID);
      p -> plotWindow -> polygonMode = false;
      p -> plotWindow -> lineMode = false;
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_ENDSEGMENT: {
      glEndList();
      p -> plotWindow -> polygonMode = false;
      p -> plotWindow -> lineMode = false;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_REDRAWSEGMENT: {
      long *pSegmentID = reinterpret_cast<long *>(wParam);
      if ( *pSegmentID && glIsList(*pSegmentID) ) {
glGetError();
         glLoadName(*pSegmentID);
OPENGL_ERROR_CHECK
         glCallList(*pSegmentID);
      }
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_PLAYSEGMENT: {
      long *pSegmentID = (long *)wParam;
      glCallList(*pSegmentID);
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_CLOSESEGMENT: {

      strCall_CloseSegment *ps = (strCall_CloseSegment *)wParam;

      glEnd();

#if 1
glGetError();
//OPENGL_ERROR_CHECK
      glEndList();
OPENGL_ERROR_CHECK
      if ( ps -> drawOnClose ) 
         glCallList((GLuint)ps -> segmentID);
#endif
//OPENGL_ERROR_CHECK

      p -> plotWindow -> polygonMode = false;
      p -> plotWindow -> lineMode = false;

      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_DELETESEGMENT: {
      long *pSegmentID = (long *)wParam;
      if ( *pSegmentID ) 
         glDeleteLists(*pSegmentID,1);
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_BEGINSURFACE: {

      strCall_BeginSurface *ps = (strCall_BeginSurface *)wParam;

      glDeleteLists(ps -> segmentID,1);
      glNewList(ps -> segmentID,GL_COMPILE);
 
      float fvTopColor[] = {CLR_BLACK};
      float fvBottomColor[] = {CLR_BLACK};

      if ( ps -> pPropTopColor ) {
         BYTE *pb = (BYTE *)fvTopColor;
         ps -> pPropTopColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
      }

      if ( ps -> pPropBottomColor ) {
         BYTE *pb = (BYTE *)fvBottomColor;
         ps -> pPropBottomColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
      }

      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,fvTopColor);

      glMaterialfv(GL_BACK,GL_AMBIENT_AND_DIFFUSE,fvBottomColor);

      glFrontFace(GL_CCW);
      glDisable(GL_CULL_FACE);

      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
 
      p -> plotWindow -> enableLighting();

      delete ps;

      }
      break;


   case WM_OPENGLIMPLEMENTATION_ENDSURFACE: {
      long *pSegmentID = reinterpret_cast<long*>(wParam);
OPENGL_ERROR_CHECK
      glEnd();
OPENGL_ERROR_CHECK
      glEndList();
OPENGL_ERROR_CHECK
      glCallList(*pSegmentID);
OPENGL_ERROR_CHECK
      p -> plotWindow -> disableLighting();
      delete pSegmentID;
      }
      break;


   case WM_OPENGLIMPLEMENTATION_BEGINWIREFRAME: {

      strCall_BeginWireframe *ps = reinterpret_cast<strCall_BeginWireframe*>(wParam);

      glDeleteLists(ps -> segmentID,1);

      GLfloat fvColor[] = {CLR_BLACK};
      if ( ps -> pPropLineColor ) {
         BYTE *pb = (BYTE *)fvColor;
         ps -> pPropLineColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
      }

      long lineWeight;
      if ( ps -> pPropLineWeight )
         ps -> pPropLineWeight -> get_longValue(&lineWeight);
      else
         lineWeight = 1;

      glLineWidth((float)lineWeight);

      glColor3fv(fvColor);

      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glFrontFace(GL_CCW);         
      glCullFace(GL_FRONT);

      glNewList(ps -> segmentID,GL_COMPILE);

      glBegin(GL_QUADS);

      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_ENDWIREFRAME: {
      long *pSegmentID = reinterpret_cast<long*>(wParam);
OPENGL_ERROR_CHECK
      glEnd();
OPENGL_ERROR_CHECK
      glEndList();
OPENGL_ERROR_CHECK
      glCallList(*pSegmentID);
OPENGL_ERROR_CHECK
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_BEGINSOLID: {

      strCall_BeginSolid *ps = reinterpret_cast<strCall_BeginSolid *>(wParam);

      float fvTopColor[] = {CLR_BLACK};
      float fvBottomColor[] = {CLR_BLACK};

      if ( ps -> pPropTopColor ) {
         BYTE *pb = (BYTE *)fvTopColor;
         ps -> pPropTopColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
      }

      if ( ps -> pPropBottomColor ) {
         BYTE *pb = (BYTE *)fvBottomColor;
         ps -> pPropBottomColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
      }

      glDeleteLists(ps -> segmentID,1);

      glNewList(ps -> segmentID,GL_COMPILE);
 
      glEnable(GL_DEPTH_TEST);
      glFrontFace(GL_CCW);
      glDisable(GL_CULL_FACE);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
 
      glMaterialfv(GL_FRONT,GL_SPECULAR,fvTopColor);
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,fvTopColor);
      glMaterialfv(GL_BACK,GL_SPECULAR,fvBottomColor);
      glMaterialfv(GL_BACK,GL_AMBIENT_AND_DIFFUSE,fvBottomColor);
 
      p -> plotWindow -> enableLighting();
 
      glBegin(GL_QUADS);

      //glColor3fv(fvTopColor);

      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_ENDSOLID: {
      long *pSegmentID = reinterpret_cast<long*>(wParam);
      glEnd();
      glEndList();
      glCallList(*pSegmentID);
      p -> plotWindow -> disableLighting();
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_BEGINTRIANGLESOLID: {

      strCall_BeginSolid *ps = reinterpret_cast<strCall_BeginSolid *>(wParam);

      float fvTopColor[] = {CLR_BLACK};
      float fvBottomColor[] = {CLR_BLACK};

      if ( ps -> pPropTopColor ) {
         BYTE *pb = (BYTE *)fvTopColor;
         ps -> pPropTopColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
      }

      if ( ps -> pPropBottomColor ) {
         BYTE *pb = (BYTE *)fvBottomColor;
         ps -> pPropBottomColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
      }

      glDeleteLists(ps -> segmentID,1);

      glNewList(ps -> segmentID,GL_COMPILE);
 
      //glEnable(GL_DEPTH_TEST);
      //glFrontFace(GL_CCW);         
      //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
 
      glMaterialfv(GL_FRONT,GL_SPECULAR,fvTopColor);
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,fvTopColor);
      glMaterialfv(GL_BACK,GL_SPECULAR,fvBottomColor);
      glMaterialfv(GL_BACK,GL_AMBIENT_AND_DIFFUSE,fvBottomColor);
 
      p -> plotWindow -> enableLighting();
 
      glBegin(GL_TRIANGLE_STRIP);

      //glColor3fv(fvTopColor);

      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_ENDTRIANGLESOLID: {
      long *pSegmentID = reinterpret_cast<long*>(wParam);
      glEnd();
      glEndList();
      glCallList(*pSegmentID);
      p -> plotWindow -> disableLighting();
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_BEGINMODE: {
      glBegin((GLint)wParam);
      }
      break;

   case WM_OPENGLIMPLEMENTATION_ENDMODE: {
      glEnd();
      }
      break;

   case WM_OPENGLIMPLEMENTATION_VERTEX: {
      DataPoint *dp = reinterpret_cast<DataPoint*>(wParam);
      glVertex3d(dp -> x,dp -> y,dp -> z);  
      }
      break;

   case WM_OPENGLIMPLEMENTATION_NORMAL3D: {
      double *pNormal = reinterpret_cast<double *>(wParam);
      glNormal3dv(pNormal);
      delete pNormal;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_GETPICKBOXHITS: {
      strCall_GetPickBoxHits *ps = reinterpret_cast<strCall_GetPickBoxHits*>(wParam);
      try {
      ps -> returnValue = p -> plotWindow -> getPickBoxHits(ps -> ptl,ps -> pickWindowSize,ps -> hitTable,ps -> hitTableSize,
                                                               ps -> pCallLists,ps -> hitTableHits);
      } catch ( std::exception ex ) {
         printf("hello world");
MessageBox(NULL,ex.what(),"",MB_OK);
      }
      }
      break;

   case WM_OPENGLIMPLEMENTATION_WINDOWTODATA: {

      strCall_WindowToData *ps = (strCall_WindowToData *)wParam;

      ps -> result = S_OK;

      double mMatrix[16],pMatrix[16];
      DataPoint dpWorking;
      int vport[4];
   
      glGetDoublev(GL_MODELVIEW_MATRIX,mMatrix);
      glGetDoublev(GL_PROJECTION_MATRIX,pMatrix);
      glGetIntegerv(GL_VIEWPORT,vport);
   
      double y = (double)p -> plotWindow -> openGLState.windowCY - ps -> dpSource.y;

      gluUnProject(ps -> dpSource.x,y,ps -> dpSource.z,mMatrix,pMatrix,vport,&dpWorking.x,&dpWorking.y,&dpWorking.z);
   
      memcpy(&ps -> dpTarget,&dpWorking,sizeof(DataPoint));
      }
      break;


   case WM_OPENGLIMPLEMENTATION_DATATOWINDOW: {

      strCall_DataToWindow *ps = reinterpret_cast<strCall_DataToWindow*>(wParam);

      DataPoint dpWorking;
   
      switch ( ps -> uom ) {
      case UNIT_PIXEL: {
   
         double mMatrix[16],pMatrix[16];
         double depthRange[2];
         int vport[4];
   
         glGetDoublev(GL_MODELVIEW_MATRIX,mMatrix);
         glGetDoublev(GL_PROJECTION_MATRIX,pMatrix);
         glGetIntegerv(GL_VIEWPORT,vport);
         glGetDoublev(GL_DEPTH_RANGE,depthRange);
   
         gluProject(ps -> dpSource.x,ps -> dpSource.y,ps -> dpSource.z,mMatrix,pMatrix,vport,&dpWorking.x,&dpWorking.y,&dpWorking.z);
   
         dpWorking.y = (double)p -> plotWindow -> openGLState.windowCY - dpWorking.y;
   
         memcpy(ps -> pdpTarget,&dpWorking,sizeof(DataPoint));
   
         }
         break;
    
      case UNIT_PERCENT: {
    
         double win0x,win0y,win0z;
         double win1x,win1y,win1z;
    
         ps -> pdpTarget -> x = p -> plotWindow -> openGLState.extentsXMin + (p -> plotWindow -> openGLState.extentsXMax - p -> plotWindow -> openGLState.extentsXMin)*ps -> dpSource.x/100.0;
         ps -> pdpTarget -> y = p -> plotWindow -> openGLState.extentsYMin + (p -> plotWindow -> openGLState.extentsYMax - p -> plotWindow -> openGLState.extentsYMin)*ps -> dpSource.y/100.0;
         ps -> pdpTarget -> z = p -> plotWindow -> openGLState.extentsZMin + (p -> plotWindow -> openGLState.extentsZMax - p -> plotWindow -> openGLState.extentsZMin)*ps -> dpSource.z/100.0;
   
         gluProject(p -> plotWindow -> openGLState.extentsXMin,p -> plotWindow -> openGLState.extentsYMin,p -> plotWindow -> openGLState.extentsZMin,
                              p -> plotWindow -> openGLState.modelMatrix,p -> plotWindow -> openGLState.projectionMatrix,p -> plotWindow -> openGLState.viewPort,&win0x,&win0y,&win0z);
         gluProject(ps -> pdpTarget -> x,ps -> pdpTarget -> y,ps -> pdpTarget -> z,p -> plotWindow -> openGLState.modelMatrix,p -> plotWindow -> openGLState.projectionMatrix,p -> plotWindow -> openGLState.viewPort,&win1x,&win1y,&win1z);
    
         ps -> pdpTarget -> x = win1x - win0x;
         ps -> pdpTarget -> y = win1y - win0y;
         ps -> pdpTarget -> z = win1z - win0z;
         }
    
         break;
    
      }
      }
      break;
 
   case WM_OPENGLIMPLEMENTATION_DATALISTTOWINDOW: {

      strCall_DataListToWindow *ps = reinterpret_cast<strCall_DataListToWindow*>(wParam);

      double mMatrix[16],pMatrix[16];
      double depthRange[2];
      int vport[4];
   
      glGetDoublev(GL_MODELVIEW_MATRIX,mMatrix);
      glGetDoublev(GL_PROJECTION_MATRIX,pMatrix);
      glGetIntegerv(GL_VIEWPORT,vport);
      glGetDoublev(GL_DEPTH_RANGE,depthRange);
   
      long k = -1;
      DataList* pSource = ps -> dlSource;
      DataList* pTarget = ps -> dlTarget;

      while ( pSource ) {
   
         k++;
   
         if ( pSource -> data.x == -DBL_MAX || pSource -> data.y == -DBL_MAX || pSource -> data.z == -DBL_MAX || 
               pSource -> data.x == DBL_MAX || pSource -> data.y == DBL_MAX || pSource -> data.z == DBL_MAX ) {
            memcpy(&pTarget -> data,&pSource -> data,sizeof(DataPoint));
            pSource = pSource -> next;
            pTarget = pTarget -> next;
            continue;
         }
   
         gluProject(pSource -> data.x,pSource -> data.y,pSource -> data.z,mMatrix,pMatrix,vport,&pTarget -> data.x,&pTarget -> data.y,&pTarget -> data.z);

         pTarget -> data.y =  (double)p -> plotWindow -> openGLState.windowCY - pTarget -> data.y;
   
         pSource = pSource -> next;
         pTarget = pTarget -> next;
      }
      }
      break;

   case WM_OPENGLIMPLEMENTATION_PIXELSTOUNITS: {

      strCall_PixelsToUnits *ps = reinterpret_cast<strCall_PixelsToUnits *>(wParam);
   
      int rasterpos1[4],rasterpos2[4];
      double pixelsPerUnit;
    
      glRasterPos3d(0.0,0.0,0.0);
      glGetIntegerv(GL_CURRENT_RASTER_POSITION,rasterpos1);
    
      switch ( ps -> direction ) {
      case 'X':
      case 'x':
         glRasterPos3d(1.0,0.0,0.0);
         glGetIntegerv(GL_CURRENT_RASTER_POSITION,rasterpos2);
         pixelsPerUnit = (double)(rasterpos2[0] - rasterpos1[0]);
         break;
    
      case 'Y':
      case 'y':
         glRasterPos3d(0.0,1.0,0.0);
         glGetIntegerv(GL_CURRENT_RASTER_POSITION,rasterpos2);
         pixelsPerUnit = (double)(rasterpos2[1] - rasterpos1[1]);
         break;
      
      default:
         glRasterPos3d(1.0,0.0,0.0);
         glGetIntegerv(GL_CURRENT_RASTER_POSITION,rasterpos2);
         pixelsPerUnit = (double)(rasterpos2[0] - rasterpos1[0]);
         break;
   
      }
    
      *ps -> answer = ps -> inPixels / pixelsPerUnit;
      }
      break;
    
   case WM_OPENGLIMPLEMENTATION_UNITSTOPIXELS: {

      strCall_UnitsToPixels *ps = reinterpret_cast<strCall_UnitsToPixels*>(wParam);

      int rasterpos1[4],rasterpos2[4];
    
      glRasterPos3d(0.0,0.0,0.0);
   
      glGetIntegerv(GL_CURRENT_RASTER_POSITION,rasterpos1);
      switch ( ps -> direction ) {
      case 'X':
      case 'x':
         glRasterPos3d(ps -> inUnits,0.0,0.0);
         glGetIntegerv(GL_CURRENT_RASTER_POSITION,rasterpos2);
         *ps -> answer = (double)(rasterpos2[0] - rasterpos1[0]);
         break;
    
      case 'Y':
      case 'y':
         glRasterPos3d(0.0,ps -> inUnits,0.0);
         glGetIntegerv(GL_CURRENT_RASTER_POSITION,rasterpos2);
         *ps -> answer = (double)(rasterpos2[1] - rasterpos1[1]);
         break;
   
      default:
         glRasterPos3d(ps -> inUnits,0.0,0.0);
         glGetIntegerv(GL_CURRENT_RASTER_POSITION,rasterpos2);
         *ps -> answer = (double)(rasterpos2[0] - rasterpos1[0]);
         break;
   
      }
      }
      break;
 

   case WM_OPENGLIMPLEMENTATION_PIXELPOINT: {

      strCall_PixelPoint *ps = reinterpret_cast<strCall_PixelPoint*>(wParam);

      ps -> result = S_OK;

      double currentRasterPos[4],rasterpos[4] = {0.0,0.0,0.0,0.0};
      int isValid;
   
      glGetDoublev(GL_CURRENT_RASTER_POSITION,currentRasterPos);
   
      glRasterPos3d(ps -> pBeginning -> x,ps -> pBeginning -> y,ps -> pBeginning -> z);
   
      glGetDoublev(GL_CURRENT_RASTER_POSITION,rasterpos);
   
      glGetIntegerv(GL_CURRENT_RASTER_POSITION_VALID,&isValid);
   
      if ( ! isValid ) {
         ps -> pAnswer -> x = 0.0;
         ps -> pAnswer -> y = 0.0;
         ps -> pAnswer -> z = 0.0;
         glRasterPos3d(currentRasterPos[0],currentRasterPos[1],currentRasterPos[2]);
         ps -> result = S_FALSE;
         break;
      }
   
      ps -> pAnswer -> x = rasterpos[0];
      ps -> pAnswer -> y = rasterpos[1];
      ps -> pAnswer -> z = rasterpos[2];
   
      glRasterPos3d(currentRasterPos[0],currentRasterPos[1],currentRasterPos[2]);

      }
      break;

   case WM_OPENGLIMPLEMENTATION_PIXELVECTOR: {

      strCall_PixelVector *ps = reinterpret_cast<strCall_PixelVector*>(wParam);

      unsigned char rasterPositionValid;

      double currentRasterPos[4],rasterpos1[4],rasterpos2[4];
         
      DataPoint dpStart = {ps -> pBeginning -> x,ps -> pBeginning -> y,ps -> pBeginning -> z};
      DataPoint dpEnd = {ps -> pEnding -> x,ps -> pEnding -> y,ps -> pEnding -> z};

      ps -> result = S_OK;

      glGetDoublev(GL_CURRENT_RASTER_POSITION,currentRasterPos);
   
      glRasterPos3d(dpStart.x,dpStart.y,dpStart.z);
      glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID,&rasterPositionValid);
      if ( ! rasterPositionValid ) {
         dpStart.x = dpStart.x - (ps -> pBeginning -> x - ps -> pEnding -> x)/100.0;
         glRasterPos3d(dpStart.x,dpStart.y,dpStart.z);
         glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID,&rasterPositionValid);
         if ( ! rasterPositionValid ) {
            dpStart.y = dpStart.y - (ps -> pBeginning -> y - ps -> pEnding -> y)/100.0;
            glRasterPos3d(dpStart.x,dpStart.y,dpStart.z);
            glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID,&rasterPositionValid);
            if ( ! rasterPositionValid ) {
               dpStart.z = dpStart.z - (ps -> pBeginning -> z - ps -> pEnding -> z)/100.0;
               glRasterPos3d(dpStart.x,dpStart.y,dpStart.z);
               glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID,&rasterPositionValid);
               if ( ! rasterPositionValid ) 
                  ps -> result = S_FALSE;
            }   
         }
      }

      glGetDoublev(GL_CURRENT_RASTER_POSITION,rasterpos1);
   
      glRasterPos3d(dpEnd.x,dpEnd.y,dpEnd.z);
      glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID,&rasterPositionValid);
      if ( ! rasterPositionValid ) {
         dpEnd.x = dpEnd.x - (ps -> pEnding -> x - ps -> pBeginning -> x)/100.0;
         glRasterPos3d(dpEnd.x,dpEnd.y,dpEnd.z);
         glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID,&rasterPositionValid);
         if ( ! rasterPositionValid ) {
            dpEnd.y = dpEnd.y - (ps -> pEnding -> y - ps -> pBeginning -> y)/100.0;
            glRasterPos3d(dpEnd.x,dpEnd.y,dpEnd.z);
            glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID,&rasterPositionValid);
            if ( ! rasterPositionValid ) {
               dpEnd.z = dpEnd.z - (ps -> pEnding -> z - ps -> pBeginning -> z)/100.0;
               glRasterPos3d(dpEnd.x,dpEnd.y,dpEnd.z);
               glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID,&rasterPositionValid);
               if ( ! rasterPositionValid ) 
                  ps -> result = S_FALSE;
            }   
         }
      }

      glGetDoublev(GL_CURRENT_RASTER_POSITION,rasterpos2);
   
      ps ->  pAnswer -> x = (rasterpos2[0] - rasterpos1[0]);
      ps ->  pAnswer -> y = (rasterpos2[1] - rasterpos1[1]);
      ps ->  pAnswer -> z = (rasterpos2[2] - rasterpos1[2]);
   
      glRasterPos3d(currentRasterPos[0],currentRasterPos[1],currentRasterPos[2]);

      }
      break;

   case WM_OPENGLIMPLEMENTATION_READPIXELS: {

      strCall_ReadPixels *ps = (strCall_ReadPixels *)wParam;

      GLint bufferSize[4] = {0};
      glGetIntegerv(GL_SCISSOR_BOX,bufferSize);
      
      p -> plotWindow -> GetPixels(ps -> x1,ps -> y1,ps -> x2,ps -> y2,bufferSize[3],ps -> pResult);

      }
      break;

   case WM_OPENGLIMPLEMENTATION_ISRENDERED: {

      BOOL *prv = (BOOL *)wParam;

      if ( ! p -> plotWindow ) {
         *prv = FALSE;
         break;
      }

      //
      //NTC: 12-29-2017: isRendered only becomes true when something is drawn and finalize is called.
      // I think there might be situations where the client has setup OpenGL (i.e., set the active window)
      // but if it doesn't draw and finalize, isRendered remains false.
      // The intent of the flag is to prevent openGL mouse related activity (i.e. pick) before the graphic
      // is complete, however, so the logic that uses this flag must allow normal windows mouse activity.
      // 
      *prv = p -> plotWindow -> isRendered;
   
      }
      break;

   default:
      break;
   }

   if ( p && lParam ) {

#if 1
      HANDLE hSemaphore = reinterpret_cast<HANDLE>(lParam);
      ReleaseSemaphore(hSemaphore,1,NULL);
#else
      ReleaseMutex(p -> hBlockingMutex);
#endif

   }

   return (LRESULT)TRUE;
   }