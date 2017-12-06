/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>
#include <math.h>

#include <commctrl.h>

#include <gl\gl.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Graphic.h"
#include "list.cpp"
 
   LRESULT CALLBACK G::textHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   G *p = (G *)GetWindowLong(hwnd,GWL_USERDATA);
   switch ( msg ) {
   case WM_INITDIALOG: {
      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);
      p = (G *)pPage -> lParam;
      SetWindowLong(hwnd,GWL_USERDATA,(long)p);
      }
      return LRESULT(FALSE);
 
   default:
      break;
   }
   return LRESULT(FALSE);
   }

   LRESULT CALLBACK G::patchPainterProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {
   case WM_PAINT: {
      PAINTSTRUCT ps;
      HBRUSH hb;
      HDC hdc;
      IGProperty *pp = NULL;
      int k = SendMessage(GetDlgItem(p -> hwndLightingSettings,IDDI_CHOOSE_LIGHT_NO),SBM_GETPOS,0,0);
      while ( TRUE ) {
         if ( hwnd == GetDlgItem(p -> hwndLightingSettings,IDDI_LIGHT_AMBIENT_BACKGROUND) ) {
            pp = p -> ppPropertyAmbientLight[k];
            break;
         }
         if ( hwnd == GetDlgItem(p -> hwndLightingSettings,IDDI_LIGHT_DIFFUSE_BACKGROUND) ) {
            pp = p -> ppPropertyDiffuseLight[k];
            break;
         }
         if ( hwnd == GetDlgItem(p -> hwndLightingSettings,IDDI_LIGHT_SPECULAR_BACKGROUND) ) {
            pp = p -> ppPropertySpecularLight[k]; 
            break;
         }
         if ( hwnd == GetDlgItem(p -> hwndBackgroundSettings,IDDI_LIGHT_BACKGROUND_BACKGROUND) ) {
            pp = p -> propertyBackgroundColor;
            break;
         }
         return LRESULT(FALSE);
      }
      float fv[4];
      BYTE *pb = (BYTE *)fv;
      pp -> get_binaryValue(sizeof(fv),(BYTE**)&pb);
      BYTE vb[3];
      COLORREF cr;
      vb[0] = (BYTE)(255.0f*fv[0]);
      vb[1] = (BYTE)(255.0f*fv[1]);
      vb[2] = (BYTE)(255.0f*fv[2]);
 
      cr = RGB(vb[0],vb[1],vb[2]);
      hb = CreateSolidBrush(cr);
 
      hdc = BeginPaint(hwnd,&ps);
      FillRect(hdc,&ps.rcPaint,hb);

      EndPaint(hwnd,&ps);
 
      }
      return LRESULT(FALSE);
 
   default:
      break;
   }

   return CallWindowProc(G::defaultPatchPainter,hwnd,msg,wParam,lParam);
   }

