// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Function.h"

   Function::_IEnumerateConnections::_IEnumerateConnections(
             IUnknown* pHostObj,
             ULONG cConnections,
             CONNECTDATA* paConnections,
             ULONG initialIndex) :
     refCount(0), 
     pParentUnknown(pHostObj),
     enumeratorIndex(initialIndex),
     countConnections(cConnections)
   {
 
   connections = new CONNECTDATA[countConnections];
   memset(connections,0,countConnections * sizeof(CONNECTDATA));
   for ( UINT i = 0; i < countConnections; i++ ) {
      connections[i] = paConnections[i]; 
//      connections[i].pUnk -> AddRef();
   }
 
   return;
   }
 
 
 
   Function::_IEnumerateConnections::~_IEnumerateConnections() {
   for ( UINT i = 0; i < countConnections; i++ ) 
;//      if ( connections[i].pUnk ) connections[i].pUnk -> Release();
   delete [] connections;
   return;
   }
 
 
   STDMETHODIMP Function::_IEnumerateConnections::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL;
   if ( IID_IUnknown != riid && IID_IEnumConnections != riid) return E_NOINTERFACE;
   *ppv = (LPVOID)this;
   AddRef();
   return S_OK;
   }
 
 
   STDMETHODIMP_(ULONG) Function::_IEnumerateConnections::AddRef() {
   pParentUnknown -> AddRef();
   return ++refCount;
   }
 
 
 
   STDMETHODIMP_(ULONG) Function::_IEnumerateConnections::Release() {
   pParentUnknown -> Release();
   if ( 0 == --refCount ) {
     refCount++;
     delete this;
     return 0;
   }
   return refCount;
   }
 

 
   STDMETHODIMP Function::_IEnumerateConnections::Next(ULONG countRequested,CONNECTDATA* pConnections,ULONG* countEnumerated) {
   ULONG countReturned;

   if ( NULL == pConnections ) return E_POINTER;

   for ( countReturned = 0; enumeratorIndex < countConnections && countRequested > 0; pConnections++, enumeratorIndex++, countReturned++, countRequested-- ) {
      *pConnections = connections[enumeratorIndex];
   }

   if ( NULL != countEnumerated )
      *countEnumerated = countReturned;

   return countReturned > 0 ? S_OK : S_FALSE;
   }


   STDMETHODIMP Function::_IEnumerateConnections::Skip(ULONG cSkip) {
   if ( (enumeratorIndex + cSkip) < countConnections ) return S_FALSE;
   enumeratorIndex += cSkip;
   return S_OK;
   }
 
 
 
   STDMETHODIMP Function::_IEnumerateConnections::Reset() {
   enumeratorIndex = 0;
   return S_OK;
   }
 
 
 
   STDMETHODIMP Function::_IEnumerateConnections::Clone(IEnumConnections** ppIEnum) {
   _IEnumerateConnections* p = new _IEnumerateConnections(pParentUnknown,countConnections,connections,enumeratorIndex);
   return p -> QueryInterface(IID_IEnumConnections,(void **)ppIEnum);
   }
