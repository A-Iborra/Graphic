/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>

#include "Graphic_resource.h"

#include "utils.h"
#include "Properties_i.h"
#include "OpenGLImplementor.h"

   unsigned int __stdcall OpenGLImplementor::runImplementationMsgQueue(void *pt) {

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
      p -> plotWindow = NULL; // <-- this is set so that the constructor for the new plotWindow won't try to copy from this object's data.
      p -> plotWindow = new PlotWindow(p -> hwndBase,p,p -> pIEvaluator);
      p -> plotWindowList.insert(p -> plotWindowList.end(),p -> plotWindow);//Add(p -> plotWindow,NULL,reinterpret_cast<long>(p -> hwndBase));
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETTARGETWINDOW: {
      p -> plotWindow -> saveState();
      HWND hwndNew = (HWND)wParam;
      PlotWindow* pwTemp = NULL;
      for ( PlotWindow *pw : p -> plotWindowList ) {
         if ( pw -> hwnd == hwndNew ) {
            pwTemp = pw;
            break;
         }
      }
      if ( ! pwTemp ) {
         pwTemp = new PlotWindow(hwndNew,p,p -> pIEvaluator);
         p -> plotWindowList.push_back(pwTemp);
      }
      p -> plotWindow = pwTemp;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_RESETTARGETWINDOW: {
      PlotWindow *pwTemp = NULL;
      if ( p -> plotWindowList.size() ) {
         pwTemp = p -> plotWindowList.back();
         p -> plotWindowList.remove(pwTemp);
         delete pwTemp;
      }
      if ( p -> plotWindowList.size() ) {
         p -> plotWindow = p -> plotWindowList.back();
         p -> plotWindow -> restoreState();
         wglMakeCurrent(p -> plotWindow -> deviceContext,p -> plotWindow -> renderingContext);
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
      memcpy(p -> plotWindow -> viewPort,newViewPort,sizeof(p -> plotWindow -> viewPort));
      p -> plotWindow -> setViewPort(newViewPort);   
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETLIGHTING: {
      strCall_SetLighting *ps = reinterpret_cast<strCall_SetLighting*>(wParam);
      p -> plotWindow -> setLighting(ps -> pPropLightEnabled,
                           ps -> pPropAmbientLight,
                           ps -> pPropDiffuseLight,
                           ps -> pPropSpecularLight,
                           ps -> pPropLightPos,ps -> pPropCountLights,ps -> pPropShinyness);
      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_SETCOLOR: {
      IGProperty *pColor = (IGProperty *)wParam;
      float fv[4];
      BYTE *pb = (BYTE *)fv;
      pColor -> get_binaryValue(sizeof(fv),(BYTE**)&pb);
      glColor3fv(fv);
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

      glGetIntegerv(GL_VIEWPORT,pm -> viewPort);
      glGetDoublev(GL_MODELVIEW_MATRIX,pm -> modelMatrix);
      glGetDoublev(GL_PROJECTION_MATRIX,pm -> projectionMatrix);

      pm -> xScaleFactor = p -> plotWindow -> xScaleFactor;
      pm -> yScaleFactor = p -> plotWindow -> yScaleFactor;
      pm -> zScaleFactor = p -> plotWindow -> zScaleFactor;

      pm -> extentsXMin = p -> plotWindow -> extentsXMin;
      pm -> extentsYMin = p -> plotWindow -> extentsYMin;
      pm -> extentsZMin = p -> plotWindow -> extentsZMin;
      pm -> extentsXMax = p -> plotWindow -> extentsXMax;
      pm -> extentsYMax = p -> plotWindow -> extentsYMax;
      pm -> extentsZMax = p -> plotWindow -> extentsZMax;

      p -> matrixList.insert(p -> matrixList.end(),pm);

      }
      break;

   case WM_OPENGLIMPLEMENTATION_POP: {

      transformationMatrixes* pm = NULL;

      if ( ! p -> matrixList.size() ) 
         break;

      pm = p -> matrixList.back();

      p -> matrixList.remove(pm);

      glViewport(pm -> viewPort[0],pm -> viewPort[1],pm -> viewPort[2],pm -> viewPort[3]);

      glMatrixMode(GL_PROJECTION);
      glLoadMatrixd(pm -> projectionMatrix);
      memcpy(p -> plotWindow -> projectionMatrix,pm -> projectionMatrix,sizeof(pm -> projectionMatrix));

      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixd(pm -> modelMatrix);
      memcpy(p -> plotWindow -> modelMatrix,pm -> modelMatrix,sizeof(pm -> modelMatrix));

      glGetIntegerv(GL_VIEWPORT,p -> plotWindow -> viewPort);

      p -> plotWindow -> xScaleFactor = pm -> xScaleFactor;
      p -> plotWindow -> yScaleFactor = pm -> yScaleFactor;
      p -> plotWindow -> zScaleFactor = pm -> zScaleFactor;

      p -> plotWindow -> extentsXMin = pm -> extentsXMin;
      p -> plotWindow -> extentsYMin = pm -> extentsYMin;
      p -> plotWindow -> extentsZMin = pm -> extentsZMin;
      p -> plotWindow -> extentsXMax = pm -> extentsXMax;
      p -> plotWindow -> extentsYMax = pm -> extentsYMax;
      p -> plotWindow -> extentsZMax = pm -> extentsZMax;

      delete pm;

      }
      break;

   case WM_OPENGLIMPLEMENTATION_RESETDEPTH:
      p -> plotWindow -> resetDepth();
      break;

   case WM_OPENGLIMPLEMENTATION_FLUSH:
      glFlush();
      break;

   case WM_OPENGLIMPLEMENTATION_NEWLINE: {
      DataPoint *dp = reinterpret_cast<DataPoint*>(wParam);
      if ( p -> plotWindow -> lineMode ) 
         glEnd();
      p -> plotWindow -> lineMode = true;
      glBegin(GL_LINE_STRIP);
      glVertex3d(dp -> x,dp -> y,dp -> z);  
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

      strCall_OpenSegment *ps = reinterpret_cast<strCall_OpenSegment*>(wParam);
      
      float fvColor[] = {CLR_BLACK};
      if ( ps -> pPropColor ) {
         BYTE *pb = (BYTE *)fvColor;
         ps -> pPropColor -> get_binaryValue(sizeof(fvColor),(BYTE**)&pb);
      }

      double lw = 1.0;
      if ( ps -> pPropLineWeight )
         ps -> pPropLineWeight -> get_doubleValue(&lw);

      glDeleteLists(ps -> segmentID,1);

      glNewList(ps -> segmentID,GL_COMPILE);

      glColor3f(fvColor[0],fvColor[1],fvColor[2]);
      glLineWidth((float)lw);

      glEnable(GL_LINE_SMOOTH);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

      glBegin(GL_LINE_STRIP);

      delete ps;

      }
      break;

   case WM_OPENGLIMPLEMENTATION_STARTSEGMENT: {
      long *pSegmentID = reinterpret_cast<long *>(wParam);
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
      long *pSegmentID = reinterpret_cast<long *>(wParam);
      glCallList(*pSegmentID);
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_CLOSESEGMENT: {
      strCall_CloseSegment *ps = reinterpret_cast<strCall_CloseSegment *>(wParam);
      glEnd();
//CHECKME causing erasure      glFlush();
      glEndList();
      if ( ps -> drawOnClose ) {
         glCallList(ps -> segmentID);
//CHECKME causing erasure         glFlush();
      }
      p -> plotWindow -> polygonMode = false;
      p -> plotWindow -> lineMode = false;
      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_DELETESEGMENT: {
      long *pSegmentID = reinterpret_cast<long *>(wParam);
      if ( *pSegmentID ) 
         glDeleteLists(*pSegmentID,1);
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_BEGINSURFACE: {

      strCall_BeginSurface *ps = reinterpret_cast<strCall_BeginSurface*>(wParam);

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

      glShadeModel(GL_SMOOTH);
      glEnable(GL_LIGHTING);

      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,fvTopColor);
      glMaterialfv(GL_BACK,GL_AMBIENT_AND_DIFFUSE,fvBottomColor);

      glFrontFace(GL_CCW);         
      glDisable(GL_CULL_FACE);

      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

      glEnable(GL_DEPTH_TEST);
   
      glBegin(GL_QUADS);

      delete ps;

      }
      break;


   case WM_OPENGLIMPLEMENTATION_ENDSURFACE: {
      long *pSegmentID = reinterpret_cast<long*>(wParam);
      glEnd();
      glDisable(GL_LIGHTING);
      glFlush();
      glEndList();
      glCallList(*pSegmentID);
      glFlush();
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_BEGINWIREFRAME: {

      strCall_BeginWireframe *ps = reinterpret_cast<strCall_BeginWireframe*>(wParam);

      glDeleteLists(ps -> segmentID,1);
      glNewList(ps -> segmentID,GL_COMPILE);

      float fvColor[] = {CLR_BLACK};
      if ( ps -> pPropLineColor ) {
         BYTE *pb = (BYTE *)fvColor;
         ps -> pPropLineColor -> get_binaryValue(4 * sizeof(float),(BYTE**)&pb);
      }

      long lineWeight;
      ps -> pPropLineWeight -> get_longValue(&lineWeight);

      glLineWidth((float)lineWeight);
      glColor3fv(fvColor);

      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glFrontFace(GL_CCW);         
      glCullFace(GL_FRONT);
      glBegin(GL_QUADS);
      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_ENDWIREFRAME: {
      long *pSegmentID = reinterpret_cast<long*>(wParam);
      glEnd();
      glFlush();
      glEndList();
      glCallList(*pSegmentID);
      glFlush();
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
 
      glEnable(GL_LIGHTING);
 
      glEnable(GL_DEPTH_TEST);

      glFrontFace(GL_CCW);         

      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
 
      glBegin(GL_QUADS);
 
      glMaterialfv(GL_FRONT,GL_SPECULAR,fvTopColor);
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,fvTopColor);
      glMaterialfv(GL_BACK,GL_SPECULAR,fvBottomColor);
      glMaterialfv(GL_BACK,GL_AMBIENT_AND_DIFFUSE,fvBottomColor);
 
      glColor3fv(fvTopColor);
      delete ps;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_ENDSOLID: {
      long *pSegmentID = reinterpret_cast<long*>(wParam);
      glEnd();
      glDisable(GL_LIGHTING);
      glFlush();
      glEndList();
      glCallList(*pSegmentID);
      glFlush();
      delete pSegmentID;
      }
      break;

   case WM_OPENGLIMPLEMENTATION_VERTEX: {
      DataPoint *dp = reinterpret_cast<DataPoint*>(wParam);
glGetError();
      glVertex3d(dp -> x,dp -> y,dp -> z);  
OPENGL_ERROR_CHECK
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
      ps -> returnValue = p -> plotWindow -> getPickBoxHits(ps -> ptl,ps -> pickWindowSize,ps -> hitTable,ps -> hitTableSize,
                                                               ps -> pCallLists,ps -> hitTableHits);
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
   
      double y = (double)p -> plotWindow -> windowCY - ps -> dpSource.y;
   
      if ( (int)ps -> dpSource.x < vport[0] ) {
         ps -> result = S_FALSE;
         break;
      }
      if ( (int)ps -> dpSource.x > vport[2] + vport[0] ) {
         ps -> result = S_FALSE;
         break;
      }
      if ( (int)y < vport[1] ) {
         ps -> result = S_FALSE;
         break;
      }
      if ( (int)y > vport[3] + vport[1] ) {
         ps -> result = S_FALSE;
         break;
      }
   
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
   
         dpWorking.y = (double)p -> plotWindow -> windowCY - dpWorking.y;
   
         memcpy(ps -> pdpTarget,&dpWorking,sizeof(DataPoint));
   
         }
         break;
    
      case UNIT_PERCENT: {
    
// //      double modelMatrix[16],projectionMatrix[16];
         double win0x,win0y,win0z;
         double win1x,win1y,win1z;
// //      int viewport[4];
    
         ps -> pdpTarget -> x = p -> plotWindow -> extentsXMin + (p -> plotWindow -> extentsXMax - p -> plotWindow -> extentsXMin)*ps -> dpSource.x/100.0;
         ps -> pdpTarget -> y = p -> plotWindow -> extentsYMin + (p -> plotWindow -> extentsYMax - p -> plotWindow -> extentsYMin)*ps -> dpSource.y/100.0;
         ps -> pdpTarget -> z = p -> plotWindow -> extentsZMin + (p -> plotWindow -> extentsZMax - p -> plotWindow -> extentsZMin)*ps -> dpSource.z/100.0;

// //      glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
// //      glGetDoublev(GL_PROJECTION_MATRIX,projectionMatrix);
// //      glGetIntegerv(GL_VIEWPORT,viewport);
    
         gluProject(p -> plotWindow -> extentsXMin,p -> plotWindow -> extentsYMin,p -> plotWindow -> extentsZMin,p -> plotWindow -> modelMatrix,p -> plotWindow -> projectionMatrix,p -> plotWindow -> viewPort,&win0x,&win0y,&win0z);
         gluProject(ps -> pdpTarget -> x,ps -> pdpTarget -> y,ps -> pdpTarget -> z,p -> plotWindow -> modelMatrix,p -> plotWindow -> projectionMatrix,p -> plotWindow -> viewPort,&win1x,&win1y,&win1z);
    
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
   
         pTarget -> data.y =  (double)p -> plotWindow -> windowCY - pTarget -> data.y;
   
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

      long cx = ps -> x2 - ps -> x1;
      long cy = ps -> y2 - ps -> y1;

      long rowSize = 4 * sizeof(GLfloat) * cx;

      BYTE *pSwap = new BYTE[cy * rowSize];

      glReadPixels(ps -> x1,p -> plotWindow -> windowCY - ps -> y2,cx,cy,GL_RGBA,GL_FLOAT,pSwap);

      for ( int k = 0; k < cy; k++ ) 
         memcpy(ps -> pResult + k * rowSize,pSwap + (cy - k - 1) * rowSize,rowSize);

      delete [] pSwap;

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