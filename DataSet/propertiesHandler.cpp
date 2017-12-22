
#include <windows.h>
#include <olectl.h>
#include <CommCtrl.h>

#include "General.h"
#include "Graphic_resource.h"

#include "DataSet.h"

   LRESULT EXPENTRY DataSet::dataSetPropertiesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   
   DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (DataSet *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      SetWindowText(GetDlgItem(hwnd,IDDI_DATASET_NAME),p -> szName);
      SetWindowText(GetDlgItem(hwnd,IDDI_DATASET_DATASOURCE),p -> szDataSource);

      }
      break;


   case WM_COMMAND: {

      switch ( LOWORD(wParam) ) {

      case IDDI_DATASET_NAME:
         GetDlgItemText(hwnd,IDDI_DATASET_NAME,p -> szName,64);
         break;

      case IDDI_DATASET_DATASOURCE:
         GetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE,p -> szDataSource,MAX_PATH);
         break;

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
         openFileName.lpstrTitle = "Select the existing data file";

         if ( ! GetOpenFileName(&openFileName) )
            break;

         strcpy(p -> szDataSource,openFileName.lpstrFile);

         SetDlgItemText(hwnd,IDDI_DATASET_DATASOURCE,p -> szDataSource);

         }
         break;

      default:
         break;

      }
      break;

      }
      break;

   default:
      break;

   }

   return (LRESULT)0;
   }


   LRESULT EXPENTRY DataSet::dataSetInstructionsHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   
   DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (DataSet *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      char szInstructions[4096];

      LoadString(hModule,IDSTRING_DATASET_INSTRUCTIONS,szInstructions,4096);
      SetDlgItemText(hwnd,IDDI_DATASET_INSTRUCTIONS,szInstructions);
      LoadString(hModule,IDSTRING_DATASET_INSTRUCTIONS2,szInstructions,4096);
      SetDlgItemText(hwnd,IDDI_DATASET_INSTRUCTIONS2,szInstructions);
      LoadString(hModule,IDSTRING_DATASET_INSTRUCTIONS3,szInstructions,4096);
      SetDlgItemText(hwnd,IDDI_DATASET_INSTRUCTIONS3,szInstructions);
      LoadString(hModule,IDSTRING_DATASET_INSTRUCTIONS4,szInstructions,4096);
      SetDlgItemText(hwnd,IDDI_DATASET_INSTRUCTIONS4,szInstructions);

      }
      break;


   default:
      break;

   }

   return (LRESULT)0;
   }