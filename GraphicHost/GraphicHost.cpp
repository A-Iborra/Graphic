
#include "GraphicHost.h"
#include "utils.h"

   WNDPROC GraphicHost::nativeStaticHandler = NULL;

   GraphicHost::GraphicHost(HWND hs) :
      hwndSite(hs) {

   pIOleClientSite = new _IOleClientSite(this);

   pIOleInPlaceSite = new _IOleInPlaceSite(this);

   SetWindowLongPtr(hwndSite,GWLP_USERDATA,(ULONG_PTR)this);

   HRESULT rc = CoCreateInstance(CLSID_GSystemGraphic,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IOleObject,reinterpret_cast<void **>(&pIOleObject_Graphic));

   pIOleObject_Graphic -> QueryInterface(IID_IGSGraphic,reinterpret_cast<void **>(&pIGraphic));

   pIOleObject_Graphic -> QueryInterface(IID_IOleInPlaceObject,reinterpret_cast<void **>(&pIOleInPlaceObject_Graphic));

   nativeStaticHandler = (WNDPROC)SetWindowLongPtr(hwndSite,GWLP_WNDPROC,(ULONG_PTR)graphicHandler);

   pIOleObject_Graphic -> SetClientSite(static_cast<IOleClientSite *>(pIOleClientSite));

   RECT rect;
   GetWindowRect(hwndSite,&rect);

   long cx = rect.right - rect.left;
   long cy = rect.bottom - rect.left;

   SIZEL sizel{rect.right - rect.left,rect.bottom - rect.left};

   pixelsToHiMetric(&sizel,&sizel);

   pIOleObject_Graphic -> SetExtent(DVASPECT_CONTENT,&sizel);

   CoCreateInstance(CLSID_InnoVisioNateProperties,NULL,CLSCTX_INPROC_SERVER,IID_IGProperties,reinterpret_cast<void **>(&pIGProperties));

   pIGProperties -> Add(L"Graphic",&pIGProperty_Graphic);

   pIGProperty_Graphic -> put_type(TYPE_OBJECT_STORAGE_ARRAY);

   pIGProperties -> put_FileName(L"Graphic.settings");

   pIOleObject_Graphic -> QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void **>(&pIGPropertiesClient_Graphic));

   VARIANT_BOOL wasSuccessful;

   pIGProperties -> LoadFile(&wasSuccessful);

   if ( wasSuccessful ) {
      pIGProperty_Graphic -> clearStorageObjects();
      pIGProperty_Graphic -> addStorageObject(pIOleObject_Graphic);
      pIGProperty_Graphic -> readStorageObjects();
      pIGProperty_Graphic -> clearStorageObjects();
   }

   pIGraphic -> put_AllowUserSetFunctionVisibility(VARIANT_TRUE);

   pIOleObject_Graphic -> DoVerb(OLEIVERB_SHOW,NULL,pIOleClientSite,0L,hwndSite,&rect);

   return;
   }


   GraphicHost::~GraphicHost() {

   pIGPropertiesClient_Graphic -> SavePrep();

   pIGProperty_Graphic -> clearStorageObjects();
   pIGProperty_Graphic -> addStorageObject(pIOleObject_Graphic);
   pIGProperty_Graphic -> writeStorageObjects();
   pIGProperty_Graphic -> clearStorageObjects();

   pIGProperties -> Save();

   pIGraphic -> Release();

   pIOleObject_Graphic -> Release();

   return;
   }


   long __stdcall GraphicHost::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      *ppv = static_cast<IUnknown*>(this);
   else

   if ( IID_IOleClientSite == riid ) 
      *ppv = reinterpret_cast<void*>(pIOleClientSite);
   else

   if ( IID_IOleInPlaceSite == riid ) 
      *ppv = reinterpret_cast<void*>(pIOleInPlaceSite);
   else

      return E_NOINTERFACE;

   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();
   return S_OK;
   }

   unsigned long __stdcall GraphicHost::AddRef() {
   return 1;
   }
   unsigned long __stdcall GraphicHost::Release() {
   return 1;
   }

