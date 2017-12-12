/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>

#include "Graphic_resource.h"
#include "Utils.h"

#include "Properties_i.h"
#include "ViewSet_i.c"
#include "ViewSet.h"
#include "Axis_i.h"
       

   HRESULT ViewSet::Initialize(HWND ho,IOpenGLImplementation *pIOpenGLImp,
                               IEvaluator *piev,
                               IGProperty* parentPlotView,
                               IGProperty *parentTheta,
                               IGProperty *parentPhi,
                               IGProperty *parentSpin,
                               IGProperty *parentFloor,
                               IGProperty *parentCeiling,
                               IDataSet* pIDS,
                               IAxis* pIAxis_x,
                               IAxis* pIAxis_y,
                               IAxis* pIAxis_z) {
 
   hwndOwner = ho;

   pIOpenGLImplementation = pIOpenGLImp;
   pIEvaluator = piev;
   parentPropertyPlotView = parentPlotView;
   parentPropertyPhi = parentPhi;
   parentPropertyTheta = parentTheta;
   parentPropertySpin = parentSpin;
   parentPropertyFloor = parentFloor;
   parentPropertyCeiling = parentCeiling;

   pIDataSet = pIDS;
 
   pIAxis_X = pIAxis_x;
   pIAxis_Y = pIAxis_y;
   pIAxis_Z = pIAxis_z;
 
   pIOpenGLImplementation -> AddRef();

   pIDataSet -> AddRef();

   pIAxis_X -> AddRef();
   pIAxis_Y -> AddRef();
   pIAxis_Z -> AddRef();
 
   iProperties -> Include(parentPropertyPlotView);

   return S_OK;
   }


   HRESULT ViewSet::Properties(void (__stdcall *pWhenDoneCallback)(void *),void *pArg) {
   pPostDialogClientCallback = pWhenDoneCallback;
   pPostDialogClientCallbackArg = pArg;
   initWindows();
   ShowWindow(hwndViewSet,SW_SHOW);
   return S_OK;
   }