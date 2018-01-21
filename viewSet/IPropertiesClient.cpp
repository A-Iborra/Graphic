// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ViewSet.h"

   HRESULT ViewSet::SavePrep() {
   return S_OK;
   }


   HRESULT ViewSet::InitNew() {
   return Loaded();
   }
 
 
   HRESULT ViewSet::Loaded() {
//   SetWindowPos(HWND_TOP,hwndViewSet,ptlFrame.x,ptlFrame.y,sizelFrame.cx,sizelFrame.cy,0);
   return S_OK;
   }
 
 
   HRESULT ViewSet::Saved() {
   return S_OK;
   }


   HRESULT ViewSet::IsDirty() {
   return S_FALSE;
   }


   HRESULT ViewSet::GetClassID(BYTE *pCLSID) {
   *(CLSID*)pCLSID = CLSID_ViewSet;
   return S_OK;
   }
