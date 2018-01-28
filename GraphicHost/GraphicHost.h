#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <OleCtl.h>
#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"
#include "GraphicControl_i.h"
#include "Properties_i.h"

  class GraphicHost : public IUnknown {

  public:

     GraphicHost();
     ~GraphicHost();

      void initialize();

      HWND HWNDSite() { return hwndSite; };

   // IUnknown

     STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();

  private:

   // IOleInPlaceSite

     class _IOleInPlaceSite : public IOleInPlaceSite {

     public:

        _IOleInPlaceSite(GraphicHost * pp) : pParent(pp) {};

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

        GraphicHost* pParent;

     } * pIOleInPlaceSite{NULL};

//     IOleClientSite

     class _IOleClientSite : public IOleClientSite {

     public:

        _IOleClientSite(GraphicHost* pp);

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

      protected:

        GraphicHost* pParent;

     } * pIOleClientSite{NULL};

   protected:

      IGProperties *pIGProperties{NULL};
      IGProperty *pIGProperty_Graphic{NULL};
      IGPropertiesClient *pIGPropertiesClient_Graphic{NULL};
      IGProperty *pIGProperty_SizeAndPos{NULL};

      IGSGraphic *pIGraphic{NULL};

      IOleObject *pIOleObject_Graphic{NULL};
      IOleInPlaceObject *pIOleInPlaceObject_Graphic{NULL};

      HWND hwndSite{NULL};

      unsigned int refCount{0};

      DWORD dwConnectionCookie{0};

      RECT rcFrame{0,0,0,0};

      static LRESULT EXPENTRY graphicHandler(HWND,UINT,WPARAM,LPARAM);


   };

   LRESULT __stdcall frameHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

#ifdef DEFINE_DATA

   HINSTANCE hInstance;

   char szApplicationDataDirectory[MAX_PATH];

   HWND hwndFrame = NULL;
   HWND hwndGraphic = NULL;
   
   IOleObject *pIOleObject_Graphic = NULL;

   GraphicHost *pGraphicHost = NULL;

#else
   
   extern HINSTANCE hInstance;

   extern char szApplicationDataDirectory[];

   extern IOleObject *pIOleObject_Graphic;

   extern HWND hwndFrame;
   extern HWND hwndGraphic;

   extern GraphicHost *pGraphicHost;

#endif
