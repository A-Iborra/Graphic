/*

                       Copyright (c) 1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>
#include <CommCtrl.h>
#include <olectl.h>

#include "general.h"
#include "Graphic_resource.h"
#include "gmessage.h"

#include "VList.h"

#include "DataSet.h"

   HRESULT DataSet::BeforeAllPropertyPages() {
   return E_NOTIMPL;
   }


   HRESULT DataSet::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {
   return E_NOTIMPL;
   }


   HRESULT DataSet::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {
   return E_NOTIMPL;
   }


   HRESULT DataSet::Apply() {
   return E_NOTIMPL;
   }


   HRESULT DataSet::IsPageDirty(long pageNumber,BOOL* isDirty) {
   return E_NOTIMPL;
   }


   HRESULT DataSet::Help(BSTR bstrHelpDir) {
   return  E_NOTIMPL;
   }


   HRESULT DataSet::TranslateAccelerator(long,long* pResult) {
   return E_NOTIMPL;
   }


   HRESULT DataSet::AfterAllPropertyPages(BOOL userCanceled) {
   return E_NOTIMPL;
   }


   HRESULT DataSet::DestroyPropertyPage(long index) {
   return E_NOTIMPL;
   }

   HRESULT DataSet::GetPropertySheetHeader(void *pv) {
   
   if ( ! pv )
      return E_POINTER;

   PROPSHEETHEADER *pHeader = reinterpret_cast<PROPSHEETHEADER *>(pv);

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;// | PSH_RESIZABLE | PSH_AEROWIZARD;
   pHeader -> hInstance = hModule;
   pHeader -> pszIcon = NULL;
   pHeader -> pszCaption = " DataSet Properties";
   pHeader -> pfnCallback = NULL;

   return S_OK;
   }

   HRESULT DataSet::get_PropertyPageCount(long *pCount) {

   if ( ! pCount )
      return E_POINTER;

   *pCount = 2;

   IGPropertyPageClient *pPlotProperties = NULL;

   pIPlot -> QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void **>(&pPlotProperties));

   long plotPropertiesCount = 0L;

   pPlotProperties -> get_PropertyPageCount(&plotPropertiesCount);

   *pCount += plotPropertiesCount;

   pPlotProperties -> Release();

   return S_OK;
   }

   HRESULT DataSet::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   //pPropSheetPages[0].dwFlags = PSP_USETITLE;
   //pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   //pPropSheetPages[0].hInstance = hModule;
   //pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_DATASET_INSTRUCTIONS);
   //pPropSheetPages[0].pfnDlgProc = (DLGPROC)dataSetInstructionsHandler;
   //pPropSheetPages[0].pszTitle = "Instructions";
   //pPropSheetPages[0].lParam = (LPARAM)this;
   //pPropSheetPages[0].pfnCallback = NULL;

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_DATASET_PROPERTIES);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)dataSetPropertiesHandler;
   pPropSheetPages[0].pszTitle = "Data Set";
   pPropSheetPages[0].lParam = (LPARAM)this;
   pPropSheetPages[0].pfnCallback = NULL;

   if ( isFunction ) {

      pPropSheetPages[1].dwFlags = PSP_USETITLE;
      pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
      pPropSheetPages[1].hInstance = hModule;
      pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_DATASET_FUNCTION);
      pPropSheetPages[1].pfnDlgProc = (DLGPROC)dataSetFunctionDataHandler;
      pPropSheetPages[1].pszTitle = "Function";
      pPropSheetPages[1].lParam = (LPARAM)this;
      pPropSheetPages[1].pfnCallback = NULL;

   } else {

      pPropSheetPages[1].dwFlags = PSP_USETITLE;
      pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
      pPropSheetPages[1].hInstance = hModule;
      pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_DATASET_EXCEL);
      pPropSheetPages[1].pfnDlgProc = (DLGPROC)dataSetExcelHandler;
      pPropSheetPages[1].pszTitle = "Excel Input";
      pPropSheetPages[1].lParam = (LPARAM)this;
      pPropSheetPages[1].pfnCallback = NULL;

   }

   IGPropertyPageClient *pPlotProperties = NULL;

   pIPlot -> QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void **>(&pPlotProperties));

   long plotPropertiesCount = 0L;

   pPlotProperties -> get_PropertyPageCount(&plotPropertiesCount);

   pPlotProperties -> GetPropertySheets((void *)(pPropSheetPages + 2));

   static char szNewName[8][128];

   for ( long k = 0; k < plotPropertiesCount; k++ ) {
      PROPSHEETPAGE *pPage = pPropSheetPages + 2 + k;
      sprintf(szNewName[k],"Plot-%s",pPage -> pszTitle);
      pPage -> pszTitle = szNewName[k];
   }

   pPlotProperties -> Release();

   return S_OK;
   }