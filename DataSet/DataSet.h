// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <algorithm>
#include <functional>

#include <OCIdl.h>
#include <list>
#include <CommCtrl.h>

#import "C:\Program Files (x86)\Common Files\Microsoft Shared\Office12\MSO.dll" \
      rename("RGB","MSORGB") \
      rename("IAccessible","msoIAccessible") \
      rename("DocumentProperties","msoDocumentProperties")

using namespace Office;

#import "C:\Program Files (x86)\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB" 

using namespace VBIDE;

#import "C:\Program Files (x86)\Microsoft Office\Office12\excel.exe" \
      rename( "RGB", "excelRGB") \
      rename( "ReplaceText","excelReplaceText") \
      rename( "CopyFile", "excelCopyFile" ) \
      rename( "DialogBox","excelDialogBox")

#include "GSystem_i.h"
#include "Properties_i.h"
#include "DataSet_i.h"

#include "Variable_i.h"
#include "Evaluator_i.h"
#include "OpenGLImplementation_i.h"
#include "Function_i.h"

#include "Plot_i.h"
#include "GraphicControl_i.h"

#include "Graphic_resource.h"

#include "List.h"
#include "utils.h"

#define EXCEL_VALUE_SIZE MAX_PATH
#define ERROR_MESSAGE_DURATION   5000

   struct boundingBox {
      boundingBox(double *pxmin,double *pymin,double *pzmin,double *pxmax,double *pymax,double *pzmax) :
                pxMin(pxmin),pyMin(pymin),pzMin(pzmin),pxMax(pxmax),pyMax(pymax),pzMax(pzmax) {};
      double *pxMin,*pyMin,*pzMin,*pxMax,*pyMax,*pzMax;
   };

   struct extents {
      double xMin,yMin,zMin,xMax,yMax,zMax;
   };

   class DataSet : 
         public IOleObject,
         public IOleInPlaceObject,
         public IOleControl,
         public IDataObject,
         public IViewObjectEx,
         public IProvideClassInfo2,
         public IRunnableObject,
         public IQuickActivate,
         public IDataSet,
         public IGPropertiesClient,
         public IGPropertyPageClient,
         public IConnectionPointContainer { 

   public:
 
      DataSet(IUnknown *pUnknownOuter);

      ~DataSet();
 
//   IDataSet
 
      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_(ULONG, AddRef)();
      STDMETHOD_(ULONG, Release)();
 

      STDMETHOD(put_Name)(BSTR);
      STDMETHOD(get_Name)(BSTR *);

      STDMETHOD(put_DataSource)(BSTR);
      STDMETHOD(get_DataSource)(BSTR *);

      STDMETHOD(put_IsFunctionSource)(VARIANT_BOOL);
      STDMETHOD(get_IsFunctionSource)(VARIANT_BOOL *);

      STDMETHOD(put_IFunction)(void *);
      STDMETHOD(get_IFunction)(void **);

      STDMETHOD(put_IPlot)(void *);
      STDMETHOD(get_IPlot)(void **);

      STDMETHOD(Initialize)(void * pvIDataSet_Domain,void *pvIOpenGLImplementation,
                              IGProperty* pIPropertyLineColor,IGProperty* pIPropertyLineWeight,
                              IGProperty *parentPropertyPlotView,
                              IGProperty *parentPropertyDefault2DPlotSubType,
                              IGProperty *parentPropertyDefault3DPlotSubType,
                              IGProperty *parentPropertyBackgroundColor,
                              IGProperty *parentPropertyXFloor,IGProperty *parentPropertyXCeiling,
                              IGProperty *parentPropertyYFloor,IGProperty *parentPropertyYCeiling,
                              IGProperty *parentPropertyZFloor,IGProperty *parentPropertyZCeiling,
                              void (__stdcall *pCallback)(void *,ULONG_PTR),void *,ULONG_PTR);

      STDMETHOD(put_OnChangeCallback)(void (__stdcall *)(void *,ULONG_PTR),void *,ULONG_PTR);

      STDMETHOD(put_maxX)(double);                     
      STDMETHOD(get_maxX)(double *getVal);                     
      STDMETHOD(put_maxY)(double);                     
      STDMETHOD(get_maxY)(double *getVal);                     
      STDMETHOD(put_maxZ)(double);
      STDMETHOD(get_maxZ)(double *getVal);                     
      STDMETHOD(put_minX)(double);
      STDMETHOD(get_minX)(double *getVal);                     
      STDMETHOD(put_minY)(double);
      STDMETHOD(get_minY)(double *getVal);                     
      STDMETHOD(put_minZ)(double);
      STDMETHOD(get_minZ)(double *getVal);                     
                                                                 
      STDMETHOD(put_XFloor)(IGProperty *);
      STDMETHOD(put_XCeiling)(IGProperty *);

      STDMETHOD(put_YFloor)(IGProperty *);
      STDMETHOD(put_YCeiling)(IGProperty *);

      STDMETHOD(put_ZFloor)(IGProperty *);
      STDMETHOD(put_ZCeiling)(IGProperty *);

      STDMETHOD(get_top)(DataList **getTop);                    
      STDMETHOD(get_bottom)(DataList **getBottom);              
 
      STDMETHOD(get_countPoints)(long*);
 
      enum DataArity __stdcall DataArity();

      STDMETHOD(peek)(DataList *afterItem,DataList **getItem);                     
      STDMETHOD(peekInt)(int item,DataList **getItem);
 
      STDMETHOD(peekGDI)(DataList *afterItem,DataList **getItem);                     
 
      STDMETHOD(get)(DataList *AfterItem,DataPoint *data,DataList **nextItem);                   
      STDMETHOD(set)(DataList *item,DataPoint *data);

      STDMETHOD(pushString)(char *valueString);
 
      STDMETHOD(pushDataPoint)(DataPoint *point);
 
      STDMETHOD(popDataPoint)(DataPoint *point);
 
      STDMETHOD(ReSet)();
 
      STDMETHOD(insert)(DataList *insertBefore,DataPoint *data);
     
      STDMETHOD(addXYZ)(double *,double *, double *);

      STDMETHOD(addMovePoint)();

      STDMETHOD(makeUnitSize)();
 
      STDMETHOD(ResetLimits)(DataPoint*);
 
      STDMETHOD(SetDomain)(DataPoint*,DataPoint*);

      STDMETHOD(SetDomainXYZ)(double *xmin,double *ymin,double *zmin,double *xmax,double *ymax,double *zmax);
 
      STDMETHOD(GetDomain)(DataPoint*,DataPoint*);
 
      STDMETHOD(IncludeDomain)(IDataSet*);
 
      STDMETHOD(get_IsIncludedDomain)(IDataSet*,boolean *);

      STDMETHOD(RemoveIncludedDomain)(IDataSet*);
 
      STDMETHOD(SetColorRGB)(double *red,double *green,double *blue);

      STDMETHOD(Scale)(DataPoint*);
 
      STDMETHOD(Translate)(DataPoint*);
 
      STDMETHOD(TranslateGDI)(POINT*);
 
      STDMETHOD(Rotate)(char,double);
 
      STDMETHOD(RotateVector)(double,DataPoint*);

      STDMETHOD(Shear)(char,double);
 
      STDMETHOD(Move)(DataPoint*);
 
      STDMETHOD(GenerateBoundingBox)(IDataSet*);
 
      STDMETHOD(GenerateGDICoordinates)(IUnknown*);

      STDMETHOD(PushExtents)();

      STDMETHOD(PopExtents)();

      STDMETHOD(Evaluate)(BSTR,double *);

      STDMETHOD(GetDomainGDI)(struct DataPoint* minPoint,struct DataPoint* maxPoint);

      STDMETHOD(Start)();
 
      STDMETHOD(AdviseGSystemStatusBar)(IGSystemStatusBar*);

      STDMETHOD(AdviseGSGraphicServices)(/*IGSGraphicServices*/ void *);

   private:
 
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

   //public:

     void fire_Started(long expectedIterations);
     void fire_Paused();
     void fire_Resumed();
     void fire_Stopped();
 
     void fire_TakeValues(long iterationNumber,char* pszNames,char* szValues,char** pszCookedResults);
     void fire_Finished();

     void fire_Clear();

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

         _IConnectionPoint(DataSet *pp,REFIID outGoingInterfaceType);
	      ~_IConnectionPoint();

	      IUnknown *AdviseSink() { return adviseSink; };

      private:

         int getSlot();
         int findSlot(DWORD dwCookie);

	      IUnknown *adviseSink{NULL};
	      DataSet *pParent{NULL};
         DWORD nextCookie{0L};
	      int countConnections{0};
         int countLiveConnections{0};

         REFIID outGoingInterfaceType;

         CONNECTDATA *connections{NULL};

         long refCount{0};

         //friend class Function;

      } *pIConnectionPoint{NULL};

      struct _IEnumConnectionPoints : IEnumConnectionPoints {
      public:

         STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

 	      STDMETHOD (Next)(ULONG cConnections,IConnectionPoint **rgpcn,ULONG *pcFetched);
         STDMETHOD (Skip)(ULONG cConnections);
         STDMETHOD (Reset)();
         STDMETHOD (Clone)(IEnumConnectionPoints **);

	      _IEnumConnectionPoints(DataSet *pp,_IConnectionPoint **cp,int connectionPointCount);
         ~_IEnumConnectionPoints();

      private:

         int cpCount{0};
         int enumeratorIndex{0};
	      DataSet *pParent{NULL};

	      _IConnectionPoint **connectionPoints{NULL};

      } *enumConnectionPoints{NULL};

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

         ULONG refCount{0};
         IUnknown *pParentUnknown{NULL};
         ULONG enumeratorIndex{0L};
         ULONG countConnections{0L};
         CONNECTDATA *connections{NULL};

      } *enumConnections{NULL};

//      IOleInPlaceActiveObject

      struct _IOleInPlaceActiveObject : IOleInPlaceActiveObject {

         _IOleInPlaceActiveObject(DataSet *pp) : pParent(pp) {};
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

         DataSet* pParent;

      } *pIOleInPlaceActiveObject{NULL};

   public:

      class _IGSFunctioNaterEvents : public IGSFunctioNaterEvents {
      public:

         _IGSFunctioNaterEvents(DataSet *pp,IGSFunctioNater *pIFunction,std::function<void()>* pOnFinished);
         ~_IGSFunctioNaterEvents();

         STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

      private:

         STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
         STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
         STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
         STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

         STDMETHOD(UndefinedVariable)(BSTR);
         STDMETHOD(UndefinedFunction)(BSTR);
         STDMETHOD(Clear)();
         STDMETHOD(Parsed)();
         STDMETHOD(Started)(long);
         STDMETHOD(Paused)();
         STDMETHOD(Resumed)();
         STDMETHOD(Stopped)();
         STDMETHOD(TakeValues)(long,long,SAFEARRAY**,SAFEARRAY**);
         STDMETHOD(TakeResults)(long iterationNumber,BSTR bstrResults);
         STDMETHOD(Finished)();

         DataSet *pParent{NULL};
         IGSFunctioNater *pIFunction{NULL};

         IConnectionPoint* pIConnectionPoint{NULL};
         DWORD dwConnectionCookie{0};

         std::function<void()>* pExecuteOnFinish;

     } *pIGSFunctioNaterEvents{NULL};
 
   private:

      int initWindows();

      long loadExcelWorkbook(HWND hwndSheetList,HWND hwndRangeList,HWND hwndErrorReport,char *pszWorkbookName,char *pszRelevantSheetName,char *pszRelevantRangeName);
      Excel::_Workbook *openExcelWorkbook(char *pszWorkbookName,bool *pWasOpen);
      long loadExcelNamedRange(HWND hwndDestinationList,HWND hwndErrorReport,char *pszWorkbookName,char *pszRangeName);
      long loadExcelCellRange(HWND hwndDestinationList,HWND hwndErrorReport,char *pszWorkbookName,char *pszWorksheetName,char *pszCellRange);
      Excel::_Worksheet *getExcelWorksheet(Excel::_Workbook *pIWorkbook,char *pszSheetName);
      Excel::_Worksheet *getExcelWorksheet(Excel::_Workbook *pIWorkbook,WCHAR *pszwSheetName);
      Excel::NamePtr findNamedRangeName(Excel::_Workbook *pIWorkbook,char *pszRangeName);
      long loadExcelWorksheet(char *pszWorkbookName,char *pszWorksheetName);
      long launchExcel(char *pszWorkbookName);
      long populateData(HWND hwndListView,HWND hwndErrorReport,bool createColumns,SAFEARRAY *pArray);
      long exportToExcel(char *pszWorkbookName,char *pszWorksheetName,char *pszTopCell,bool isAutoCall = false);
      bool peekExcelCells(char *pszWorkbookName,char *pszRangeName,char *pszWorksheetName,char *pszCellRange,bool *pContainsHeaderRow);
      bool dataHasHeaderRow(SAFEARRAY *pArray);
      static DWORD WINAPI threadedExportToExcel(void *pvArg);

      bool cancelExcelExport{false};

      long loadDataSet(HWND hwndListView);
 
      int resetLimits(const DataPoint&);
      int resizeBounds();

      Excel::_Application *pIApplication{NULL};

      IOleClientSite *pIOleClientSite{NULL};
      IOleAdviseHolder *pOleAdviseHolder{NULL};
      IDataAdviseHolder *pDataAdviseHolder{NULL};
      IOleInPlaceSite *pIOleInPlaceSite{NULL};
      IAdviseSink *pAdviseSink{NULL};
      IPropertyNotifySink *pIPropertyNotifySink{NULL};
      IGSystemStatusBar *pIGSystemStatusBar{NULL};
      IGSGraphicServices *pIGSGraphicServices{NULL};

      int freezeEvents{0};

      DWORD adviseSink_dwAspect{0L};
      DWORD adviseSink_advf{0L};
      short askedForInPlaceObject{0};

      IUnknown *pIUnknownProperties{NULL};
      IGProperties *pIGProperties{NULL};

      HWND hwndSpecDialog{NULL};

      HWND hwndMainPropertiesPage{NULL};
      HWND hwndMainPropertiesPageError{NULL};
      HWND hwndExcelSettings{NULL};
      HWND hwndExcelSettingsError{NULL};
      HWND hwndFunctionDataSettings{NULL};

      SIZEL containerSize{0L,0L};

      boundingBox *pBoundingBox{NULL};
 
      IUnknown *pUnknownOuter{NULL};
 
      unsigned int refCount{1};
 
      BYTE propertiesStart{0};

      char szName[64];
      char szDataSource[MAX_PATH];
      char szSpreadsheetName[MAX_PATH];
      char szNamedRange[MAX_PATH];
      char szCellRange[MAX_PATH];
      char szExportWorkbookName[MAX_PATH];
      char szExportWorksheetName[64];
      char szExportWorksheetTopLeftCell[32];
      double xMax,xMin,yMax,yMin,zMax,zMin;
      double currentColor[3]{0.0,0.0,0.0};
      char szEquations[1024];
      bool isFunction{false};
      bool isEmbedded{false};
      bool hasHeaderRow{false};
      bool hasHeaderRowDetermined{false};

      BYTE propertiesEnd{0};

      long countX{0};
      long countY{0};
      long countZ{0};

      CLSID CLSID_excel;

      IGProperty *pPropertyState{NULL};

      IGProperty *pPropertyXFloor{NULL};
      IGProperty *pPropertyXCeiling{NULL};

      IGProperty *pPropertyYFloor{NULL};
      IGProperty *pPropertyYCeiling{NULL};

      IGProperty *pPropertyZFloor{NULL};
      IGProperty *pPropertyZCeiling{NULL};

      IGProperty* pPropertyPlots{NULL};

      IGProperty* pPropertyEmbeddedData{NULL};

      DataList *firstData{NULL};
      DataList *topData{NULL};
      DataList *gdiData{NULL};

      IEvaluator *pIEvaluator{NULL};
      IGSFunctioNater *pIFunction{NULL};

      IPlot *pIPlot{NULL};

      void (__stdcall *pWhenChangedCallback)(void *,ULONG_PTR);
      void *pWhenChangedCallbackArg;
      ULONG_PTR whenChangedCallbackCookie;

      std::function<void()> *pOneShotOnFinishedLambda{NULL};

      std::list<IDataSet *> otherDomains;
      std::list<extents *> extentsStack;

      static WNDPROC nativeEditHandler;
      static WNDPROC nativeStaticHandler;

      static LRESULT EXPENTRY dataSetDialogHandler(HWND,UINT,WPARAM,LPARAM);
   
      //static LRESULT EXPENTRY dataSetInstructionsHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT EXPENTRY dataSetPropertiesHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT EXPENTRY dataSetExcelHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT EXPENTRY dataSetFunctionDataHandler(HWND,UINT,WPARAM,LPARAM);

      static LRESULT EXPENTRY overrideEditHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT EXPENTRY statusAndErrorTextStaticHandler(HWND,UINT,WPARAM,LPARAM);

   };

   extern DataPoint movePoint;

   extern HMODULE hModule;
   extern long oleMiscStatus;
   extern ITypeInfo* pITypeInfo_IGSFunctioNaterEvents;
