
#pragma once

#include "list.h"
#include <list>

#include "general.h"

#include "GSystem_i.h"
#include "GraphicControl_i.h"
#include "Properties_i.h"
#include "DataSet_i.h"

#include "Variable_i.h"
#include "Evaluator_i.h"

#include "OpenGLImplementation_i.h"

#include "Plot_i.h"
#include "Text_i.h"

#include "Axis_i.h"

  class Axis : public IAxis, public IGPropertiesClient, public IGPropertyPageClient, public IGraphicSegmentAction {
  public:

     Axis(IUnknown *pUnkOuter);
     ~Axis();

     __declspec(dllexport) IPlot *Plot();

//      IPropertiesClient

     STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_(ULONG, AddRef)();
     STDMETHOD_(ULONG, Release)();

  private:

     STDMETHOD(SavePrep)();
     STDMETHOD(InitNew)();
     STDMETHOD(Loaded)();
     STDMETHOD(Saved)();
     STDMETHOD(IsDirty)();
     STDMETHOD(GetClassID)(BYTE *pCLSID);

//      IPropertyPageClient

     STDMETHOD(BeforeAllPropertyPages)();
     STDMETHOD(GetPropertyPagesInfo)(long* countPages,SAFEARRAY** stringDescriptions,SAFEARRAY** stringHelpDirs,SAFEARRAY** pSize);
     STDMETHOD(CreatePropertyPage)(long,HWND,RECT*,BOOL,HWND * hwndPropertyPage);
     STDMETHOD(Apply)();
     STDMETHOD(IsPageDirty)(long,BOOL*);
     STDMETHOD(Help)(BSTR bstrHelpDir);
     STDMETHOD(TranslateAccelerator)(long,long*);
     STDMETHOD(AfterAllPropertyPages)(BOOL);
     STDMETHOD(DestroyPropertyPage)(long);

     STDMETHOD(GetPropertySheetHeader)(void *pHeader);
     STDMETHOD(get_PropertyPageCount)(long *pCount);
     STDMETHOD(GetPropertySheets)(void *pSheets);

//      IDispatch

     STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
     STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
     STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
     STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

//      IAxis

     STDMETHOD(get_Type)(char *getType);

      /* Text properties */

     STDMETHOD(put_DrawText)(VARIANT_BOOL);
     STDMETHOD(get_DrawText)(VARIANT_BOOL*);

     STDMETHOD(put_LabelText)(BSTR);
     STDMETHOD(get_LabelText)(BSTR*);

     STDMETHOD(get_RepresentativeText)(IDispatch **);
     STDMETHOD(put_TickLabelPrecision)(double);
     STDMETHOD(get_TickLabelPrecision)(double*);

     STDMETHOD(get_SegmentID)(long*);
     STDMETHOD(get_ActionTable)(IGraphicSegmentAction**);
     STDMETHOD(get_SegmentCount)(long *);
     STDMETHOD(GetSegments)(long *);

     STDMETHOD(put_StartPoint)(SAFEARRAY *);
     STDMETHOD(get_StartPoint)(SAFEARRAY **);
     STDMETHOD(put_StartPointX)(double );
     STDMETHOD(get_StartPointX)(double *);
     STDMETHOD(put_StartPointY)(double );
     STDMETHOD(get_StartPointY)(double *);
     STDMETHOD(put_StartPointZ)(double );
     STDMETHOD(get_StartPointZ)(double *);

     STDMETHOD(put_EndPoint)(SAFEARRAY *);
     STDMETHOD(get_EndPoint)(SAFEARRAY **);
     STDMETHOD(put_EndPointX)(double );
     STDMETHOD(get_EndPointX)(double *);
     STDMETHOD(put_EndPointY)(double );
     STDMETHOD(get_EndPointY)(double *);
     STDMETHOD(put_EndPointZ)(double );
     STDMETHOD(get_EndPointZ)(double *);

     STDMETHOD(put_DefinesDomain)(VARIANT_BOOL);
     STDMETHOD(get_DefinesDomain)(VARIANT_BOOL*);

     STDMETHOD(put_LineWeight)(long);
     STDMETHOD(get_LineWeight)(long *);
     STDMETHOD(put_LineColor)(SAFEARRAY *);
     STDMETHOD(get_LineColor)(SAFEARRAY **);

     STDMETHOD(put_TickCount)(long);
     STDMETHOD(get_TickCount)(long *);
     STDMETHOD(put_TickSize)(double);
     STDMETHOD(get_TickSize)(double *);
     STDMETHOD(put_TickPercentAbove)(double);
     STDMETHOD(get_TickPercentAbove)(double *);
     STDMETHOD(put_GridLinesPerTick)(long);
     STDMETHOD(get_GridLinesPerTick)(long*);

   // Methods

     STDMETHOD(Initialize)(HWND hwndOwner,char theType,IAxis*,IAxis*,IAxis*,IGProperty*,IGProperty*,
                              IGProperty*,IGProperty*,IDataSet*,IOpenGLImplementation*,IEvaluator*,void (__stdcall *pWhenChangedCallback)(void *),void *pWhenChangedArg);

     STDMETHOD(get_DataSet)(IDataSet **);

      /* Text Methods */

     STDMETHOD(PrepText)();
     STDMETHOD(GetTextList)(void **getList);

     STDMETHOD(PrepData)();
     STDMETHOD(Draw)();
     STDMETHOD(DrawLabels)();
     STDMETHOD(Redraw)();
     STDMETHOD(Erase)();

     STDMETHOD(AdviseGSystemStatusBar)(IGSystemStatusBar*);

     STDMETHOD(ShowProperties)(HWND hwndParent);

// IGraphicSegmentAction

     STDMETHOD(Selector)();
     STDMETHOD(Unselector)();
     STDMETHOD(MenuRequest)();
     STDMETHOD(LeftMouse)();
     STDMETHOD(RightMouse)();
     STDMETHOD(MouseMove)(POINT* ptMouse);
     STDMETHOD(MouseRelease)();

     char type;

     HWND hwndOwner;
     HWND hwndObjectWindow;
     HWND hwndStyleSettings,hwndPositionSettings,hwndTextSettings,hwndColorSettings;
     HMENU hMainMenu,hMenu;
     RECT rectPropertiesPosition;

     DataPoint dpLabelPosition;

     WNDPROC defaultPatchPainter;
     WNDPROC defaultPatchPainter2;

     DataPoint origin,endPoint,minPoint,maxPoint,uvDirection;
     double tickAbove,tickBelow,tickLength;
     double tickPctAbove;
     long tickCount,gridLinesPerTick;
     double phi,theta;

     USHORT selected,expectingCommands;

     void (__stdcall *pWhenChangedCallback)(void *);
     void *pWhenChangedCallbackArg;

     IPlot *pIPlot;

     IGProperty* pParentPropertyPlotView;
     IGProperty* pParentPropertyFloor;
     IGProperty* pParentPropertyCeiling;
     IGProperty* pParentPropertyOpenGLText;

     IDataSet *pIDataSetDomain;
     IText *pLabel;
     IText *pRepresentativeText;
     IAxis *pXAxis,*pYAxis,*pZAxis;

     List<IText> textList;

     IUnknown *pIUnknownOuter;
     IUnknown *pIUnknownProperties;

     IOpenGLImplementation *pIOpenGLImplementation;
     IEvaluator *pIEvaluator;
     IGSystemStatusBar* pIGSystemStatusBar;

     IGProperties *pIGProperties;

     IGProperty *propertyDrawText;
     IGProperty *propertyAutoPrecision;

     IGProperty *propertyTickLength;
     IGProperty *propertyTickLengthUnits;
     IGProperty *propertyTickStraddleStyle;
     IGProperty *propertyTickStraddleAbove;
     IGProperty *propertyTickCount;
     IGProperty *propertyGridLinesPerTick;
     IGProperty *propertyAxisPrecision;
     IGProperty *propertyCustomColors;

     IGProperty *propertyOriginAtMinpoint;
     IGProperty *propertyOriginXValue;
     IGProperty *propertyOriginYValue;
     IGProperty *propertyOriginZValue;

     IGProperty *propertyEndpointAtMaxpoint;
     IGProperty *propertyEndpointXValue;
     IGProperty *propertyEndpointYValue;
     IGProperty *propertyEndpointZValue;
     IGProperty *propertyDeterminesDomain;

     IGProperty *propertyPropertiesPosition;

     IGProperty *propertyLabel;
     IGProperty *propertyDrawLabel;
     IGProperty *propertyAxisLabelSize;
     IGProperty *propertyAxisLabelSizeUnits;
     IGProperty *propertyLabelColor;
     IGProperty *propertyAxisLabelColorTrackLineColor;
     IGProperty *propertyLabelPositionNatural;
     IGProperty *propertyLabelPositionXValue;
     IGProperty *propertyLabelPositionYValue;
     IGProperty *propertyLabelPositionZValue;
     IGProperty *propertyLabelAnglePerpendicular;
     IGProperty *propertyLabelAngleParallel;
     IGProperty *propertyLabelAngleOther;
     IGProperty *propertyLabelAngleOtherValue;
     IGProperty *currentPropertyAxisLabelColor;

     IGProperty *propertyTickLabelSize;
     IGProperty *propertyTickLabelSizeUnits;
     IGProperty *propertyTickLabelColor;
     IGProperty *propertyTickLabelColorTrackLineColor;
     IGProperty *currentPropertyTickLabelColor;

     IGProperty *propertyLineColor;
     IGProperty *propertyLineWeight;

     IGProperty *propertyTextCount;

     IGProperty *propertyDefaultFont;

     IGProperty *propertyPlotType;

     unsigned int refCount;
     short axisTickColorTrackLineColor,axisLabelColorTrackLineColor;
     short drawTickLabels,doAutoPrecision,determinesDomain;
     short drawAxisLabel,labelPositionNatural,labelAnglePerpendicular,labelAngleParallel,labelAngleOther;

     WNDPROC oldSampleHandler;

     static LRESULT CALLBACK handler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK styleHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK positionHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK textHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK colorHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK patchPainterProc(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);

     int initWindows();
     int preparePlot();

     BSTR axisValue(double numberIn,double spread,int *precision,double maxError,double *errorBack,
                     double *answer,int doPower,int *powerBack);
     double myRound(double x,int power);

  };

#define AXIS_NOTEBOOK_X        10
#define AXIS_NOTEBOOK_Y        50
#define AXIS_NOTEBOOK_CX      500
#define AXIS_NOTEBOOK_CY      500
#define AXIS_NOTEBOOK_TAB_CX   60
#define AXIS_NOTEBOOK_TAB_CY   40

#define TICK_STRADDLE_NONE      0
#define TICK_STRADDLE_ABOVE     1
#define TICK_STRADDLE_BELOW     2
#define TICK_STRADDLE_BOTH      3

#define TICK_UNITS_PERCENT_TEXT "% plot size"
#define TICK_UNITS_DATA         2
#define TICK_UNITS_DATA_TEXT    "% data range"

#define DEFAULT_TICK_COUNT      4
#define DEFAULT_TICK_PERCENT    5.0
#define DEFAULT_TICK_UNITS      UNIT_PERCENT

#define DEFAULT_TICK_STRADDLE_STYLE  TICK_STRADDLE_BOTH
#define DEFAULT_TICK_STRADDLE_ABOVE  50.0

  extern HMODULE hModule;
  extern char szModuleName[];
  extern BSTR wstrModuleName;
  extern ITypeInfo *pITypeInfo;
