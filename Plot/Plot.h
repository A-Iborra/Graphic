
#pragma once

#include "basePlot.h"

  class Plot : public BasePlot, 
               public IPlot, 
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

     STDMETHOD(Initialize)(IDataSet *,IOpenGLImplementation *,IEvaluator *,IGProperty*,IGProperty *,IGProperty *,IGProperty *,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IGProperty **,IGProperty **,IGProperty **,IGProperty **,IGProperty **);

     STDMETHOD(PrepareForData)();
     STDMETHOD(TakeDataPoint)(DataPoint*);
     STDMETHOD(TakeDataString)(BSTR);
     STDMETHOD(ResetData)();
     STDMETHOD(LastPoint)(DataPoint*);

     STDMETHOD(StubPlot)();
     STDMETHOD(AddText)(void *text,unsigned char *name,int idNumber);
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

     STDMETHOD(get_SegmentID)(long*);

     STDMETHOD(put_ActionTable)(IGraphicSegmentAction *);
     STDMETHOD(get_ActionTable)(IGraphicSegmentAction **);

     STDMETHOD(put_DataSet)(IDataSet *newDataSet);
     STDMETHOD(get_DataSet)(IDataSet **getDataSet);

     STDMETHOD(put_PlotNotify)(IPlotNotify *);
     STDMETHOD(get_PlotNotify)(IPlotNotify **);

     STDMETHOD(put_Color)(SAFEARRAY *);
     STDMETHOD(get_Color)(SAFEARRAY **);

     STDMETHOD(put_ColorProperty)(IGProperty*);
     STDMETHOD(get_ColorProperty)(IGProperty**);

     STDMETHOD(put_LineWeight)(IGProperty*);
     STDMETHOD(get_LineWeight)(IGProperty**);

     STDMETHOD(put_ParentWindow)(HWND);

     STDMETHOD(get_SegmentCount)(long *);
     STDMETHOD(GetSegments)(long *);

     STDMETHOD(EditProperties)();

     STDMETHOD(AdviseGSystemStatusBar)(IGSystemStatusBar*);

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

     __declspec(dllexport) static HANDLE PlotData(List<Plot> *pList);

     static int pCount;
     int pID;

     HANDLE plotThread;
     HWND hwndParentWindow,hwndObjectWindow,hwndSample;
     HWND hwndDimensionSettings,hwndTypeSettings,hwndColorSettings;
     HMENU hMainMenu,hMenu;
     BSTR bstrName;

     static WNDPROC defaultPatchPainter;
     static WNDPROC defaultStaticWindowHandler;

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
     double defaultSolidSizeInPercentDomain;

     MessageTable messageTable;

     IUnknown *pIUnknownOuter;
     IAdviseSink *pIViewObjectEx_IAdviseSink;

     IGProperties *pIProperties;
     IUnknown *pIUnknownProperties;

     IGProperty *pOwnerPropertyPlotView;
     IGProperty *pOwnerPropertyPlotType;
     IGProperty *pOwnerPropertyTheta;
     IGProperty *pOwnerPropertyPhi;
     IGProperty *pOwnerPropertySpin;
     IGProperty *pOwnerPropertyBackgroundColor;
     IGProperty *pOwnerPropertyCountLights;
     IGProperty **pOwnerPropertiesLightOn;
     IGProperty **pOwnerPropertiesAmbientLight;
     IGProperty **pOwnerPropertiesDiffuseLight;
     IGProperty **pOwnerPropertiesSpecularLight;
     IGProperty **pOwnerPropertiesLightPosition;

     IPlotNotify *pIPlotNotify;
     IGSystemStatusBar* pIGSystemStatusBar;

     DWORD adviseSink_dwAspect;
     DWORD adviseSink_advf;

     IGProperty *propertyName;

     IGProperty *propertyOverrideOwnerView;
     IGProperty *propertyOverrideOwnerType;

     IGProperty *propertyPlotView;
     IGProperty *propertyPlotType;

     IGProperty *propertyTopSurfaceColor;
     IGProperty *propertyBottomSurfaceColor;
     IGProperty *propertyLineColor;
     IGProperty *propertyLineWeight;
     IGProperty *propertyLineEnd;
     IGProperty *propertyLineJoin;
     IGProperty *propertyCustomColors;
     IGProperty *propertyPropertiesPosition;
     IGProperty *propertyDefaultSolidSizeInPercentDomain;

     IGProperty* propertyDataExtents;

     IGProperty *propertyPlotNumber;

     HANDLE PlotThread();

     int init();
     int stacks();
     int stacks2D();
     int stacks3D();
     int blocks();
     int blocks2D();
     int blocks3D();
     int balls();
     int balls2D();
     int balls3D();

     static LRESULT CALLBACK handler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK dimensionHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK typeHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK colorHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK patchPainterProc(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK sampleGraphicHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);

     static __declspec(dllexport) unsigned int __stdcall plotter(void*);
     friend unsigned int __stdcall redrawThread(void *);

     unsigned int refCount;

     friend class PlotFactory;
  };

  extern HMODULE hModule;
  extern char szModuleName[1024];
  extern BSTR wstrModuleName;
  extern ITypeInfo *pITypeInfo;

#define DECLARE_PLANE \
   DataPoint homePoint,firstPoint,secondPoint,thirdPoint,fourthPoint,v[4]; \
   double xProd0[3],xProd1[3],xProd2[3],avgNormal[3];                      \
   int vk; 

#define DRAW_PLANE                 \
   v[0] = firstPoint;               \
   v[1] = secondPoint;              \
   v[2] = thirdPoint;               \
   v[3] = fourthPoint;              \
                                    \
   xProd0[0] = v[1].x - v[0].x;     \
   xProd0[1] = v[1].y - v[0].y;     \
   xProd0[2] = v[1].z - v[0].z;     \
   xProd1[0] = v[3].x - v[0].x;     \
   xProd1[1] = v[3].y - v[0].y;     \
   xProd1[2] = v[3].z - v[0].z;     \
   VxV(xProd0,xProd1,xProd2);       \
   unitVector(xProd2,xProd0);       \
   avgNormal[0] = xProd0[0];        \
   avgNormal[1] = xProd0[1];        \
   avgNormal[2] = xProd0[2];        \
                                    \
   xProd0[0] = v[2].x - v[1].x;     \
   xProd0[1] = v[2].y - v[1].y;     \
   xProd0[2] = v[2].z - v[1].z;     \
   xProd1[0] = v[0].x - v[1].x;     \
   xProd1[1] = v[0].y - v[1].y;     \
   xProd1[2] = v[0].z - v[1].z;     \
   VxV(xProd0,xProd1,xProd2);       \
   unitVector(xProd2,xProd0);       \
   avgNormal[0] += xProd0[0];       \
   avgNormal[1] += xProd0[1];       \
   avgNormal[2] += xProd0[2];       \
                                    \
   xProd0[0] = v[3].x - v[2].x;     \
   xProd0[1] = v[3].y - v[2].y;     \
   xProd0[2] = v[3].z - v[2].z;     \
   xProd1[0] = v[1].x - v[2].x;     \
   xProd1[1] = v[1].y - v[2].y;     \
   xProd1[2] = v[1].z - v[2].z;     \
   VxV(xProd0,xProd1,xProd2);       \
   unitVector(xProd2,xProd0);       \
   avgNormal[0] += xProd0[0];       \
   avgNormal[1] += xProd0[1];       \
   avgNormal[2] += xProd0[2];       \
                                    \
   xProd0[0] = v[1].x - v[3].x;     \
   xProd0[1] = v[1].y - v[3].y;     \
   xProd0[2] = v[1].z - v[3].z;     \
   xProd1[0] = v[2].x - v[3].x;     \
   xProd1[1] = v[2].y - v[3].y;     \
   xProd1[2] = v[2].z - v[3].z;     \
   VxV(xProd0,xProd1,xProd2);       \
   unitVector(xProd2,xProd0);       \
   avgNormal[0] += xProd0[0];       \
   avgNormal[1] += xProd0[1];       \
   avgNormal[2] += xProd0[2];       \
                                    \
   avgNormal[0] /= 4.0;             \
   avgNormal[1] /= 4.0;             \
   avgNormal[2] /= 4.0;             \
                                    \
   pIOpenGLImplementation -> Normal3dv(avgNormal);                          \
                                                                            \
   for ( vk = 0; vk < 4; vk++ ) pIOpenGLImplementation -> Vertex(&v[vk]);   \



