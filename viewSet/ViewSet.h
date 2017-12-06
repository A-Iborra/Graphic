
#pragma once

#include <ocidl.h>
 
#include "list.h"

#include "GSystem_i.h"
#include "Properties_i.h"
#include "DataSet_i.h"

#include "Variable_i.h"
#include "Evaluator_i.h"

#include "OpenGLImplementation_i.h"

#include "Plot_i.h"
#include "Text_i.h"
#include "Axis_i.h"

#include "viewSet_i.h"

  class ViewSet : 
         public IViewSet, 
         public IGPropertiesClient,
         public IGPropertyPageClient {
  public:

     ViewSet(IUnknown *pUnkOuter);
     ~ViewSet();

//      IViewSet

     STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_(ULONG, AddRef)();
     STDMETHOD_(ULONG, Release)();

     STDMETHOD(Initialize)(HWND hwndOwner,IOpenGLImplementation*,IEvaluator*,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IGProperty*,IDataSet*,IAxis*,IAxis*,IAxis*);

     STDMETHOD(put_ParentWindow)(HWND);

     STDMETHOD(Properties)();

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

  private:

     HWND hwndOwner;
     HWND hwndParent,hwndViewSet,hwndGraphic;
     HWND hwndGraphicContainer;

     POINTL ptlFrame;
     SIZEL sizelFrame;

     BOOL isVisible;

     IDataSet* pIDataSet;
     IAxis* pIAxis_X,*pIAxis_Y,*pIAxis_Z;
     IText* pIText_X,*pIText_Y,*pIText_Z;

     IUnknown *pIUnknownOuter;
     IUnknown *pIUnknownProperties;

     IGProperties *iProperties;
     IGProperty *parentPropertyPlotView;
     IGProperty *parentPropertyPhi,*parentPropertyTheta,*parentPropertySpin;
     IGProperty *parentPropertyFloor,*parentPropertyCeiling;

     IOpenGLImplementation *pIOpenGLImplementation;
     IEvaluator *pIEvaluator;

     unsigned int refCount;

     int render();
     int calcSize();
     int initWindows();
     int size();

     static LRESULT CALLBACK viewsetHandler(HWND hwnd,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK viewsetGraphicHandler(HWND hwnd,UINT,WPARAM,LPARAM);

  };

#define VIEWSET_MINCX  400        
#define VIEWSET_MINCY  300

  extern HMODULE hModule;
  extern char szModuleName[1024];
  extern OLECHAR wstrModuleName[1024];

