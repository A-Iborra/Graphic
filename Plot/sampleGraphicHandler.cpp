/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Plot.h"


   LRESULT CALLBACK Plot::sampleGraphicHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   Plot *p = reinterpret_cast<Plot *>(GetWindowLong(hwnd,GWL_USERDATA));
 
   switch ( msg ) {
 
   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      EndPaint(hwnd,&ps);
      if ( ! p ) return (LRESULT)0;
      LRESULT rc = p -> defaultStaticWindowHandler(hwnd,msg,wParam,lParam);
      p -> pIOpenGLImplementation -> SetTargetWindow(hwnd);
      p -> pIOpenGLImplementation -> SetUp(p -> pIDataSet,p -> overrideOwnerView ? p -> propertyPlotView : p -> pOwnerPropertyPlotView,
                                             p -> pOwnerPropertyTheta,p -> pOwnerPropertyPhi,p -> pOwnerPropertySpin);
      p -> pIOpenGLImplementation -> SetLighting(p -> pOwnerPropertiesLightOn,
                                                 p -> pOwnerPropertiesAmbientLight,
                                                 p -> pOwnerPropertiesDiffuseLight,
                                                 p -> pOwnerPropertiesSpecularLight,
                                                 p -> pOwnerPropertiesLightPosition,
                                                 p -> pOwnerPropertyCountLights,NULL);
      p -> pIOpenGLImplementation -> Erase(p -> pOwnerPropertyBackgroundColor);
      p -> Draw();
      p -> pIOpenGLImplementation -> ResetTargetWindow();
      return rc;
      }
 
 
   default:
      break;
   }
 
   return p -> defaultStaticWindowHandler(hwnd,msg,wParam,lParam);
   }