// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define DEFINE_DATA

#include "PlotTypes.h"

#include "PlotTypes_i.c"
#include "Properties_i.c"
#include "GSystem_i.c"

   PlotTypes::PlotTypes() {

   refCount = 1;

   itemName[1] = SysAllocString(L"Natural");
   itemName[2] = SysAllocString(L"WireFrame");
   itemName[3] = SysAllocString(L"Surface");
   itemName[4] = SysAllocString(L"Histogram");
   itemName[5] = SysAllocString(L"Blocks");
   itemName[6] = SysAllocString(L"Balls");

   itemIs3DOnly[1] = false;
   itemIs3DOnly[2] = true;
   itemIs3DOnly[3] = true;
   itemIs3DOnly[4] = false;
   itemIs3DOnly[5] = false;
   itemIs3DOnly[6] = false;

   itemUseMaterialShading[1] = false;
   itemUseMaterialShading[2] = true;
   itemUseMaterialShading[3] = true;
   itemUseMaterialShading[4] = true;
   itemUseMaterialShading[5] = true;
   itemUseMaterialShading[6] = true;

   itemHasProperties[1] = true;
   itemHasProperties[2] = true;
   itemHasProperties[3] = true;
   itemHasProperties[4] = true;
   itemHasProperties[5] = true;
   itemHasProperties[6] = true;

   executePrep = new std::function<void(void *,void *,void*)>( [=](void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet)
      {
         pIPlot = (IPlot *)pvIPlot;
         pIOpenGLImplementation = (IOpenGLImplementation *)pvIOpenGLImplementation;
         pIDataSet = (IDataSet *)pvIDataSet;
         pIPlot -> get_TopSurfaceColorProperty(&propertyTopSurfaceColor);
         pIPlot -> get_BottomSurfaceColorProperty(&propertyBottomSurfaceColor);
         pIPlot -> get_LineColorProperty(&propertyLineColor);
         pIPlot -> get_LineWeightProperty(&propertyLineWeight);
      });

   itemExecute[1] = new std::function<void(long,commonProperties *pProperties,void *,void *,void *)>( 
            [=](long segmentID,commonProperties *pProperties,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { natural(pProperties,segmentID); });

   itemExecute[2] = new std::function<void(long,commonProperties *pProperties,void *,void *,void *)>( 
            [=](long segmentID,commonProperties *pProperties,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { wireFrame(pProperties,segmentID); });

   itemExecute[3] = new std::function<void(long,commonProperties *pProperties,void *,void *,void *)>( 
            [=](long segmentID,commonProperties *pProperties,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { surface(pProperties,segmentID); });

   itemExecute[4] = new std::function<void(long,commonProperties *pProperties,void *,void *,void *)>( 
            [=](long segmentID,commonProperties *pProperties,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { stacks(pProperties,segmentID); });

   itemExecute[5] = new std::function<void(long,commonProperties *pProperties,void *,void *,void *)>( 
            [=](long segmentID,commonProperties *pProperties,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { blocks(pProperties,segmentID); });

   itemExecute[6] = new std::function<void(long,commonProperties *pProperties,void *,void *,void *)>( 
            [=](long segmentID,commonProperties *pProperties,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { balls(pProperties,segmentID); });

   CoCreateInstance(CLSID_InnoVisioNateProperties,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IGProperties,reinterpret_cast<void **>(&pIGProperties));
 
   itemPropertyPageClients[1] = new _IGPropertyPageClient(this,IDDIALOG_NATURAL,propertiesHandler,"Natural");
   itemPropertyPageClients[2] = new _IGPropertyPageClient(this,IDDIALOG_WIREFRAME,propertiesHandler,"WireFrame");
   itemPropertyPageClients[3] = new _IGPropertyPageClient(this,IDDIALOG_SURFACE,propertiesHandler,"Surface");
   itemPropertyPageClients[4] = new _IGPropertyPageClient(this,IDDIALOG_HISTOGRAM,propertiesHandler,"Histogram");
   itemPropertyPageClients[5] = new _IGPropertyPageClient(this,IDDIALOG_BLOCKS,blocksPropertiesHandler,"Block");
   itemPropertyPageClients[6] = new _IGPropertyPageClient(this,IDDIALOG_BALLS,propertiesHandler,"Balls");

   pIGProperties -> Add(L"all binary",&propertyAllBinary);

   propertyAllBinary -> directAccess(TYPE_BINARY,&properties,sizeof(commonProperties));

   return;
   }


   PlotTypes::~PlotTypes() {

   if ( pIGProperties )
      pIGProperties -> Release();

   for ( std::pair<long,_IGPropertyPageClient *> p : itemPropertyPageClients )
      p.second -> Release();

   for ( std::pair<long,void *> p : allocatedInstancePropertiesStorage )  {
      if ( ((commonProperties *)p.second) -> pvAdditionalProperties )
         delete [] ((commonProperties *)p.second) -> pvAdditionalProperties;
      delete [] p.second;
   }
   return;
   }


   commonProperties *PlotTypes::findProperties(long itemNumber,REFIID instanceGUID) {

   BSTR bstrCLSID;

   StringFromCLSID(instanceGUID,&bstrCLSID);

   long key = HashCode(bstrCLSID);

   CoTaskMemFree(bstrCLSID);

   if ( ! ( allocatedInstancePropertiesStorage.find(key) == allocatedInstancePropertiesStorage.end() )  )
      return allocatedInstancePropertiesStorage[key];

   if ( ! ( GUID_NULL == instanceGUID ) ) {
      InitNew(itemNumber,instanceGUID);
      return allocatedInstancePropertiesStorage[key];
   }

   return &properties;
   }