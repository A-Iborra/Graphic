
#include "PlotTypes.h"

#include "resource.h"

   static char szTemp[128];

   LRESULT EXPENTRY PlotTypes::blocksPropertiesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   PlotTypes *p = (PlotTypes *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {

   case WM_INITDIALOG: {
      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;
      p = (PlotTypes *)pPage -> lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      sprintf_s(szTemp,128,"%3.1f",p -> pActiveProperties -> defaultSolidSizeInPercentDomain);
      SetDlgItemText(hwnd,IDDI_BALLS_SIZE_PERCENT_DOMAIN,szTemp);
      SendDlgItemMessage(hwnd,IDDI_BALLS_SIZE_PERCENT_DOMAIN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));
      }
      return LRESULT(FALSE);

   case WM_COMMAND: {

      if ( ! p )
         break;

      GetWindowText((HWND)lParam,szTemp,128);

      long controlId = LOWORD(wParam);

      switch ( controlId ) {

      case IDDI_BALLS_SIZE_PERCENT_DOMAIN: {
         p -> pActiveProperties -> defaultSolidSizeInPercentDomain = atof(szTemp);
         }
         break;

      default:
         break;

      }
      }
      break;

   case WM_NOTIFY: { 

      //if ( holdUpdates )
      //   break;

      NM_UPDOWN *pn = (NM_UPDOWN *)lParam;

      if ( pn -> hdr.code != UDN_DELTAPOS ) 
         break;
   
      if ( 0 == pn -> iDelta )
         break;

      HWND hwndEdit = (HWND)SendMessage(pn -> hdr.hwndFrom,UDM_GETBUDDY,(WPARAM)0L,(LPARAM)0L);

      GetWindowText(hwndEdit,szTemp,32);

      float x = (float)atof(szTemp);

      x += (float)pn -> iDelta * 1.0f;

      sprintf(szTemp,"%3.1f",x);

      SetWindowText(hwndEdit,szTemp);

      }
      break;

   default:
      break;

   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }