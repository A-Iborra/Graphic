
#include "plot.h"

#include "list.cpp"

   BasePlot::BasePlot() : 
      pIDataSet(NULL),
      pIDataSetDomain(NULL),
      pIGraphicSegment(NULL),
      pIOpenGLImplementation(NULL),
      pWhenChangedCallback(NULL),
      pWhenChangedCallbackArg(NULL),
      whenChangedCallbackCookie(0L),
      pIEvaluator(NULL),
      pPropertyLastDrawLineWeight(NULL),
      externalDataSet(FALSE),
      eraseMode(FALSE),
      refCount(0) {

   CoCreateInstance(CLSID_InnoVisioNateProperty,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IGProperty,reinterpret_cast<void **>(&pPropertyLastDrawLineWeight));

   pPropertyLastDrawLineWeight -> put_type(TYPE_LONG);

   CoCreateInstance(CLSID_GraphicSegment,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IGraphicSegment,reinterpret_cast<void **>(&pIGraphicSegment));
 
   return;
   }
 
 
   BasePlot::~BasePlot() {
   if ( ! externalDataSet ) 
      if ( pIDataSet ) pIDataSet -> Release();

   if ( pPropertyLastDrawLineWeight ) 
      pPropertyLastDrawLineWeight -> Release();

   IText *t;
   while ( t = textList.GetFirst() ) {
      t -> Release();
      textList.Remove(t);
   }

   if ( pIDataSetDomain ) {
      pIDataSetDomain -> RemoveIncludedDomain(pIDataSet);
      pIDataSetDomain -> Release();
   }

   if ( pIGraphicSegment )
      pIGraphicSegment -> Release();

   pIGraphicSegment = NULL;

   return;
   }


   int BasePlot::ReplaceDataSet(IDataSet *newDataSet) {
   if ( pIDataSet) pIDataSet -> Release();
   pIDataSet = newDataSet;
   if ( pIDataSet) pIDataSet -> AddRef();
   pIDataSet -> put_floor(pPropertyFloor);
   pIDataSet -> put_ceiling(pPropertyCeiling);
   return TRUE;
   }
 
 
   HANDLE BasePlot::PlotData(DataPoint v[],IGraphicSegment *segOveride) {

   VARIANT_BOOL isInitalized;
   pIOpenGLImplementation -> IsInitialized(&isInitalized);
   if ( ! isInitalized ) return (HANDLE)TRUE;

   IGraphicSegment* s;
 
   if ( segOveride ) 
      s = segOveride;
   else
      s = pIGraphicSegment;
 
   s -> Open();
 
   int k = 0;
   while ( v[k].x != -DBL_MAX ) vertex(v[k++]);
 
   s -> Close(TRUE);
 
   return (HANDLE)TRUE;
   }
 

   HRESULT BasePlot::PlotData(IDataSet *ds) {

   DataList *item = NULL;
 
   pIGraphicSegment -> Open();
 
   ds -> peek(item,&item);
   while ( item ) {
      if ( -DBL_MAX == item -> data.x ) {
         while ( item && -DBL_MAX == item -> data.x ) ds -> peek(item,&item);
         if ( ! item ) break;
         pIOpenGLImplementation -> NewLine(&item -> data);
      }
      pIOpenGLImplementation -> Vertex(&item -> data);
      ds -> peek(item,&item);
   }
 
   pIGraphicSegment -> Close(TRUE);
 
   IText *t = NULL;
   while ( t = textList.GetNext(t) ) 
      t -> Draw();

   return S_OK;
   }
 
   
   int BasePlot::newline(DataPoint p) {
   pIOpenGLImplementation -> NewLine(&p);
   return TRUE;
   }
 
 
   int BasePlot::vertex(DataPoint p) {
   pIOpenGLImplementation -> Vertex(&p);
   return TRUE;
   }
 
 

