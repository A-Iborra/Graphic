
#include "DataSet.h"

#include "General.h"
#include "Graphic_resource.h"

   char szRowcountFormat[128];
   char szDimensionalityFormat[128];
   char szRangeFormat[128];
   char szFloorCeilingFormat[128];
   char szTemp[512];
   long countPoints;

   LRESULT EXPENTRY DataSet::dataSetPropertiesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   
   DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (DataSet *)pPage -> lParam;

      p -> hwndMainPropertiesPage = hwnd;
      p -> hwndMainPropertiesPageError = GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_ERROR);

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      GetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_ROWCOUNT,szRowcountFormat,128);

      GetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_ARITY,szDimensionalityFormat,128);

      GetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_DIMENSION1_RANGE,szRangeFormat,128);

      GetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_SINGLE_VALUE_LABEL,szFloorCeilingFormat,128);

      SetWindowText(GetDlgItem(hwnd,IDDI_DATASET_NAME),p -> szName);
      SetWindowText(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),p -> szDataSource);

      DataSet::nativeStaticHandler = (WNDPROC)SetWindowLongPtr(p -> hwndMainPropertiesPageError,GWLP_WNDPROC,(ULONG_PTR)DataSet::statusAndErrorTextStaticHandler);

      SetWindowText(p -> hwndMainPropertiesPageError,"");
      SetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_STATUS,"");

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_STATUS),GWLP_WNDPROC,(ULONG_PTR)DataSet::statusAndErrorTextStaticHandler);

      SetWindowLongPtr(p -> hwndMainPropertiesPageError,GWLP_USERDATA,(ULONG_PTR)RGB(255,0,0));

      HDC hdc = GetDC(hwnd);
      COLORREF cr = GetTextColor(hdc);
      ReleaseDC(hwnd,hdc);

      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_STATUS),GWLP_USERDATA,(ULONG_PTR)cr);

      ShowWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_CANCEL),SW_HIDE);

      if ( p -> isFunction ) {
         ShowWindow(GetDlgItem(hwnd,IDDI_DATASET_GETDATASOURCE),SW_HIDE);
         ShowWindow(GetDlgItem(hwnd,IDDI_DATASET_OPEN_WORKBOOK),SW_HIDE);
         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),FALSE);
      }

      if ( p -> szExportWorkbookName[0] ) {
         p -> loadExcelWorkbook(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST),NULL,p -> hwndMainPropertiesPageError,p -> szExportWorkbookName,p -> szExportWorksheetName,NULL);
         SendDlgItemMessage(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST,CB_GETCURSEL,0L,0L),(LPARAM)p -> szExportWorksheetName);
         SetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_DEST,p -> szExportWorkbookName);
         SendDlgItemMessage(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST,CB_ADDSTRING,(WPARAM)0L,(LPARAM)"<new>");
         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_DEST_OPEN),TRUE);
         FILE *fx = fopen(p -> szExportWorkbookName,"rb");
         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT),! ( NULL == fx ) ? TRUE : FALSE);
         fclose(fx);
      } else {
         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_DEST_OPEN),FALSE);
      }

      if ( p -> szExportWorksheetTopLeftCell[0] )
         SetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_CELL,p -> szExportWorksheetTopLeftCell);
   
      if ( ! p -> isFunction ) {
         LoadString(hModule,IDSTRING_DATASET_DATASOURCE_EMBED_INSTRUCTIONS,szTemp,512);
         SetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE_EMBED_INSTRUCTIONS,szTemp);
         SendMessage(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_IS_EMBEDDED),BM_SETCHECK,(WPARAM)(p -> isEmbedded ? BST_CHECKED : BST_UNCHECKED),0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EMBEDDED_UPDATE),p -> isEmbedded ? TRUE : FALSE);
      } else {
         ShowWindow(GetDlgItem(hwnd,IDSTRING_DATASET_DATASOURCE_EMBED_INSTRUCTIONS),SW_HIDE);
         ShowWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_IS_EMBEDDED),SW_HIDE);
         ShowWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EMBEDDED_UPDATE),SW_HIDE);
      }

      double floor,ceiling;

      p -> pPropertyZFloor -> get_doubleValue(&floor);
      p -> pPropertyZCeiling -> get_doubleValue(&ceiling);

      if ( ! ( floor == ceiling ) ) {
         p -> pPropertyZFloor -> setWindowItemText(hwnd,IDDI_DATASET_DATA_BOX_FLOOR);
         p -> pPropertyZCeiling -> setWindowItemText(hwnd,IDDI_DATASET_DATA_BOX_CEILING);
      }

      p -> pPropertyXFloor -> get_doubleValue(&floor);
      p -> pPropertyXCeiling -> get_doubleValue(&ceiling);

      if ( ! ( floor == ceiling ) ) {
         p -> pPropertyXFloor -> setWindowItemText(hwnd,IDDI_DATASET_DATA_BOX_X_RANGE_FLOOR);
         p -> pPropertyXCeiling -> setWindowItemText(hwnd,IDDI_DATASET_DATA_BOX_X_RANGE_CEILING);
      }

      p -> pPropertyYFloor -> get_doubleValue(&floor);
      p -> pPropertyYCeiling -> get_doubleValue(&ceiling);

      if ( ! ( floor == ceiling ) ) {
         p -> pPropertyYFloor -> setWindowItemText(hwnd,IDDI_DATASET_DATA_BOX_Y_RANGE_FLOOR);
         p -> pPropertyYCeiling -> setWindowItemText(hwnd,IDDI_DATASET_DATA_BOX_Y_RANGE_CEILING);
      }

      }
      break;

   case WM_SHOWWINDOW: {
      if ( ! wParam )
         break;
      p -> get_countPoints(&countPoints);
      if ( 0 == countPoints ) {
         sprintf_s(szTemp,512,"The data set is empty or has not been imported yet. You can click Populate to load the data.");
         SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_POPULATE,"Populate");
         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_IS_EMBEDDED),FALSE);
      } else {
         sprintf_s(szTemp,512,szRowcountFormat,countPoints);
         SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_POPULATE,"Reload");
         EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_IS_EMBEDDED),TRUE);
      }
      EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EMBEDDED_UPDATE),p -> isEmbedded ? TRUE : FALSE);
      SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_ROWCOUNT,szTemp);
      SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_DIMENSION1_RANGE,"");
      SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_DIMENSION2_RANGE,"");
      SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_DIMENSION3_RANGE,"");

      if ( 0 == countPoints ) {
         sprintf_s(szTemp,512,"The dimensionality of the data is not known.");
         SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_ARITY,szTemp);
      } else {
         enum DataArity dataArity = p -> DataArity();
         sprintf_s(szTemp,512,szDimensionalityFormat,dataArity == DATA_ARITY_1D ? "1" : (dataArity == DATA_ARITY_2D ? "2" : "3"));
         SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_ARITY,szTemp);
         switch ( dataArity ) {
         case DATA_ARITY_3D:
         case DATA_ARITY_2D:
         case DATA_ARITY_1D:

            sprintf_s(szTemp,512,szRangeFormat,1,p -> xMin,p -> xMax);
            SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_DIMENSION1_RANGE,szTemp);

            if ( DATA_ARITY_1D == dataArity )
               break;

            sprintf_s(szTemp,512,szRangeFormat,2,p -> yMin,p -> yMax);
            SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_DIMENSION2_RANGE,szTemp);

            if ( DATA_ARITY_2D == dataArity )
               break;

            sprintf_s(szTemp,512,szRangeFormat,3,p -> zMin,p -> zMax);
            SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_DIMENSION3_RANGE,szTemp);

            //if ( p -> zMin == p -> zMax ) {
            //   sprintf_s(szTemp,512,szFloorCeilingFormat,3,"3rd");
            //   SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_SINGLE_VALUE_LABEL,szTemp);
            //}

         }

      }
      }
      break;

   case WM_DESTROY:
      p -> hwndMainPropertiesPage = NULL;
      p -> hwndMainPropertiesPageError = NULL;
      break;

   case WM_COMMAND: {

      long notifyCode = HIWORD(wParam);

      switch ( LOWORD(wParam) ) {

      case IDDI_DATASET_NAME:
         GetDlgItemText(hwnd,IDDI_DATASET_NAME,p -> szName,64);
         if ( p -> hwndSpecDialog )
            SetDlgItemText(p -> hwndSpecDialog,IDDI_DATASET_SPEC_NAME,p -> szName);
         break;

      case IDDI_DATASET_DATASOURCE:
         GetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE,p -> szDataSource,MAX_PATH);
         if ( p -> hwndSpecDialog )
            SetDlgItemText(p -> hwndSpecDialog,IDDI_DATASET_SPEC_DATASOURCE,p -> szDataSource);
         break;

      case IDDI_DATASET_DATA_BOX_X_RANGE_FLOOR:
         if ( EN_CHANGE == notifyCode )
            p -> pPropertyXFloor -> getWindowItemValue(hwnd,IDDI_DATASET_DATA_BOX_X_RANGE_FLOOR);
         return (LRESULT)0L;

      case IDDI_DATASET_DATA_BOX_X_RANGE_CEILING:
         if ( EN_CHANGE == notifyCode ) 
            p -> pPropertyXCeiling -> getWindowItemValue(hwnd,IDDI_DATASET_DATA_BOX_X_RANGE_CEILING);
         return (LRESULT)0L;

      case IDDI_DATASET_DATA_BOX_Y_RANGE_FLOOR:
         if ( EN_CHANGE == notifyCode )
            p -> pPropertyYFloor -> getWindowItemValue(hwnd,IDDI_DATASET_DATA_BOX_Y_RANGE_FLOOR);
         return (LRESULT)0L;

      case IDDI_DATASET_DATA_BOX_Y_RANGE_CEILING:
         if ( EN_CHANGE == notifyCode ) 
            p -> pPropertyYCeiling -> getWindowItemValue(hwnd,IDDI_DATASET_DATA_BOX_Y_RANGE_CEILING);
         return (LRESULT)0L;

      case IDDI_DATASET_DATA_BOX_FLOOR:
         if ( EN_CHANGE == notifyCode )
            p -> pPropertyZFloor -> getWindowItemValue(hwnd,IDDI_DATASET_DATA_BOX_FLOOR);
         return (LRESULT)0L;

      case IDDI_DATASET_DATA_BOX_CEILING:
         if ( EN_CHANGE == notifyCode ) 
            p -> pPropertyZCeiling -> getWindowItemValue(hwnd,IDDI_DATASET_DATA_BOX_CEILING);
         return (LRESULT)0L;

      case IDDI_DATASET_DATASOURCE_EXPORT_DEST:
         if ( ! ( EN_CHANGE == notifyCode ) )
            break;
         GetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_DEST,p -> szExportWorkbookName,MAX_PATH);
         break;

      case IDDI_DATASET_DATASOURCE_EXPORT_CELL:
         if ( ! ( EN_CHANGE == notifyCode ) )
            break;
         GetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_CELL,p -> szExportWorksheetTopLeftCell,MAX_PATH);
         break;

      case IDDI_DATASET_OPEN_WORKBOOK:
         p -> launchExcel(p -> szDataSource);
         break;

      case IDDI_DATASET_DATA_BOX_POPULATE: {
         char szButton[32];
         GetWindowText((HWND)lParam,szButton,512);
         if ( p -> pIFunction ) {
            std::function<void()> *pOnFinished = new std::function<void()>([p,hwnd]() {
                     dataSetPropertiesHandler(hwnd,WM_SHOWWINDOW,(WPARAM)1,0L);
                     delete p -> pIGSFunctioNaterEvents; 
                     p -> pIGSFunctioNaterEvents = NULL; });
            p -> pIGSFunctioNaterEvents = new _IGSFunctioNaterEvents(p,p -> pIFunction,pOnFinished);
            if ( 0 == strcmp(szButton,"Reload") )  
               p -> ReSet();
            sprintf(szTemp,"The function %s is running.",p -> szDataSource);
            SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_ROWCOUNT,szTemp);
            p -> pIFunction -> Start();
         } else {
            szTemp[0] = '\0';
            if ( ! p -> szDataSource[0] )
               sprintf_s(szTemp,512,"The input workbook has not been specified. Please select the data source workbook.");
            else if ( ! p -> szSpreadsheetName[0] )
               sprintf_s(szTemp,512,"The input worksheet has not been specified. Please visit the Excel Input page to specify it.");
            else if ( ! p -> szCellRange[0] && ! p -> szNamedRange[0] )
               sprintf_s(szTemp,512,"The cell or named range in the worksheet has not been specified. Visit the Excel Input page to enter it.");
            if ( szTemp[0] )
               SetWindowText(p -> hwndMainPropertiesPageError,szTemp);
            else {
               if ( 0 == strcmp(szButton,"Reload") ) 
                  p -> ReSet();
               char *psz = strrchr(p -> szDataSource,'\\');
               if ( ! psz )
                  psz = strrchr(p -> szDataSource,'/');
               if ( ! psz )
                  psz = p -> szDataSource - 1;
               sprintf(szTemp,"The workbook %s is being loaded",psz + 1);
               SetDlgItemText(hwnd,IDDI_DATASET_DATA_BOX_ROWCOUNT,szTemp);
               p -> pOneShotOnFinishedLambda = new std::function<void()>([hwnd]() { DataSet::dataSetPropertiesHandler(hwnd,WM_SHOWWINDOW,(WPARAM)1,0L); });
               p -> Start();
            }
         }
         }
         break;

      case IDDI_DATASET_DATASOURCE_EXPORT_DEST_OPEN:
         p -> launchExcel(p -> szExportWorkbookName);
         break;

      case IDDI_DATASET_DATASOURCE_EXPORT:
         EnableWindow((HWND)lParam,FALSE);
         p -> exportToExcel(p -> szExportWorkbookName,p -> szExportWorksheetName,p -> szExportWorksheetTopLeftCell);
         break;

      case IDDI_DATASET_DATASOURCE_EXPORT_CANCEL:
         p -> cancelExcelExport = true;
         break;

      case IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST:
         if ( ! ( CBN_SELCHANGE == notifyCode ) )
            break;
         SendDlgItemMessage(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST,CB_GETCURSEL,0L,0L),(LPARAM)p -> szExportWorksheetName);
         if ( 0 == strcmp("<new>",p -> szExportWorksheetName) ) {
            p -> loadExcelWorkbook(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST),NULL,p -> hwndMainPropertiesPageError,p -> szExportWorkbookName,"<new>",NULL);
            SendDlgItemMessage(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST,CB_GETLBTEXT,(WPARAM)SendDlgItemMessage(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST,CB_GETCURSEL,0L,0L),(LPARAM)p -> szExportWorksheetName);
         }
         break;

      case IDDI_DATASET_GETEXPORT_DEST:
      case IDDI_DATASET_GETDATASOURCE: {

         OPENFILENAME openFileName = {0};

         char szFilter[MAX_PATH],szFile[MAX_PATH];
                     
         memset(szFilter,0,sizeof(szFilter));
         memset(szFile,0,sizeof(szFile));
                     
         sprintf(szFilter,"Excel files");
         long k = strlen(szFilter) + sprintf(szFilter + strlen(szFilter) + 1,"*.xl*");
         k = k + sprintf(szFilter + k + 2,"All Files");
         sprintf(szFilter + k + 3,"*.*");
                     
         openFileName.lStructSize = sizeof(OPENFILENAME);
         openFileName.hwndOwner = hwnd;
         openFileName.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
         openFileName.lpstrFilter = szFilter;
         openFileName.lpstrFile = szFile;
         openFileName.lpstrDefExt = "xlsx";
         openFileName.nFilterIndex = 1;
         openFileName.nMaxFile = MAX_PATH;
         openFileName.lpstrTitle = "Select the existing Excel Workbook";

         if ( IDDI_DATASET_GETEXPORT_DEST == LOWORD(wParam) ) {
            openFileName.Flags &= ! OFN_FILEMUSTEXIST;
            openFileName.lpstrTitle = "Select or create an Excel Workbook";
         }

         if ( ! GetOpenFileName(&openFileName) )
            break;

         if ( IDDI_DATASET_GETEXPORT_DEST == LOWORD(wParam) ) {

            strcpy(p -> szExportWorkbookName,openFileName.lpstrFile);

            SetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_DEST,p -> szExportWorkbookName);

            p -> loadExcelWorkbook(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST),NULL,p -> hwndMainPropertiesPageError,p -> szExportWorkbookName,p -> szExportWorksheetName,NULL);

            SendDlgItemMessage(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_SHEET_LIST,CB_ADDSTRING,(WPARAM)0L,(LPARAM)"<new>");

            EnableWindow(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE_EXPORT_DEST_OPEN),TRUE);

         } else {

            strcpy(p -> szDataSource,openFileName.lpstrFile);

            memset(p -> szNamedRange,0,sizeof(p -> szNamedRange));
            memset(p -> szCellRange,0,sizeof(p -> szCellRange));
            memset(p -> szSpreadsheetName,0,sizeof(p -> szSpreadsheetName));
            p -> hasHeaderRow = false;
            p -> hasHeaderRowDetermined = false;

            SetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE,p -> szDataSource);

            if ( p -> hwndExcelSettings ) {
               SendDlgItemMessage(p -> hwndExcelSettings,IDDI_DATASET_EXCEL_WB_NAMEDRANGES_LIST,CB_RESETCONTENT,0L,0L);
               SendDlgItemMessage(p -> hwndExcelSettings,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS,LVM_DELETEALLITEMS,0L,0L);
               SendDlgItemMessage(p -> hwndExcelSettings,IDDI_DATASET_EXCEL_SPREADSHEET_LIST,CB_RESETCONTENT,0L,0L);
               LVCOLUMN lvColumn = {0};
               lvColumn.mask = LVCF_WIDTH;
               while ( SendDlgItemMessage(p -> hwndExcelSettings,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS,LVM_GETCOLUMN,0,(LPARAM)&lvColumn) )
                  SendDlgItemMessage(p -> hwndExcelSettings,IDDI_DATASET_EXCEL_NAMEDRANGE_CONTENTS,LVM_DELETECOLUMN,0L,0L);
            }

         }

         }
         break;

      case IDDI_DATASET_DATASOURCE_IS_EMBEDDED:
         p -> isEmbedded = SendMessage((HWND)lParam,BM_GETCHECK,0L,0L) == BST_CHECKED;
         break;

      case IDDI_DATASET_DATASOURCE_EMBEDDED_UPDATE:
         DataSet::dataSetPropertiesHandler(hwnd,WM_COMMAND,MAKEWPARAM(IDDI_DATASET_DATA_BOX_POPULATE,0),(LPARAM)GetDlgItem(hwnd,IDDI_DATASET_DATA_BOX_POPULATE));
         break;

      default:
         return (LRESULT)0L;

      }

      DataSet::dataSetPropertiesHandler(hwnd,WM_SHOWWINDOW,(WPARAM)1,0L);

      break;

      }
      break;

   default:
      break;

   }

   return (LRESULT)0;
   }

   //
   //NTC: 12-28-2017: To re-instate the instructions page, uncomment this, it's declaration in DataSet.h
   // and add the property page in IPropertyPageClient.cpp
   //
   //LRESULT EXPENTRY DataSet::dataSetInstructionsHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   //
   //DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   //switch ( msg ) {

   //case WM_INITDIALOG: {

   //   PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

   //   p = (DataSet *)pPage -> lParam;

   //   SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

   //   char szInstructions[4096];

   //   LoadString(hModule,IDSTRING_DATASET_INSTRUCTIONS,szInstructions,4096);
   //   SetDlgItemText(hwnd,IDDI_DATASET_INSTRUCTIONS,szInstructions);
   //   LoadString(hModule,IDSTRING_DATASET_INSTRUCTIONS2,szInstructions,4096);
   //   SetDlgItemText(hwnd,IDDI_DATASET_INSTRUCTIONS2,szInstructions);
   //   LoadString(hModule,IDSTRING_DATASET_INSTRUCTIONS3,szInstructions,4096);
   //   SetDlgItemText(hwnd,IDDI_DATASET_INSTRUCTIONS3,szInstructions);
   //   LoadString(hModule,IDSTRING_DATASET_INSTRUCTIONS4,szInstructions,4096);
   //   SetDlgItemText(hwnd,IDDI_DATASET_INSTRUCTIONS4,szInstructions);

   //   }
   //   break;


   //default:
   //   break;

   //}

   //return (LRESULT)0;
   //}
