// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Plot.h"
#include "Graphic_resource.h"

 
   HRESULT Plot::LeftMouse() {
   return S_OK;  
   }
 
 
   HRESULT Plot::RightMouse() {
   return S_OK;
   }
 
 
   HRESULT Plot::MenuRequest() {

   POINT ptlMouse;

   GetCursorPos(&ptlMouse);

   if ( ! pIPlotNotify )
      EnableMenuItem(hMenu,IDMI_PLOT_DELETE,MF_BYCOMMAND | MF_GRAYED);
   else
      EnableMenuItem(hMenu,IDMI_PLOT_DELETE,MF_BYCOMMAND | MF_ENABLED);

   if ( hwndObjectWindow )
      DestroyWindow(hwndObjectWindow);

   hwndObjectWindow = CreateWindowEx(0,"p-handler","",0,0,0,0,0,HWND_DESKTOP,NULL,hModule,(void *)this);
 
   TrackPopupMenuEx(hMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON,ptlMouse.x,ptlMouse.y,hwndObjectWindow,NULL);

   return 1;
   }
 
 
   HRESULT Plot::Unselector() {
   return S_OK;
   }
 
 
   HRESULT Plot::Selector() {
   selected = ! selected;
   char szText[MAX_PROPERTY_SIZE];
   BSTR bstrName;
   propertyName -> get_stringValue(&bstrName);
   if ( bstrName && 0 < (DWORD)wcslen(bstrName) ) {
      WideCharToMultiByte(CP_ACP,0,bstrName,-1,szText,MAX_PROPERTY_SIZE,0,0);
      SysFreeString(bstrName);
   } else
      sprintf(szText,"Plot # %d",pID);
   if ( pIGSystemStatusBar )
      pIGSystemStatusBar -> put_StatusText(0,szText);
   return S_OK;
   }
 
 
   HRESULT Plot::MouseMove(POINT* ptMouse) {
   return S_OK;
   }


   HRESULT Plot::MouseRelease() {
   return S_OK;
   }

   HRESULT Plot::DefaultAction() {
   HWND hwndOwner = NULL;
   POINT ptCursor = {0};
   GetCursorPos(&ptCursor);
   hwndOwner = WindowFromPoint(ptCursor);
   IUnknown* pIUnknown;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
   pIProperties -> ShowProperties(hwndOwner ? hwndOwner : GetForegroundWindow(),pIUnknown);
   pIUnknown -> Release();
   if ( pWhenChangedCallback ) 
      pWhenChangedCallback(pWhenChangedCallbackArg,whenChangedCallbackCookie);
   return S_OK;
   }