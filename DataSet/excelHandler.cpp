
#include <windows.h>
#include <olectl.h>
#include <CommCtrl.h>

#include "General.h"
#include "Graphic_resource.h"

#include "DataSet.h"

   LRESULT EXPENTRY DataSet::dataSetExcelHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   
   DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (DataSet *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndExcelSettings = hwnd;

      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),FALSE);
      //EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET),FALSE);

      //if ( p -> szSpreadsheetName[0] ) {
      //   SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET,p -> szSpreadsheetName);
      //   char szLabel[128];
      //   sprintf(szLabel,"Named ranges in %s:",p -> szSpreadsheetName);
      //   SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGES_LABEL,szLabel);
      //   p -> loadExcelWorksheet();
      //}

      RECT rcDialog;
      RECT rcRangeList;

      GetWindowRect(hwnd,&rcDialog);

      GetWindowRect(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST),&rcRangeList);

      SetWindowPos(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),HWND_TOP,16,rcRangeList.bottom - rcDialog.top + 16,rcDialog.right - rcDialog.left - 32,rcDialog.bottom - rcDialog.top - (rcRangeList.bottom - rcDialog.top + 16) - 16,0L);

      if ( p -> szDataSource[0] )
         p -> loadExcelWorkbook();

      if ( p -> szNamedRange[0] )
         p -> loadExcelNamedRange();

      }
      break;

   case WM_SHOWWINDOW:
   case WM_ACTIVATE:
      SetWindowText(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),p -> szDataSource);
      //SetWindowText(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET),p -> szSpreadsheetName);
      break;

   case WM_COMMAND: {

      long notifyCode = HIWORD(wParam);

      switch ( LOWORD(wParam) ) {

      //case IDDI_DATASET_EXCEL_SPREADSHEET_LIST: {
      //   if ( CBN_SELCHANGE == notifyCode ) {
      //      char szSelectedSheet[64];
      //      SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCURSEL,0L,0L),(LPARAM)szSelectedSheet);
      //      if ( strcmp(szSelectedSheet,p -> szSpreadsheetName) ) {
      //         strcpy(p -> szSpreadsheetName,szSelectedSheet);
      //         SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET,p -> szSpreadsheetName);
      //         char szLabel[128];
      //         sprintf(szLabel,"Named ranges in %s:",p -> szSpreadsheetName);
      //         SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGES_LABEL,szLabel);
      //         p -> loadExcelWorksheet();
      //      }
      //   }
      //   }
      //   break;

      case IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST: {
         if ( CBN_SELCHANGE == notifyCode ) {
            char szSelectedRange[64];
            SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCURSEL,0L,0L),(LPARAM)szSelectedRange);
            if ( strcmp(szSelectedRange,p -> szNamedRange) ) {
               strcpy(p -> szNamedRange,szSelectedRange);
               p -> loadExcelNamedRange();
               //SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET,p -> szSpreadsheetName);
               //char szLabel[128];
               //sprintf(szLabel,"Named ranges in %s:",p -> szSpreadsheetName);
               //SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGES_LABEL,szLabel);
               //p -> loadExcelWorksheet();
            }
         }
         }
         break;

      case IDDI_DATASET_EXCEL_OPEN_WORKBOOK: {
         p -> launchExcel();
         }
         break;

      default:
         break;

      }
      }
      break;

   case WM_DESTROY:
      p -> hwndExcelSettings = NULL;
      break;

   default:
      break;

   }

   return (LRESULT)0;
   }