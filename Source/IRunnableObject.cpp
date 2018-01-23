// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "GraphicControl_i.h"

   static int isObjectRunning = 0;

   STDMETHODIMP G::GetRunningClass(CLSID *pcid) {
   *pcid = CLSID_GSystemGraphic;
   return S_OK;
   }

   STDMETHODIMP G::Run(LPBC) { 
   isObjectRunning = 1;
   return S_OK; 
   }

   int __stdcall G::IsRunning() { 
   return isObjectRunning; 
   }

   STDMETHODIMP G::LockRunning(BOOL,BOOL) { 
   return S_OK; 
   }

   STDMETHODIMP G::SetContainedObject(BOOL) { 
   return S_OK; 
   }

