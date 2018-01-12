/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/
#pragma once

#include <ocidl.h>
#include <cGuid.h>
#include <wTypes.h>

#include <list>

#define MULTITHREADED_EVALUATOR

#include "GSystem_i.h"

#include "list.h"
#include "Utils.h"

#include "VList.h"

#include "Properties_i.h"
#include "Variable_i.h"
#include "Evaluator_i.h"
#include "DataSet_i.h"
#include "OpenGLImplementation_i.h"
#include "GraphicControl_i.h"

#include "Function_i.h"
#include "Plot_i.h"

   class Function;

#include "ConnectedEvaluator.h"

  class Function : 
        public IOleObject,
        public IOleInPlaceObject,
        public IOleControl,
        public IDataObject,
        public IViewObjectEx,
        public IProvideClassInfo2,
        public IRunnableObject,
        public IQuickActivate,
        public IGSFunctioNater,
        public IGPropertiesClient,
        public IGPropertyPageClient,
        public IConnectionPointContainer {

  public:

     Function(IUnknown* pUnkOuter);
     virtual ~Function();

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();

  private:

     STDMETHOD (GetTypeInfoCount)(UINT *pctinfo);
     STDMETHOD (GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
     STDMETHOD (GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
     STDMETHOD (Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

//   IFunction

     STDMETHOD(put_Expression)(BSTR putExpression);
     STDMETHOD(get_Expression)(BSTR* getExpression);
     STDMETHOD(put_ExpressionLabel)(BSTR strExpressionLabel);
     STDMETHOD(get_ExpressionLabel)(BSTR* pstrExpressionLabel);
     STDMETHOD(put_ResultsLabel)(BSTR strResultsLabel);
     STDMETHOD(get_ResultsLabel)(BSTR* pstrResultsLabel);
     STDMETHOD(put_AllowPropertySettings)(VARIANT_BOOL bAllowPropertySettings);
     STDMETHOD(get_AllowPropertySettings)(VARIANT_BOOL* pbAllowPropertySettings);
     STDMETHOD(put_AllowControlVisibilitySettings)(VARIANT_BOOL bAllowControlVisibilitySettings);
     STDMETHOD(get_AllowControlVisibilitySettings)(VARIANT_BOOL* pbAllowControlVisibilitySettings);
     STDMETHOD(put_ShowVariables)(VARIANT_BOOL bShowVariables);
     STDMETHOD(get_ShowVariables)(VARIANT_BOOL* bShowVariables);
     STDMETHOD(put_ShowControls)(VARIANT_BOOL bShowControls);
     STDMETHOD(get_ShowControls)(VARIANT_BOOL* bShowControls);
     STDMETHOD(put_ShowStart)(VARIANT_BOOL bShowStart);
     STDMETHOD(get_ShowStart)(VARIANT_BOOL* bShowStart);
     STDMETHOD(put_ShowPause)(VARIANT_BOOL bShowPause);
     STDMETHOD(get_ShowPause)(VARIANT_BOOL* bShowPause);
     STDMETHOD(put_ShowResume)(VARIANT_BOOL bShowResume);
     STDMETHOD(get_ShowResume)(VARIANT_BOOL* bShowResume);
     STDMETHOD(put_ShowStop)(VARIANT_BOOL bShowStop);
     STDMETHOD(get_ShowStop)(VARIANT_BOOL* bShowStop);
     STDMETHOD(put_ShowResults)(VARIANT_BOOL bShowResults);
     STDMETHOD(get_ShowResults)(VARIANT_BOOL* pbShowResults);
     STDMETHOD(put_ShowExpression)(VARIANT_BOOL bShowExpression);
     STDMETHOD(get_ShowExpression)(VARIANT_BOOL* pbShowExpression);

     STDMETHOD(put_DefaultMinValue)(BSTR );
     STDMETHOD(get_DefaultMinValue)(BSTR* );
     STDMETHOD(put_DefaultMaxValue)(BSTR );
     STDMETHOD(get_DefaultMaxValue)(BSTR* );
     STDMETHOD(put_DefaultStepCount)(long );
     STDMETHOD(get_DefaultStepCount)(long* );

     STDMETHOD(DefineIndependentVariable)(BSTR variableName,BSTR minValue,BSTR maxValue,long stepCount);
     STDMETHOD(DefineSimpleVariable)(BSTR variableName,VARIANT value);
     STDMETHOD(UndefineVariable)(BSTR variableName);

     STDMETHOD(Start)();
     STDMETHOD(Pause)();
     STDMETHOD(Resume)();
     STDMETHOD(Stop)();

     STDMETHOD(put_Enabled)(VARIANT_BOOL);
     STDMETHOD(get_Enabled)(VARIANT_BOOL*);

     STDMETHOD(PersistTo)(BSTR bstrFileName);
     STDMETHOD(PersistFrom)(BSTR bstrFileName);

     STDMETHOD(parseExpression)();

     STDMETHOD(EditProperties)();

     STDMETHOD(put_IPlot)(void *);
     STDMETHOD(get_IPlot)(void **);

     STDMETHOD(put_IDataSet)(void *);
     STDMETHOD(get_IDataSet)(void **);

     STDMETHOD(Initialize)(IDataSet* pIDataSet_Domain,IOpenGLImplementation *pIOpenGLImplementation,
                              IGProperty* pIPropertyLineColor,IGProperty* pIPropertyLineWeight,
                              IGProperty *parentPropertyPlotView,
                              IGProperty *parentPropertyDefault2DPlotSubType,
                              IGProperty *parentPropertyDefault3DPlotSubType,
                              IGProperty *parentPropertyBackgroundColor,
                              IGProperty *parentPropertyFloor,
                              IGProperty *parentPropertyCeiling,
                              void (__stdcall *pCallback)(void *,ULONG_PTR),void *,ULONG_PTR);

     STDMETHOD(put_OnChangeCallback)(void (__stdcall *pOnChange)(void *,ULONG_PTR),void *,ULONG_PTR);

     STDMETHOD(get_AnyControlVisible)(VARIANT_BOOL* pbAnyVisible);

     STDMETHOD(AdviseGSystemStatusBar)(IGSystemStatusBar *);

     STDMETHOD(AdviseGSGraphicServices)(/*IGSGraphicServices*/ void *);

//     IOleObject 

     STDMETHOD (SetClientSite)(IOleClientSite *pClientSite);
     STDMETHOD (GetClientSite)(IOleClientSite **ppCLientSite);
     STDMETHOD (SetHostNames)(LPCOLESTR szContainerApp,LPCOLESTR szContainerObj);
     STDMETHOD (Close)(DWORD dwSaveOption);
     STDMETHOD (SetMoniker)(DWORD dwWhichMoniker, IMoniker *pmk);
     STDMETHOD (GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker,IMoniker **ppmk);
     STDMETHOD (InitFromData)(IDataObject *pDataObject,BOOL fCreation,DWORD dwReserved);
     STDMETHOD (GetClipboardData)(DWORD dwReserved,IDataObject **ppDataObject);
     STDMETHOD (DoVerb)(LONG iVerb,LPMSG lpmsg,IOleClientSite *pActiveSite,LONG lindex,HWND hwndParent,LPCRECT lprcPosRect);
     STDMETHOD (EnumVerbs)(IEnumOLEVERB **ppenumOleVerb);
     STDMETHOD (Update)();
     STDMETHOD (IsUpToDate)();
     STDMETHOD (GetUserClassID)(CLSID * pClsid);
     STDMETHOD (GetUserType)(DWORD dwFormOfType, LPOLESTR *pszUserType);
     STDMETHOD (SetExtent)(DWORD dwDrawAspect, LPSIZEL lpsizel);
     STDMETHOD (GetExtent)(DWORD dwDrawAspect, LPSIZEL lpsizel);
     
     STDMETHOD (Advise)(IAdviseSink *pAdvSink, DWORD * pdwConnection);
     STDMETHOD (Unadvise)(DWORD dwConnection);
     STDMETHOD (EnumAdvise)(IEnumSTATDATA **ppenumAdvise);
     STDMETHOD (GetMiscStatus)(DWORD dwAspect, DWORD * pdwStatus);        
     
     STDMETHOD (SetColorScheme)(LPLOGPALETTE lpLogpal);

//      IOleWindow

     STDMETHOD (GetWindow)(HWND *);
     STDMETHOD (ContextSensitiveHelp)(BOOL);

//      IOleInPlaceObject

     STDMETHOD (InPlaceActivate)();
     STDMETHOD (InPlaceDeactivate)();
     STDMETHOD (UIDeactivate)();
     STDMETHOD (SetObjectRects)(LPCRECT,LPCRECT);
     STDMETHOD (ReactivateAndUndo)();

//      IOleControl

     STDMETHOD (GetControlInfo)(CONTROLINFO *);
     STDMETHOD (OnMnemonic)(MSG *);
     STDMETHOD (OnAmbientPropertyChange)(long);
     STDMETHOD (FreezeEvents)(int);

//      IDataObject

     STDMETHOD (GetData)(FORMATETC *,STGMEDIUM *);
     STDMETHOD (GetDataHere)(FORMATETC *,STGMEDIUM *);
     STDMETHOD (QueryGetData)(FORMATETC *);
     STDMETHOD (GetCanonicalFormatEtc)(FORMATETC *,FORMATETC *);
     STDMETHOD (SetData)(FORMATETC *,STGMEDIUM *,BOOL);
     STDMETHOD (EnumFormatEtc)(DWORD,IEnumFORMATETC **);
     STDMETHOD (DAdvise)(FORMATETC *,DWORD,IAdviseSink *,DWORD *);
     STDMETHOD (DUnadvise)(DWORD);
     STDMETHOD (EnumDAdvise)(IEnumSTATDATA **);

//      IViewObject

     STDMETHOD (Draw)(unsigned long,long,void *,DVTARGETDEVICE *,HDC,HDC,const struct _RECTL *,const struct _RECTL *,int (__stdcall *)(unsigned long),unsigned long);
     STDMETHOD (GetColorSet)(DWORD,long,void *,DVTARGETDEVICE *,HDC,LOGPALETTE **);
     STDMETHOD (Freeze)(DWORD,long,void *,DWORD *);
     STDMETHOD (Unfreeze)(DWORD);
     STDMETHOD (SetAdvise)(DWORD,DWORD,IAdviseSink *);
     STDMETHOD (GetAdvise)(DWORD *,DWORD *,IAdviseSink **);

//      IViewObject2

     STDMETHOD (GetExtent)(unsigned long,long,DVTARGETDEVICE *,struct tagSIZE *);

//      IViewObjectEx

     STDMETHOD (GetRect)(DWORD dwAspect,RECTL *);
     STDMETHOD (GetViewStatus)(DWORD *);
     STDMETHOD (QueryHitPoint)(DWORD dwAspect,const struct tagRECT *pRectBounds,POINT ptlHit,long lCloseHint,DWORD *dwHitResult);
     STDMETHOD (QueryHitRect)(DWORD dwAspect,const struct tagRECT *pRectBounds,const struct tagRECT *rctHit,long lCloseHint,DWORD *dwHitResult);
     STDMETHOD (GetNaturalExtent)(DWORD dwExtent,LONG lIndex,DVTARGETDEVICE *ptd,HDC hicTargetDev,DVEXTENTINFO *extentInfo,SIZEL *);

//      IProvideClassInfo

     STDMETHOD (GetClassInfo)(ITypeInfo **);

//      IProvideClassInfo2

     STDMETHOD (GetGUID)(DWORD,GUID *);

//      IRunnableObject

     STDMETHOD (GetRunningClass)(CLSID *);
     STDMETHOD (Run)(LPBC);
     int __stdcall IsRunning(void);
     STDMETHOD (LockRunning)(BOOL,BOOL);
     STDMETHOD (SetContainedObject)(BOOL);

//      IQuickActivate

     STDMETHOD(QuickActivate)(QACONTAINER* pQAContainer,QACONTROL* pQAControl);
     STDMETHOD(SetContentExtent)(SIZEL* pSizel);
     STDMETHOD(GetContentExtent)(SIZEL* pSizel);

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

//      IConnectionPointContainer

     STDMETHOD(FindConnectionPoint)(REFIID riid,IConnectionPoint **);
     STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints **);

  public:

     long fire_UndefinedVariable(BSTR variableName);
     void fire_UndefinedFunction(BSTR functionName);

     void fire_Started(long expectedIterations);
     void fire_Paused();
     void fire_Resumed();
     void fire_Stopped();
 
     void fire_TakeValues(long iterationNumber,char* pszNames,char* szValues,char** pszCookedResults);
     void fire_Finished();

     void fire_Clear();
     void fire_Parsed();

     void fire_DivideByZero();
     void fire_InvalidArgument(BSTR bstrFunction,double argumentValue);

     IVariable* UnknownVariable(char* variableName,bool* createdNew);
     int UnknownFunction(char* functionName);

     HWND hWnd() { return hwndSpecDialog; }

  private:

//      IOleInPlaceActiveObject

     struct _IOleInPlaceActiveObject : IOleInPlaceActiveObject {

        _IOleInPlaceActiveObject(Function *pp) : pParent(pp) {};
		  ~_IOleInPlaceActiveObject();

        STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

        STDMETHOD (GetWindow)(HWND *);
        STDMETHOD (ContextSensitiveHelp)(BOOL);

        STDMETHOD (TranslateAccelerator)(LPMSG);
        STDMETHOD (OnFrameWindowActivate)(BOOL);
        STDMETHOD (OnDocWindowActivate)(BOOL);
        STDMETHOD (ResizeBorder)(LPCRECT ,IOleInPlaceUIWindow *,BOOL);
        STDMETHOD (EnableModeless)(BOOL);

      private:

         Function* pParent;

     } *pIOleInPlaceActiveObject;
 
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

        _IConnectionPoint(Function *pp,REFIID outGoingInterfaceType);
		  ~_IConnectionPoint();
		  IUnknown *AdviseSink() { return adviseSink; };

     private:

        int getSlot();
        int findSlot(DWORD dwCookie);

		  IUnknown *adviseSink;
		  Function *pParent;
        DWORD nextCookie;
		  int countConnections,countLiveConnections;

        REFIID outGoingInterfaceType;

        CONNECTDATA *connections;

        long refCount;

        friend class Function;

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

	     _IEnumConnectionPoints(Function *pp,_IConnectionPoint **cp,int connectionPointCount);
       ~_IEnumConnectionPoints();

     private:

        int cpCount,enumeratorIndex;
		  Function *pParent;
		  _IConnectionPoint **connectionPoints;

     } *enumConnectionPoints;

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

     } *enumConnections;

     int defineFunction(char* pszTheExpression = NULL);

     int initWindows();
     int resize();
     short anyVisible();
     int hideAllTabs();
     int showVariableTab(IVariable* pV,char* pszVariableName);

     static unsigned long __stdcall evaluate(void *);

     static LRESULT EXPENTRY functionDialogHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT EXPENTRY functionVariablesHandler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT EXPENTRY functionPropertiesHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     //static LRESULT EXPENTRY functionPropertiesVisibilityHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT EXPENTRY expressionHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);

     WNDPROC oldExpressionHandler;
     WNDPROC oldResultLabelHandler;
     WNDPROC oldExpressionLabelHandler;

     IOleClientSite *pIOleClientSite;
     IStorage *pStorage;
     IOleAdviseHolder *pOleAdviseHolder;
     IDataAdviseHolder *pDataAdviseHolder;
     IAdviseSink *pAdviseSink;
     IOleInPlaceSite *pIOleInPlaceSite;
     IPropertyNotifySink *pIPropertyNotifySink;
     IGSystemStatusBar *pIGSystemStatusBar{NULL};
     IGSGraphicServices *pIGSGraphicServices{NULL};

     IPlot *pIPlot;
     IDataSet *pIDataSet;

     void (__stdcall *pWhenChangedCallback)(void *,ULONG_PTR);
     void *pWhenChangedCallbackArg;
     ULONG_PTR whenChangedCallbackCookie;

     DWORD adviseSink_dwAspect,adviseSink_advf;

     int refCount;

     static long functionCount;
     long iterationCount;
     long resultingHeight;
     long resultingWidth;
     int freezeEvents;

     int controlSpacing;
     HWND hwndSpecDialog,hwndVariables,hwndVariablesTab,hwndErrors,hwndProperties,hwndPropertiesVisibility;
     short allowUserProperties,allowUserPropertiesControls,userMode,enteringData,stopAllProcessing,askedForInPlaceObject;
     short isDesignMode;
     COLORREF backgroundColor,foregroundColor;

     union {
         short visibleItems[10];
         struct {
            short expressionVisible,resultsVisible,variablesVisible,controlsVisible,startVisible,pauseVisible,resumeVisible,stopVisible,plotPropertiesVisible,dataSetPropertiesVisible;
         };
     };

     USHORT ignoreEvalErrors,expectEvalUndefines,functionDefined;
     RECT rectDialog,rectLabel1,rectLabel2,rectExpression,rectResults,rectVariables,rectControls,rectProperties;

     ULONG threadID;

     IUnknown* pIUnknownProperties;
     IGProperties *iProperties;
     IGProperty* pIPropertyExpression;
     IGProperty* pIPropertyExpressionLabel;
     IGProperty* pIPropertyResultsLabel;

     IGProperty* pIPropertyPropertiesVisible;
     IGProperty* pIPropertyPropertiesControlVisibility;

     IGProperty* pIPropertyExpressionVisible;
     IGProperty* pIPropertyResultsVisible;
     IGProperty* pIPropertyVariablesVisible;
     IGProperty* pIPropertyControlsVisible;
     IGProperty* pIPropertyStartVisible;
     IGProperty* pIPropertyPauseVisible;
     IGProperty* pIPropertyResumeVisible;
     IGProperty* pIPropertyStopVisible;
     IGProperty* pIPropertyPlotPropertiesVisible;
     IGProperty* pIPropertyDataSetPropertiesVisible;
     IGProperty* pIPropertyVariables;
     IGProperty* pIPropertyManuallyAddedVariables;

     IGProperty* pIPropertyPlots;
     IGProperty* pIPropertyDataSets;

     IStream* pIStream_Marshalling;
     HGLOBAL hglMarshalling;

     ConnectedEvaluator *evaluator;

     char expression[MAX_PROPERTY_SIZE];
     char expressionLabel[MAX_PROPERTY_SIZE];
     char resultsLabel[MAX_PROPERTY_SIZE];

     BSTR bstrDefaultMinValue,bstrDefaultMaxValue;
     long defaultStepCount;
  
     VList* pVariableList;
     VList* pPropertyDialogVariableList;
     VList* pCurrentVariableList;
     VList* pManuallyAddedVariables;

     std::list<IVariable **> variableToDeleteList;

     IVariable* currentShowingVariable;

     friend class FunctionFactory;
     friend struct NonDelegatingUnknown;
     friend struct _IOleInPlaceActiveObject;
     friend class _IEvaluatorEvents;

  };

   extern HMODULE hModule;
   extern char szModuleName[1024];
   extern BSTR wstrModuleName;
   extern ITypeInfo *pITypeInfo_IGSFunctioNater;
   extern ITypeInfo* pIEvaluatorEventsTypeInfo;
   extern ITypeInfo *pITypeInfo_IGSFunctioNaterEvents;
   extern long oleMiscStatus;


#define CONTAINERSIZE_CX   256
#define CONTAINERSIZE_CY   334
