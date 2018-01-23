
#include "ObjectInOffice.h"

#include "utils.h"

   STDMETHODIMP ObjectInOffice::GetClassID(CLSID *pcid) {
   *pcid = CLSID_ObjectInOffice;
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::IsDirty() {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::InitNew(IStorage *is) {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::Load(IStorage *is) {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::Save(IStorage *is,BOOL sameAsLoad) {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::SaveCompleted(IStorage*) {
   return S_OK;
   }
 
 
   STDMETHODIMP ObjectInOffice::HandsOffStorage() {
   return S_OK;
   }