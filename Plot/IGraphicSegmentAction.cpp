/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "Graphic_resource.h"

#include "Plot.h"

 
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

   TrackPopupMenu(hMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON,ptlMouse.x,ptlMouse.y,0,hwndObjectWindow,NULL);

   return 1;
   }
 
 
   HRESULT Plot::Unselector() {
   return S_OK;
   }
 
 
   HRESULT Plot::Selector() {
   selected = ! selected;
   char szText[MAX_PROPERTY_SIZE];
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