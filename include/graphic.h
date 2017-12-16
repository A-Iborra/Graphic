
#pragma once


#include <windows.h>
#include <commctrl.h>

#include "Graphic_resource.h"

#include "GSystem_i.h"
#include "Properties_i.h"
#include "DataSet_i.h"
#include "Variable_i.h"
#include "Evaluator_i.h"
#include "OpenGLImplementation_i.h"
#include "Plot_i.h"
#include "Axis_i.h"
#include "ViewSet_i.h"
#include "Text_i.h"
#include "Function_i.h"

#include "GraphicControl_i.h"

#include "ContainedFunction.h"

#include "List.h"

  struct graphicCursor;

  class G : 
        public IOleObject,
        public IOleInPlaceObject,
        public IOleInPlaceActiveObject,
        public IOleControl,
        public IDataObject,
        public IViewObjectEx,
        public IProvideClassInfo2,
        public IRunnableObject,
        public IPlotNotify,
        public ITextNotify,
        public IAdviseSink,
        public ISpecifyPropertyPages,
        public IGSGraphic, 
        public IGSystemStatusBar  {

//      IDispatch

     public:

     STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();

     private:

     STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
     STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
     STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
     STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

//  IGGraphic

     STDMETHOD(put_Size)(long,long);
     STDMETHOD(put_Position)(long,long);

     STDMETHOD(put_Floor)(double);
     STDMETHOD(get_Floor)(double *);

     STDMETHOD(put_Ceiling)(double);
     STDMETHOD(get_Ceiling)(double *);

     STDMETHOD(put_PlotView)(PlotViews plotView);
     STDMETHOD(get_PlotView)(PlotViews* plotView);
     STDMETHOD(put_PlotType)(PlotTypes plotType);
     STDMETHOD(get_PlotType)(PlotTypes* plotType);

     STDMETHOD(put_ViewTheta)(float);
     STDMETHOD(get_ViewTheta)(float*);
     STDMETHOD(put_ViewPhi)(float);
     STDMETHOD(get_ViewPhi)(float*);
     STDMETHOD(put_ViewSpin)(float);
     STDMETHOD(get_ViewSpin)(float*);

     STDMETHOD(put_LightOn)(long,VARIANT_BOOL);
     STDMETHOD(get_LightOn)(long,VARIANT_BOOL*);
     STDMETHOD(put_LightPosition)(long,VARIANT *);
     STDMETHOD(get_LightPosition)(long,VARIANT *);

     STDMETHOD(put_BackgroundColor)(long);
     STDMETHOD(get_BackgroundColor)(long*);

     STDMETHOD(put_LeftMargin)(long);
     STDMETHOD(get_LeftMargin)(long*);
     STDMETHOD(put_TopMargin)(long);
     STDMETHOD(get_TopMargin)(long*);
     STDMETHOD(put_RightMargin)(long);
     STDMETHOD(get_RightMargin)(long*);
     STDMETHOD(put_BottomMargin)(long);
     STDMETHOD(get_BottomMargin)(long*);
     STDMETHOD(put_Margins)(long,long,long,long);

     STDMETHOD(get_Axis)(BSTR,IDispatch**);
     STDMETHOD(get_Plot)(long,IDispatch**);
     STDMETHOD(get_Function)(BSTR,IDispatch**);
     STDMETHOD(get_Text)(BSTR,IDispatch**);

     STDMETHOD(put_ShowFunctions)(VARIANT_BOOL);
     STDMETHOD(get_ShowFunctions)(VARIANT_BOOL *);

     STDMETHOD(ClearAllData)();

     STDMETHOD(PrepareForData)(long sourceID);

     STDMETHOD(TakeDataString)(BSTR rawData,long plotID);
     STDMETHOD(TakeDataArray)(SAFEARRAY **,long plotID);
     STDMETHOD(TakeFile)(BSTR fileName,long plotID);
     STDMETHOD(TakeDataSet)(long ds,long sourceID);

/* Properties: Other categories */

     STDMETHOD(put_AutoPlotViewDetection)(VARIANT_BOOL);
     STDMETHOD(get_AutoPlotViewDetection)(VARIANT_BOOL*);

     STDMETHOD(put_PickBoxSize)(long);
     STDMETHOD(get_PickBoxSize)(long*);

     STDMETHOD(put_DenyUserPropertySettings)(VARIANT_BOOL);
     STDMETHOD(get_DenyUserPropertySettings)(VARIANT_BOOL*);

     STDMETHOD(put_UseStatusBar)(VARIANT_BOOL);
     STDMETHOD(get_UseStatusBar)(VARIANT_BOOL*);

     STDMETHOD(put_UseGraphicsCursor)(VARIANT_BOOL);
     STDMETHOD(get_UseGraphicsCursor)(VARIANT_BOOL*);

     STDMETHOD(FinishedWithData)(long plotID);
     
     STDMETHOD(PrepareForDataSets)();

     STDMETHOD(Draw)(long sourceID);

     STDMETHOD(SetProperties)();

     STDMETHOD(AddAxis)(char type,IDispatch *);

     STDMETHOD(AddText)(BSTR,IDispatch **);
     STDMETHOD(AddTextInteractive)(IDispatch **pText = NULL);
     STDMETHOD(RemoveText)(IDispatch *);
     STDMETHOD(RemoveAllText)();

     STDMETHOD(AddFunction)(BSTR,IDispatch **pFunction = NULL);
     STDMETHOD(AddFunctionInteractive)(IDispatch **pFunction = NULL);
     STDMETHOD(RemoveFunction)(IDispatch*);
     STDMETHOD(RemoveAllFunctions)();

     STDMETHOD(AddPlot)(long id,IDispatch **);
     STDMETHOD(RemovePlot)(long id);
     STDMETHOD(RemoveAllPlots)();

     STDMETHOD(Synchronise)();

//  IGSystemStatusBar

     STDMETHOD(put_StatusText)(long paneIndex,char* theText);
     STDMETHOD(get_StatusText)(long paneIndex,char** theText);

    private:

//     IOleObject 

     STDMETHOD(SetClientSite)(IOleClientSite *pClientSite);
     STDMETHOD(GetClientSite)(IOleClientSite **ppCLientSite);
     STDMETHOD(SetHostNames)(LPCOLESTR szContainerApp,LPCOLESTR szContainerObj);
     STDMETHOD(Close)(DWORD dwSaveOption);
     STDMETHOD(SetMoniker)(DWORD dwWhichMoniker, IMoniker *pmk);
     STDMETHOD(GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker,IMoniker **ppmk);
     STDMETHOD(InitFromData)(IDataObject *pDataObject,BOOL fCreation,DWORD dwReserved);
     STDMETHOD(GetClipboardData)(DWORD dwReserved,IDataObject **ppDataObject);
     STDMETHOD(DoVerb)(LONG iVerb,LPMSG lpmsg,IOleClientSite *pActiveSite,LONG lindex,HWND hwndParent,LPCRECT lprcPosRect);
     STDMETHOD(EnumVerbs)(IEnumOLEVERB **ppenumOleVerb);
     STDMETHOD(Update)();
     STDMETHOD(IsUpToDate)();
     STDMETHOD(GetUserClassID)(CLSID * pClsid);
     STDMETHOD(GetUserType)(DWORD dwFormOfType, LPOLESTR *pszUserType);
     STDMETHOD(SetExtent)(DWORD dwDrawAspect, LPSIZEL lpsizel);
     STDMETHOD(GetExtent)(DWORD dwDrawAspect, LPSIZEL lpsizel);
     
     STDMETHOD(Advise)(IAdviseSink *pAdvSink, DWORD * pdwConnection);
     STDMETHOD(Unadvise)(DWORD dwConnection);
     STDMETHOD(EnumAdvise)(IEnumSTATDATA **ppenumAdvise);
     STDMETHOD(GetMiscStatus)(DWORD dwAspect, DWORD * pdwStatus);        
     
     STDMETHOD(SetColorScheme)(LPLOGPALETTE lpLogpal);

//      IOleWindow

     STDMETHOD(GetWindow)(HWND *);
     STDMETHOD(ContextSensitiveHelp)(BOOL);

//      IOleInPlaceObject

     STDMETHOD(InPlaceActivate)();
     STDMETHOD(InPlaceDeactivate)();
     STDMETHOD(UIDeactivate)();
     STDMETHOD(SetObjectRects)(LPCRECT,LPCRECT);
     STDMETHOD(ReactivateAndUndo)();

//      IOleInPlaceActiveObject

     STDMETHOD(TranslateAccelerator)(LPMSG);
     STDMETHOD(OnFrameWindowActivate)(BOOL);
     STDMETHOD(OnDocWindowActivate)(BOOL);
     STDMETHOD(ResizeBorder)(LPCRECT ,IOleInPlaceUIWindow *,BOOL);
     STDMETHOD(EnableModeless)(BOOL);

//      IOleControl

     STDMETHOD(GetControlInfo)(CONTROLINFO *);
     STDMETHOD(OnMnemonic)(MSG *);
     STDMETHOD(OnAmbientPropertyChange)(long);
     STDMETHOD(FreezeEvents)(int);

//      IDataObject

     STDMETHOD(GetData)(FORMATETC *,STGMEDIUM *);
     STDMETHOD(GetDataHere)(FORMATETC *,STGMEDIUM *);
     STDMETHOD(QueryGetData)(FORMATETC *);
     STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *,FORMATETC *);
     STDMETHOD(SetData)(FORMATETC *,STGMEDIUM *,BOOL);
     STDMETHOD(EnumFormatEtc)(DWORD,IEnumFORMATETC **);
     STDMETHOD(DAdvise)(FORMATETC *,DWORD,IAdviseSink *,DWORD *);
     STDMETHOD(DUnadvise)(DWORD);
     STDMETHOD(EnumDAdvise)(IEnumSTATDATA **);

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

//      IProvideClassInfo

     STDMETHOD(GetClassInfo)(ITypeInfo **);

//      IProvideClassInfo2

     STDMETHOD(GetGUID)(DWORD,GUID *);

//      IRunnableObject

     STDMETHOD(GetRunningClass)(CLSID *);
     STDMETHOD(Run)(LPBC);
     int __stdcall IsRunning(void);
     STDMETHOD(LockRunning)(BOOL,BOOL);
     STDMETHOD(SetContainedObject)(BOOL);

//      IPropertiesClient

     class _IGPropertiesClient : public IGPropertiesClient {

        public:
        _IGPropertiesClient(G* pp) : pParent(pp) {};

        STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

        STDMETHOD(SavePrep)();
        STDMETHOD(InitNew)();
        STDMETHOD(Loaded)();
        STDMETHOD(Saved)();
        STDMETHOD(IsDirty)();
        STDMETHOD(GetClassID)(BYTE *pCLSID);

         G* pParent;

     } * pIPropertiesClient;

//      IPropertyPageClient

     class _IGPropertyPageClient : public IGPropertyPageClient {

        public:
        _IGPropertyPageClient(G* pp) : pParent(pp) {};

        STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

        STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
        STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
        STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
        STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

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

        G* pParent;

     } *pIPropertyPageClient;

//      IPlotNotify

     STDMETHOD(DeletePlot)(IPlot *);

//      ITextNotify

     STDMETHOD(DeleteText)(IText *);

//      IAdviseSink

     void __stdcall OnDataChange(FORMATETC *,STGMEDIUM *);
     void __stdcall OnViewChange(DWORD dwAspect,LONG lindex);
     void __stdcall OnRename(IMoniker *);
     void __stdcall OnSave();
     void __stdcall OnClose();

//      ISpecifyPropertyPages

     STDMETHOD(GetPages)(CAUUID *pPages);

     class _IPropertyPage : public IPropertyPage {
     public:

        _IPropertyPage(G *p,CLSID clsid) : pParent(p),theCLSID(clsid) {};
        ~_IPropertyPage() {};

        // IUnknown

        STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

        // IPropertyPage

     private:

        STDMETHOD(SetPageSite)(IPropertyPageSite *pPageSite);
        STDMETHOD(Activate)(HWND hWndParent, LPCRECT prc, BOOL fModal);
        STDMETHOD(Deactivate)();
        STDMETHOD(GetPageInfo)(PROPPAGEINFO *pPageInfo);
        STDMETHOD(SetObjects)(ULONG cObjects, IUnknown **ppUnk);
        STDMETHOD(Show)(UINT nCmdShow);
        STDMETHOD(Move)(LPCRECT prc);
        STDMETHOD(IsPageDirty)();
        STDMETHOD(Apply)();
        STDMETHOD(Help)(LPCOLESTR pszHelpDir);
        STDMETHOD(TranslateAccelerator)(LPMSG pMsg);

        G *pParent;
        CLSID theCLSID;

     } *pIPropertyPage[8];

/* Not in the interface */

     public:

     G(IUnknown *pUnkOuter);
     ~G();

     IPropertyPage *PropertyPage(long index) { return static_cast<IPropertyPage *>(pIPropertyPage[index]); };

     private:

     IUnknown* pIUnknownThis;
     IOleClientSite *pIOleClientSite;
     IOleInPlaceSite *pIOleInPlaceSite;
     IOleAdviseHolder *pOleAdviseHolder;
     IDataAdviseHolder *pDataAdviseHolder;
     IAdviseSink *pAdviseSink;

     DWORD adviseSink_dwAspect,adviseSink_advf;

     int refCount;

     HWND hwndOwner;
     HWND hwndFrame;
     HWND hwndGraphic;
     HWND hwndStatusBar;
     HMENU hMenuPlot;
     HWND hwndAppearanceSettings,hwndStyleSettings,hwndTextSettings;
     HWND hwndLightingSettings,hwndBackgroundSettings,hwndPlotSettings,hwndFunctionSettings;
     HWND hwndAxisSettings;
     HWND hwndDataSourcesDialog,hwndDataSourcesTab,hwndDataSourcesFunctions;

     POINTL ptlZoomAnchor,ptlZoomFloat,ptlPickBox,ptlMouseBeforeMenu,ptlLastMouse;
     BOOL trackedMouse;
     POINTL rightMouseClickPosition;
     float zLevel;

     static WNDPROC defaultPatchPainter;
     static WNDPROC defaultStatusBarHandler;

     int eraseMode,rendering;
     double xPixelsPerUnit,yPixelsPerUnit;
     double floorZ,ceilingZ;

     unsigned int hitTableHits,*hitTable;
     long currentPlotSourceID;

     char windowTitle[32];
     char szName[64];
     ULONG windowStyle,windowFrameFlags,windowStorageBytes;

     List<IPlot> plotList;
     List<IAxis> axisList;
     List<IText> textList;
     List<IGSFunctioNater> functionList;
     List<IGSFunctioNater> cachedFunctionList;
     List<ContainedFunction> containedFunctionList;
     List<graphicCursor> graphicCursorList;

     List<HANDLE> renderThreadList;

     SIZEL containerSize,pickBoxSize;
     RECT rectStatusText;
     RECT margins;
     PlotViews plotView;
     PlotTypes plotType;
     PlotViews defaultPlotView;

     long default2DPlotType;
     long default3DPlotType;

     long plotCount;
     long textCount;
     long functionCount;
     long supportedLightCount;
     long statusTextWidth[2];

     BOOL autoClear,autoPlotViewDetection,showStatusBar,useGraphicsCursor;
     BOOL showMargins,stretchToMargins,showFunctions;
     BOOL allowUserFunctionControlVisibilityAccess;
     BOOL denyUserPropertySettings;
     BOOL isRunning;

     char plotMarginUnits[16];
     char ***lightPositions;

     IUnknown *pIUnknownOuter;

     IClassFactory *pPlot_IClassFactory;
     IClassFactory *pFunction_IClassFactory;

     IViewSet *pIViewSet;
     IDataSet* pIDataSetMaster;
     IAxis *xaxis,*yaxis,*zaxis;
     IGraphicSegmentAction* pSelectedGraphicSegmentAction;
     IGSystemStatusBar* pIGSystemStatusBar;

     IOpenGLImplementation *pIOpenGLImplementation;

     IEvaluator *pIEvaluator;

     IGProperties *pIGProperties;

     IGProperty* propertyAutoClear;

     IGProperty* propertyPlotView;
     IGProperty* propertyDefault2DPlotType;
     IGProperty *propertyDefault3DPlotType;

     IGProperty* propertyPlotType;

     IGProperty* propertyBackgroundColor;

     IGProperty* propertyTextColor,*propertyTextBackgroundColor;

     IGProperty* propertyViewPhi,*propertyViewTheta,*propertyViewSpin;

     IGProperty* propertyDenyUserPropertySettings;
     IGProperty* propertyAutoPlotViewDetection;
     IGProperty* propertyShowFunctions;
     IGProperty* propertyShowStatusBar;
     IGProperty* propertyUseGraphicsCursor;
     IGProperty* propertyShowMargins;
     IGProperty* propertyAllowUserFunctionControlVisibilityAccess;

     IGProperty* propertyPlotMarginUnits;
     IGProperty* propertyPlotLeftMargin,*propertyPlotRightMargin;
     IGProperty* propertyPlotTopMargin,*propertyPlotBottomMargin;
     IGProperty* propertyPlotMargins;
     IGProperty* propertyPlotMarginsStretchAll;

     IGProperty** ppPropertyLightOn;

     IGProperty** ppPropertyAmbientLight;
     IGProperty** ppPropertyDiffuseLight;
     IGProperty** ppPropertySpecularLight;

     IGProperty** ppPropertyLightPos;

     IGProperty* propertyCountLights;
     IGProperty* propertyCustomColors;
     IGProperty* propertySpecularReference;
     IGProperty* propertyAmbientReference;
     IGProperty* propertyShinyness;

     IGProperty* propertyPrintProperties;
 
     IGProperty* propertyDataExtents;

     IGProperty* propertyFunctions;
     IGProperty* propertyPlots;
     IGProperty* propertyTexts;
     IGProperty* propertyAxiis;

     IGProperty* propertyStatusText;

     IGProperty* propertyFloor;
     IGProperty* propertyCeiling;

     IGProperty* propertyRenderOpenGLAxisText;

     int erase();
     int eraseBackground();
     int redraw();
     int stop();
     int wait();

     HMENU hwndMenuPlot() { return hMenuPlot; };

     int oleSetWindowRect(RECT* rcPos);
     int initWindows();

     IPlot* newPlot(long plotID);

     IGSFunctioNater* newFunction(bool deferVisibility);
     void deleteFunction(IGSFunctioNater*);
     int connectFunction(IGSFunctioNater* pIFunction,bool deferVisibility,bool doVisibilityOnly);
     int unConnectFunction(IGSFunctioNater* pIFunction);

     IText* newText();

     int render(long sourceID);
     static unsigned __stdcall threadRender(void *);

     int pick(POINTL *ptl,unsigned int (__stdcall *actionFunction)(void *),int forceToThread);
     int doPickBox(POINTL *ptl);
     void savePickBox(HDC hdc,POINTL *pt);
     void erasePickBox(HDC hdcTarget);
     void drawPickBox(HDC hdc,POINTL *pt);
     int statusPosition();
     int drawGraphicCursor(POINTL *pPtlPickBox,int doPickBox);
     int eraseGraphicCursor();
     int stubDraw();

     void saveBox(POINTL *pAnchor,POINTL *pFloat,HDC hdcTarget,HBITMAP *pBitmaps);
     void restoreBox(POINTL *pAnchor,POINTL *pFloat,HDC hdcTarget,HBITMAP *pBitmaps);

     HBITMAP boxBitmaps[5] { NULL, NULL, NULL, NULL , NULL};
     HBITMAP pickBoxBitmap {NULL};

     int getSegments(long **pSegments);

     static void __stdcall someObjectChanged(void *);
     static void __stdcall styleHandlerSomeObjectChanged(void *);

     static LRESULT CALLBACK graphicFrameHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK graphicHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK posSizeHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK styleHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK textHandler(HWND hwnd,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK lightingHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK backgroundHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK axisHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK plotHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK functionHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK dataSourcesHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK sampleGraphicHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK patchPainterProc(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK statusBarHandler(HWND,UINT,WPARAM,LPARAM);

     void setColorBackground();
     void changeType();
     void clearData();

     static unsigned int __stdcall processSelections(void *someObject);
     static unsigned int __stdcall processMenus(void *someObject);

     friend class ContainedFunction;
     friend class _IGPropertiesClient;
     friend class _IGPropertyClient;
     friend class _IGPropertyPageClient;

  };


   struct graphicCursor {
      graphicCursor() { memset(this,0,sizeof(struct graphicCursor)); };
      ~graphicCursor() { 
      if ( ! ( NULL == crossBitmaps[0] ) )
         DeleteObject(crossBitmaps[0]);
      if ( ! ( NULL == crossBitmaps[1] ) )
         DeleteObject(crossBitmaps[1]);
      if ( ! ( NULL == crossBitmaps[2] ) )
         DeleteObject(crossBitmaps[2]);
      if ( ! ( NULL == crossBitmaps[3] ) )
         DeleteObject(crossBitmaps[3]);
      if ( ! ( NULL == crossBitmaps[4] ) )
         DeleteObject(crossBitmaps[4]);
      if ( ! ( NULL == crossBitmaps[5] ) )
         DeleteObject(crossBitmaps[5]);
      };
      POINTL ptlGraphicCursor[12];
      HBITMAP crossBitmaps[6];
      POINTL ptlCrossBitmaps[6];
      SIZEL sizelCrossBitmaps[6];
   };

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

MIDL_DEFINE_GUID(CLSID,CLSID_GSystemGraphicPropertiesPosSize,0x8CAEFE80,0x55E6,0x11d3,0x83,0x65,0x00,0x60,0x08,0xBD,0x5B,0xC3);
MIDL_DEFINE_GUID(CLSID,CLSID_GSystemGraphicPropertiesStyle,0x8CAEED81,0x55E6,0x11d3,0x83,0x65,0x00,0x60,0x08,0xBD,0x5B,0xC3);
MIDL_DEFINE_GUID(CLSID,CLSID_GSystemGraphicPropertiesBackground,0x8CAEFE82,0x55E6,0x11d3,0x83,0x65,0x00,0x60,0x08,0xBD,0x5B,0xC3);
MIDL_DEFINE_GUID(CLSID,CLSID_GSystemGraphicPropertiesText,0x8CAEFE83,0x55E6,0x11d3,0x83,0x65,0x00,0x60,0x08,0xBD,0x5B,0xC3);
MIDL_DEFINE_GUID(CLSID,CLSID_GSystemGraphicPropertiesLighting,0x8CAEFE84,0x55E6,0x11d3,0x83,0x65,0x00,0x60,0x08,0xBD,0x5B,0xC3);
MIDL_DEFINE_GUID(CLSID,CLSID_GSystemGraphicPropertiesAxis,0x8CAEFE85,0x55E6,0x11d3,0x83,0x65,0x00,0x60,0x08,0xBD,0x5B,0xC3);
MIDL_DEFINE_GUID(CLSID,CLSID_GSystemGraphicPropertiesPlot,0x8CAEFE86,0x55E6,0x11d3,0x83,0x65,0x00,0x60,0x08,0xBD,0x5B,0xC3);
MIDL_DEFINE_GUID(CLSID,CLSID_GSystemGraphicPropertiesFunctions,0x8CAEFE87,0x55E6,0x11d3,0x83,0x65,0x00,0x60,0x08,0xBD,0x5B,0xC3);

  extern G *pStaticObject;

  extern HMODULE hModule;
  extern char szModuleName[];
  extern BSTR bstrModuleName;
  extern ITypeInfo* pITypeInfo_CLSID_G;
  extern ITypeInfo* pITypeInfo_IG;
  extern ITypeInfo* pITypeInfo_IGSFunctioNaterEvents;

  extern long oleMisc;

#define PLOT_MARGIN_LEFT       5
#define PLOT_MARGIN_RIGHT      5
#define PLOT_MARGIN_BOTTOM     5
#define PLOT_MARGIN_TOP        5
#define PLOT_MARGIN_UNITS      "percent"

#define WINDOW_MARGIN_LEFT     50
#define WINDOW_MARGIN_RIGHT    25
#define WINDOW_MARGIN_TOP      25
#define WINDOW_MARGIN_BOTTOM   25
#define WINDOW_MARGIN_UNITS    UNIT_PIXEL

#define PLOT_DEFAULT_WIDTH     350
#define PLOT_DEFAULT_HEIGHT    250
#define PLOT_DEFAULT_UNITS     UNIT_PIXEL

#define PLOT_CLIENT_XLEFT      100
#define PLOT_CLIENT_YBOTTOM    100

#define PICK_WINDOW_SIZE         5
#define HIT_TABLE_SIZE         128

#define STANDARD_STATUS_TEXT_WIDTH  1

#define SUPPORTED_LIGHT_COUNT  8

#ifdef _DEBUG
#define DEBUG_TONE   Beep(1000,100);
#else
#define DEBUG_TONE
#endif
