
#include "PlotTypes.h"

#include "resource.h"

   static char szTemp[128];

   LRESULT EXPENTRY PlotTypes::blocksPropertiesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   PlotTypes *p = (PlotTypes *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   blocksProperties *pBlocksProperties = NULL;   
   if ( p )
      pBlocksProperties = (blocksProperties *)p -> pActiveProperties -> pvAdditionalProperties;
 
   switch (msg) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;
      p = (PlotTypes *)pPage -> lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      if ( NULL == p -> pActiveProperties -> pvAdditionalProperties )
         p -> pActiveProperties -> pvAdditionalProperties = (void *)new blocksProperties();

      pBlocksProperties = (blocksProperties *)p -> pActiveProperties -> pvAdditionalProperties;

      p -> pActiveProperties -> additionalpropertiesSize = sizeof(blocksProperties);

      sprintf_s(szTemp,128,"%3.1f",pBlocksProperties -> sizeInPercentDomain[0]);
      SetDlgItemText(hwnd,IDDI_BLOCKS_SIZE_PERCENT_X_DOMAIN,szTemp);
      SendDlgItemMessage(hwnd,IDDI_BLOCKS_SIZE_PERCENT_X_DOMAIN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));

      sprintf_s(szTemp,128,"%3.1f",pBlocksProperties -> sizeInPercentDomain[1]);
      SetDlgItemText(hwnd,IDDI_BLOCKS_SIZE_PERCENT_Y_DOMAIN,szTemp);
      SendDlgItemMessage(hwnd,IDDI_BLOCKS_SIZE_PERCENT_Y_DOMAIN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));

      sprintf_s(szTemp,128,"%3.1f",pBlocksProperties -> sizeInPercentDomain[2]);
      SetDlgItemText(hwnd,IDDI_BLOCKS_SIZE_PERCENT_Z_DOMAIN,szTemp);
      SendDlgItemMessage(hwnd,IDDI_BLOCKS_SIZE_PERCENT_Z_DOMAIN_SPIN,UDM_SETRANGE,0,MAKELONG(100,0));

      SendDlgItemMessage(hwnd,IDDI_BLOCKS_KEEP_CUBIC,BM_SETCHECK,(WPARAM)pBlocksProperties -> keepCubic ? BST_CHECKED : BST_UNCHECKED,0L);

      EnableWindow(GetDlgItem(hwnd,IDDI_BLOCKS_SIZE_PERCENT_Y_DOMAIN),pBlocksProperties -> keepCubic ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_BLOCKS_SIZE_PERCENT_Z_DOMAIN),pBlocksProperties -> keepCubic ? FALSE : TRUE);

      }
      return LRESULT(FALSE);

   case WM_SHOWWINDOW: {

      if ( ! p )
         break;

      if ( ! pBlocksProperties )
         break;

      EnableWindow(GetDlgItem(hwnd,IDDI_BLOCKS_SIZE_PERCENT_Y_DOMAIN),pBlocksProperties -> keepCubic ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwnd,IDDI_BLOCKS_SIZE_PERCENT_Z_DOMAIN),pBlocksProperties -> keepCubic ? FALSE : TRUE);
      }

      break;

   case WM_COMMAND: {

      if ( ! p )
         break;

      GetWindowText((HWND)lParam,szTemp,128);

      long controlId = LOWORD(wParam);

      switch ( controlId ) {

      case IDDI_BLOCKS_KEEP_CUBIC: {
         pBlocksProperties -> keepCubic = (BST_CHECKED == SendDlgItemMessage(hwnd,IDDI_BLOCKS_KEEP_CUBIC,BM_GETCHECK,0L,0L));
         }
         break;

      case IDDI_BLOCKS_SIZE_PERCENT_X_DOMAIN: {
         pBlocksProperties -> sizeInPercentDomain[0] = atof(szTemp);
         }
         break;

      case IDDI_BLOCKS_SIZE_PERCENT_Y_DOMAIN: {
         pBlocksProperties -> sizeInPercentDomain[1] = atof(szTemp);
         }
         break;

      case IDDI_BLOCKS_SIZE_PERCENT_Z_DOMAIN: {
         pBlocksProperties -> sizeInPercentDomain[2] = atof(szTemp);
         }
         break;

      default:
         break;

      }

      blocksPropertiesHandler(hwnd,WM_SHOWWINDOW,(WPARAM)1L,0L);

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