// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Evaluator.h"
#include <olectl.h>


   Evaluator::_IConnectionPoint::_IConnectionPoint(Evaluator *pp) : 
      pParent(pp), 
      adviseSink(0),
      nextCookie(400),
      countLiveConnections(0),
      countConnections(ALLOC_CONNECTIONS)
   { 
   connections = new CONNECTDATA[countConnections];
   memset(connections, 0, countConnections * sizeof(CONNECTDATA));
   return;
   };
 
 
   Evaluator::_IConnectionPoint::~_IConnectionPoint() {
   for ( int k = 0; k < countConnections; k++ ) 
      if ( connections[k].pUnk ) connections[k].pUnk -> Release();
   delete [] connections;
   return;
   }
 
 
   HRESULT Evaluator::_IConnectionPoint::QueryInterface(REFIID riid,void **ppv) {
   return pParent -> QueryInterface(riid,ppv);
   }
 
   STDMETHODIMP_(ULONG) Evaluator::_IConnectionPoint::AddRef() {
   return pParent -> AddRef();
   }
 
   STDMETHODIMP_(ULONG) Evaluator::_IConnectionPoint::Release() {
   return pParent -> Release();
   }
 
 
   STDMETHODIMP Evaluator::_IConnectionPoint::GetConnectionInterface(IID *pIID) {
   if ( pIID == 0 ) return E_POINTER;
   *pIID = IID_IEvaluatorEvents;
   return S_OK;
   }
 
 
   STDMETHODIMP Evaluator::_IConnectionPoint::GetConnectionPointContainer(IConnectionPointContainer **ppCPC) {
   return pParent -> QueryInterface(IID_IConnectionPointContainer,(void **)ppCPC);
   }
 
 
   STDMETHODIMP Evaluator::_IConnectionPoint::Advise(IUnknown *pUnkSink,DWORD *pdwCookie) {
   HRESULT hr;
   IUnknown* pISink = 0;
 
   hr = pUnkSink -> QueryInterface(IID_IEvaluatorEvents,(void **)&pISink);
 
   if ( hr == E_NOINTERFACE ) return CONNECT_E_NOCONNECTION;
   if ( ! SUCCEEDED(hr) ) return hr; 
   if ( ! pISink ) return CONNECT_E_CANNOTCONNECT;
 
   int freeSlot;
   *pdwCookie = 0;
 
   freeSlot = getSlot();
 
//??????   pISink -> AddRef();
 
   connections[freeSlot].pUnk = pISink;
   connections[freeSlot].dwCookie = nextCookie;
 
   *pdwCookie = nextCookie++;
 
   countLiveConnections++;
 
   return S_OK;
   }
 
 
   STDMETHODIMP Evaluator::_IConnectionPoint::Unadvise(DWORD dwCookie) {
 
   if ( 0 == dwCookie ) return E_INVALIDARG;
 
   int slot = findSlot(dwCookie);
 
   if ( slot == -1 ) return CONNECT_E_NOCONNECTION;
 
   if ( connections[slot].pUnk ) connections[slot].pUnk -> Release();
 
   connections[slot].dwCookie = 0;
 
   countLiveConnections--;
 
   return S_OK;
   }
 
   STDMETHODIMP Evaluator::_IConnectionPoint::EnumConnections(IEnumConnections **ppEnum) {
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
 
 
   int Evaluator::_IConnectionPoint::getSlot() {
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
 
 
   int Evaluator::_IConnectionPoint::findSlot(DWORD dwCookie) {
   for ( int i = 0; i < countConnections; i++ )
      if ( dwCookie == connections[i].dwCookie ) return i;
   return -1;
   }