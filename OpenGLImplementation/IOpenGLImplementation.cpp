// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"
#include <process.h>

   long __stdcall OpenGLImplementor::GetTypeInfoCount(UINT *i) { 
   *i = 1; 
   return S_OK; 
   } 
  
 		
   long __stdcall OpenGLImplementor::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo;
   pITypeInfo -> AddRef();
   return S_OK; 
   } 
  
 
   long __stdcall OpenGLImplementor::GetIDsOfNames(REFIID riid,OLECHAR **rgszNames,UINT cNames,LCID lcid,DISPID *rgdispid) { 
   return DispGetIDsOfNames(pITypeInfo, rgszNames, cNames, rgdispid); 
   } 
 
       
   long __stdcall OpenGLImplementor::Invoke(DISPID dispidMember,
                            REFIID riid,
                            LCID lcid,
                            WORD wFlags, 
                            DISPPARAMS *pdispparams,
                            VARIANT *pvarResult,
                            EXCEPINFO *pexcepinfo, 
                            UINT *puArgErr) { 
   void *ppv;
   QueryInterface(IID_IDispatch,&ppv);
   return pITypeInfo -> Invoke(ppv,dispidMember,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr);
   }
  

   HRESULT OpenGLImplementor::Initialize(IEvaluator *piev) {
   pIEvaluator = piev;
   return S_OK;
   }


   HRESULT OpenGLImplementor::Start() {
   hCreationSemaphore = CreateSemaphore(NULL,0,1,NULL);
   unsigned threadAddr;
   implementationMsgThread = (HANDLE)_beginthreadex(NULL,0,runImplementationMsgQueue,(void *)this,0L,&threadAddr);   
   WaitForSingleObject(hCreationSemaphore,INFINITE);
   CloseHandle(hCreationSemaphore);

Sleep(10);

   return S_OK;
   }

   HRESULT OpenGLImplementor::Stop() {
   hDestructionSemaphore = CreateSemaphore(NULL,0,1,NULL);
   PostMessage(hwndObjectWindow,WM_OPENGLIMPLEMENTATION_STOP,0L,0L);
   WaitForSingleObject(implementationMsgThread,INFINITE);
   return S_OK;
   }


   HRESULT OpenGLImplementor::SetBaseWindow(HWND hwndB) {
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SETBASEWINDOW,hwndB)
   return S_OK;
   }


   HRESULT OpenGLImplementor::SetTargetWindow(HWND hwndNew) {
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SETTARGETWINDOW,hwndNew)
   return S_OK;
   }
 

   HDC OpenGLImplementor::TargetDC() {
   if ( ! plotWindow )
      return NULL;
   return plotWindow -> deviceContext;
   }
 
   HWND OpenGLImplementor::TargetHWND() {
   if ( ! plotWindow )
      return NULL;
   return plotWindow -> hwnd;
   }
 
 
   HRESULT OpenGLImplementor::SetViewProperties(
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

   if ( ! plotWindow )
      return E_UNEXPECTED;

   strCall_SetViewProperties *pstr = new strCall_SetViewProperties();
   pstr -> pPropPlotView = pPropPlotView;
   pstr -> pPropRotationTheta = pPropRotationTheta;
   pstr -> pPropRotationPhi = pPropRotationPhi;
   pstr -> pPropRotationSpin = pPropRotationSpin;
   pstr -> pPropLeftMargin = pPropLeftMargin;
   pstr -> pPropTopMargin = pPropTopMargin;
   pstr -> pPropRightMargin = pPropRightMargin;
   pstr -> pPropBottomMargin = pPropBottomMargin;
   pstr -> pPropMarginUnits = pPropMarginUnits;
   pstr -> pPropStretchToMargins = pPropStretchToMargins;

   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SETVIEWPROPERTIES,pstr)

   return S_OK;
   }


   HRESULT OpenGLImplementor::UnSetViewProperties() {
   if ( ! plotWindow ) return E_UNEXPECTED;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_UNSETVIEWPROPERTIES,0L)
   return S_OK;
   }


   HRESULT OpenGLImplementor::Set2D(IDataSet *id,long m,char dr,char du) {
   if ( ! plotWindow ) return E_UNEXPECTED;
   strCall_Set2D *pstr = new strCall_Set2D();
   pstr -> iDataSet = id;
   pstr -> margins = m;
   pstr -> directionRight = dr;
   pstr -> directionUp = du;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SET2D,pstr)
   return S_OK;
   }


   HRESULT OpenGLImplementor::SetUp(IDataSet *masterDataSet,
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
   if ( ! plotWindow ) 
      return E_UNEXPECTED;
   strCall_Setup *pstr = new strCall_Setup();
   pstr -> masterDataSet = masterDataSet;
   pstr -> pPropPlotView = pPropPlotView;
   pstr -> pPropRotationTheta = pPropRotationTheta;
   pstr -> pPropRotationPhi = pPropRotationPhi;
   pstr -> pPropRotationSpin = pPropRotationSpin;
   pstr -> pPropLeftMargin = pPropLeftMargin;
   pstr -> pPropTopMargin = pPropTopMargin;
   pstr -> pPropRightMargin = pPropRightMargin;
   pstr -> pPropBottomMargin = pPropBottomMargin;
   pstr -> pPropMarginUnits = pPropMarginUnits;
   pstr -> pPropStretchToMargins = pPropStretchToMargins;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SETUP,pstr)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::IsInitialized(VARIANT_BOOL *b) {
   if ( ! plotWindow ) return E_UNEXPECTED;
   *b = plotWindow -> initialized;
   return S_OK;
   }


   HRESULT OpenGLImplementor::Translate(DataPoint* dp) {
   if ( ! plotWindow ) return E_UNEXPECTED;
   DataPoint *dps = new DataPoint();
   dps -> x = dp -> x;
   dps -> y = dp -> y;
   dps -> z = dp -> z;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_TRANSLATE,dps)
   return S_OK;
   }


   HRESULT OpenGLImplementor::Rotate(char axis,double angle) {
   if ( ! plotWindow ) return E_UNEXPECTED;
   strCall_Rotate *ps = new strCall_Rotate;
   ps -> axis = axis;
   ps -> angle = angle;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_ROTATE,ps)
   return S_OK;
   }


   HRESULT OpenGLImplementor::Scale(DataPoint* dp) {
   if ( ! plotWindow ) return E_UNEXPECTED;
   DataPoint *dps = new DataPoint();
   dps -> x = dp -> x;
   dps -> y = dp -> y;
   dps -> z = dp -> z;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SCALE,dps)
   return S_OK;
   }


   HRESULT OpenGLImplementor::SetLighting(
                IGProperty *pPropLightEnabled[],
                IGProperty *pPropAmbientLight[],
                IGProperty *pPropDiffuseLight[],
                IGProperty *pPropSpecularLight[],
                IGProperty *pPropLightPos[],
                IGProperty *pPropCountLights,
                IGProperty *pPropShinyness) {
 
   if ( ! plotWindow ) 
      return E_UNEXPECTED;
   strCall_SetLighting *ps = new strCall_SetLighting;
   ps -> pPropLightEnabled = pPropLightEnabled;
   ps -> pPropAmbientLight = pPropAmbientLight;
   ps -> pPropDiffuseLight = pPropDiffuseLight;
   ps -> pPropSpecularLight = pPropSpecularLight;
   ps -> pPropLightPos = pPropLightPos;
   ps -> pPropCountLights = pPropCountLights;
   ps -> pPropShinyness = pPropShinyness;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SETLIGHTING,ps)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::Push() {
   if ( ! plotWindow ) 
      return E_UNEXPECTED;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_PUSH,0L)
   return S_OK;
   }


   HRESULT OpenGLImplementor::Pop() {
   if ( ! plotWindow ) 
      return E_UNEXPECTED;
   if ( 0 == matrixList.size() )
      return E_UNEXPECTED;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_POP,0L)
   return S_OK;
   }


   HRESULT OpenGLImplementor::put_ViewPort(int *newViewPort) {
   if ( ! newViewPort ) return E_POINTER;
   if ( ! plotWindow ) return E_UNEXPECTED;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SETVIEWPORT,newViewPort)
   return S_OK;
   }


   HRESULT OpenGLImplementor::get_ViewPort(int *pViewPort) {
   if ( ! pViewPort ) return E_POINTER;
   if ( ! plotWindow ) return E_UNEXPECTED;
   memcpy(pViewPort,plotWindow -> openGLState.viewPort,sizeof(plotWindow -> openGLState.viewPort));
   return S_OK;
   }


   HRESULT OpenGLImplementor::get_SupportedLightCount(long *pSupportedLights) {
   if ( ! pSupportedLights ) return E_POINTER;
   *pSupportedLights = SUPPORTED_LIGHT_COUNT;
   return S_OK;
   }

   HRESULT OpenGLImplementor::GetExtents(double *minx,double *miny,double *minz,double *maxx,double *maxy,double *maxz) {
   if ( ! plotWindow )
      return E_UNEXPECTED;
   if ( minx ) *minx = plotWindow -> openGLState.extentsXMin; 
   if ( miny ) *miny = plotWindow -> openGLState.extentsYMin; 
   if ( minz ) *minz = plotWindow -> openGLState.extentsZMin; 
   if ( maxx ) *maxx = plotWindow -> openGLState.extentsXMax; 
   if ( maxy ) *maxy = plotWindow -> openGLState.extentsYMax; 
   if ( maxz ) *maxz = plotWindow -> openGLState.extentsZMax; 
   return S_OK;
   }     

 
   HRESULT OpenGLImplementor::get_HDC(HDC *getHDC) {
   if ( ! getHDC ) return E_POINTER;
   if ( ! plotWindow )
      return E_UNEXPECTED;
   *getHDC = plotWindow -> deviceContext;
   return S_OK;
   }
 
   HRESULT OpenGLImplementor::get_HWND(HWND *getHWND) {
   if ( ! getHWND ) return E_POINTER;
   if ( ! plotWindow )
      return E_UNEXPECTED;
   *getHWND = plotWindow -> hwnd;
   return S_OK;
   }


   HRESULT OpenGLImplementor::get_MousePositionClient(POINT *ptMouse) {
   if ( ! ptMouse ) return E_POINTER;
   if ( ! plotWindow )
      return E_UNEXPECTED;
   GetCursorPos(ptMouse);
   RECT rectWindow;
   GetWindowRect(plotWindow -> hwnd,&rectWindow);
   ptMouse -> x -= rectWindow.left;
   ptMouse -> y -= rectWindow.top;
   return S_OK;
   }
 

   HRESULT OpenGLImplementor::Erase(IGProperty *pPropBackgroundColor) {
   if ( ! plotWindow ) 
      return E_FAIL;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_ERASE,pPropBackgroundColor)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::ResetDepth() {
   if ( ! plotWindow ) return E_FAIL;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_RESETDEPTH,0L)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::PrintSetup() {
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::PrintFinish() {
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::Finalize() {
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_FINALIZE,0L)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::NewLine(DataPoint *p) {
   DataPoint *pdp = new DataPoint();
   pdp -> x = p -> x;
   pdp -> y = p -> y;
   pdp -> z = p -> z;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_NEWLINE,pdp)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::NewPolygon(DataPoint *p) {
   DataPoint *pdp = new DataPoint();
   pdp -> x = p -> x;
   pdp -> y = p -> y;
   pdp -> z = p -> z;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_NEWPOLYGON,pdp)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::Vertex(DataPoint *p) {
   DataPoint *pdp = new DataPoint();
   pdp -> x = p -> x;
   pdp -> y = p -> y;
   pdp -> z = p -> z;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_VERTEX,pdp)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::SetColor(float *pfv) {
   GLfloat fv[] = {pfv[0],pfv[1],pfv[2],pfv[3]};
   //BYTE *pb = (BYTE *)fv;
   //pColor -> get_binaryValue(sizeof(fv),(BYTE**)&pb);
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SETCOLOR,fv)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::SetLineWeight(float lineWeight) {
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SETLINEWEIGHT,(WPARAM)lineWeight)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::BeginSurface(long segmentID,IGProperty *pPropTopColor,IGProperty *pPropBottomColor) {
   strCall_BeginSurface *ps = new strCall_BeginSurface();
   ps -> segmentID = segmentID;
   ps -> pPropTopColor = pPropTopColor;
   ps -> pPropBottomColor = pPropBottomColor;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_BEGINSURFACE,ps)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::EndSurface(long segmentID) {
   long *ps = new long(segmentID);
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_ENDSURFACE,ps)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::BeginWireframe(long segmentID,IGProperty *pPropLineColor,IGProperty *pPropLineWeight) {
   strCall_BeginWireframe *ps = new strCall_BeginWireframe();
   ps -> segmentID = segmentID;
   ps -> pPropLineColor = pPropLineColor;
   ps -> pPropLineWeight = pPropLineWeight;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_BEGINWIREFRAME,ps)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::EndWireframe(long segmentID) {
   long *ps = new long(segmentID);
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_ENDWIREFRAME,ps)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::BeginSolids(long segmentID,IGProperty *pPropTopColor,IGProperty *pPropBottomColor) {
   strCall_BeginSolid *ps = new strCall_BeginSolid();
   ps -> segmentID = segmentID;
   ps -> pPropTopColor = pPropTopColor;
   ps -> pPropBottomColor = pPropBottomColor;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_BEGINSOLID,ps)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::EndSolids(long segmentID) {
   long *pSegment = new long(segmentID);
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_ENDSOLID,pSegment)
   return S_OK;
   }
 
   HRESULT OpenGLImplementor::BeginTriangleSolids(long segmentID,IGProperty *pPropTopColor,IGProperty *pPropBottomColor) {
   strCall_BeginSolid *ps = new strCall_BeginSolid();
   ps -> segmentID = segmentID;
   ps -> pPropTopColor = pPropTopColor;
   ps -> pPropBottomColor = pPropBottomColor;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_BEGINTRIANGLESOLID,ps)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::EndTriangleSolids(long segmentID) {
   long *pSegment = new long(segmentID);
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_ENDTRIANGLESOLID,pSegment)
   return S_OK;
   }
 

   HRESULT OpenGLImplementor::BeginOpenGLMode(long mode) {
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_BEGINMODE,mode)
   return S_OK;
   }


   HRESULT OpenGLImplementor::EndOpenGLMode() {
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_ENDMODE,0)
   return S_OK;
   }

#define TOP 0x8
#define RIGHT 0x2
#define BOTTOM 0x4
#define LEFT 0x1

   HRESULT OpenGLImplementor::ClipPlane(DataPoint *pStart,DataPoint *pEnd,DataPoint *pMin,DataPoint *pMax) {

/* Note: This is not yet a 3-D clipping routine */

/* Both points entirely within the volume ?. Return means trivially accepted. */
   if ( pStart -> x >= pMin -> x && pStart -> x <= pMax -> x &&
        pStart -> y >= pMin -> y && pStart -> y <= pMax -> y &&
        pStart -> z >= pMin -> z && pStart -> z <= pMax -> z &&
        pEnd -> x >= pMin -> x && pEnd -> x <= pMax -> x &&
        pEnd -> y >= pMin -> y && pEnd -> y <= pMax -> y &&
        pEnd -> z >= pMin -> z && pEnd -> z <= pMax -> z ) return S_OK;

/* Both points entirely in a half-volume ?. Return means trivially rejected. */
   if ( pStart -> x < pMin -> x && pEnd -> x < pMin -> x ) return S_FALSE;
   if ( pStart -> y < pMin -> y && pEnd -> y < pMin -> y ) return S_FALSE;
   if ( pStart -> z < pMin -> z && pEnd -> z < pMin -> z ) return S_FALSE;

   if ( pStart -> x > pMax -> x && pEnd -> x > pMax -> x ) return S_FALSE;
   if ( pStart -> y > pMax -> y && pEnd -> y > pMax -> y ) return S_FALSE;
   if ( pStart -> z > pMax -> z && pEnd -> z > pMax -> z ) return S_FALSE;

/* Line needs to be clipped */
 
   struct {
      unsigned int region : 4;
   } testStart,testEnd,testTest;

   DataPoint *pAffected;

   while ( true ) {

      testStart.region = 0x0;
      if ( pStart -> y > pMax -> y )
         testStart.region |= 0x8;
      if ( pStart -> y < pMin -> y )
         testStart.region |= 0x4;
      if ( pStart -> x > pMax -> x )
         testStart.region |= 0x2;
      if ( pStart -> x < pMin -> x )
         testStart.region |= 0x1;

      testEnd.region = 0x0;
      if ( pEnd -> y > pMax -> y )
         testEnd.region |= 0x8;
      if ( pEnd -> y < pMin -> y )
         testEnd.region |= 0x4;
      if ( pEnd -> x > pMax -> x )
         testEnd.region |= 0x2;
      if ( pEnd -> x < pMin -> x )
         testEnd.region |= 0x1;

      if ( testStart.region ) {
         pAffected = pStart;
         testTest.region = testStart.region;
      } else {
         if ( testEnd.region ) {
            pAffected = pEnd;
            testTest.region = testEnd.region;
         } else
            break;
      }

      if ( testTest.region & TOP ) {
         pAffected -> x = pStart -> x + (pEnd -> x - pStart -> x) * (pMax -> y - pStart -> y) / (pEnd -> y - pStart -> y);
         pAffected -> y = pMax -> y;
      } else {
         if ( testTest.region & BOTTOM ) {
            pAffected -> x = pStart -> x + (pEnd -> x - pStart -> x) * (pMin -> y - pStart -> y) / (pEnd -> y - pStart -> y);
            pAffected -> y = pMin -> y;
         } else {
            if ( testTest.region & RIGHT ) {
               pAffected -> y = pStart -> y + (pEnd -> y - pStart -> y) * (pMax -> x - pStart -> x) / (pEnd -> x - pStart -> x);
               pAffected -> x = pMax -> x;
            } else {
               pAffected -> y = pStart -> y + (pEnd -> y - pStart -> y) * (pMin -> x - pStart -> x) / (pEnd -> x - pStart -> x);
               pAffected -> x = pMin -> x;
            }
         }
      }

   }
   
   return S_OK;
   }


   HRESULT OpenGLImplementor::get_BackgroundColor(IGProperty** pProp) {
   if ( ! pProp ) return E_POINTER;
   if ( ! plotWindow ) return E_UNEXPECTED;
   if ( ! plotWindow -> pSaved_BackgroundColor ) return E_UNEXPECTED;
   *pProp = plotWindow -> pSaved_BackgroundColor;
   return S_OK;
   }


   static double piOver2 = 0.0;

   HRESULT OpenGLImplementor::get_ViewThetaRadians(double *pTheta) {

   if ( ! pTheta )
      return E_POINTER;

   if ( ! plotWindow ) 
      return E_UNEXPECTED;

   *pTheta = plotWindow -> openGLState.rotationTheta;

   return S_OK;
   }


   HRESULT OpenGLImplementor::get_ViewPhiRadians(double *pPhi) {

   if ( ! pPhi )
      return E_POINTER;

   if ( ! plotWindow )
      return E_UNEXPECTED;

   *pPhi = plotWindow -> openGLState.rotationPhi;

   return S_OK;
   }


   HRESULT OpenGLImplementor::get_ViewSpinRadians(double *pSpin) {
   if ( ! pSpin ) 
      return E_POINTER;
   if ( ! plotWindow ) 
      return E_UNEXPECTED;
   *pSpin = plotWindow -> openGLState.rotationSpin;
   return S_OK;
   }


   HRESULT OpenGLImplementor::get_ViewThetaDegrees(double *pTheta) {
   if ( ! pTheta )
      return E_POINTER;
   if ( ! plotWindow ) 
      return E_UNEXPECTED;
   if ( 0.0 == piOver2 )
      piOver2 = 2.0 * atan(1.0);
   *pTheta = plotWindow -> openGLState.rotationTheta * 90.0 / piOver2;
   return S_OK;
   }


   HRESULT OpenGLImplementor::get_ViewPhiDegrees(double *pPhi) {
   if ( ! pPhi )
      return E_POINTER;
   if ( ! plotWindow )
      return E_UNEXPECTED;
   if ( 0.0 == piOver2 )
      piOver2 = 2.0 * atan(1.0);
   *pPhi = plotWindow -> openGLState.rotationPhi * 90.0 / piOver2;
   return S_OK;
   }


   HRESULT OpenGLImplementor::get_ViewSpinDegrees(double *pSpin) {
   if ( ! pSpin ) 
      return E_POINTER;
   if ( ! plotWindow ) 
      return E_UNEXPECTED;
   if ( 0.0 == piOver2 )
      piOver2 = 2.0 * atan(1.0);
   *pSpin = plotWindow -> openGLState.rotationSpin * 90.0 / piOver2;
   return S_OK;
   }


   HRESULT OpenGLImplementor::get_PlotView(long *pView) {
   if ( ! pView ) 
      return E_POINTER;
   if ( ! plotWindow ) 
      return E_UNEXPECTED;
   *pView = plotWindow -> openGLState.plotView;
   return S_OK;
   }


   HRESULT OpenGLImplementor::Normal3dv(double *normal) {
   double *pNormal = new double[3];
   pNormal[0] = normal[0];
   pNormal[1] = normal[1];
   pNormal[2] = normal[2];
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_NORMAL3D,pNormal)
   return S_OK;
   }
 
   HRESULT OpenGLImplementor::NormalDp(DataPoint *pDPNormal) {
   double *pNormal = new double[3];
   pNormal[0] = pDPNormal -> x;
   pNormal[1] = pDPNormal -> y;
   pNormal[2] = pDPNormal -> z;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_NORMAL3D,pNormal)
   return S_OK;
   }

   HRESULT OpenGLImplementor::SetColor3dv(double *pDoubleColors) {
   GLfloat *pColors = new GLfloat[4];
   pColors[0] = (GLfloat)pDoubleColors[0];
   pColors[1] = (GLfloat)pDoubleColors[1];
   pColors[2] = (GLfloat)pDoubleColors[2];
   pColors[3] = 0.0;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SETCOLOR,pColors)
   delete [] pColors;
   return S_OK;
   }
 

   HRESULT OpenGLImplementor::WindowToData(DataPoint *dpSource,DataPoint *dpTarget) {

   if ( ! dpSource || ! dpTarget ) 
      return E_POINTER;

   if ( ! plotWindow ) 
      return E_UNEXPECTED;

   strCall_WindowToData ps;

   ps.dpSource.x = dpSource -> x;
   ps.dpSource.y = dpSource -> y;
   ps.dpSource.z = dpSource -> z;
   
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_WINDOWTODATA,&ps)
   
   dpTarget -> x = ps.dpTarget.x;
   dpTarget -> y = ps.dpTarget.y;
   dpTarget -> z = ps.dpTarget.z;

   return ps.result;
   }
 
 
   HRESULT OpenGLImplementor::DataToWindow(DataPoint *dpSource,UnitOfMeasure uom,DataPoint *dpTarget) {
 
   if ( ! dpSource || ! dpTarget ) return E_POINTER;
   if ( ! plotWindow ) return E_UNEXPECTED;

   strCall_DataToWindow ps;

   ps.dpSource.x = dpSource -> x;
   ps.dpSource.y = dpSource -> y;
   ps.dpSource.z = dpSource -> z;

   ps.uom = uom;

   ps.pdpTarget = dpTarget;
   
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_DATATOWINDOW,&ps)

   return S_OK;
   }
 
   
   HRESULT OpenGLImplementor::DataListToWindow(DataList *dlSource,DataList *dlTarget) {

   if ( ! dlSource || ! dlTarget ) return E_POINTER;
   if ( ! plotWindow ) return E_UNEXPECTED;

   strCall_DataListToWindow ps;

   ps.dlSource = dlSource;
   ps.dlTarget = dlTarget;

   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_DATALISTTOWINDOW,&ps)

   return S_OK;
   }


   HRESULT OpenGLImplementor::PixelsToUnits(char direction,double inPixels,double *answer) {
   
   strCall_PixelsToUnits ds;

   ds.direction = direction;
   ds.inPixels = inPixels;
   ds.answer = answer;

   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_PIXELSTOUNITS,&ds)

   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::UnitsToPixels(char direction,double inUnits,double *answer) {

   strCall_UnitsToPixels ds;

   ds.direction = direction;
   ds.inUnits = inUnits;
   ds.answer = answer;
   
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_UNITSTOPIXELS,&ds)

   return S_OK;
   }


   HRESULT OpenGLImplementor::PixelPoint(DataPoint *pBeginning,DataPoint *pAnswer) {

   if ( ! pBeginning ) return E_POINTER;

   if ( ! pAnswer ) return E_POINTER;

   strCall_PixelPoint ds;
   ds.pBeginning = pBeginning;
   ds.pAnswer = pAnswer;
   
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_PIXELPOINT,&ds)

   return ds.result;
   }


   HRESULT OpenGLImplementor::PixelVector(DataPoint *pBeginning,DataPoint *pEnding,DataPoint *pAnswer) {

   if ( ! pBeginning ) return E_POINTER;
   if ( ! pEnding ) return E_POINTER;
   if ( ! pAnswer ) return E_POINTER;

   strCall_PixelVector ds;
   ds.pBeginning = pBeginning;
   ds.pEnding = pEnding;
   ds.pAnswer = pAnswer;

   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_PIXELVECTOR,&ds)

   return ds.result;
   }


   HRESULT OpenGLImplementor::AdviseGSystemStatusBar(IGSystemStatusBar* p) {
   if ( ! p ) {
      if ( ! pIGSystemStatusBar ) return E_UNEXPECTED;
      pIGSystemStatusBar = NULL;
   }
   pIGSystemStatusBar = p;
   return S_OK;
   }

   HRESULT OpenGLImplementor::GetPixels(long x1,long y1,long x2,long y2,BYTE *pResult) {

   strCall_ReadPixels readPixels = {0};

   readPixels.x1 = x1;
   readPixels.y1 = y1;
   readPixels.x2 = x2;
   readPixels.y2 = y2;
   readPixels.pResult = pResult;

   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_READPIXELS,&readPixels)

   return S_OK;
   }


   HRESULT OpenGLImplementor::EnableOpenGLCapability(int glEnum,BOOL isEnabled) {

   strCall_SetCapability setCap = {0};

   setCap.glEnum = glEnum;
   setCap.isEnabled = isEnabled;

   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_SET_CAPABILITY,&setCap);

   return S_OK;
   }


   HRESULT OpenGLImplementor::GetPickBoxHits(POINTL *ptl,long pickWindowWidth,long pickWindowHeight,unsigned int *hitTable,long hitTableSize,long *pCallLists,unsigned int *hitTableHits) {
   strCall_GetPickBoxHits *ps = new strCall_GetPickBoxHits();
   ps -> ptl = ptl;
   ps -> pickWindowWidth = pickWindowWidth;
   ps -> pickWindowHeight = pickWindowHeight;
   ps -> hitTable = hitTable;
   ps -> hitTableSize = hitTableSize;
   ps -> pCallLists = pCallLists;
   ps -> hitTableHits = hitTableHits;
   ps -> returnValue = S_OK;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_GETPICKBOXHITS,ps)
   HRESULT hr = ps -> returnValue;
   delete ps;
   return hr;
   }

   BOOL OpenGLImplementor::IsRendered() {

   BOOL rv = FALSE;

   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_ISRENDERED,&rv)

   return rv;
   }
