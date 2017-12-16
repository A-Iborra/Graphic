
#pragma once

#include "ocidl.h"

  class G;

  class ContainedObject : public IUnknown {

  public:

     ContainedObject(G* pParent,HWND hwndTab,IUnknown* pIUnknownObject,REFIID riidEventsInterface);
     ~ContainedObject();

     int connectEvents();
     int unConnectEvents();

     void ReSize();

     HWND HWNDSite() { return hwndSite; };

     void Show();

     void Hide();

   // IUnknown

     STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();

  private:

   // IOleInPlaceSite

     class _IOleInPlaceSite : public IOleInPlaceSite {

     public:

        _IOleInPlaceSite(ContainedObject* pp) : pParent(pp) {};

        STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

     private:

        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();
   
        STDMETHOD(GetWindow)(HWND*);
        STDMETHOD(ContextSensitiveHelp)(BOOL);
        STDMETHOD(CanInPlaceActivate)();
        STDMETHOD(OnInPlaceActivate)();
        STDMETHOD(OnUIActivate)();
        STDMETHOD(GetWindowContext)(IOleInPlaceFrame**,IOleInPlaceUIWindow**,RECT* position,RECT* clip,OLEINPLACEFRAMEINFO*);
        STDMETHOD(Scroll)(SIZE);
        STDMETHOD(OnUIDeactivate)(BOOL);
        STDMETHOD(OnInPlaceDeactivate)();
        STDMETHOD(DiscardUndoState)();
        STDMETHOD(DeactivateAndUndo)();
        STDMETHOD(OnPosRectChange)(const RECT*);

      protected:

        ContainedObject* pParent;

        friend class ContainedObject;

     } * pIOleInPlaceSite;

//     IOleClientSite

     class _IOleClientSite : public IOleClientSite {

     public:

        _IOleClientSite(ContainedObject* pp);

        STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

     private:

        STDMETHOD(SaveObject)();
        STDMETHOD(GetMoniker)(DWORD,DWORD,IMoniker**);
        STDMETHOD(GetContainer)(IOleContainer**);
        STDMETHOD(ShowObject)();
        STDMETHOD(OnShowWindow)(BOOL);
        STDMETHOD(RequestNewObjectLayout)();

        class _IDispatch : public IDispatch {

        public:

           _IDispatch(_IOleClientSite *pp) : pParent(pp) {};

           STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
           STDMETHOD_ (ULONG, AddRef)();
           STDMETHOD_ (ULONG, Release)();

        private:

           STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
           STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
           STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
           STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

           _IOleClientSite *pParent;

         } iDispatch;

      protected:

        ContainedObject* pParent;

        friend class ContainedObject;

     } * pIOleClientSite;

   protected:

     IUnknown* pIUnknownObject;

     unsigned int refCount;
     HWND hwndTab,hwndSite;
     G* pParent;

     REFIID riidEventsInterface;
     IConnectionPoint* pIConnectionPoint;
     DWORD dwConnectionCookie;

     friend class _IOleInPlaceSite;
     friend class _IOleClientSite;

  };

