// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "basePlot.h"
#include <list>
#include <map>

#include "GMessage.h"

  class Plot : public BasePlot, 
               public IPlot, 
               public IPlotServices,
               public IGraphicSegmentAction,
               public IGPropertiesClient,
               public IGPropertyPageClient,
               public IViewObjectEx  {
  private:

     Plot(IUnknown *pUnkOuter);
     ~Plot();
                                                      
     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();

//      IDispatch

     STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
     STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
     STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
     STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

//      IPlot

     STDMETHOD(Initialize)(IDataSet *,IOpenGLImplementation *,IEvaluator *,IGProperty*,IGProperty *,IGProperty *,IGProperty *,IGProperty *,
                           IGProperty*,
                           IGProperty *pPropertyXFloor,IGProperty *pPropertyXCeiling,
                           IGProperty *pPropertyYFloor,IGProperty *pPropertyYCeiling,
                           IGProperty *pPropertyZFloor,IGProperty *pPropertyZCeiling,
                              void (__stdcall *pWhenDoneCallback)(void *,ULONG_PTR),void *pArg,ULONG_PTR);

     STDMETHOD(PrepareForData)();
     STDMETHOD(TakeDataPoint)(DataPoint*);
     STDMETHOD(TakeDataString)(BSTR);
     STDMETHOD(ResetData)();
     STDMETHOD(LastPoint)(DataPoint*);

     STDMETHOD(StubPlot)();
     STDMETHOD(AddText)(void *text,char *name,int idNumber);
     STDMETHOD(RemoveText)(void *t);
     STDMETHOD(PrepData)();
     STDMETHOD(Draw)();
     STDMETHOD(Redraw)();
     STDMETHOD(Erase)();
     STDMETHOD(Plotter)(long cntPlots,IPlot*** thePlotList);
     STDMETHOD(PlotList)(long cntPlots,IPlot*** theList,long *theThreadHandle);

     STDMETHOD(put_Name)(BSTR);
     STDMETHOD(get_Name)(BSTR*);

     STDMETHOD(put_OkToPlot)(long okayToPlot);
     STDMETHOD(get_OkToPlot)(long *okayToPlot);

     STDMETHOD(put_AutoPlotViewDetection)(long autoPlotViewDetection);
     STDMETHOD(get_AutoPlotViewDetection)(long *autoPlotViewDetection);

     STDMETHOD(put_OverrideOwnerPlotView)(VARIANT_BOOL);
     STDMETHOD(get_OverrideOwnerPlotView)(VARIANT_BOOL*);

     STDMETHOD(put_OverrideOwnerPlotType)(VARIANT_BOOL);
     STDMETHOD(get_OverrideOwnerPlotType)(VARIANT_BOOL*);

     STDMETHOD(put_PlotViewProperty)(IGProperty*);
     STDMETHOD(get_PlotViewProperty)(IGProperty**);

     STDMETHOD(put_PlotTypeProperty)(IGProperty*);
     STDMETHOD(get_PlotTypeProperty)(IGProperty**);

     STDMETHOD(get_PlotTypeUsesMaterialShading)(long the2DTypes,long the3DTypes,VARIANT_BOOL *);

     STDMETHOD(get_SegmentID)(long*);

     STDMETHOD(put_ActionTable)(IGraphicSegmentAction *);
     STDMETHOD(get_ActionTable)(IGraphicSegmentAction **);

     STDMETHOD(put_IDataSet)(IDataSet *newDataSet);
     STDMETHOD(get_IDataSet)(IDataSet **getDataSet);

     enum DataArity __stdcall DataArity();

     STDMETHOD(put_PlotNotify)(IPlotNotify *);
     STDMETHOD(get_PlotNotify)(IPlotNotify **);

     STDMETHOD(put_Color)(SAFEARRAY *);
     STDMETHOD(get_Color)(SAFEARRAY **);


     STDMETHOD(put_LineColorProperty)(IGProperty*);
     STDMETHOD(get_LineColorProperty)(IGProperty**);

     STDMETHOD(put_LineWeightProperty)(IGProperty*);
     STDMETHOD(get_LineWeightProperty)(IGProperty**);


     STDMETHOD(put_TopSurfaceColorProperty)(IGProperty*);
     STDMETHOD(get_TopSurfaceColorProperty)(IGProperty**);


     STDMETHOD(put_BottomSurfaceColorProperty)(IGProperty*);
     STDMETHOD(get_BottomSurfaceColorProperty)(IGProperty**);


     STDMETHOD(put_ParentWindow)(HWND);

     STDMETHOD(get_SegmentCount)(long *);
     STDMETHOD(GetSegments)(long *);

     STDMETHOD(EditProperties)();

     STDMETHOD(AdviseGSystemStatusBar)(IGSystemStatusBar*);

     STDMETHOD(AdviseGSGraphicServices)(void *);

//       IPlotServices

     STDMETHOD(GetPlotTypesInformation)(SAFEARRAY **pp2DTypeIDs,SAFEARRAY **pp3DTypeIds,
                                          SAFEARRAY **pp2DTypeNames,SAFEARRAY **pp3DTypeNames,
                                          SAFEARRAY **pp2DTypeInstanceNumbers,SAFEARRAY **pp3DTypeInstanceNumbers,
                                          SAFEARRAY **pp2DTypeInstances,SAFEARRAY **pp3DTypeInstances);

//       IGraphicSegmentAction

     STDMETHOD(Selector)();
     STDMETHOD(Unselector)();
     STDMETHOD(MenuRequest)();
     STDMETHOD(LeftMouse)();
     STDMETHOD(RightMouse)();
     STDMETHOD(MouseMove)(POINT* ptMouse);
     STDMETHOD(MouseRelease)();

//      IGPropertiesClient

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

//      IViewObject

      STDMETHOD(Draw)(unsigned long,long,void *,DVTARGETDEVICE *,HDC,HDC,const struct _RECTL *,const struct _RECTL *,int (__stdcall *)(unsigned long),unsigned long);
      STDMETHOD(GetColorSet)(DWORD,long,void *,DVTARGETDEVICE *,HDC,LOGPALETTE **);
      STDMETHOD(Freeze)(DWORD,long,void *,DWORD *);
      STDMETHOD(Unfreeze)(DWORD);
      STDMETHOD(SetAdvise)(DWORD,DWORD,IAdviseSink *);
      STDMETHOD(GetAdvise)(DWORD *,DWORD *,IAdviseSink **);

      //      IViewObject2

      STDMETHOD(GetExtent)(unsigned long,long,DVTARGETDEVICE *,struct tagSIZE *);

//      IViewObjectEx

      STDMETHOD(GetRect)(DWORD dwAspect,RECTL *);
      STDMETHOD(GetViewStatus)(DWORD *);
      STDMETHOD(QueryHitPoint)(DWORD dwAspect,const struct tagRECT *pRectBounds,POINT ptlHit,long lCloseHint,DWORD *dwHitResult);
      STDMETHOD(QueryHitRect)(DWORD dwAspect,const struct tagRECT *pRectBounds,const struct tagRECT *rctHit,long lCloseHint,DWORD *dwHitResult);
      STDMETHOD(GetNaturalExtent)(DWORD dwExtent,LONG lIndex,DVTARGETDEVICE *ptd,HDC hicTargetDev,DVEXTENTINFO *extentInfo,SIZEL *);

     int pID;

     HANDLE plotThread;
     HWND hwndParentWindow,hwndObjectWindow;
     HWND hwndDimensionSettings,hwndTypeSettings,hwndColorSettings;
     HMENU hMainMenu,hMenu;

     static HWND hwndSampleGraphic;
     static HWND hwndSampleGraphicSurface;

     SIZEL sizeMainGraphic{0L,0L};

     BSTR bstrName;

     IDataSet *baselineDataSet;

     static HANDLE globalPlotThread;
     DataPoint lastPoint;
     DataList *naturalEndOfData;
     Z_INDEX *zIndex;
     RECT rectPropertiesPosition;

     int okToPlot,autoViewDetection,haveAnyData,selected;
     bool overrideOwnerView,overrideOwnerType;

     IPlot** pIPlots;
     long currentPlotCount;

     MessageTable messageTable;

     IUnknown *pIUnknownOuter;
     IAdviseSink *pIViewObjectEx_IAdviseSink;

     IGProperties *pIProperties;
     IUnknown *pIUnknownProperties;

     IGProperty *pOwnerPropertyPlotView;
     IGProperty *pOwnerProperty2DPlotType;
     IGProperty *pOwnerProperty3DPlotType;
     IGProperty *pOwnerPropertyBackgroundColor;

     IPlotNotify *pIPlotNotify;
     IGSystemStatusBar *pIGSystemStatusBar;
     IGSGraphicServices *pIGSGraphicServices;

     DWORD adviseSink_dwAspect;
     DWORD adviseSink_advf;

     IGProperty *propertyName;

     IGProperty *propertyOverrideOwnerView;
     IGProperty *propertyOverrideOwnerType;

     IGProperty *propertyPlotView;
     IGProperty *property2DPlotType;
     IGProperty *property3DPlotType;

     IGProperty *propertyTopSurfaceColor;
     IGProperty *propertyBottomSurfaceColor;
     IGProperty *propertyLineColor;
     IGProperty *propertyLineWeight;
     IGProperty *propertyLineEnd;
     IGProperty *propertyLineJoin;
     IGProperty *propertyCustomColors;
     IGProperty *propertyPropertiesPosition;

     IGProperty* propertyDataExtents;

     IGProperty *propertyPlotNumber;

     IGProperty* propertyDataSet{NULL};

     IGProperty *propertyPlotTypesGlobal{NULL};

     IGProperty *propertyPlotTypesInstance{NULL};

     IGProperty *propertyPlotTypes2DInstanceIdentifiers{NULL};
     IGProperty *propertyPlotTypes3DInstanceIdentifiers{NULL};

     IGProperty *propertyPlotTypesStorage{NULL};

     HANDLE PlotThread();

     std::map<long,GUID> plotType2DInstanceGUIDS;
     std::map<long,GUID> plotType3DInstanceGUIDS;

     static long instanceCount;

     static long findKnownPlotTypeProviders();
     static std::list<CLSID *> plotTypeProviderGUIDs;

     static std::map<gc2DPlotTypes,IGSystemPlotType *> plotType2DProviderInstances;
     static std::map<gc2DPlotTypes,char *> plotType2DProviderNames;
     static std::map<gc2DPlotTypes,long> plotType2DInstanceNumber;

     static std::map<gc3DPlotTypes,IGSystemPlotType *> plotType3DProviderInstances;
     static std::map<gc3DPlotTypes,char *> plotType3DProviderNames;
     static std::map<gc3DPlotTypes,long> plotType3DInstanceNumber;

     static LRESULT CALLBACK handler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK dimensionHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK typeHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK colorHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK patchPainterProc(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);

     static LRESULT CALLBACK sampleGraphicHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK sampleGraphicSurfaceHandler(HWND,UINT,WPARAM,LPARAM);

     static DWORD WINAPI sampleGraphicThread(void *pvArg);

     static LRESULT CALLBACK scrollPaneHandler(HWND,UINT,WPARAM,LPARAM);

     static WNDPROC defaultPatchPainter;

     static __declspec(dllexport) unsigned int __stdcall plotter(void*);
     friend unsigned int __stdcall redrawThread(void *);

     unsigned int refCount;

     friend class PlotFactory;
  };

  extern HMODULE hModule;
  extern char szModuleName[1024];
  extern BSTR wstrModuleName;
  extern ITypeInfo *pITypeInfo;

  extern bool isSampleInUse;
