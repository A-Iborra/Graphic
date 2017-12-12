/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>
#include <olectl.h>
#include <stdio.h>

#include <list>

#include "Graphic_resource.h"
#include "utils.h"

#include "Graphic.h"
#include "List.cpp"


   LRESULT CALLBACK G::functionHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndFunctionSettings = hwnd;

      char szTemp[32];
      sprintf(szTemp,"There are %ld functions defined",p -> functionList.Count());
      SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_FUNCTIONS_COUNT),szTemp);

      RECT rect;

      GetClientRect(GetDlgItem(p -> hwndFunctionSettings,IDDI_GRAPHIC_PLOTS_GRAPHIC),&rect);

      LVCOLUMN lvColumn;
      LVITEM lvItem;
      GetWindowRect(GetDlgItem(p -> hwndFunctionSettings,IDDI_GRAPHIC_FUNCTIONS_LIST),&rect);
      memset(&lvColumn,0,sizeof(LVCOLUMN));
      lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
      lvColumn.fmt = LVCFMT_LEFT;
      lvColumn.cx = rect.right - rect.left - 4;
      lvColumn.pszText = "expression";
      SendDlgItemMessage(p -> hwndFunctionSettings,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn);
      IGSFunctioNater *pIFunction = NULL;
      while ( pIFunction = p -> functionList.GetNext(pIFunction) ) {//for ( IGSFunctioNater *pIFunction : p -> functionList ) {
         memset(&lvItem,0,sizeof(LVITEM));
         long n;
         BSTR bstrExpression;
         pIFunction -> get_Expression(&bstrExpression);
         char* pszTemp = new char[n = wcslen(bstrExpression) + 1];
         memset(pszTemp,0,n);
         WideCharToMultiByte(CP_ACP,0,bstrExpression,n,pszTemp,n,0,0);
         SysFreeString(bstrExpression);
         lvItem.mask = LVIF_TEXT | LVIF_PARAM;
         lvItem.iItem = 32768;
         lvItem.state = LVIS_SELECTED;
         lvItem.stateMask = -1;
         lvItem.pszText = pszTemp;
         lvItem.lParam = reinterpret_cast<long>(pIFunction);
         SendDlgItemMessage(p -> hwndFunctionSettings,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_INSERTITEM,0L,(LPARAM)&lvItem);
      }

      if ( p -> isRunning )
         ShowWindow(GetDlgItem(p -> hwndFunctionSettings,IDDI_GRAPHIC_FUNCTIONS_ALLOWCTLVISPROPS),SW_HIDE);
      else
         p -> propertyAllowUserFunctionControlVisibilityAccess -> setWindowItemChecked(p -> hwndFunctionSettings,IDDI_GRAPHIC_FUNCTIONS_ALLOWCTLVISPROPS);

      }
      return LRESULT(FALSE);
 
   case WM_NOTIFY:
      switch ( wParam ) {
      case IDDI_GRAPHIC_FUNCTIONS_LIST: {
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETITEMCOUNT,0L,0L);
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_FUNCTIONS_EDIT),itemCount && selectedCount ? TRUE : FALSE);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_FUNCTIONS_DELETE),itemCount && selectedCount ? TRUE : FALSE);
         }
         break;
      }
      break;

   case WM_COMMAND:

      switch ( LOWORD(wParam) ) {

      case IDDI_GRAPHIC_FUNCTIONS_ADD: {
         IGSFunctioNater* pFunction;
         IUnknown* pIUnknown;
         pFunction = p -> newFunction(true);
         pFunction -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIGProperties -> ShowProperties(p -> hwndOwner,pIUnknown);
         pIUnknown -> Release();
         char szTemp[MAX_PROPERTY_SIZE];
         BSTR bstrExpression;
         pFunction -> get_Expression(&bstrExpression);
         memset(szTemp,0,sizeof(szTemp));
         WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,szTemp,sizeof(szTemp),0,0);
         SysFreeString(bstrExpression);
         if ( szTemp[0] ) {
            LVITEM lvItem;
            memset(&lvItem,0,sizeof(LVITEM));
            lvItem.mask = LVIF_TEXT | LVIF_PARAM;
            lvItem.iItem = 32768;
            lvItem.state = LVIS_SELECTED;
            lvItem.stateMask = -1;
            lvItem.pszText = szTemp;
            lvItem.lParam = reinterpret_cast<long>(pFunction);
            SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_INSERTITEM,0L,(LPARAM)&lvItem);
            sprintf(szTemp,"There are %ld functions defined",p -> functionList.Count());
            SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_FUNCTIONS_COUNT),szTemp);
            EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_FUNCTIONS_EDIT),TRUE);
            EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_FUNCTIONS_DELETE),TRUE);
            ContainedFunction *pf = (ContainedFunction *)NULL;
            while ( pf = p -> containedFunctionList.GetNext(pf) )
               ShowWindow(pf -> HWNDSite(),SW_HIDE);
            SendMessage(p -> hwndDataSourcesFunctions,TCM_SETCURSEL,(WPARAM)(SendMessage(p -> hwndDataSourcesFunctions,TCM_GETITEMCOUNT,0L,0L) - 1),0L);
            pf = p -> containedFunctionList.Get(reinterpret_cast<long>(pFunction));
            ShowWindow(pf -> HWNDSite(),SW_SHOW);
         } else {
            p -> deleteFunction(pFunction);
         }
         }
         break;

      case IDDI_GRAPHIC_FUNCTIONS_EDIT: {
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETITEMCOUNT,0L,0L);
         LVITEM lvItem;
         if ( ! itemCount ) {
            MessageBox(GetParent(hwnd),"There aren't any functions to edit.","Note",MB_OK);
            break;
         }
         if ( ! selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select one of the expressions, then select Edit.","Note",MB_OK);
            break;
         }
         if ( 1 < selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select only one of the expressions, then select Edit.","Note",MB_OK);
            break;
         }
         for ( int k = 0; k < itemCount; k++ ) {
            memset(&lvItem,0,sizeof(LVITEM));
            lvItem.iItem = k;
            lvItem.mask = LVIF_STATE | LVIF_PARAM;
            lvItem.stateMask = LVIS_SELECTED;
            SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);
            if ( lvItem.state && LVIS_SELECTED ) {
               char szTemp[MAX_PROPERTY_SIZE];
               BSTR bstr;
               IGSFunctioNater* pf = reinterpret_cast<IGSFunctioNater*>(lvItem.lParam);
               IUnknown* pIUnknown;
               pf -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
               p -> pIGProperties -> ShowProperties(hwnd,pIUnknown);
               lvItem.mask = LVIF_TEXT;
               lvItem.pszText = szTemp;
               bstr = SysAllocStringLen(NULL,MAX_PROPERTY_SIZE);
               pf -> get_Expression(&bstr);
               memset(szTemp,0,sizeof(szTemp));
               WideCharToMultiByte(CP_ACP,0,bstr,MAX_PROPERTY_SIZE,szTemp,MAX_PROPERTY_SIZE,0,0);
               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_SETITEM,0L,(LPARAM)&lvItem);
               pIUnknown -> Release();
               break;
            }
         }
         }
         break;

      case IDDI_GRAPHIC_FUNCTIONS_DELETE: {
         LVITEM lvItem;
         char szTemp[MAX_PROPERTY_SIZE];
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETITEMCOUNT,0L,0L);
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         if ( ! itemCount ) {
            MessageBox(GetParent(hwnd),"There aren't any functions to delete.","Note",MB_OK);
            break;
         }
         if ( ! selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select one or more of the expressions, then select Delete.","Note",MB_OK);
            break;
         }
         long response = IDYES;
         if ( 1 < selectedCount ) 
            response = MessageBox(GetParent(hwnd),"Are you sure you want to delete the selected functions?","Note",MB_YESNO | MB_DEFBUTTON2);
         if ( IDYES == response ) {
            List<int> toDelete;
            memset(&lvItem,0,sizeof(LVITEM));
            lvItem.mask = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
            lvItem.stateMask = LVIS_SELECTED;
            lvItem.pszText = szTemp;
            lvItem.cchTextMax = MAX_PROPERTY_SIZE;
            for ( int k = 0; k < itemCount; k++ ) {
               lvItem.iItem = k;
               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);
               if ( lvItem.state && LVIS_SELECTED ) {
                  if ( 1 == selectedCount ) {
                     char szPrompt[MAX_PROPERTY_SIZE];
                     sprintf(szPrompt,"Are you sure you want to delete the function: %s?",szTemp);
                     response = MessageBox(GetParent(hwnd),szPrompt,"Note",MB_YESNO | MB_DEFBUTTON2);
                  }
                  if ( IDYES == response ) {
                     p -> deleteFunction(reinterpret_cast<IGSFunctioNater*>(lvItem.lParam));
                     toDelete.Add(new int(lvItem.iItem));
                  }
               }
            }
            int* pi;
            while ( pi = toDelete.GetLast() ) {
               SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_DELETEITEM,(WPARAM)*pi,0L);
               toDelete.Remove(pi);
               delete pi;
            }
         }
         }   
         break;

      case IDDI_GRAPHIC_FUNCTIONS_ALLOWCTLVISPROPS:
         p -> propertyAllowUserFunctionControlVisibilityAccess -> getWindowItemChecked(hwnd,IDDI_GRAPHIC_FUNCTIONS_ALLOWCTLVISPROPS);
         break;

      case IDDI_GRAPHIC_FUNCTIONS_LIST:
         errorSound();
         break;
      }
 
   case WM_PAINT: {
      long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETITEMCOUNT,0L,0L);
      long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_FUNCTIONS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_FUNCTIONS_EDIT),itemCount && selectedCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_FUNCTIONS_DELETE),itemCount && selectedCount ? TRUE : FALSE);
      }
      break;

   default:
      break;
   }
 
   return LRESULT(FALSE);
   }


