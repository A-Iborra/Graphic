
#include "Graphic.h"
#include <CommCtrl.h>
#include <stdio.h>

   HRESULT G::put_StatusText(long paneIndex,char* pText) { 

   propertyStatusText -> put_szValue(pText);

   char *pszText = new char[strlen(pText) + 2];
   memset(pszText,0,strlen(pText) + 2);
   pszText[0] = ' ';
   strcpy(pszText + 1,pText);

   RECT rectText,rectStatusBar;

   GetWindowRect(hwndStatusBar,&rectStatusBar);

   HDC hdc = GetWindowDC(hwndStatusBar);
   DrawText(hdc,pText,-1,&rectText,DT_CALCRECT);
   ReleaseDC(hwndStatusBar,hdc);

   statusTextWidth[paneIndex] = rectText.right - rectText.left;

   if ( 1 == paneIndex ) {
      int sbWidths[2] = {rectStatusBar.right - rectStatusBar.left - statusTextWidth[paneIndex],-1};
      SendMessage(hwndStatusBar,SB_SETPARTS,(WPARAM)2,(LPARAM)sbWidths);
   }

   PostMessage(hwndStatusBar,SB_SETTEXT,(WPARAM)paneIndex,(LPARAM)pszText);

   return S_OK; 
   } 


   HRESULT G::get_StatusText(long paneIndex,char** ppText) { 
   SendMessage(hwndStatusBar,SB_GETTEXT,(WPARAM)paneIndex,(LPARAM)*ppText);
   return S_OK; 
   } 
