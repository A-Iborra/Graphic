/*

                       Copyright (c) 1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>
#include <CommCtrl.h>

#include "Function.h"


   HRESULT Function::QuickActivate(QACONTAINER* qaContainer,QACONTROL* qaControl) {
   DWORD dwDummy;

   SetClientSite(qaContainer -> pClientSite);

   Advise(qaContainer -> pAdviseSink,&dwDummy);

   if ( qaContainer -> pPropertyNotifySink ) {
      if ( pIPropertyNotifySink ) 
         pIPropertyNotifySink -> Release();
      qaContainer -> pPropertyNotifySink -> QueryInterface(IID_IPropertyNotifySink,reinterpret_cast<void**>(&pIPropertyNotifySink));
      iProperties -> ConnectPropertyNotifySink(pIPropertyNotifySink,&qaControl -> dwPropNotifyCookie);
   }

   if ( qaContainer -> pUnkEventSink )
      pIConnectionPoint -> Advise(qaContainer -> pUnkEventSink,&qaControl -> dwEventCookie);

   if ( qaContainer -> pAdviseSink )
      SetAdvise(DVASPECT_CONTENT,0,qaContainer -> pAdviseSink);

   qaControl -> cbSize = sizeof(QACONTROL);
   qaControl -> dwPointerActivationPolicy = 0xFFFF;

   GetViewStatus(&qaControl -> dwViewStatus);

   GetMiscStatus(DVASPECT_CONTENT, &qaControl -> dwMiscStatus);

   return S_OK;
   }


   HRESULT Function::SetContentExtent(SIZEL* pSizel) {
   return S_OK;
   }


   HRESULT Function::GetContentExtent(SIZEL* pSizel) {
   return S_OK;
   }