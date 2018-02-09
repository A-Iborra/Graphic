// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ViewSet.h"

#include <olectl.h>

   ViewSet::_IConnectionPoint::_IConnectionPoint(ViewSet *pp,REFIID outgoingIFace) : 
      pParent(pp), 
      adviseSink(0),
      nextCookie(400),
      refCount(1),
      outGoingInterfaceType(outgoingIFace),
      countLiveConnections(0),
      countConnections(ALLOC_CONNECTIONS)
   { 
   connections = new CONNECTDATA[countConnections];
   memset(connections, 0, countConnections * sizeof(CONNECTDATA));
   return;
   };
 
 
   ViewSet::_IConnectionPoint::~_IConnectionPoint() {
   delete [] connections;
   countConnections = 0;
   return;
   }
 
 
   HRESULT ViewSet::_IConnectionPoint::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
  
    if ( riid == IID_IUnknown )
       *ppv = static_cast<void*>(this); 
    else
 
       if ( riid == IID_IConnectionPoint ) 
          *ppv = static_cast<IConnectionPoint *>(this);
       else
 
          return pParent -> QueryInterface(riid,ppv);
    
   AddRef();
   return S_OK;
   }
 

   STDMETHODIMP_(ULONG) ViewSet::_IConnectionPoint::AddRef() {
   return ++refCount;
   }
 

   STDMETHODIMP_(ULONG) ViewSet::_IConnectionPoint::Release() {
   if ( 0 == --refCount ) {
      delete this;
      return 0;
   }
   return refCount;
   }
 
 
   STDMETHODIMP ViewSet::_IConnectionPoint::GetConnectionInterface(IID *pIID) {
   if ( ! pIID ) return E_POINTER;
   *pIID = outGoingInterfaceType;
   return S_OK;
   }
 
 
   STDMETHODIMP ViewSet::_IConnectionPoint::GetConnectionPointContainer(IConnectionPointContainer **ppCPC) {
   return pParent -> QueryInterface(IID_IConnectionPointContainer,(void **)ppCPC);
   }
 
 
   STDMETHODIMP ViewSet::_IConnectionPoint::Advise(IUnknown *pUnkSink,DWORD *pdwCookie) {

   IGSGraphicEvents * pIEvents = 0;
 
   *pdwCookie = 0;

   HRESULT hr = pUnkSink -> QueryInterface(outGoingInterfaceType,(void **)&pIEvents);
 
   if ( hr == E_NOINTERFACE ) 
      return CONNECT_E_NOCONNECTION;

   if ( ! SUCCEEDED(hr) ) 
      return hr; 

   if ( ! pIEvents )
      return CONNECT_E_CANNOTCONNECT;
 
   int freeSlot = getSlot();
 
   connections[freeSlot].pUnk = reinterpret_cast<IUnknown *>(pIEvents);

   connections[freeSlot].dwCookie = nextCookie;
 
   *pdwCookie = nextCookie++;
 
   countLiveConnections++;
 
   return S_OK;
   }
 
 
   STDMETHODIMP ViewSet::_IConnectionPoint::Unadvise(DWORD dwCookie) {
 
   if ( 0 == dwCookie ) return CONNECT_E_NOCONNECTION;
 
   int slot = findSlot(dwCookie);
 
   if ( slot == -1 ) return CONNECT_E_NOCONNECTION;
 
   if ( connections[slot].pUnk ) connections[slot].pUnk -> Release();
 
   connections[slot].dwCookie = 0;
   connections[slot].pUnk = NULL;
 
   countLiveConnections--;
 
   return S_OK;
   }
 
   STDMETHODIMP ViewSet::_IConnectionPoint::EnumConnections(IEnumConnections **ppEnum) {
   CONNECTDATA *tempConnections;
   int i,j;
 
   *ppEnum = NULL;
 
   if ( countLiveConnections == 0 ) return OLE_E_NOCONNECTION;
 
   tempConnections = new CONNECTDATA[countLiveConnections];
 
   for ( i = 0, j = 0; i < countConnections && j < countLiveConnections; i++) {
     if ( 0 != connections[i].dwCookie ) {
       tempConnections[j].pUnk = (IUnknown *)connections[i].pUnk;
       tempConnections[j].dwCookie = connections[i].dwCookie;
       j++;
     }
   }
 
   _IEnumerateConnections *p = new _IEnumerateConnections(this,countLiveConnections,tempConnections,0);
   p -> QueryInterface(IID_IEnumConnections,(void **)ppEnum);
 
   delete [] tempConnections;
 
   return S_OK;
   }
 
 
   int ViewSet::_IConnectionPoint::getSlot() {
   CONNECTDATA* moreConnections;
   int i;
   i = findSlot(0);
   if ( i > -1 ) return i;
   moreConnections = new CONNECTDATA[countConnections + ALLOC_CONNECTIONS];
   memset( moreConnections, 0, sizeof(CONNECTDATA) * (countConnections + ALLOC_CONNECTIONS));
   memcpy( moreConnections, connections, sizeof(CONNECTDATA) * countConnections);
   delete [] connections;
   connections = moreConnections;
   countConnections += ALLOC_CONNECTIONS;
   return countConnections - ALLOC_CONNECTIONS;
   }
 
 
   int ViewSet::_IConnectionPoint::findSlot(DWORD dwCookie) {
   for ( int i = 0; i < countConnections; i++ )
      if ( dwCookie == connections[i].dwCookie ) return i;
   return -1;
   }
