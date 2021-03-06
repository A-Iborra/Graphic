// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Windows.h>
#include <ocidl.h>
#include <stddef.h>

#include "GSystem_i.h"
#include "Properties_i.h"

#include "Variable_i.h"
#include "Evaluator_i.h"

#include "DataSet_i.h"

#include "OpenGLImplementation_i.h"

#include "plot_i.h"
#include "text_i.h"

#include "GraphicControl_i.h"

#include "list.h"
#include "General.h"

#define DEFAULT_FONT "Tahoma"

  struct fontListData {
     long countVariations;
     LOGFONTA lf[1];
  };

  class Text : public IText,
               public IOleObject,
               public IOleInPlaceObject,
               public IOleInPlaceActiveObject,
               public IOleControl,
               public IGPropertyPageClient,
               public IGraphicSegmentAction {
  public:

     Text(IUnknown *pUnkOuter);
     ~Text();

     // IUnknown

     STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_(ULONG, AddRef)();
     STDMETHOD_(ULONG, Release)();

  private:

//  IDispatch

     STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
     STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
     STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
     STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

// IText

     STDMETHOD(put_Text)(BSTR setText);
     STDMETHOD(get_Text)(BSTR *getText);

     STDMETHOD(put_Format)(long setFormat);
     STDMETHOD(get_Format)(long* getFormat);
     STDMETHOD(put_Font)(BSTR fontName);
     STDMETHOD(get_Font)(BSTR *fontName);
     STDMETHOD(put_SizeProperty)(IGProperty*,IGProperty*);
     STDMETHOD(put_Size)(double);
     STDMETHOD(get_Size)(double*);
     STDMETHOD(put_SizeUnits)(enum TextSizeUnits);
     STDMETHOD(get_SizeUnits)(enum TextSizeUnits *);
     STDMETHOD(put_Position)(SAFEARRAY *);
     STDMETHOD(get_Position)(SAFEARRAY **);
     STDMETHOD(put_PositionString)(BSTR);
     STDMETHOD(get_PositionString)(BSTR *);
     STDMETHOD(put_PositionX)(double);
     STDMETHOD(get_PositionX)(double*);
     STDMETHOD(put_PositionY)(double);
     STDMETHOD(get_PositionY)(double*);
     STDMETHOD(put_PositionZ)(double);
     STDMETHOD(get_PositionZ)(double*);
     STDMETHOD(put_WindowPosition)(POINTL *setPosition);
     STDMETHOD(get_WindowPosition)(POINTL *getPosition);

     STDMETHOD(put_Color)(SAFEARRAY *);
     STDMETHOD(get_Color)(SAFEARRAY **);

     STDMETHOD(put_LineWeight)(double);
     STDMETHOD(get_LineWeight)(double *);

     STDMETHOD(get_SegmentID)(long*);

     STDMETHOD(get_ActionTable)(IGraphicSegmentAction**);

     STDMETHOD(put_CoordinatePlane)(CoordinatePlane);
     STDMETHOD(get_CoordinatePlane)(CoordinatePlane*);

     STDMETHOD(put_FlipHorizontal)(VARIANT_BOOL);
     STDMETHOD(get_FlipHorizontal)(VARIANT_BOOL *);
     STDMETHOD(put_FlipVertical)(VARIANT_BOOL);
     STDMETHOD(get_FlipVertical)(VARIANT_BOOL *);

     STDMETHOD(put_Rotation)(double);
     STDMETHOD(get_Rotation)(double *);

     STDMETHOD(get_minX)(double*);
     STDMETHOD(get_minY)(double*);
     STDMETHOD(get_minZ)(double*);

     STDMETHOD(get_maxX)(double*);
     STDMETHOD(get_maxY)(double*);
     STDMETHOD(get_maxZ)(double*);

     STDMETHOD(put_PartOfWorldDomain)(VARIANT_BOOL);
     STDMETHOD(get_PartOfWorldDomain)(VARIANT_BOOL*);

     STDMETHOD(put_Description)(BSTR);
     STDMETHOD(get_Description)(BSTR*);

     STDMETHOD(put_PartOfMainGraphic)(VARIANT_BOOL);
     STDMETHOD(get_PartOfMainGraphic)(VARIANT_BOOL*);

     STDMETHOD(get_SegmentCount)(long *);
     STDMETHOD(GetSegments)(long *);

     STDMETHOD(TextColorProperty)(IGProperty*);

     STDMETHOD(put_ShowContentPropertyPage)(VARIANT_BOOL);
     STDMETHOD(get_ShowContentPropertyPage)(VARIANT_BOOL*);

     STDMETHOD(put_ShowStylePropertyPage)(VARIANT_BOOL);
     STDMETHOD(get_ShowStylePropertyPage)(VARIANT_BOOL*);

     STDMETHOD(put_ShowOrientationPropertyPage)(VARIANT_BOOL);
     STDMETHOD(get_ShowOrientationPropertyPage)(VARIANT_BOOL*);

     STDMETHOD(put_EnablePositionSettings)(VARIANT_BOOL);
     STDMETHOD(get_EnablePositionSettings)(VARIANT_BOOL*);

     STDMETHOD(put_ShowPositionSettings)(VARIANT_BOOL);
     STDMETHOD(get_ShowPositionSettings)(VARIANT_BOOL*);

     STDMETHOD(put_TextNotify)(ITextNotify *);

     STDMETHOD(Initialize)(IOpenGLImplementation *,IEvaluator *,IDataSet *,
                              IGProperty *pPropXFloor,IGProperty* pPropXCeiling,
                              IGProperty *pPropYFloor,IGProperty* pPropYCeiling,
                              IGProperty *pPropZFloor,IGProperty* pPropZCeiling,
                              char *text,DataPoint *position,void (__stdcall *pWhenChangedCallback)(void *,ULONG_PTR),void *,ULONG_PTR);

     STDMETHOD(PrepData)();
     STDMETHOD(Draw)();
     STDMETHOD(Erase)();
     STDMETHOD(Redraw)();

     STDMETHOD(AdviseGSystemStatusBar)(IGSystemStatusBar*);

     STDMETHOD(EditProperties)();

     STDMETHOD(CopyFrom)(IText*);

     STDMETHOD(put_GDIBoundingBox)(RECT *);
     STDMETHOD(get_GDIBoundingBox)(RECT *);

     STDMETHOD(LineUp)();

     STDMETHOD(get_DataSet)(IDataSet **);

     STDMETHOD(HasContent)();

     STDMETHOD(AdviseGSGraphicServices)(void *);

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

//      IGraphicSegmentAction 

     STDMETHOD(Selector)();
     STDMETHOD(Unselector)();
     STDMETHOD(MenuRequest)();
     STDMETHOD(LeftMouse)();
     STDMETHOD(RightMouse)();
     STDMETHOD(MouseMove)(POINT* ptMouse);
     STDMETHOD(MouseRelease)();
     STDMETHOD(DefaultAction)();

  private:

//      IGPropertiesClient

     class _IGPropertiesClient : public IGPropertiesClient {
     public:
        _IGPropertiesClient() { pParent = PARENT_PTR(Text,iPropertiesClient); };

        STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();

        STDMETHOD(SavePrep)();
        STDMETHOD(InitNew)();
        STDMETHOD(Loaded)();
        STDMETHOD(Saved)();
        STDMETHOD(IsDirty)();
        STDMETHOD(GetClassID)(BYTE *pCLSID);

        Text* pParent;

        friend class Text;

     } iPropertiesClient;

     //friend class _IGPropertiesClient;

     IUnknown *pIUnknownOuter;

     IOleClientSite *pIOleClientSite;
     IOleInPlaceSite *pIOleInPlaceSite;
     IOleAdviseHolder *pOleAdviseHolder;
     IDispatch *pAmbientDispatch;

     IOpenGLImplementation *pIOpenGLImplementation;
     IEvaluator *pIEvaluator;

     void (__stdcall *pWhenChangedCallback)(void *,ULONG_PTR);
     void *pWhenChangedCallbackArg;
     ULONG_PTR whenChangedCallbackCookie;

     IBasePlot *pIBasePlot;
     IBasePlot *pIBasePlotBoundingBox;
     IDataSet *pIDataSet;
     IDataSet *pIDataSetBoundingBox;
     IDataSet *pIDataSetWorld;
     IGSystemStatusBar* pIGSystemStatusBar;
     IGSGraphicServices *pIGSGraphicServices;
     ITextNotify *pITextNotify;

     SIZEL containerSize;
     POINT ptSelectPoint,ptSelectOffset;

     HFONT hFont,hOriginalFont;
     HCURSOR moveCursor;
     LOGFONT logicalFont;

     HWND hwndFrame;
     HWND hwndObjectWindow,hwndSample,hwndStyle,hwndContent,hwndOrientation;

     HMENU hMainMenu,hMenu;

     HBITMAP hbmBoundingBoxBackground{NULL};
     RECT rcOnScreen{0,0,0,0};
     RECT rcSavedBoundingBox{0,0,0,0};
     RECT rcFallBackBoundingBox{0,0,0,0};

     TextFormat format;
     CoordinatePlane coordinatePlane;
     DataPoint dpStart;
     DataPoint dpSelectOffsetRestore;
     DataPoint dpTranslateFormatGDI;
     char szFace[MAX_PATH];
     double planeHeight;
     double fontHeight,fontWidth,fontAscent,fontDescent;
     double fontSize,fontWeight;
     double rotation;
     BOOL flipHorizontal,flipVertical;
     BOOL partOfWorldDomain;
     BOOL showContentPropertyPage;
     BOOL showStylePropertyPage;
     BOOL showOrientationPropertyPage;
     BOOL enablePositionSettings;
     BOOL showPositionSettings;
     BOOL partOfMainGraphic;

     unsigned int refCount;

     IGProperties *pIProperties;

     IGProperty *propertyTextColor;
     IGProperty *propertyLineWeight;

     IGProperty *propertyFaceStyle;
     IGProperty *propertySizeUnits;

     IGProperty *propertyFormat;
     IGProperty *propertyPropertiesPosition;
     
     IGProperty *propertyPositionString;
     IGProperty *propertyPositionX;
     IGProperty *propertyPositionY;
     IGProperty *propertyPositionZ;

     IGProperty *propertyCoordinatePlane;
     IGProperty *propertyPlaneHeight;
     IGProperty *propertyFlipHorizontal;
     IGProperty *propertyFlipVertical;

     IGProperty *propertyPartOfWorldDomain;

     IGProperty *propertyShowContentPropertyPage;
     IGProperty *propertyShowStylePropertyPage;
     IGProperty *propertyShowOrientationPropertyPage;

     IGProperty *propertyEnablePositionSettings;
     IGProperty *propertyShowPositionSettings;

     IGProperty *propertyDescription;
     IGProperty *propertyPartOfMainGraphic;

     IGProperty *propertyContent;

     IGProperty *propertyRotation;

     long renderText(HDC hdc = (HDC)NULL,char *theText = (char*)NULL);
     long renderGlyph(HDC hdc,char c,DataPoint& startPosition);
     int drawSample();
     int eraseBoundingBoxGDI();
     void saveBoundingBoxBackground();
     int statusPosition();
     int createFont(LOGFONT *);

     void renderOpenGLPoints();

     static void __stdcall renderTextNewGlyph(void *pvThis,void *pvOther);
     static void __stdcall renderTextNewContour(void *pvThis,void *pvOther);
     static void __stdcall renderTextEndContour(void *pvThis,void *pvOther);
     static void __stdcall renderTextEndGlyph(void *pvThis,void *pvOther);

     WNDPROC oldSampleHandler;
     WNDPROC defaultPatchPainter;

     static LRESULT CALLBACK handler(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK styleHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK contentHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK orientationHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK sampleHandler(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);
     static LRESULT CALLBACK patchPainterProc(HWND hwnd,UINT msg,WPARAM mp1,LPARAM mp2);

     static int __stdcall fontListEnumerator(const LOGFONTA *,const TEXTMETRICA *,DWORD,LPARAM);
     static int __stdcall fontSelectEnumerator(const LOGFONTA *,const TEXTMETRICA *,DWORD,LPARAM);

  };

#define DEFAULT_TAB_SPACES        3
#define RANGE 1000.0
#define RANGEOVER2 500.0

extern ITypeInfo *pITypeInfo;
extern HMODULE hModule;
