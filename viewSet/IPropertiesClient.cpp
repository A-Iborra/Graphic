/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "utils.h"

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
