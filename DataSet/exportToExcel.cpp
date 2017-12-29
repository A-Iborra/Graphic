
#include "DataSet.h"

   struct exportParameters {
      DataSet *pDataSet;
      char szWorkbookName[MAX_PATH];
      char szWorksheetName[MAX_PATH];
      char szTopCell[64];
      bool isAutoCall;
   };

   long DataSet::exportToExcel(char *pszWorkbookName,char *pszWorksheetName,char *pszTopCell,bool isAutoCall) {

   if ( NULL == pszWorkbookName )
      pszWorkbookName = szExportWorkbookName;

   if ( NULL == pszWorksheetName )
      pszWorksheetName = szExportWorksheetName;

   if ( NULL == pszTopCell )
      pszTopCell = szExportWorksheetTopLeftCell;

   static exportParameters *p = new exportParameters();

   p -> pDataSet = this;
   strcpy(p -> szWorkbookName,pszWorkbookName);
   strcpy(p -> szWorksheetName,pszWorksheetName);
   strcpy(p -> szTopCell,pszTopCell);
   p -> isAutoCall = isAutoCall;

   DWORD threadId;

   CreateThread(NULL,0,threadedExportToExcel,(void *)p,0L,&threadId);

   return 0;
   }


   DWORD WINAPI DataSet::threadedExportToExcel(void *pvArg) {

   exportParameters *p = (exportParameters *)pvArg;

   char *pszWorkbookName = p -> szWorkbookName;
   char *pszWorksheetName = p -> szWorksheetName;
   char *pszTopCell = p -> szTopCell;
   bool isAutoCall = p -> isAutoCall;

   DataSet *pThis = p -> pDataSet;

   static char szMessage[512];

   DataList *pDataList = NULL;
   DataPoint dp;

   pThis -> get(pDataList,&dp,&pDataList);

   if ( ! pDataList && ! isAutoCall ) {

      std::function<void()> *pOnFinished = new std::function<void()>([pThis]() {
               pThis -> exportToExcel(NULL,NULL,NULL,true);
               delete pThis -> pIGSFunctioNaterEvents; 
               pThis -> pIGSFunctioNaterEvents = NULL; });

      pThis -> pIGSFunctioNaterEvents = new _IGSFunctioNaterEvents(pThis,pThis -> pIFunction,pOnFinished);

      if ( pThis -> hwndMainPropertiesPage ) {
         sprintf(szMessage,"No data was found in the dataset. The system is generating the data now.");
         SetDlgItemText(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT_STATUS,szMessage);
      }

      pThis -> pIFunction -> Start();

      return 0;
      
   }

   if ( isAutoCall )
      SetDlgItemText(p -> pDataSet -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT_STATUS,"Data generation is complete. The system is exporting the data.");

   ShowWindow(GetDlgItem(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT_CANCEL),SW_SHOW);

   CoInitialize(NULL);

   bool wasOpen = false;

   Excel::_Workbook *pIWorkbook = pThis -> openExcelWorkbook(pszWorkbookName,&wasOpen);

   if ( NULL == pIWorkbook ) {
      char szMessage[1024];
      char *p = strrchr(pszWorkbookName,'\\');
      if ( ! p )
         p = strrchr(pszWorkbookName,'//');
      if ( ! p )
         p = pszWorkbookName - 1;
      sprintf(szMessage,"The workbook %s does not exist or cannot be opened.",p + 1);
      if ( pThis -> hwndMainPropertiesPage )
         SetDlgItemText(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_ERROR,szMessage);
      else
         MessageBox(NULL,szMessage,"Export error",MB_ICONEXCLAMATION | MB_TOPMOST);
      ShowWindow(GetDlgItem(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT_CANCEL),SW_HIDE);
      EnableWindow(GetDlgItem(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT),TRUE);
      return 0;
   }

   Excel::_WorksheetPtr pIWorksheet = pThis -> getExcelWorksheet(pIWorkbook,pszWorksheetName);

   if ( NULL == pIWorksheet ) {
      char szMessage[1024];
      char *p = strrchr(pszWorkbookName,'\\');
      if ( ! p )
         p = strrchr(pszWorkbookName,'//');
      if ( ! p )
         p = pszWorkbookName - 1;
      sprintf(szMessage,"The workbook %s does not contain the worksheet %s ",p + 1,pszWorksheetName);
      if ( pThis -> hwndMainPropertiesPage )
         SetDlgItemText(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_ERROR,szMessage);
      else
         MessageBox(NULL,szMessage,"Export error",MB_ICONEXCLAMATION | MB_TOPMOST);
      if ( pIWorkbook && ! wasOpen ) 
         pIWorkbook -> Close(VARIANT_TRUE);
      pIWorkbook -> Release();
      pThis -> pIApplication -> Release();
      return 0;
   }

   VARIANT vRange;
   vRange.vt = VT_BSTR;

   vRange.bstrVal = SysAllocStringLen(NULL,32);
   
   MultiByteToWideChar(CP_ACP,0,pszTopCell,-1,vRange.bstrVal,32);

   Excel::RangePtr pRange = pIWorksheet -> Cells -> GetRange(vRange);

   VARIANT row;
   row.vt = VT_I4;

   VARIANT col;
   col.vt = VT_I4;

   VARIANT value;
   value.vt = VT_BSTR;

   value.bstrVal = SysAllocStringLen(NULL,256);

   row.lVal = 0;

   SetDlgItemText(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT_STATUS,"0 rows ...");

   try {

   if ( pDataList ) while ( 1 ) {

      if ( pThis -> cancelExcelExport ) 
         break;

      row.lVal++;

      col.lVal = 1;

      if ( 0 == ( row.lVal % 1000 ) && pThis -> hwndMainPropertiesPage ) {
         sprintf_s(szMessage,512,"%ld rows ...",row.lVal);
         SetDlgItemText(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT_STATUS,szMessage);
      }

      swprintf(value.bstrVal,L"%lf",dp.x);

      pRange -> PutItem(row,col,value);

      swprintf(value.bstrVal,L"%lf",dp.y);

      col.lVal = 2;

      pRange -> PutItem(row,col,value);

      swprintf(value.bstrVal,L"%lf",dp.z);

      col.lVal = 3;

      pRange -> PutItem(row,col,value);

      if ( ! pDataList )
         break;

      pThis -> get(pDataList,&dp,&pDataList);

   } 

   } catch ( _com_error e ) {

printf("\n hello world");

   }

   pRange -> Release();

   pIWorksheet -> Release();

   if ( pIWorkbook && ! wasOpen ) 
      pIWorkbook -> Close(VARIANT_TRUE);

   pIWorkbook -> Release();

   pThis -> pIApplication -> Release();

   char *pszw = strrchr(pszWorkbookName,'\\');
   if ( ! pszw )
      pszw = strrchr(pszWorkbookName,'/');
   if ( ! pszw )
      pszw = pszWorkbookName - 1;
   if ( pThis -> cancelExcelExport )
      sprintf_s(szMessage,512,"%ld rows from the dataset were exported before cancellation to:\n     %s in worksheet %s",row.lVal,pszw + 1,pszWorksheetName);
   else
      sprintf_s(szMessage,512,"%ld rows from the dataset have been exported to:\n     %s in worksheet %s",row.lVal,pszw + 1,pszWorksheetName);
   if ( pThis -> hwndMainPropertiesPage ) {
      ShowWindow(GetDlgItem(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT_CANCEL),SW_HIDE);
      SetDlgItemText(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT_STATUS,szMessage);
      EnableWindow(GetDlgItem(pThis -> hwndMainPropertiesPage,IDDI_DATASET_DATASOURCE_EXPORT),TRUE);
   } else
      MessageBox(NULL,szMessage,"Export error",MB_ICONEXCLAMATION | MB_TOPMOST);

   if ( pThis -> pIGSystemStatusBar )
      pThis -> pIGSystemStatusBar -> put_StatusText(0,szMessage);

   pThis -> cancelExcelExport = false;

   CoUninitialize();

   return row.lVal;
   }