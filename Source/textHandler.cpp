// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include <stdio.h>

   LRESULT CALLBACK G::textHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndTextSettings = hwnd;

      char szTemp[128];

      sprintf(szTemp,"There are %ld text components defined",p -> textList.Count());

      SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_TEXTS_COUNT),szTemp);

      RECT rect;

      LVCOLUMN lvColumn;
      LVITEM lvItem;
      GetWindowRect(GetDlgItem(hwnd,IDDI_GRAPHIC_TEXTS_LIST),&rect);
      memset(&lvColumn,0,sizeof(LVCOLUMN));
      lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
      lvColumn.fmt = LVCFMT_LEFT;
      lvColumn.cx = rect.right - rect.left - 4;
      lvColumn.pszText = "Text";
      SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn);

      IText *pIText = NULL;

      while ( pIText = p -> textList.GetNext(pIText) ) {

         memset(&lvItem,0,sizeof(LVITEM));
         BSTR bstrName;
         pIText -> get_Text(&bstrName);

         long n = min(MAX_PATH,wcslen(bstrName));
         char szTemp[MAX_PATH];
         memset(szTemp,0,sizeof(szTemp));
         if ( 0 == n )
            strcpy(szTemp,"<nocontent>");
         else
            WideCharToMultiByte(CP_ACP,0,bstrName,-1,szTemp,n,0,0);
         SysFreeString(bstrName);

         lvItem.mask = LVIF_TEXT | LVIF_PARAM;
         lvItem.iItem = 32768;
         lvItem.state = LVIS_SELECTED;
         lvItem.stateMask = -1;
         lvItem.pszText = szTemp;
         lvItem.lParam = reinterpret_cast<long>(pIText);

         lvItem.iItem = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_INSERTITEM,0L,(LPARAM)&lvItem);

      }

      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_TEXTS_EDIT),FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_TEXTS_DELETE),FALSE);

      }
      return LRESULT(FALSE);

   case WM_NOTIFY:
      switch ( wParam ) {
      case IDDI_GRAPHIC_TEXTS_LIST: {
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_GETITEMCOUNT,0L,0L);
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_TEXTS_EDIT),itemCount && selectedCount ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_TEXTS_DELETE),itemCount && selectedCount ? TRUE : FALSE);
         }
         break;
      }
      break;
 
   case WM_COMMAND:

      switch ( LOWORD(wParam) ) {

      case IDDI_GRAPHIC_TEXTS_ADD: {

         IText *pIText = p -> newText();
         IUnknown* pIUnknown;
         pIText -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIGProperties -> ShowProperties(hwnd,pIUnknown);
         pIUnknown -> Release();

         //G::sampleGraphicHandler(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,0L,(LPARAM)NULL);

         char szTemp[MAX_PROPERTY_SIZE];
         BSTR bstrExpression;
         pIText -> get_Text(&bstrExpression);
         memset(szTemp,0,sizeof(szTemp));
         WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,szTemp,sizeof(szTemp),0,0);
         SysFreeString(bstrExpression);

         if ( ! szTemp[0] )
            strcpy(szTemp,"<nocontent>");

         LVITEM lvItem;
         memset(&lvItem,0,sizeof(LVITEM));
         lvItem.mask = LVIF_TEXT | LVIF_PARAM;
         lvItem.iItem = 32768;
         lvItem.state = LVIS_SELECTED;
         lvItem.stateMask = -1;
         lvItem.pszText = szTemp;
         lvItem.lParam = reinterpret_cast<long>(pIText);
         lvItem.iItem = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_INSERTITEM,0L,(LPARAM)&lvItem);

         sprintf(szTemp,"There are %ld text components defined",p -> dataSetList.Count());
         SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_TEXTS_COUNT),szTemp);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_TEXTS_EDIT),TRUE);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_TEXTS_DELETE),TRUE);

         }
         break;

      case IDDI_GRAPHIC_TEXTS_EDIT: {

         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_GETITEMCOUNT,0L,0L);
         LVITEM lvItem;

         if ( ! itemCount ) {
            MessageBox(GetParent(hwnd),"There isn't any text to edit.","Note",MB_OK);
            break;
         }
         if ( ! selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select one of the text components, then select Edit.","Note",MB_OK);
            break;
         }
         if ( 1 < selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select only one of the text components, then select Edit.","Note",MB_OK);
            break;
         }

         for ( int k = 0; k < itemCount; k++ ) {

            memset(&lvItem,0,sizeof(LVITEM));
            lvItem.iItem = k;
            lvItem.mask = LVIF_STATE | LVIF_PARAM;
            lvItem.stateMask = LVIS_SELECTED;
            SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);

            if ( lvItem.state && LVIS_SELECTED ) {

               char szTemp[MAX_PROPERTY_SIZE];
               BSTR bstr;
               IText *pIText = (IText *)lvItem.lParam;

               IUnknown* pIUnknown;
               pIText -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
               p -> pIGProperties -> ShowProperties(hwnd,pIUnknown);
               pIUnknown -> Release();

              // G::sampleGraphicHandler(hwndSampleGraphic,WMG_POSITION_SAMPLE_GRAPHIC,1L,(LPARAM)NULL);

               lvItem.mask = LVIF_TEXT;
               lvItem.pszText = szTemp;

               pIText -> get_Text(&bstr);
               memset(szTemp,0,sizeof(szTemp));
               WideCharToMultiByte(CP_ACP,0,bstr,MAX_PROPERTY_SIZE,szTemp,MAX_PROPERTY_SIZE,0,0);
               SysFreeString(bstr);

               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_SETITEM,0L,(LPARAM)&lvItem);

               break;
            }

         }

         }
         break;

      case IDDI_GRAPHIC_TEXTS_DELETE: {

         LVITEM lvItem;
         char szTemp[MAX_PROPERTY_SIZE];
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_GETITEMCOUNT,0L,0L);
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         if ( ! itemCount ) {
            MessageBox(GetParent(hwnd),"There aren't any components to delete.","Note",MB_OK);
            break;
         }
         if ( ! selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select one or more text components, then select Delete.","Note",MB_OK);
            break;
         }
         long response = IDYES;
         if ( 1 < selectedCount ) 
            response = MessageBox(GetParent(hwnd),"Are you sure you want to delete the selected text components?","Note",MB_YESNO | MB_DEFBUTTON2);
         if ( IDYES == response ) {
            List<int> toDelete;
            memset(&lvItem,0,sizeof(LVITEM));
            lvItem.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
            lvItem.stateMask = LVIS_SELECTED;
            lvItem.pszText = szTemp;
            lvItem.cchTextMax = MAX_PROPERTY_SIZE;
            for ( int k = 0; k < itemCount; k++ ) {
               lvItem.iItem = k;
               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);
               if ( lvItem.state && LVIS_SELECTED ) {
                  if ( 1 == selectedCount ) {
                     char szPrompt[MAX_PROPERTY_SIZE];
                     sprintf(szPrompt,"Are you sure you want to delete the text component: %s?",szTemp);
                     response = MessageBox(GetParent(hwnd),szPrompt,"Note",MB_YESNO | MB_DEFBUTTON2);
                  }
                  if ( IDYES == response ) {
                     p -> deleteText(reinterpret_cast<IText *>(lvItem.lParam));
                     toDelete.Add(new int(lvItem.iItem));
                  }
               }
            }
            int* pi;
            while ( pi = toDelete.GetLast() ) {
               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_TEXTS_LIST,LVM_DELETEITEM,(WPARAM)*pi,0L);
               toDelete.Remove(pi);
               delete pi;
            }
         }

         }
         break;

      default:
         break;

      }

   default:
      break;
   }

   return LRESULT(FALSE);
   }
