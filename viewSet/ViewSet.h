// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <ocidl.h>
 
#include "list.h"
#include "utils.h"

#include "GSystem_i.h"
#include "Properties_i.h"

#include "Variable_i.h"
#include "Evaluator_i.h"

#include "DataSet_i.h"

#include "OpenGLImplementation_i.h"

#include "Plot_i.h"
#include "Text_i.h"
#include "Axis_i.h"

#include "GraphicControl_i.h"

#include "viewSet_i.h"


  class ViewSet : 
         public IViewSet, 
         public IGPropertiesClient,
         public IGPropertyPageClient,
         public IConnectionPointContainer {

  public:

      ViewSet(IUnknown *pUnkOuter);
      ~ViewSet();

//      IViewSet

      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_(ULONG, AddRef)();
      STDMETHOD_(ULONG, Release)();

      STDMETHOD(Initialize)(IOpenGLImplementation*,IEvaluator*,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IDataSet*,IAxis*,IAxis*,IAxis*);

      STDMETHOD(Properties)(HWND hwndParent,void (__stdcall *pWhenDoneCallback)(void *),void *pArg);

//      IPropertiesClient

      STDMETHOD(SavePrep)();
      STDMETHOD(InitNew)();
      STDMETHOD(Loaded)();
      STDMETHOD(Saved)();
      STDMETHOD(IsDirty)();
      STDMETHOD(GetClassID)(BYTE *pCLSID);

//      IPropertyPageClient

      STDMETHOD(BeforeAllPropertyPages)();
      STDMETHOD(GetPropertyPagesInfo)(long* countPages,SAFEARRAY** stringDescriptions,SAFEARRAY** stringHelpDirs,SAFEARRAY** pSize);
      STDMETHOD(CreatePropertyPage)(long,HWND,RECT*,BOOL,HWND* hwndPropertyPage);
      STDMETHOD(Apply)();
      STDMETHOD(IsPageDirty)(long,BOOL*);
      STDMETHOD(Help)(BSTR bstrHelpDir);
      STDMETHOD(TranslateAccelerator)(long,long*);
      STDMETHOD(AfterAllPropertyPages)(BOOL);
      STDMETHOD(DestroyPropertyPage)(long);

      STDMETHOD(GetPropertySheetHeader)(void *pHeader);
      STDMETHOD(get_PropertyPageCount)(long *pCount);
      STDMETHOD(GetPropertySheets)(void *pSheets);

//      IConnectionPointContainer

      STDMETHOD(FindConnectionPoint)(REFIID riid,IConnectionPoint **);
      STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints **);

      void fire_Clicked();
      void fire_ReDraw();

      struct _IConnectionPoint : IConnectionPoint {
      public:

         STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

         STDMETHOD (GetConnectionInterface)(IID *);
         STDMETHOD (GetConnectionPointContainer)(IConnectionPointContainer **ppCPC);
         STDMETHOD (Advise)(IUnknown *pUnk,DWORD *pdwCookie);
         STDMETHOD (Unadvise)(DWORD);
         STDMETHOD (EnumConnections)(IEnumConnections **ppEnum);

         _IConnectionPoint(ViewSet *pp,REFIID outGoingInterfaceType);
	      ~_IConnectionPoint();
	      IUnknown *AdviseSink() { return adviseSink; };

      private:

         int getSlot();
         int findSlot(DWORD dwCookie);

	      IUnknown *adviseSink;
	      ViewSet *pParent;
         DWORD nextCookie;
	      int countConnections,countLiveConnections;

         REFIID outGoingInterfaceType;

         CONNECTDATA *connections;

         long refCount;

      } *pIConnectionPoint;

      struct _IEnumConnectionPoints : IEnumConnectionPoints {
      public:

         STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

 	      STDMETHOD (Next)(ULONG cConnections,IConnectionPoint **rgpcn,ULONG *pcFetched);
         STDMETHOD (Skip)(ULONG cConnections);
         STDMETHOD (Reset)();
         STDMETHOD (Clone)(IEnumConnectionPoints **);

	      _IEnumConnectionPoints(ViewSet *pp,_IConnectionPoint **cp,int connectionPointCount);
         ~_IEnumConnectionPoints();

      private:

         int cpCount,enumeratorIndex;
	      ViewSet *pParent;
	      _IConnectionPoint **connectionPoints;

      } *pIEnumConnectionPoints;

      struct _IEnumerateConnections : public IEnumConnections {
      public:

         _IEnumerateConnections(IUnknown* pParentUnknown,ULONG cConnections,CONNECTDATA* paConnections,ULONG initialIndex);
         ~_IEnumerateConnections();

         STDMETHODIMP         QueryInterface(REFIID, void **);
         STDMETHODIMP_(ULONG) AddRef();
         STDMETHODIMP_(ULONG) Release();
         STDMETHODIMP         Next(ULONG, CONNECTDATA*, ULONG*);
         STDMETHODIMP         Skip(ULONG);
         STDMETHODIMP         Reset();
         STDMETHODIMP         Clone(IEnumConnections**);

      private:

         ULONG       refCount;
         IUnknown    *pParentUnknown;
         ULONG       enumeratorIndex;
         ULONG       countConnections;
         CONNECTDATA *connections;

      } *pIEnumerateConnections;

  private:

     HWND hwndViewSet,hwndGraphic;
     HWND hwndGraphicContainer;

     POINTL ptlFrame;
     SIZEL sizelFrame;

     BOOL isVisible;

     IDataSet* pIDataSet;
     IAxis* pIAxis_X,*pIAxis_Y,*pIAxis_Z;

     IUnknown *pIUnknownOuter;
     IUnknown *pIUnknownProperties;

     IGProperties *iProperties;
     IGProperty *parentPropertyPlotView;
     IGProperty *parentPropertyPhi,*parentPropertyTheta,*parentPropertySpin;
     IGProperty *parentPropertyFloor,*parentPropertyCeiling;

     IOpenGLImplementation *pIOpenGLImplementation;
     IEvaluator *pIEvaluator;

     void (__stdcall *pPostDialogClientCallback)(void *) {NULL};
     void *pPostDialogClientCallbackArg {NULL};

     unsigned int refCount;

     int render();
     int calcSize();
     int initWindows(HWND hwndParent);
     int size();

     static LRESULT CALLBACK viewsetHandler(HWND hwnd,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK viewsetGraphicHandler(HWND hwnd,UINT,WPARAM,LPARAM);

  };

#define VIEWSET_MINCX  400        
#define VIEWSET_MINCY  300

  extern HMODULE hModule;
  extern char szModuleName[1024];
  extern OLECHAR wstrModuleName[1024];

