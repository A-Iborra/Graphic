
#define DEFINE_DATA

#include "ObjectInOffice.h"

   ObjectInOffice::ObjectInOffice(IUnknown *pUnkOuter) {
   return;
   }


   ObjectInOffice::~ObjectInOffice() {

   if ( pIOleClientSite )
      pIOleClientSite -> Release();

   if ( pIOleInPlaceSite )
      pIOleInPlaceSite -> Release();

   return;
   };


   void ObjectInOffice::initWindows() {

   WNDCLASS gClass;
 
   if ( ! hwndOwner )
      pIOleInPlaceSite -> GetWindow(&hwndOwner);

   if ( ! hwndOwner )
      return;

   memset(&gClass,0,sizeof(WNDCLASS));
   gClass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
   gClass.lpfnWndProc = handler;
   gClass.cbClsExtra = 128;
   gClass.cbWndExtra = 128;
   gClass.hInstance = hModule;
   gClass.hIcon = NULL;
   gClass.hCursor = NULL;
   gClass.hbrBackground = 0;
   gClass.lpszMenuName = NULL;
   gClass.lpszClassName = "objectInOffice";

   RegisterClass(&gClass);
  
   hwndMain = CreateWindowEx(0*WS_EX_CONTROLPARENT,"objectInOffice","",WS_CHILD,0,0,0,0,hwndOwner,NULL,hModule,(void *)this);

   return;
   }