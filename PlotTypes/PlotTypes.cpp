
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

   itemShowProperties[1] = new std::function<void(long,HWND,REFIID)>( [](long itemNumber,HWND hwndParent,REFIID refInstanceGUID ) { return; });
   itemShowProperties[2] = itemShowProperties[1];
   itemShowProperties[3] = itemShowProperties[1];
   itemShowProperties[4] = itemShowProperties[1];
   itemShowProperties[5] = itemShowProperties[1];
   itemShowProperties[6] = itemShowProperties[1];

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

   itemExecute[1] = new std::function<void(long,void *,void *,void *)>( [=](long segmentID,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { natural(segmentID); });
   itemExecute[2] = new std::function<void(long,void *,void *,void *)>( [=](long segmentID,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { wireFrame(segmentID); });
   itemExecute[3] = new std::function<void(long,void *,void *,void *)>( [=](long segmentID,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { surface(segmentID); });
   itemExecute[4] = new std::function<void(long,void *,void *,void *)>( [=](long segmentID,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { stacks(segmentID); });
   itemExecute[5] = new std::function<void(long,void *,void *,void *)>( [=](long segmentID,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { blocks(segmentID); });
   itemExecute[6] = new std::function<void(long,void *,void *,void *)>( [=](long segmentID,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) { balls(segmentID); });

   CoCreateInstance(CLSID_InnoVisioNateProperties,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IGProperties,reinterpret_cast<void **>(&pIGProperties));
 
   itemPropertyPageClients[1] = new _IGPropertyPageClient(this,IDDIALOG_NATURAL,propertiesHandler,"Natural");
   itemPropertyPageClients[2] = new _IGPropertyPageClient(this,IDDIALOG_WIREFRAME,propertiesHandler,"WireFrame");
   itemPropertyPageClients[3] = new _IGPropertyPageClient(this,IDDIALOG_SURFACE,propertiesHandler,"Surface");
   itemPropertyPageClients[4] = new _IGPropertyPageClient(this,IDDIALOG_HISTOGRAM,propertiesHandler,"Histogram");
   itemPropertyPageClients[5] = new _IGPropertyPageClient(this,IDDIALOG_BLOCKS,propertiesHandler,"Block");
   itemPropertyPageClients[6] = new _IGPropertyPageClient(this,IDDIALOG_BALLS,propertiesHandler,"Balls");

   pIGProperties -> Add(L"all binary",&propertyAllBinary);

   propertyAllBinary -> directAccess(TYPE_BINARY,&propertiesStart,offsetof(PlotTypes,propertiesEnd) - offsetof(PlotTypes,propertiesStart));

   return;
   }

   PlotTypes::~PlotTypes() {

   if ( pIGProperties )
      pIGProperties -> Release();

   for ( std::pair<long,_IGPropertyPageClient *> p : itemPropertyPageClients )
      p.second -> Release();

   return;
   }


