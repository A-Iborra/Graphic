/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <ole2.h>
#include <olectl.h>

#include "Evaluator.h"

   Evaluator::_IEnumerateConnections::_IEnumerateConnections(
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
 
   for ( UINT i = 0; i < countConnections; i++ )
      connections[i] = paConnections[i];
 
   return;
   }
 
 
 
   Evaluator::_IEnumerateConnections::~_IEnumerateConnections() {
   delete [] connections;
   return;
   }
 
 
   STDMETHODIMP Evaluator::_IEnumerateConnections::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL;
   if ( IID_IUnknown != riid && IID_IEnumConnections != riid) return E_NOINTERFACE;
   *ppv = (LPVOID)this;
   AddRef();
   return S_OK;
   }
 
 
   STDMETHODIMP_(ULONG) Evaluator::_IEnumerateConnections::AddRef() {
   pParentUnknown -> AddRef();
   return ++refCount;
   }
 
 
 
   STDMETHODIMP_(ULONG) Evaluator::_IEnumerateConnections::Release() {
   pParentUnknown -> Release();
   if ( 0 == --refCount ) {
     refCount++;
     delete this;
     return 0;
   }
   return refCount;
   }
 
 
 
   STDMETHODIMP Evaluator::_IEnumerateConnections::Next(ULONG countRequested,CONNECTDATA* pConnections,ULONG* countEnumerated) {
   ULONG countReturned;

   if ( NULL == pConnections ) return E_POINTER;

   for ( countReturned = 0; enumeratorIndex < countConnections && countRequested > 0; pConnections++, enumeratorIndex++, countReturned++, countRequested-- ) {
      *pConnections = connections[enumeratorIndex];
   }

   if ( NULL != countEnumerated )
      *countEnumerated = countReturned;

   return countReturned > 0 ? S_OK : S_FALSE;
   }

 
 
   STDMETHODIMP Evaluator::_IEnumerateConnections::Skip(ULONG cSkip) {
   if ( (enumeratorIndex + cSkip) < countConnections ) return S_FALSE;
   enumeratorIndex += cSkip;
   return S_OK;
   }
 
 
 
   STDMETHODIMP Evaluator::_IEnumerateConnections::Reset() {
   enumeratorIndex = 0;
   return S_OK;
   }
 
 
 
   STDMETHODIMP Evaluator::_IEnumerateConnections::Clone(IEnumConnections** ppIEnum) {
   _IEnumerateConnections* p = new _IEnumerateConnections(pParentUnknown,countConnections,connections,enumeratorIndex);
   return p -> QueryInterface(IID_IEnumConnections,(void **)ppIEnum);
   }
