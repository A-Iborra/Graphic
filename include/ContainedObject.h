
#pragma once

#include "ocidl.h"

  template<class Parent> class ContainedObject : public IOleInPlaceSite, public IOleClientSite {

  public:

     ContainedObject(Parent* pParent,HWND hwndTab = NULL,IUnknown* pIUnknownObject = NULL);
     ~ContainedObject();

     void ReSize();

     HWND HWNDSite() { return hwndSite; };

     virtual int connectEvents() = 0;
     virtual int unConnectEvents() = 0;

  private:

   // IOleInPlaceSite

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

//     IOleClientSite

     STDMETHOD(SaveObject)();
     STDMETHOD(GetMoniker)(DWORD,DWORD,IMoniker**);
     STDMETHOD(GetContainer)(IOleContainer**);
     STDMETHOD(ShowObject)();
     STDMETHOD(OnShowWindow)(BOOL);
     STDMETHOD(RequestNewObjectLayout)();

   protected:

     IUnknown* pIUnknownObject;

     unsigned int refCount;
     HWND hwndTab,hwndSite;
     Parent* pParent;

     IConnectionPoint* pIConnectionPoint;
     DWORD dwConnectionCookie;

  };

