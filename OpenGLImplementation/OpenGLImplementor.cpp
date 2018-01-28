// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"

#include "Graphic_resource.h"

#include "utils.h"
#include "Properties_i.h"

   OpenGLImplementor::OpenGLImplementor(IUnknown *pUnkOuter) : 

        pIUnknown_Outer(pUnkOuter),

        plotWindowList(),

        pIOleClientSite(NULL),
        pIOleInPlaceSite(NULL),
        pIAdviseSink(NULL),
        pIOleAdviseHolder(NULL),
        pIGSystemStatusBar(NULL),

        pIUnknownProperties(NULL),
        pIProperties(NULL),
        pIPropertySize(NULL),

        adviseSink_dwAspect(0),
        adviseSink_advf(0),

        hwndControl(NULL),
        hwndProperties(NULL),
        hwndBase(NULL),
        hwndParent(NULL),

        hwndObjectWindow(NULL),
        hCreationSemaphore(NULL),
        hDestructionSemaphore(NULL),
//        hBlockingSemaphore(NULL),
        hBlockingMutex(NULL),

        implementationMsgThread(NULL),

        refCount(0),
        plotWindow(NULL)

   {

   IUnknown *pIUnknownThis;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));

   HRESULT rc = CoCreateInstance(CLSID_InnoVisioNateProperties,pIUnknownThis,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IUnknown,reinterpret_cast<void **>(&pIUnknownProperties));

   pIUnknownProperties -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&pIProperties));
   pIUnknownProperties -> Release();

   pIProperties -> Add(L"container size",&pIPropertySize);
   pIPropertySize -> put_type(TYPE_BINARY);
   pIPropertySize -> put_size(sizeof(SIZEL));

   pIProperties -> Add(L"use hardware acceleration",&pIPropertyAllowHardwareAcceleration);

   SIZEL sizeContainer = { 100,100 };
   pIPropertySize -> put_binaryValue(sizeof(SIZEL),reinterpret_cast<unsigned char*>(&sizeContainer));

   hCreationSemaphore = NULL;
   implementationMsgThread = NULL;

   refCount = 0;

   };
 

   OpenGLImplementor::~OpenGLImplementor() {

/*
   if ( implementationMsgThread ) {
      PostMessage(hwndObjectWindow,WM_QUIT,0L,0L);
      WaitForSingleObject(implementationMsgThread,INFINITE);
   }
*/

   if ( pIOleInPlaceSite ) pIOleInPlaceSite -> Release();

   if ( pIOleClientSite ) pIOleClientSite -> Release();

   pIOleInPlaceSite = NULL;
   pIOleClientSite = NULL;

   SetParent(hwndControl,NULL);
   DestroyWindow(hwndControl);
   hwndControl = NULL;

   pIProperties -> Release();

   transformationMatrixes *pm = NULL;
   while ( matrixList.size() ) {
      pm = matrixList.back();
      matrixList.remove(pm);
      delete pm;
   }

   PlotWindow *pw = NULL;
   while ( plotWindowList.size() ) {
      pw = plotWindowList.back();
      plotWindowList.remove(pw);
      delete pw;
   }

   return;
   }


   int OpenGLImplementor::initWindows() {

   hMenu = GetSubMenu(LoadMenu((HINSTANCE)hModule,MAKEINTRESOURCE(IDMENU_OPENGL)),0);

   SIZEL sizel = {100, 100};
   pIPropertySize -> get_binaryValue(sizeof(SIZEL),reinterpret_cast<unsigned char **>(&sizel));

   hwndControl = CreateWindowEx(0L,"Static","Open GLImplementor",WS_CHILD | WS_BORDER | WS_VISIBLE,0,0,sizel.cx,sizel.cy,hwndParent,NULL,hModule,NULL);

   SetWindowLongPtr(hwndControl,GWLP_USERDATA,(ULONG_PTR)this);

   oldHandler = (WNDPROC)SetWindowLongPtr(hwndControl,GWLP_WNDPROC,(ULONG_PTR)handler);

   DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_OPENGL_PROPERTIES),RT_DIALOG));

   hwndProperties = CreateDialogIndirectParam(hModule,dt,hwndParent,(DLGPROC)propertiesHandler,(ULONG_PTR)this);

   return TRUE;
   }


   LRESULT OpenGLImplementor::handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   OpenGLImplementor* p = reinterpret_cast<OpenGLImplementor*>(GetWindowLongPtr(hwnd,GWLP_USERDATA));
   switch ( msg ) {
   case WM_RBUTTONDOWN: {
      POINTS ptsMouse = MAKEPOINTS(wParam);
      TrackPopupMenu(p -> hMenu,0,ptsMouse.x,ptsMouse.y,0,hwnd,NULL);
errorSound();
      }
      return LRESULT(0);
      return LRESULT(0);
   }
   return CallWindowProc(p -> oldHandler,hwnd,msg,wParam,lParam);
   }


   LRESULT OpenGLImplementor::propertiesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   OpenGLImplementor* p = (OpenGLImplementor *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
   switch ( msg ) {
   case WM_INITDIALOG: {
      long setChecked;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)lParam);
      p = (OpenGLImplementor *)lParam;
      p -> pIPropertyAllowHardwareAcceleration -> get_longValue(&setChecked);
      SendMessage(GetDlgItem(hwnd,IDDI_OPENGL_ALLOWHARDWAREACCELERATION),BM_SETCHECK,setChecked,0L);
      }
      return LRESULT(TRUE);

   case WM_COMMAND: {
      int idControl = LOWORD(wParam);
      switch ( idControl ) {
      case IDDI_OPENGL_ALLOWHARDWAREACCELERATION: {
         long isChecked = (long)SendMessage(reinterpret_cast<HWND>(lParam),BM_GETCHECK,0L,0L);
         p -> pIPropertyAllowHardwareAcceleration -> put_longValue(isChecked);
      }
      default:
         break;
      }
      }
      return LRESULT(0);

   default:
      break;

   }
   return LRESULT(0);
   }