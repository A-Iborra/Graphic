#pragma once

#include <Windows.h>

#include "utils.h"
#include "resource.h"

#include "ObjectInOffice_i.h"

   class ObjectInOffice : public IOleObject, 
                           public IPersistStorage,
                           public IRunnableObject,
                           public IDataObject,
                           public IViewObjectEx,
                           public IObjectInOffice {

   public:

      ObjectInOffice(IUnknown *pOuter);
      ~ObjectInOffice();

      // IUnknown

      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();

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

      // IPersistStorage

      STDMETHOD(GetClassID)(CLSID *);
      STDMETHOD(IsDirty)();
      STDMETHOD(InitNew)(IStorage *);
      STDMETHOD(Load)(IStorage *);
      STDMETHOD(Save)(IStorage *,BOOL);
      STDMETHOD(SaveCompleted)(IStorage *);
      STDMETHOD(HandsOffStorage)();

      // IRunnableObject

      STDMETHOD(GetRunningClass)(CLSID *);
      STDMETHOD(Run)(LPBC);
      int __stdcall IsRunning(void);
      STDMETHOD(LockRunning)(BOOL,BOOL);
      STDMETHOD(SetContainedObject)(BOOL);

      // IDataObject

      STDMETHOD(GetData)(FORMATETC *,STGMEDIUM *);
      STDMETHOD(GetDataHere)(FORMATETC *,STGMEDIUM *);
      STDMETHOD(QueryGetData)(FORMATETC *);
      STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *,FORMATETC *);
      STDMETHOD(SetData)(FORMATETC *,STGMEDIUM *,BOOL);
      STDMETHOD(EnumFormatEtc)(DWORD,IEnumFORMATETC **);
      STDMETHOD(DAdvise)(FORMATETC *,DWORD,IAdviseSink *,DWORD *);
      STDMETHOD(DUnadvise)(DWORD);
      STDMETHOD(EnumDAdvise)(IEnumSTATDATA **);

      // IViewObject

      STDMETHOD(Draw)(unsigned long,long,void *,DVTARGETDEVICE *,HDC,HDC,const struct _RECTL *,const struct _RECTL *,int (__stdcall *)(ULONG_PTR),ULONG_PTR);
      STDMETHOD(GetColorSet)(DWORD,long,void *,DVTARGETDEVICE *,HDC,LOGPALETTE **);
      STDMETHOD(Freeze)(DWORD,long,void *,DWORD *);
      STDMETHOD(Unfreeze)(DWORD);
      STDMETHOD(SetAdvise)(DWORD,DWORD,IAdviseSink *);
      STDMETHOD(GetAdvise)(DWORD *,DWORD *,IAdviseSink **);

      // IViewObject2

      STDMETHOD(GetExtent)(unsigned long,long,DVTARGETDEVICE *,struct tagSIZE *);

      // IViewObjectEx

      STDMETHOD(GetRect)(DWORD dwAspect,RECTL *);
      STDMETHOD(GetViewStatus)(DWORD *);
      STDMETHOD(QueryHitPoint)(DWORD dwAspect,const struct tagRECT *pRectBounds,POINT ptlHit,long lCloseHint,DWORD *dwHitResult);
      STDMETHOD(QueryHitRect)(DWORD dwAspect,const struct tagRECT *pRectBounds,const struct tagRECT *rctHit,long lCloseHint,DWORD *dwHitResult);
      STDMETHOD(GetNaturalExtent)(DWORD dwExtent,LONG lIndex,DVTARGETDEVICE *ptd,HDC hicTargetDev,DVEXTENTINFO *extentInfo,SIZEL *);

   private:

      void initWindows();

      long refCount{0};

      IOleClientSite *pIOleClientSite{NULL};
      IOleInPlaceSite *pIOleInPlaceSite{NULL};
      IOleAdviseHolder *pOleAdviseHolder{NULL};
      IDataAdviseHolder *pDataAdviseHolder{NULL};

      IAdviseSink *pAdviseSink{NULL};
      DWORD adviseSink_dwAspect{0L};
      DWORD adviseSink_advf{0L};

      static LRESULT APIENTRY handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

   };

#ifdef DEFINE_DATA

   HMODULE hModule = NULL; 

   char szModuleName[MAX_PATH];
   BSTR bstrModuleName = NULL;

   long oleMisc = OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST | OLEMISC_INSIDEOUT | 
                               OLEMISC_CANTLINKINSIDE | OLEMISC_RECOMPOSEONRESIZE | OLEMISC_ALWAYSRUN;

   ObjectInOffice *pObjectInOffice = NULL;

   HWND hwndOwner = NULL;
   HWND hwndMain = NULL;
 
#else

   extern HMODULE hModule;

   extern char szModuleName[];

   extern BSTR bstrModuleName;

   extern long oleMisc;

   extern ObjectInOffice *pObjectInOffice;

   extern HWND hwndOwner;
   extern HWND hwndMain;
 
#endif

