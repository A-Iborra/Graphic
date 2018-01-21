// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"

#include "General.h"
#include "Graphic_resource.h"

#define ENABLE_LOAD_BUTTON \
EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_LOAD),( 0 < strlen(p -> szCellRange) && \
   -1 < SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCURSEL,0L,0L) && \
   0 < strlen(p -> szDataSource) ) ? TRUE : FALSE);

#define HEADER_ROW_FOUND      \
   {  \
   SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW_LABEL,"This data has a column headings row. If this is not correct, uncheck the option below.");   \
   SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);                                                              \
   }

#define HEADER_ROW_NOT_FOUND  \
   {  \
   SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW_LABEL,"This data does not have a column headings row. If this is not correct, check the option below."); \
   SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0L);                                                               \
   }

#define SET_HEADER_ROW_INFO \
   {  \
   EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW_LABEL),p -> hasHeaderRowDetermined ? TRUE : FALSE);                  \
   EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW),p -> hasHeaderRowDetermined ? TRUE : FALSE);                        \
   SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW,BM_SETCHECK,(WPARAM)p -> hasHeaderRow ? BST_CHECKED : BST_UNCHECKED,0L);  \
   }

#define CLEAR_DATA \
   {  \
   SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS,LVM_DELETEALLITEMS,0L,0L);    \
   LVCOLUMN lvColumn = {0};                                                                     \
   lvColumn.mask = LVCF_WIDTH;                                                                  \
   while ( SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS,LVM_GETCOLUMN,0,(LPARAM)&lvColumn) ) \
      SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS,LVM_DELETECOLUMN,0L,0L);                  \
   }

   static bool isAutoEditFieldSet = false;
   static bool ignoreCommands = false;

   LRESULT EXPENTRY DataSet::dataSetExcelHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   
   DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (DataSet *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndExcelSettings = hwnd;
      p -> hwndExcelSettingsError = GetDlgItem(hwnd,IDDI_DATASET_EXCEL_LOAD_ERROR);

      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),FALSE);

      RECT rcDialog;
      RECT rcRangeList;

      GetWindowRect(hwnd,&rcDialog);

      GetWindowRect(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_INPUT_BOX),&rcRangeList);

      SetWindowPos(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),HWND_TOP,16,rcRangeList.bottom - rcDialog.top + 16,rcDialog.right - rcDialog.left - 32,rcDialog.bottom - rcDialog.top - (rcRangeList.bottom - rcDialog.top + 16) - 16,0L);

      ENABLE_LOAD_BUTTON

      SetWindowLongPtr(p -> hwndExcelSettingsError,GWLP_USERDATA,(ULONG_PTR)RGB(255,0,0));

      DataSet::nativeStaticHandler = (WNDPROC)SetWindowLongPtr(p -> hwndExcelSettingsError,GWLP_WNDPROC,(ULONG_PTR)DataSet::statusAndErrorTextStaticHandler);

      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW_LABEL),FALSE);

      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW),FALSE);

      }
      break;

   case WM_SHOWWINDOW: {

      if ( ! wParam )
         break;

      if ( ignoreCommands )
         break;

      if ( ! p -> szDataSource[0] )
         break;

      char *psz = strrchr(p -> szDataSource,'\\');
      if ( ! psz )
         psz = strrchr(p -> szDataSource,'/');
      if ( ! psz )
         psz = p -> szDataSource - 1;

      SetWindowText(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),psz + 1);

      ENABLE_LOAD_BUTTON

      isAutoEditFieldSet = true;

      SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY,p -> szCellRange);

      isAutoEditFieldSet = false;

      long countSheets = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCOUNT,0L,0L);

      long countRanges = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCOUNT,0L,0L);

      if ( 0 == countRanges || 0 == countSheets ) {
         p -> loadExcelWorkbook(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST),GetDlgItem(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST),p -> hwndExcelSettingsError,p -> szDataSource,p -> szSpreadsheetName,p -> szNamedRange);
         countSheets = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCOUNT,0L,0L);
         countRanges = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCOUNT,0L,0L);
         long currentSelection = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCURSEL,0L,0L);
         if ( -1 < currentSelection ) {
            SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETLBTEXT,(WPARAM)currentSelection,(LPARAM)p -> szNamedRange);
            if ( ! p -> hasHeaderRowDetermined ) 
               p -> hasHeaderRowDetermined = p -> peekExcelCells(p -> szDataSource,p -> szNamedRange,p -> szSpreadsheetName,NULL,&p -> hasHeaderRow);
            if ( p -> hasHeaderRow )
               HEADER_ROW_FOUND
            else
               HEADER_ROW_NOT_FOUND
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

      if ( 0 == countData && ! p -> isEmbedded ) {
         if ( p -> szCellRange[0] && p -> szSpreadsheetName[0] && ! p -> szNamedRange[0] ) {
            if ( ! p -> hasHeaderRowDetermined ) 
               p -> hasHeaderRowDetermined = p -> peekExcelCells(p -> szDataSource,NULL,p -> szSpreadsheetName,p -> szCellRange,&p -> hasHeaderRow);
            p -> loadExcelCellRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),p -> hwndExcelSettingsError,p -> szDataSource, p -> szSpreadsheetName,p -> szCellRange);
         } else {
            if ( p -> szNamedRange[0] ) {
               if ( ! p -> hasHeaderRowDetermined ) 
                  p -> hasHeaderRowDetermined = p -> peekExcelCells(p -> szDataSource,p -> szNamedRange,NULL,NULL,&p -> hasHeaderRow);
               p -> loadExcelNamedRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),p -> hwndExcelSettingsError,p -> szDataSource,p -> szNamedRange);
            }
         }
      }

      SET_HEADER_ROW_INFO

      }
      break;

   case WM_COMMAND: {

      if ( ignoreCommands )
         break;

      long notifyCode = HIWORD(wParam);

      switch ( LOWORD(wParam) ) {

      case IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST:

         if ( ! ( CBN_SELCHANGE == notifyCode ) )
            break;

         p -> hasHeaderRowDetermined = false;

         SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_GETCURSEL,0L,0L),(LPARAM)p -> szNamedRange);

         p -> szCellRange[0] = '\0';

         isAutoEditFieldSet = true;

         SetDlgItemText(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY,"");

         isAutoEditFieldSet = false;

         p -> hasHeaderRowDetermined = p -> peekExcelCells(p -> szDataSource,p -> szNamedRange,NULL,NULL,&p -> hasHeaderRow);

         p -> loadExcelNamedRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),p -> hwndExcelSettingsError,p -> szDataSource,p -> szNamedRange);

         break;

      case IDDI_DATASET_EXCEL_SPREADSHEET_LIST: {

         if ( ! ( CBN_SELCHANGE == notifyCode ) )
            break;

         p -> hasHeaderRowDetermined = false;

         ignoreCommands = true;

         CLEAR_DATA

         ignoreCommands = false;

         SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_GETCURSEL,0L,0L),(LPARAM)p -> szSpreadsheetName);

         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY),TRUE);

         char szTemp[64];
         GetDlgItemText(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY,szTemp,MAX_PATH);

         if ( 0 < strlen(szTemp) ) {
            p -> hasHeaderRowDetermined = p -> peekExcelCells(p -> szDataSource,NULL,p -> szSpreadsheetName,szTemp,&p -> hasHeaderRow);
            if ( p -> hasHeaderRowDetermined ) {
               strcpy(p -> szCellRange,szTemp);
               p -> loadExcelCellRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),p -> hwndExcelSettingsError,p -> szDataSource,p -> szSpreadsheetName,p -> szCellRange);
            }
         }

         }
         break;

      case IDDI_DATASET_EXCEL_OPEN_WORKBOOK:
         p -> launchExcel(p -> szDataSource);
         break;

      case IDDI_DATASET_EXCEL_RANGE_ENTRY: {

         if ( ! ( EN_CHANGE == notifyCode ) )
            break;

         if ( isAutoEditFieldSet )
            break;

         char szTemp[64];

         GetDlgItemText(hwnd,IDDI_DATASET_EXCEL_RANGE_ENTRY,szTemp,MAX_PATH);

         if ( 0 < strlen(szTemp) )
            p -> hasHeaderRowDetermined = p -> peekExcelCells(p -> szDataSource,NULL,p -> szSpreadsheetName,szTemp,&p -> hasHeaderRow);

         strcpy(p -> szCellRange,szTemp);

         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_RANGE_LOAD),p -> hasHeaderRowDetermined ? TRUE : FALSE);

         }
         break;

      case IDDI_DATASET_EXCEL_RANGE_LOAD: {

         SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_SETCURSEL,(WPARAM)-1,0L);

         p -> szNamedRange[0] = '\0';
         p -> hasHeaderRowDetermined = p -> peekExcelCells(p -> szDataSource,NULL,p -> szSpreadsheetName,p -> szCellRange,&p -> hasHeaderRow);
         p -> loadExcelCellRange(GetDlgItem(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS),p -> hwndExcelSettingsError,p -> szDataSource,p -> szSpreadsheetName,p -> szCellRange);

         }
         break;


      case IDDI_DATASET_EXCEL_HAS_HEADER_ROW:
         p -> hasHeaderRow = SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_HAS_HEADER_ROW,BM_GETCHECK,0L,0L) == BST_CHECKED ? true : false;
         p -> hasHeaderRowDetermined = true;
         SendDlgItemMessage(hwnd,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS,LVM_DELETEALLITEMS,0L,0L);
         DataSet::dataSetExcelHandler(hwnd,WM_SHOWWINDOW,(WPARAM)TRUE,0L);
         break;

      default:
         break;

      }

      ENABLE_LOAD_BUTTON

      if ( p -> hasHeaderRowDetermined )
         if ( p -> hasHeaderRow )
            HEADER_ROW_FOUND
         else
            HEADER_ROW_NOT_FOUND

      SET_HEADER_ROW_INFO

      }
      break;

   case WM_DESTROY:
      p -> hwndExcelSettings = NULL;
      p -> hwndExcelSettingsError = NULL;
      break;

   default:
      break;

   }

   return (LRESULT)0;
   }