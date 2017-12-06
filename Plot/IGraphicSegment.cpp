/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>

#include "plot.h"
#include "GraphicSegment.h"


   long __stdcall GraphicSegment::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown *>(this); 
   else
      if ( riid == IID_IGraphicSegment ) 
         *ppv = static_cast<IGraphicSegment *>(this);
      else
         return E_NOINTERFACE;
   reinterpret_cast<IUnknown*>(*ppv) -> AddRef(); 
   return S_OK; 
   }
 
   unsigned long __stdcall GraphicSegment::AddRef() {
   return ++refCount;
   }
   unsigned long __stdcall GraphicSegment::Release() {
   if ( --refCount == 0 ) {
      delete this;
      return 0;
   }
   return refCount;
   }
 
 
   HRESULT GraphicSegment::Initialize(IOpenGLImplementation *pimp,IGProperty* propLineColor,IGProperty* propLineWeight) {
 
   if ( pIOpenGLImplementation )
      pIOpenGLImplementation -> Release();
 
   pIOpenGLImplementation = pimp;
   if ( pIOpenGLImplementation )
      pIOpenGLImplementation -> AddRef();
 
   if ( propertyLineColor )
      propertyLineColor -> Release();
   propertyLineColor = propLineColor;
   if ( propertyLineColor )
      propertyLineColor -> AddRef();
 
   if ( propertyLineWeight )
      propertyLineWeight -> Release();
   propertyLineWeight = propLineWeight;
   if ( propertyLineWeight )
      propertyLineWeight -> AddRef();

   return S_OK;
   }
 
 
   HRESULT GraphicSegment::get_SegmentID(long *getID) {
   if ( ! segmentID ) 
      Create(NULL);
   *getID = segmentID;
   return S_OK;
   }
 
 
 
   HRESULT GraphicSegment::put_ActionTable(IGraphicSegmentAction *newActionTable) {
   pIGraphicSegmentAction = newActionTable;
   return S_OK;
   }
   HRESULT GraphicSegment::get_ActionTable(IGraphicSegmentAction **getActionTable) {
   if ( ! getActionTable ) return E_POINTER;
   *getActionTable = pIGraphicSegmentAction;
   return S_OK;
   }
 
 
   HRESULT GraphicSegment::put_ColorProperty(IGProperty* pColor) {
   propertyLineColor = pColor;
   return S_OK;
   }
 
 
   HRESULT GraphicSegment::get_ColorProperty(IGProperty** ppColor) {
   if ( ! ppColor ) return E_POINTER;
   *ppColor = propertyLineColor;
   return S_OK;
   }

 
   HRESULT GraphicSegment::put_LineWeight(IGProperty* pLineWeight) {
   propertyLineWeight = pLineWeight;
   return S_OK;
   }
 
 
   HRESULT GraphicSegment::get_LineWeight(IGProperty** ppLineWeight) {
   if ( ! ppLineWeight ) return E_POINTER;
   *ppLineWeight = propertyLineWeight;
   return S_OK;
   }

 
   HRESULT GraphicSegment::Create(long *pSegmentID) {
   Destroy();
   pIOpenGLImplementation -> NewSegments(1,&segmentID);
   if ( pSegmentID )
      *pSegmentID = segmentID;
   return S_OK;
   }

   HRESULT GraphicSegment::Redraw() {
   if ( ! segmentID ) return E_UNEXPECTED;
   pIOpenGLImplementation -> RedrawSegment(segmentID);
   return S_OK;
   }
 
 
   HRESULT GraphicSegment::Open() {
   if ( ! segmentID ) 
      Create(NULL);
   pIOpenGLImplementation -> OpenSegment(segmentID,propertyLineColor,propertyLineWeight);
   return S_OK;
   }
 
 
   HRESULT GraphicSegment::Close(USHORT drawOnClose) {
   pIOpenGLImplementation -> CloseSegment(segmentID,drawOnClose);
   return S_OK;
   }


   HRESULT GraphicSegment::Destroy() {
   if ( segmentID ) 
      pIOpenGLImplementation -> DeleteSegment(segmentID);
   segmentID = 0;
   return S_OK;
   }