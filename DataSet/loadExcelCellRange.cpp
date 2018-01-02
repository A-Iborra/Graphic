#include "DataSet.h"

#include <time.h>

   long DataSet::loadExcelCellRange(HWND hwndDestinationList,HWND hwndErrorReport,char *pszWorkbookName,char *pszWorkSheetName,char *pszCellRange) {

   bool wasOpen = false;

   Excel::_Workbook *pIWorkbook = NULL;
   Excel::_Worksheet *pIWorksheet = NULL;

   if ( hwndDestinationList ) {

      SendMessage(hwndDestinationList,LVM_DELETEALLITEMS,0L,0L);

      LVCOLUMN lvColumn = {0};

      lvColumn.mask = LVCF_WIDTH;

      while ( SendMessage(hwndDestinationList,LVM_GETCOLUMN,0,(LPARAM)&lvColumn) )
         SendMessage(hwndDestinationList,LVM_DELETECOLUMN,0L,0L);

   }

   if ( NULL == hwndErrorReport ) {
      if ( hwndMainPropertiesPage && IsWindowVisible(hwndMainPropertiesPage) )
         hwndErrorReport = GetDlgItem(hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_ERROR);
      else if ( hwndExcelSettings && IsWindowVisible(hwndExcelSettings) )
         hwndErrorReport = GetDlgItem(hwndExcelSettings,IDDI_DATASET_EXCEL_LOAD_ERROR);
   }

   try {

   pIWorkbook = openExcelWorkbook(pszWorkbookName,&wasOpen);

   if ( NULL == pIWorkbook ) {

      if ( hwndErrorReport ) {
         char szMessage[256];
         sprintf_s(szMessage,256,"The Excel import failed because the workbook (%s) could not be opened.",pszWorkbookName);
         SetWindowText(hwndErrorReport,szMessage);
      }

      if ( pIApplication )
         pIApplication -> Release();

      return 0;
   }

   pIWorksheet = getExcelWorksheet(pIWorkbook,pszWorkSheetName);

   if ( ! pIWorksheet ) {

      if ( hwndErrorReport ) {
         char szMessage[256];
         sprintf_s(szMessage,256,"The Excel import failed because the worksheet (%s) in %s could not be opened.",pszWorkSheetName,pszWorkbookName);
         SetWindowText(hwndErrorReport,szMessage);
      }

      if ( pIWorksheet )
         pIWorksheet -> Release();

      if ( pIWorkbook && ! wasOpen ) 
         pIWorkbook -> Close(VARIANT_TRUE);

      if ( pIWorkbook )
         pIWorkbook -> Release();

      if ( pIApplication )
         pIApplication -> Release();

      return 0;

   }

   WCHAR szwRange[64];

   MultiByteToWideChar(CP_ACP,0,pszCellRange,-1,szwRange,64);

   _variant_t vtSafeArray = pIWorksheet -> GetCells() -> GetRange(szwRange) -> GetValue();

   if ( ! ( VT_ARRAY & vtSafeArray.vt ) ) {
      if ( hwndErrorReport ) {
         char szMessage[256];
         sprintf_s(szMessage,256,"The Excel import failed.\nPlease be sure to specify a range, not a single cell. The incorrect value may be %s",pszCellRange);
         SetWindowText(hwndErrorReport,szMessage);
      }
   } else
      populateData(hwndDestinationList,hwndErrorReport,true,vtSafeArray.parray);

   } catch ( _com_error e ) {

      char szMessage[1024];

      sprintf(szMessage,"There was an error working with the workbook %s in named range %s.\n\nThe Excel subsystem returned the error %s (%ld).\n\n"
                           "Please check your Workbook or try using a different one.",
                  szDataSource,szNamedRange,e.ErrorMessage(),e.Error());

      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);

   }

   if ( pIWorksheet )
      pIWorksheet -> Release();

   if ( pIWorkbook && ! wasOpen ) 
      pIWorkbook -> Close(VARIANT_TRUE);

   if ( pIWorkbook )
      pIWorkbook -> Release();

   if ( pIApplication )
      pIApplication -> Release();

   return 0;
   }

