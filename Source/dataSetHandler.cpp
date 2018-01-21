// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include <list>

#include "Graphic_resource.h"
#include "GMessage.h"
#include "utils.h"
#include "List.cpp"


   LRESULT CALLBACK G::dataSetHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      char szTemp[128];
      sprintf(szTemp,"There are %ld data sets defined",p -> dataSetList.Count());
      SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_DATASETS_COUNT),szTemp);

      RECT rect;

      LVCOLUMN lvColumn;
      LVITEM lvItem;
      GetWindowRect(GetDlgItem(hwnd,IDDI_GRAPHIC_DATASETS_LIST),&rect);
      memset(&lvColumn,0,sizeof(LVCOLUMN));
      lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
      lvColumn.fmt = LVCFMT_LEFT;
      lvColumn.cx = rect.right - rect.left - 4 - 128;
      lvColumn.pszText = "DataSet";
      SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn);
      lvColumn.cx = 128;
      lvColumn.pszText = "DataSource";
      SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_INSERTCOLUMN,1,(LPARAM)&lvColumn);
      IDataSet *pIDataSet = NULL;
      while ( pIDataSet = p -> dataSetList.GetNext(pIDataSet) ) {
         memset(&lvItem,0,sizeof(LVITEM));
         BSTR bstrName;
         pIDataSet -> get_Name(&bstrName);
         long n = min(MAX_PATH,wcslen(bstrName));
         char szTemp[MAX_PATH];
         memset(szTemp,0,sizeof(szTemp));
         if ( 0 == n )
            strcpy(szTemp,"<noname>");
         else
            WideCharToMultiByte(CP_ACP,0,bstrName,-1,szTemp,n,0,0);
         SysFreeString(bstrName);

         lvItem.mask = LVIF_TEXT | LVIF_PARAM;
         lvItem.iItem = 32768;
         lvItem.state = LVIS_SELECTED;
         lvItem.stateMask = -1;
         lvItem.pszText = szTemp;
         lvItem.lParam = reinterpret_cast<long>(pIDataSet);

         lvItem.iItem = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_INSERTITEM,0L,(LPARAM)&lvItem);

         lvItem.mask = LVIF_TEXT;

         pIDataSet -> get_DataSource(&bstrName);

         n = min(MAX_PATH,wcslen(bstrName));
         memset(szTemp,0,sizeof(szTemp));
         if ( 0 == n )
            strcpy(szTemp,"<unspecified>");
         else
            WideCharToMultiByte(CP_ACP,0,bstrName,-1,szTemp,n,0,0);
         SysFreeString(bstrName);

         lvItem.iSubItem = 1;

         SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_SETITEM,0L,(LPARAM)&lvItem);

      }

      }
      return LRESULT(FALSE);

   case WM_SHOWWINDOW: {

      }
      break;
 
   case WM_NOTIFY:
      switch ( wParam ) {
      case IDDI_GRAPHIC_DATASETS_LIST: {
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETITEMCOUNT,0L,0L);
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_DATASETS_EDIT),itemCount && selectedCount ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_DATASETS_DELETE),itemCount && selectedCount ? TRUE : FALSE);
         }
         break;
      }
      break;

   case WM_COMMAND:

      switch ( LOWORD(wParam) ) {

      case IDDI_GRAPHIC_DATASETS_ADD: {

         IDataSet *pDataSet = p -> newDataSet(true);
         IUnknown* pIUnknown;
         pDataSet -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIGProperties -> ShowProperties(p -> hwndOwner,pIUnknown);
         pIUnknown -> Release();

         G::sampleGraphicHandler(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,0L,(LPARAM)NULL);

         char szTemp[MAX_PROPERTY_SIZE];
         BSTR bstrExpression;
         pDataSet -> get_Name(&bstrExpression);
         memset(szTemp,0,sizeof(szTemp));
         WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,szTemp,sizeof(szTemp),0,0);
         SysFreeString(bstrExpression);

         if ( ! szTemp[0] )
            strcpy(szTemp,"<noname>");

         LVITEM lvItem;
         memset(&lvItem,0,sizeof(LVITEM));
         lvItem.mask = LVIF_TEXT | LVIF_PARAM;
         lvItem.iItem = 32768;
         lvItem.state = LVIS_SELECTED;
         lvItem.stateMask = -1;
         lvItem.pszText = szTemp;
         lvItem.lParam = reinterpret_cast<long>(pDataSet);
         lvItem.iItem = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_INSERTITEM,0L,(LPARAM)&lvItem);

         pDataSet -> get_DataSource(&bstrExpression);
         memset(szTemp,0,sizeof(szTemp));
         WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,szTemp,sizeof(szTemp),0,0);
         SysFreeString(bstrExpression);

         lvItem.iSubItem = 1;

         SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_SETITEM,0L,(LPARAM)&lvItem);

         sprintf(szTemp,"There are %ld data sets defined",p -> dataSetList.Count());
         SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_DATASETS_COUNT),szTemp);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_DATASETS_EDIT),TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_DATASETS_DELETE),TRUE);

         SendMessage(p -> hwndDataSourcesDataSets,TCM_SETCURSEL,(WPARAM)(SendMessage(p -> hwndDataSourcesDataSets,TCM_GETITEMCOUNT,0L,0L) - 1),0L);

         }
         break;

      case IDDI_GRAPHIC_DATASETS_EDIT: {

         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETITEMCOUNT,0L,0L);
         LVITEM lvItem;
         if ( ! itemCount ) {
            MessageBox(GetParent(hwnd),"There aren't any data sets to edit.","Note",MB_OK);
            break;
         }
         if ( ! selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select one of the data sets, then select Edit.","Note",MB_OK);
            break;
         }
         if ( 1 < selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select only one of the data sets, then select Edit.","Note",MB_OK);
            break;
         }
         for ( int k = 0; k < itemCount; k++ ) {

            memset(&lvItem,0,sizeof(LVITEM));
            lvItem.iItem = k;
            lvItem.mask = LVIF_STATE | LVIF_PARAM;
            lvItem.stateMask = LVIS_SELECTED;
            SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);

            if ( lvItem.state && LVIS_SELECTED ) {
               char szTemp[MAX_PROPERTY_SIZE];
               BSTR bstr;
               IDataSet * pds = (IDataSet *)lvItem.lParam;

               IUnknown* pIUnknown;
               pds -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
               p -> pIGProperties -> ShowProperties(hwnd,pIUnknown);
               pIUnknown -> Release();

               G::sampleGraphicHandler(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,1L,(LPARAM)NULL);

               lvItem.mask = LVIF_TEXT;
               lvItem.pszText = szTemp;

               pds -> get_Name(&bstr);
               memset(szTemp,0,sizeof(szTemp));
               WideCharToMultiByte(CP_ACP,0,bstr,MAX_PROPERTY_SIZE,szTemp,MAX_PROPERTY_SIZE,0,0);
               SysFreeString(bstr);

               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_SETITEM,0L,(LPARAM)&lvItem);

               pds -> get_DataSource(&bstr);
               memset(szTemp,0,sizeof(szTemp));
               WideCharToMultiByte(CP_ACP,0,bstr,MAX_PROPERTY_SIZE,szTemp,MAX_PROPERTY_SIZE,0,0);
               SysFreeString(bstr);

               lvItem.iSubItem = 1;

               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_SETITEM,0L,(LPARAM)&lvItem);

               break;
            }
         }
         }
         break;

      case IDDI_GRAPHIC_DATASETS_DELETE: {

         LVITEM lvItem;
         char szTemp[MAX_PROPERTY_SIZE];
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETITEMCOUNT,0L,0L);
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         if ( ! itemCount ) {
            MessageBox(GetParent(hwnd),"There aren't any data sets to delete.","Note",MB_OK);
            break;
         }
         if ( ! selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select one or more of the data sets, then select Delete.","Note",MB_OK);
            break;
         }
         long response = IDYES;
         if ( 1 < selectedCount ) 
            response = MessageBox(GetParent(hwnd),"Are you sure you want to delete the selected data sets?","Note",MB_YESNO | MB_DEFBUTTON2);
         if ( IDYES == response ) {
            List<int> toDelete;
            memset(&lvItem,0,sizeof(LVITEM));
            lvItem.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
            lvItem.stateMask = LVIS_SELECTED;
            lvItem.pszText = szTemp;
            lvItem.cchTextMax = MAX_PROPERTY_SIZE;
            for ( int k = 0; k < itemCount; k++ ) {
               lvItem.iItem = k;
               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);
               if ( lvItem.state && LVIS_SELECTED ) {
                  if ( 1 == selectedCount ) {
                     char szPrompt[MAX_PROPERTY_SIZE];
                     sprintf(szPrompt,"Are you sure you want to delete the data set: %s?",szTemp);
                     response = MessageBox(GetParent(hwnd),szPrompt,"Note",MB_YESNO | MB_DEFBUTTON2);
                  }
                  if ( IDYES == response ) {
                     p -> deleteDataSet(reinterpret_cast<IDataSet *>(lvItem.lParam));
                     toDelete.Add(new int(lvItem.iItem));
                  }
               }
            }
            int* pi;
            while ( pi = toDelete.GetLast() ) {
               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_DELETEITEM,(WPARAM)*pi,0L);
               toDelete.Remove(pi);
               delete pi;
            }
         }

         }   
         break;

      }
 
   case WM_PAINT: {
      long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETITEMCOUNT,0L,0L);
      long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_DATASETS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_DATASETS_EDIT),itemCount && selectedCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_DATASETS_DELETE),itemCount && selectedCount ? TRUE : FALSE);
      }
      break;

   default:
      break;
   }
 
   return LRESULT(FALSE);
   }


