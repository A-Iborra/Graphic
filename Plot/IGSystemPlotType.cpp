
#include "Plot.h"

   Plot::_IGSystemPlotType::_IGSystemPlotType(Plot *pp) :
      pParent(pp)
   {
   return;
   }

   Plot::_IGSystemPlotType::~_IGSystemPlotType() {
   return;
   }

   long __stdcall Plot::_IGSystemPlotType::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown *>(this); 
   else
      if ( riid == IID_IGSystemPlotType ) 
         *ppv = static_cast<IGSystemPlotType *>(this);
      else
         return pParent -> QueryInterface(riid,ppv);
   AddRef();
   return S_OK; 
   }

   unsigned long __stdcall Plot::_IGSystemPlotType::AddRef() {
   return ++refCount;
   }
   unsigned long __stdcall Plot::_IGSystemPlotType::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }

   HRESULT Plot::_IGSystemPlotType::get_Count(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 2;
   return S_OK;
   }


   HRESULT Plot::_IGSystemPlotType::get_Name(long itemNumber,BSTR *pBstr) {
   if ( ! pBstr )
      return E_POINTER;
   switch ( itemNumber ) {
   case 1:
      *pBstr = SysAllocString(L"WireFrame");
      break;
   case 2:
      *pBstr = SysAllocString(L"Surface");
      break;
   default:
      return E_UNEXPECTED;
   }
   return S_OK;
   }

   HRESULT Plot::_IGSystemPlotType::get_Is3DOnly(long itemNumber,VARIANT_BOOL *pAnswer) {
   if ( ! pAnswer )
      return E_POINTER;
   switch ( itemNumber ) {
   case 1:
      *pAnswer = VARIANT_TRUE;
      break;
   case 2:
      *pAnswer = VARIANT_TRUE;
      break;
   default:
      return E_UNEXPECTED;
   }
   return S_OK;
   }

   HRESULT Plot::_IGSystemPlotType::put_DataSet(void *pvIDataSet) {
   pIDataSet = (IDataSet *)pvIDataSet;
   return S_OK;
   }

   HRESULT Plot::_IGSystemPlotType::put_OpenGLImplementation(void *pvOpenGLImplementation) {
   pIOpenGLImplementation = (IOpenGLImplementation *)pvOpenGLImplementation;
   return S_OK;
   }

   HRESULT Plot::_IGSystemPlotType::Execute(long itemNumber,long segmentID,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) {

   put_DataSet(pvIDataSet);
   put_OpenGLImplementation(pvIOpenGLImplementation);

   switch ( itemNumber ) {
   case 1:
      doWireFrame(segmentID,(Plot *)pvIPlot);
      break;
   case 2:
      doSurface(segmentID,(Plot *)pvIPlot);
      break;
   }

   return S_OK;
   }