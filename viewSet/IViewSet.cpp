// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ViewSet.h"

   HRESULT ViewSet::Initialize(IOpenGLImplementation *pIOpenGLImp,
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


   HRESULT ViewSet::Properties(HWND hwndParent,void (__stdcall *pWhenDoneCallback)(void *),void *pArg) {
   pPostDialogClientCallback = pWhenDoneCallback;
   pPostDialogClientCallbackArg = pArg;
   initWindows(hwndParent);
   ShowWindow(hwndViewSet,SW_SHOW);
   return S_OK;
   }