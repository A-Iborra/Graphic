
#pragma once
#pragma warning( disable: 4786 )

#include <ocidl.h>
#include <gl\gl.h>
#include <list>

#include "GSystem_i.h"
#include "Properties_i.h"
#include "DataSet_i.h"

#include "Variable_i.h"
#include "Evaluator_i.h"

#include "OpenGLImplementation_i.h"

#include "Plot_i.h"

#include "graphics_commands.h"

   class OpenGLImplementor;
 
   struct transformationMatrixes {
      transformationMatrixes() { memset(this,0,sizeof(transformationMatrixes)); }
      double projectionMatrix[16];
      double modelMatrix[16];
      GLint viewPort[4];
      long viewPortMargins[4];
      long windowCX,windowCY;
      double xScaleFactor,yScaleFactor,zScaleFactor;
      double extentsXMin,extentsYMin,extentsZMin,extentsXMax,extentsYMax,extentsZMax;
   };


   class PlotWindow {
   public:
 
      PlotWindow(HWND,OpenGLImplementor *,IEvaluator *);
      ~PlotWindow();
 
      STDMETHOD(setViewProperties)(
                         IGProperty *pPropPlotView = NULL,
                         IGProperty *pPropRotationTheta = NULL,
                         IGProperty *pPropRotationPhi = NULL,
                         IGProperty *pPropRotationSpin = NULL,
                         IGProperty *pPropLeftMargin = NULL,
                         IGProperty *pPropTopMargin = NULL,
                         IGProperty *pPropRightMargin = NULL,
                         IGProperty *pPropBottomMargin = NULL,
                         IGProperty *pPropMarginUnits = NULL,
                         IGProperty *pPropStretchTomargins = NULL);

      STDMETHOD(unsetViewProperties)();

      STDMETHOD(set2D)(IDataSet* dataSet,long margins,char directionRight,char directionUp);

      STDMETHOD(setUp)(IDataSet *dataSet,
                         IGProperty *pPropPlotView = NULL,
                         IGProperty *pPropRotationTheta = NULL,
                         IGProperty *pPropRotationPhi = NULL,
                         IGProperty *pPropRotationSpin = NULL,
                         IGProperty *pPropLeftMargin = NULL,
                         IGProperty *pPropTopMargin = NULL,
                         IGProperty *pPropRightMargin = NULL,
                         IGProperty *pPropBottomMargin = NULL,
                         IGProperty *pPropMarginUnits = NULL,
                         IGProperty *pPropStretchTomargins = NULL);
 
      STDMETHOD(translate)(DataPoint*);
      STDMETHOD(rotate)(char axis,double angle);
      STDMETHOD(scale)(DataPoint*);

      STDMETHOD(setLighting)(
                IGProperty *pPropLightEnabled[],
                IGProperty *pPropAmbientLight[],
                IGProperty *pPropDiffuseLight[],
                IGProperty *pPropSpecularLight[],
                IGProperty *pPropLightPos[],
                IGProperty *pPropCountLights,
                IGProperty *pPropShinyness);
 
      int saveState();
      int restoreState(transformationMatrixes * = NULL);
 
      STDMETHOD(setViewPort)(int*);

      STDMETHOD(getPickBoxHits)(POINTL *ptl,long pickWindowSize,unsigned int *hitTable,long hitTableSize,long *pCallLists,unsigned int *hitTableHits);

      HBITMAP getMergedBackground(HWND,HDC);

      HRESULT GetPixels(long x1,long y1,long x2,long y2,long cyWindow,BYTE *pResult);

   private:
 
      int createRenderingContext();
 
      void finalize();

      void setOpenGLMatrices(long plotView,double phi,double theta,double spin);

      STDMETHOD(erase)(IGProperty *pPropBackgroundColor = NULL);
      STDMETHOD(resetDepth)();

      OpenGLImplementor *pParent;
      HWND hwnd;
      HDC deviceContext;

      bool initialized;
      bool isRendered;
      bool lineMode,polygonMode;

      IEvaluator *pIEvaluator;

      IGProperty *pSaved_PlotView;
      IGProperty *pSaved_RotationTheta;
      IGProperty *pSaved_RotationPhi;
      IGProperty *pSaved_RotationSpin;
      IGProperty *pSaved_LeftMargin;
      IGProperty *pSaved_TopMargin;
      IGProperty *pSaved_RightMargin;
      IGProperty *pSaved_BottomMargin;
      IGProperty *pSaved_MarginUnits;
      IGProperty *pSaved_StretchToMargins;

      IGProperty *pSaved_BackgroundColor;

      static int pixelFormat;
      static HGLRC renderingContext;

   protected:

      double xPixelsPerUnit,yPixelsPerUnit;
      float lightPosition[SUPPORTED_LIGHT_COUNT][4];
      float fvSpecularLight[SUPPORTED_LIGHT_COUNT][4],fvAmbientLight[SUPPORTED_LIGHT_COUNT][4],fvDiffuseLight[SUPPORTED_LIGHT_COUNT][4];
      long shinyness;

      transformationMatrixes openGLState;

      friend class OpenGLImplementor;
 
   };
 
 
   class OpenGLImplementor : 
         public IOpenGLImplementation,
         public IOleObject,
         public IOleInPlaceObject,
         public IOleInPlaceActiveObject,
         public IOleControl,
         public IProvideClassInfo2,
         public IViewObjectEx {
 
   public:
 
      OpenGLImplementor(IUnknown *pUnkOuter);
      ~OpenGLImplementor();
 
      IGSystemStatusBar* P_IGSystemStatusBar() { return pIGSystemStatusBar; }

      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();
 
      STDMETHOD (GetTypeInfoCount)(UINT *pctinfo);
      STDMETHOD (GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
      STDMETHOD (GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
      STDMETHOD (Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);
 
// Actions ? 

      STDMETHOD(Initialize)(IEvaluator *);
      STDMETHOD(Start)();
      STDMETHOD(Stop)();

      STDMETHOD(SetBaseWindow)(HWND);
      STDMETHOD(SetTargetWindow)(HWND hwnd);
      HDC __stdcall TargetDC();
      HWND __stdcall TargetHWND();
      STDMETHOD(PrintSetup());
      STDMETHOD(PrintFinish());
 
// Graphics State ? 

      STDMETHOD(SetViewProperties)(
                       IGProperty *pPropPlotView = NULL,
                       IGProperty *pPropRotationTheta = NULL,
                       IGProperty *pPropRotationPhi = NULL,
                       IGProperty *pPropRotationSpin = NULL,
                       IGProperty *pPropLeftMargin = NULL,
                       IGProperty *pPropTopMargin = NULL,
                       IGProperty *pPropRightMargin = NULL,
                       IGProperty *pPropBottomMargin = NULL,
                       IGProperty *pPropMarginUnits = NULL,
                       IGProperty *pPropStretchTomargins = NULL);

      STDMETHOD(UnSetViewProperties)();

      STDMETHOD(Set2D)(IDataSet* dataSet,long margins,char directionRight,char directionUp);

      STDMETHOD(SetUp)(IDataSet *dataSet,
                       IGProperty *pPropPlotView = NULL,
                       IGProperty *pPropRotationTheta = NULL,
                       IGProperty *pPropRotationPhi = NULL,
                       IGProperty *pPropRotationSpin = NULL,
                       IGProperty *pPropLeftMargin = NULL,
                       IGProperty *pPropTopMargin = NULL,
                       IGProperty *pPropRightMargin = NULL,
                       IGProperty *pPropBottomMargin = NULL,
                       IGProperty *pPropMarginUnits = NULL,
                       IGProperty *pPropStretchTomargins = NULL);
 
      STDMETHOD(IsInitialized)(VARIANT_BOOL *);

      STDMETHOD(Translate)(DataPoint*);
      STDMETHOD(Rotate)(char axis,double angle);
      STDMETHOD(Scale)(DataPoint*);

      STDMETHOD(SetLighting)(
                IGProperty *pPropLightEnabled[],
                IGProperty *pPropAmbientLight[],
                IGProperty *pPropDiffuseLight[],
                IGProperty *pPropSpecularLight[],
                IGProperty *pPropLightPos[],
                IGProperty *pPropCountLights,
                IGProperty *pPropShinyness);
 
      STDMETHOD(Push)();
      STDMETHOD(Pop)();

      STDMETHOD(put_ViewPort)(int*);
      STDMETHOD(get_ViewPort)(int*);

      STDMETHOD(get_SupportedLightCount)(long *);

// Utilities ?
      STDMETHOD(PixelsToUnits)(char direction,double inPixels,double *answer);
      STDMETHOD(UnitsToPixels)(char direction,double inUnits,double *answer);
      STDMETHOD(DataToWindow)(DataPoint *dpSource,UnitOfMeasure uom,DataPoint *dpTarget);
      STDMETHOD(WindowToData)(DataPoint *dpSource,DataPoint *dpTarget);
      STDMETHOD(DataListToWindow)(DataList*,DataList*);
      STDMETHOD(PixelPoint)(DataPoint*,DataPoint*);
      STDMETHOD(PixelVector)(DataPoint*,DataPoint*,DataPoint*);
  
      STDMETHOD(AdviseGSystemStatusBar)(IGSystemStatusBar*);

      STDMETHOD(GetPixels)(long x1,long y1,long x2,long y2,BYTE *pResult);

// Window State ? 
      STDMETHOD(get_HDC)(HDC *getDC);
      STDMETHOD(get_HWND)(HWND *getHWND);
      STDMETHOD(get_MousePositionClient)(POINT* ptMouse);

      BOOL __stdcall IsRendered();

// Data State ? 
      STDMETHOD(GetExtents)(double *minx,double *miny,double *minz,double *maxx,double *maxy,double *maxz);
 
// Drawing ?
      STDMETHOD(Erase)(IGProperty *pPropBackgroundColor = NULL);
      STDMETHOD(ResetDepth)();
      STDMETHOD(NewLine)(DataPoint* p);
      STDMETHOD(NewPolygon)(DataPoint* p);
      STDMETHOD(Vertex)(DataPoint* p);
      STDMETHOD(BeginSurface)(long segmentID,IGProperty* pPropTopColor,IGProperty* pPropBottomColor);
      STDMETHOD(EndSurface)(long segmentID);
      STDMETHOD(BeginWireframe)(long segmentID,IGProperty* pPropLineColor,IGProperty* pPropLineWeight);
      STDMETHOD(EndWireframe)(long segmentID);
      STDMETHOD(BeginSolids)(long segmentID,IGProperty* pPropTopColor,IGProperty* pPropBottomColor);
      STDMETHOD(EndSolids)(long segmentID);

      STDMETHOD(ClipPlane)(DataPoint *,DataPoint *,DataPoint *,DataPoint *);
 
      STDMETHOD(get_BackgroundColor)(IGProperty** pIPropertyBackgroundColor);
      STDMETHOD(get_ViewTheta)(double* pViewTheta);
      STDMETHOD(get_ViewPhi)(double* pViewPhi);
      STDMETHOD(get_ViewSpin)(double *);

      STDMETHOD(SetColor)(IGProperty*);
      STDMETHOD(SetLineWeight)(IGProperty*);

      STDMETHOD(Normal3dv)(double *normal);
 
// Selection ? 
      STDMETHOD(GetPickBoxHits)(POINTL *,long,unsigned int *,long,long *,unsigned int *);
 
// Segments ? 
      STDMETHOD(OpenSegment)(long segmentID,IGProperty* pPropColor,IGProperty* pPropLineWeight);
      STDMETHOD(RedrawSegment)(long segmentID);
      STDMETHOD(CloseSegment)(long segmentID,unsigned short drawOnClose);
      STDMETHOD(Finalize)();
      STDMETHOD(NewSegments)(long numberToMake,long *firstInRange);
      STDMETHOD(StartSegment)(long segmentID);
      STDMETHOD(EndSegment)();
      STDMETHOD(PlaySegment)(long segmentID);
      STDMETHOD(DeleteSegment)(long segmentID);

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
 
//      IAdviseSink
 
      void __stdcall OnDataChange(FORMATETC *,STGMEDIUM *);
      void __stdcall OnViewChange(DWORD dwAspect,LONG lindex);
      void __stdcall OnRename(IMoniker *);
      void __stdcall OnSave();
      void __stdcall OnClose();
 
//      IProvideClassInfo
 
      STDMETHOD(GetClassInfo)(ITypeInfo **);
 
//      IProvideClassInfo2
 
      STDMETHOD(GetGUID)(DWORD,GUID *);
 
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
 
      PlotWindow* GetPlotWindow() { return plotWindow; };

   private:
 
      unsigned int refCount;
 
      IUnknown* pIUnknown_Outer;

      IOleClientSite *pIOleClientSite;
      IOleInPlaceSite *pIOleInPlaceSite;
      IAdviseSink *pIAdviseSink;
      IOleAdviseHolder *pIOleAdviseHolder;
      IGSystemStatusBar* pIGSystemStatusBar;
 
      DWORD adviseSink_dwAspect,adviseSink_advf;

      HWND hwndControl;
      HWND hwndBase;
      HWND hwndParent;
      HWND hwndProperties;

      HMENU hMenu;
      WNDPROC oldHandler;

      IEvaluator *pIEvaluator;

      IUnknown* pIUnknownProperties;
      IGProperties* pIProperties;
      IGProperty* pIPropertySize;
      IGProperty* pIPropertyAllowHardwareAcceleration;

      std::list<PlotWindow *> plotWindowList;

      std::list<transformationMatrixes *> matrixList;

      PlotWindow *plotWindow;

      HWND hwndObjectWindow;
      HANDLE implementationMsgThread;
      HANDLE hCreationSemaphore,hDestructionSemaphore;
      HANDLE hBlockingMutex;

      static unsigned int __stdcall runImplementationMsgQueue(void *);

      static LRESULT __stdcall openglImplementationHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

      int initWindows();

      static LRESULT __stdcall handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
      static LRESULT __stdcall propertiesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

      friend class PlotWindow;

   };
 
    extern ITypeInfo *pITypeInfo;
    extern HMODULE hModule;

#if 1
#define SYNCHRONOUS_CALL(MSG,ARGUMENT) {    \
   if ( ! hwndObjectWindow ) return E_UNEXPECTED; \
   if ( ! implementationMsgThread ) return E_UNEXPECTED; \
   HANDLE hSemaphore = CreateSemaphore(NULL,0,1,NULL);   \
   PostMessage(hwndObjectWindow,(MSG),(WPARAM)(ARGUMENT),(LPARAM)hSemaphore);   \
   WaitForSingleObject(hSemaphore,INFINITE);    \
   CloseHandle(hSemaphore);                     \
   }
#else
#define SYNCHRONOUS_CALL(MSG,ARGUMENT) {    \
   if ( ! hwndObjectWindow ) return E_UNEXPECTED; \
   if ( ! implementationMsgThread ) return E_UNEXPECTED; \
   WaitForSingleObject(hBlockingMutex,INFINITE);    \
   PostMessage(hwndObjectWindow,(MSG),(WPARAM)(ARGUMENT),(LPARAM)0L);  \
   WaitForSingleObject(hBlockingMutex,INFINITE);    \
   }
#endif

#if defined(_DEBUG) 
#define OPENGL_ERROR_CHECK    { \
   char szError[1024];                      \
   long errorCode = glGetError();           \
   if ( errorCode ) {                       \
      sprintf(szError,"Error in OpenGL call method '%s' (%d)\n\nOpenGL returned error %d (%x)\n\n\nDebug ?",__FILE__,__LINE__,errorCode,errorCode); \
      errorCode = MessageBox(NULL,szError,"OpenGL usage exception",MB_YESNOCANCEL); \
      if ( IDCANCEL == errorCode ) _exit(0);\
      if ( IDYES == errorCode ) {           \
      _asm {   int 3   }                    \
      }                                     \
   }                                        \
   }
#else
#define OPENGL_ERROR_CHECK
#endif

#if defined(_DEBUG) 
#define WINDOWS_ERROR_CHECK    { \
   char szError[1024];                      \
   long errorCode = GetLastError();         \
   if ( errorCode ) {                       \
      sprintf(szError,"Error in Windows call method '%s' (%d)\n\nOpenGL returned error %d (%x)\n\n\nDebug ?",__FILE__,__LINE__,errorCode,errorCode); \
      errorCode = MessageBox(NULL,szError,"Windows usage exception",MB_YESNOCANCEL); \
      if ( IDCANCEL == errorCode ) _exit(0);\
      if ( IDYES == errorCode ) {           \
      _asm {   int 3   }                    \
      }                                     \
   }                                        \
   }
#else
#define OPENGL_ERROR_CHECK
#define WINDOWS_ERROR_CHECK
#endif
#define WM_OPENGLIMPLEMENTATION_SETBASEWINDOW            WM_USER + 1
#define WM_OPENGLIMPLEMENTATION_SETTARGETWINDOW          WM_USER + 2

#define WM_OPENGLIMPLEMENTATION_SETLIGHTING              WM_USER + 7
#define WM_OPENGLIMPLEMENTATION_SETCOLOR                 WM_USER + 8
#define WM_OPENGLIMPLEMENTATION_SETLINEWEIGHT            WM_USER + 9

#define WM_OPENGLIMPLEMENTATION_ERASE                    WM_USER + 20
#define WM_OPENGLIMPLEMENTATION_RESETDEPTH               WM_USER + 21
#define WM_OPENGLIMPLEMENTATION_FINALIZE                 WM_USER + 22

#define WM_OPENGLIMPLEMENTATION_TRANSLATE                WM_USER + 30

   struct strCall_Rotate {
      char axis;
      double angle;
   };

#define WM_OPENGLIMPLEMENTATION_ROTATE                   WM_USER + 31
#define WM_OPENGLIMPLEMENTATION_SCALE                    WM_USER + 32

#define WM_OPENGLIMPLEMENTATION_SETVIEWPORT              WM_USER + 40

   struct strCall_Set2D {
      IDataSet *iDataSet;
      long margins;
      char directionRight;
      char directionUp;
   };

#define WM_OPENGLIMPLEMENTATION_SETVIEWPROPERTIES        WM_USER + 41

   struct strCall_SetViewProperties {
      IGProperty *pPropPlotView;
      IGProperty *pPropRotationTheta;
      IGProperty *pPropRotationPhi;
      IGProperty *pPropRotationSpin;
      IGProperty *pPropLeftMargin;
      IGProperty *pPropTopMargin;
      IGProperty *pPropRightMargin;
      IGProperty *pPropBottomMargin;
      IGProperty *pPropMarginUnits;
      IGProperty *pPropStretchToMargins;
   };

#define WM_OPENGLIMPLEMENTATION_UNSETVIEWPROPERTIES      WM_USER + 42

#define WM_OPENGLIMPLEMENTATION_SET2D                    WM_USER + 43

   struct strCall_Setup {
      IDataSet *masterDataSet;
      IGProperty *pPropPlotView;
      IGProperty *pPropRotationTheta;
      IGProperty *pPropRotationPhi;
      IGProperty *pPropRotationSpin;
      IGProperty *pPropLeftMargin;
      IGProperty *pPropTopMargin;
      IGProperty *pPropRightMargin;
      IGProperty *pPropBottomMargin;
      IGProperty *pPropMarginUnits;
      IGProperty *pPropStretchToMargins;
   };

#define WM_OPENGLIMPLEMENTATION_SETUP                    WM_USER + 44
   
   struct strCall_SetLighting {
      IGProperty **pPropLightEnabled;
      IGProperty **pPropAmbientLight;
      IGProperty **pPropDiffuseLight;
      IGProperty **pPropSpecularLight;
      IGProperty **pPropLightPos;
      IGProperty *pPropCountLights;
      IGProperty *pPropShinyness;
   };

#define WM_OPENGLIMPLEMENTATION_PUSH                     WM_USER + 45
#define WM_OPENGLIMPLEMENTATION_POP                      WM_USER + 46


#define WM_OPENGLIMPLEMENTATION_NEWLINE                  WM_USER + 50
#define WM_OPENGLIMPLEMENTATION_NEWPOLYGON               WM_USER + 51

#define WM_OPENGLIMPLEMENTATION_VERTEX                   WM_USER + 60
#define WM_OPENGLIMPLEMENTATION_NORMAL3D                 WM_USER + 61

   struct strCall_BeginSurface {
      long segmentID;
      IGProperty *pPropTopColor;
      IGProperty *pPropBottomColor;
   };

#define WM_OPENGLIMPLEMENTATION_BEGINSURFACE             WM_USER + 70
#define WM_OPENGLIMPLEMENTATION_ENDSURFACE               WM_USER + 71

   struct strCall_BeginWireframe {
      long segmentID;
      IGProperty *pPropLineColor;
      IGProperty *pPropLineWeight;
   };

#define WM_OPENGLIMPLEMENTATION_BEGINWIREFRAME           WM_USER + 72
#define WM_OPENGLIMPLEMENTATION_ENDWIREFRAME             WM_USER + 73

   struct strCall_BeginSolid {
      long segmentID;
      IGProperty *pPropTopColor;
      IGProperty *pPropBottomColor;
   };

#define WM_OPENGLIMPLEMENTATION_BEGINSOLID               WM_USER + 74
#define WM_OPENGLIMPLEMENTATION_ENDSOLID                 WM_USER + 75

   struct strCall_GetPickBoxHits {
      POINTL *ptl;
      long pickWindowSize;
      unsigned int *hitTable;
      long hitTableSize;
      long *pCallLists;
      unsigned int *hitTableHits;
      HRESULT returnValue;
   };

#define WM_OPENGLIMPLEMENTATION_GETPICKBOXHITS           WM_USER + 81

   struct strCall_WindowToData {
      DataPoint dpSource;
      DataPoint dpTarget;
      HRESULT result;
   };

#define WM_OPENGLIMPLEMENTATION_WINDOWTODATA             WM_USER + 90

   struct strCall_DataToWindow {
      DataPoint dpSource;
      UnitOfMeasure uom;
      DataPoint *pdpTarget;
   };

#define WM_OPENGLIMPLEMENTATION_DATATOWINDOW             WM_USER + 91

   struct strCall_DataListToWindow {
      DataList *dlSource;
      DataList *dlTarget;
   };

#define WM_OPENGLIMPLEMENTATION_DATALISTTOWINDOW         WM_USER + 92

   struct strCall_PixelsToUnits {
      char direction;
      double inPixels;
      double *answer;
   };

#define WM_OPENGLIMPLEMENTATION_PIXELSTOUNITS            WM_USER + 93

   struct strCall_UnitsToPixels {
      char direction;
      double inUnits;
      double *answer;
   };

#define WM_OPENGLIMPLEMENTATION_UNITSTOPIXELS            WM_USER + 94

   struct strCall_PixelPoint {
      DataPoint *pBeginning;
      DataPoint *pAnswer;
      HRESULT result;
   };

#define WM_OPENGLIMPLEMENTATION_PIXELPOINT               WM_USER + 95

   struct strCall_PixelVector {
      DataPoint *pBeginning;
      DataPoint *pEnding;
      DataPoint *pAnswer;
      HRESULT result;
   };

#define WM_OPENGLIMPLEMENTATION_PIXELVECTOR              WM_USER + 96


   struct strCall_OpenSegment {
      long segmentID;
      IGProperty* pPropColor;
      IGProperty* pPropLineWeight;
   };

   struct strCall_NewSegments {
      long numberToMake;
      long *firstInRange;
   };

#define WM_OPENGLIMPLEMENTATION_NEWSEGMENTS              WM_USER + 100

#define WM_OPENGLIMPLEMENTATION_OPENSEGMENT              WM_USER + 101

#define WM_OPENGLIMPLEMENTATION_STARTSEGMENT             WM_USER + 102

#define WM_OPENGLIMPLEMENTATION_ENDSEGMENT               WM_USER + 103

#define WM_OPENGLIMPLEMENTATION_REDRAWSEGMENT            WM_USER + 104

#define WM_OPENGLIMPLEMENTATION_PLAYSEGMENT              WM_USER + 105

   struct strCall_CloseSegment {
      long segmentID;
      unsigned short drawOnClose;
   };

#define WM_OPENGLIMPLEMENTATION_CLOSESEGMENT             WM_USER + 106

#define WM_OPENGLIMPLEMENTATION_DELETESEGMENT            WM_USER + 107

   struct strCall_ReadPixels {
      long x1,y1,x2,y2;
      BYTE *pResult;
   };

#define WM_OPENGLIMPLEMENTATION_READPIXELS               WM_USER + 108

#define WM_OPENGLIMPLEMENTATION_ISRENDERED               WM_USER + 109

#define WM_OPENGLIMPLEMENTATION_STOP                     WM_USER + 256
