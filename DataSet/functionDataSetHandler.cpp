
#include <windows.h>
#include <olectl.h>
#include <CommCtrl.h>

#include "General.h"
#include "Graphic_resource.h"

#include "DataSet.h"

   LRESULT EXPENTRY DataSet::dataSetFunctionDataHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   
   DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (DataSet *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndFunctionDataSettings = hwnd;

      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),FALSE);

      SetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE,p -> szDataSource);

      RECT rcDialog;
      RECT rcRangeList;

      GetWindowRect(hwnd,&rcDialog);

      GetWindowRect(GetDlgItem(hwnd,IDDI_DATASET_FUNCTION_RUN),&rcRangeList);

      SetWindowPos(GetDlgItem(hwnd,IDDI_DATASET_FUNCTION_DATA),HWND_TOP,16,rcRangeList.bottom - rcDialog.top + 16,rcDialog.right - rcDialog.left - 32,rcDialog.bottom - rcDialog.top - (rcRangeList.bottom - rcDialog.top + 16) - 16,0L);

      }
      break;

   case WM_SHOWWINDOW: {
      if ( ! wParam )
         break;
      p -> loadDataSet(GetDlgItem(hwnd,IDDI_DATASET_FUNCTION_DATA));
      }
      break;

   case WM_COMMAND: {

      long notifyCode = HIWORD(wParam);

      switch ( LOWORD(wParam) ) {

      case IDDI_DATASET_FUNCTION_RUN: {

         if ( p -> pIFunction ) {

            std::function<void()> *pOnFinished = new std::function<void()>([p,hwnd]() {
                     p -> loadDataSet(GetDlgItem(hwnd,IDDI_DATASET_FUNCTION_DATA)); 
                     delete p -> pIGSFunctioNaterEvents; 
                     p -> pIGSFunctioNaterEvents = NULL; });

            p -> pIGSFunctioNaterEvents = new _IGSFunctioNaterEvents(p,p -> pIFunction,pOnFinished);
            p -> pIFunction -> Start();
         }
         }
         break;

      default:
         break;

      }
      }
      break;

   case WM_DESTROY:
      p -> hwndFunctionDataSettings = NULL;
      break;

   default:
      break;

   }

   return (LRESULT)0;
   }