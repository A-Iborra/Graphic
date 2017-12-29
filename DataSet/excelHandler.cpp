
#include <windows.h>
#include <olectl.h>
#include <CommCtrl.h>

#include "General.h"
#include "Graphic_resource.h"

#include "DataSet.h"

#define ENABLE_LOAD \
EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_LOAD),( 0 < strlen(p -> szCellRange) && \
   -1 < SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCURSEL,0L,0L) && \
   0 < strlen(p -> szDataSource) ) ? TRUE : FALSE);

   LRESULT EXPENTRY DataSet::dataSetExcelHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   
   DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (DataSet *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndExcelSettings = hwnd;

      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),FALSE);

      RECT rcDialog;
      RECT rcRangeList;

      GetWindowRect(hwnd,&rcDialog);

      GetWindowRect(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_INPUT_BOX),&rcRangeList);

      SetWindowPos(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),HWND_TOP,16,rcRangeList.bottom - rcDialog.top + 16,rcDialog.right - rcDialog.left - 32,rcDialog.bottom - rcDialog.top - (rcRangeList.bottom - rcDialog.top + 16) - 16,0L);

      ENABLE_LOAD

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_LOAD_ERROR),GWLP_USERDATA,(ULONG_PTR)RGB(255,0,0));

      DataSet::nativeStaticHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_LOAD_ERROR),GWLP_WNDPROC,(ULONG_PTR)DataSet::statusAndErrorTextStaticHandler);

      }
      break;

   case WM_SHOWWINDOW: {

      if ( ! wParam )
         break;

      SetWindowText(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),p -> szDataSource);

      if ( ! p -> szDataSource[0] )
         break;

      ENABLE_LOAD

      SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY,p -> szCellRange);

      long countSheets = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCOUNT,0L,0L);

      long countRanges = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCOUNT,0L,0L);

      if ( 0 == countRanges || 0 == countSheets ) {
         p -> loadExcelWorkbook(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST),GetDlgItem(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST),p -> szDataSource,p -> szSpreadsheetName,p -> szNamedRange);
         countSheets = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCOUNT,0L,0L);
         countRanges = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCOUNT,0L,0L);
         long currentSelection = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCURSEL,0L,0L);
         if ( -1 < currentSelection ) {
            SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCURSEL,0L,0L),(LPARAM)p -> szNamedRange);
            p -> loadExcelNamedRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),p -> szDataSource,p -> szNamedRange);
         }
      }

      long currentSheet = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCURSEL,0L,0L);

      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGES_LIST_LABEL),countRanges ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST),countRanges ? TRUE : FALSE);

      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_SELECTSHEET_LABEL1),countSheets ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST),countSheets ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY),countSheets && -1 < currentSheet ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_LOAD),countSheets && - 1 < currentSheet ? TRUE : FALSE);

      long countData = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS,LVM_GETITEMCOUNT,0L,0L);

      if ( 0 == countData ) {
         if ( p -> szCellRange[0] && p -> szSpreadsheetName[0] && ! p -> szNamedRange[0] ) 
            p -> loadExcelCellRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),GetDlgItem(hwnd,IDDI_DATASET_EXCEL_LOAD_ERROR),p -> szDataSource, p -> szSpreadsheetName,p -> szCellRange);
         else 
            if ( p -> szNamedRange[0] )
               p -> loadExcelNamedRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),p -> szDataSource,p -> szNamedRange);
      }

      }
      break;

   case WM_COMMAND: {

      long notifyCode = HIWORD(wParam);

      switch ( LOWORD(wParam) ) {

      case IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST:
         if ( ! ( CBN_SELCHANGE == notifyCode ) )
            break;
         SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCURSEL,0L,0L),(LPARAM)p -> szNamedRange);
         p -> szCellRange[0] = '\0';
         SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY,p -> szCellRange);
         p -> loadExcelNamedRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),p -> szDataSource,p -> szNamedRange);
         break;

      case IDDI_DATASET_EXCEL_SPREADSHEET_LIST:
         if ( ! ( CBN_SELCHANGE == notifyCode ) )
            break;
         SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCURSEL,0L,0L),(LPARAM)p -> szSpreadsheetName);
         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY),TRUE);
         GetDlgItemText(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY,p -> szCellRange,MAX_PATH);
         if ( 0 < strlen(p -> szCellRange) )
            p -> loadExcelCellRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),GetDlgItem(hwnd,IDDI_DATASET_EXCEL_LOAD_ERROR),p -> szDataSource,p -> szSpreadsheetName,p -> szCellRange);
         break;

      case IDDI_DATASET_EXCEL_OPEN_WORKBOOK:
         p -> launchExcel(p -> szDataSource);
         break;

      case IDDI_DATASET_EXCEL_RANGE_ENTRY:
         if ( ! ( EN_CHANGE == notifyCode ) )
            break;
         GetDlgItemText(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY,p -> szCellRange,MAX_PATH);
         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_LOAD),0 < strlen(p -> szCellRange) ? TRUE : FALSE);
         break;

      case IDDI_DATASET_EXCEL_RANGE_LOAD:
         SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_SETCURSEL,(WPARAM)-1,0L);
         p -> loadExcelCellRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),GetDlgItem(hwnd,IDDI_DATASET_EXCEL_LOAD_ERROR),p -> szDataSource,p -> szSpreadsheetName,p -> szCellRange);
         break;

      default:
         break;

      }

      ENABLE_LOAD

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