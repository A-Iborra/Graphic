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

   pIText_X -> Initialize(hwndOwner,pIOpenGLImp,pIEvaluator,pIDataSet,parentPropertyFloor,parentPropertyCeiling,NULL,NULL,NULL);
   pIText_Y -> Initialize(hwndOwner,pIOpenGLImp,pIEvaluator,pIDataSet,parentPropertyFloor,parentPropertyCeiling,NULL,NULL,NULL);
   pIText_Z -> Initialize(hwndOwner,pIOpenGLImp,pIEvaluator,pIDataSet,parentPropertyFloor,parentPropertyCeiling,NULL,NULL,NULL);

   pIText_X -> put_PartOfWorldDomain(FALSE);
   pIText_Y -> put_PartOfWorldDomain(FALSE);
   pIText_Z -> put_PartOfWorldDomain(FALSE);

   pIText_X -> put_CoordinatePlane(CoordinatePlane_XY);
   pIText_Y -> put_CoordinatePlane(CoordinatePlane_YX);
   pIText_Z -> put_CoordinatePlane(CoordinatePlane_YZ);

   return S_OK;
   }


   HRESULT ViewSet::put_ParentWindow(HWND hwndP) {
   hwndGraphicContainer = hwndP;
   return S_OK;
   }


   HRESULT ViewSet::Properties() {
   IGProperties *pIGProperties;
   QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&pIGProperties));
   pIGProperties -> EditProperties(hwndGraphicContainer,L"View");
   pIGProperties -> Release();
   return S_OK;
   }