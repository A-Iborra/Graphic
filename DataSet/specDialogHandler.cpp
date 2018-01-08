/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include "DataSet.h"
#include "Graphic_resource.h"

   static bool ignoreTextSetting = false;

   LRESULT EXPENTRY DataSet::dataSetDialogHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      p = (DataSet *)lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      SetDlgItemText(hwnd,IDDI_DATASET_SPEC_NAME,p -> szName);

      char *psz = strrchr(p -> szDataSource,'\\');
      if ( ! psz )
         psz = strrchr(p -> szDataSource,'/');
      if ( ! psz )
         psz = p -> szDataSource - 1;

      SetDlgItemText(hwnd,IDDI_DATASET_SPEC_DATASOURCE,psz + 1);

      p -> nativeEditHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_DATASET_SPEC_DATASOURCE),GWLP_WNDPROC,(ULONG_PTR)DataSet::overrideEditHandler);

      }
      return LRESULT(1);

   case WM_COMMAND: {
      if ( ! p ) break; 
      WORD controlID = LOWORD(wParam);
      WORD notifyCode = HIWORD(wParam);
      switch ( controlID ) {

      case IDDI_DATASET_SPEC_NAME:
         GetDlgItemText(hwnd,IDDI_DATASET_SPEC_NAME,p -> szName,64);
         break;

      case IDDI_DATASET_SPEC_DATASOURCE: {
         if ( ignoreTextSetting )
            break;
         if ( ! ( EN_CHANGE == notifyCode ) )
            break;
         char szTemp[MAX_PATH];
         GetWindowText((HWND)lParam,szTemp,MAX_PATH);
         char *psz = strrchr(szTemp,'\\');
         if ( ! psz )
            psz = strrchr(szTemp,'/');
         if ( ! psz )
            psz = szTemp - 1;
         ignoreTextSetting = true;
         SetWindowText((HWND)lParam,psz + 1);
         ignoreTextSetting = false;
         }
         break;

      case IDDI_DATASET_SPEC_PROPERTIES: {
         IUnknown* pIUnknown;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIGProperties -> ShowProperties(hwnd,pIUnknown);
         pIUnknown -> Release();
         SetDlgItemText(hwnd,IDDI_DATASET_SPEC_NAME,p -> szName);
         char *psz = strrchr(p -> szDataSource,'\\');
         if ( ! psz )
            psz = strrchr(p -> szDataSource,'/');
         if ( ! psz )
         psz = p -> szDataSource - 1;
         SetDlgItemText(hwnd,IDDI_DATASET_SPEC_DATASOURCE,psz + 1);
         if ( p -> pWhenChangedCallback )
            p -> pWhenChangedCallback(p -> pWhenChangedCallbackArg);
         }
         break;

      case IDDI_DATASET_SPEC_PLOT:
         p -> Start();
         return LRESULT(TRUE);
 #if 0
      case IDDI_DATASET_SPEC_PAUSE:
         //p -> Pause();
         return LRESULT(TRUE);
 
      case IDDI_DATASET_SPEC_RESUME:
         //p -> Resume();
         return LRESULT(TRUE);
 
      case IDDI_DATASET_SPEC_STOP:
         //p -> fire_Stopped();
         //p -> Stop();
         return LRESULT(TRUE);
#endif

      }
      }
      break;

   default:
      break;
   }

   return 0;
   }
