// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ObjectInOffice.h"

   static int isRunning = 0;

   STDMETHODIMP ObjectInOffice::GetRunningClass(CLSID *pcid) {
   *pcid = CLSID_ObjectInOffice;
   return E_UNEXPECTED;
   }

   STDMETHODIMP ObjectInOffice::Run(LPBC) { 
   isRunning = 1;
   return S_OK; 
   }

   int __stdcall ObjectInOffice::IsRunning() { 
   return isRunning; 
   }

   STDMETHODIMP ObjectInOffice::LockRunning(BOOL,BOOL) { 
   return S_OK; 
   }

   STDMETHODIMP ObjectInOffice::SetContainedObject(BOOL) { 
   return S_OK; 
   }

