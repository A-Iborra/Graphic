
#pragma once

#include "plot.h"

  class SegmentFactory : public IClassFactory {
  public:

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();
     STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
     STDMETHOD (LockServer)(BOOL fLock);

     SegmentFactory() : refCount(0) {};
     ~SegmentFactory() {};

  private:
     int refCount;
  };


  class GraphicSegment : public IGraphicSegment {
  public:

     GraphicSegment(IUnknown *pUnkOuter = (IUnknown *)NULL);
     ~GraphicSegment();

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();

     STDMETHOD(Initialize)(IOpenGLImplementation *,IGProperty*,IGProperty*);

     STDMETHOD(get_SegmentID)(long *getID);

     STDMETHOD(put_ActionTable)(IGraphicSegmentAction *);
     STDMETHOD(get_ActionTable)(IGraphicSegmentAction **);

     STDMETHOD(put_ColorProperty)(IGProperty*);
     STDMETHOD(get_ColorProperty)(IGProperty**);

     STDMETHOD(put_LineWeight)(IGProperty*);
     STDMETHOD(get_LineWeight)(IGProperty**);

     STDMETHOD(Create)(long *);
     STDMETHOD(Open)();
     STDMETHOD(Close)(USHORT drawOnClose);
     STDMETHOD(Redraw)();
     STDMETHOD(Destroy)();

  private:

     long segmentID;
     unsigned int refCount;

     IUnknown *pIUnknownOuter;
     IOpenGLImplementation *pIOpenGLImplementation;
     IGraphicSegmentAction *pIGraphicSegmentAction;

     IGProperty *propertyLineColor;
     IGProperty *propertyLineWeight;

  };