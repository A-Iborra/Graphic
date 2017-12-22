/*

                       Copyright (c) 1996,1997,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Graphic.h"

#define SET_SUBSTYLES   \
   {                                                                                                           \
   long is3DDataArity = SendMessage(GetDlgItem(hwnd,IDMI_GRAPHIC_VIEW_3D),BM_GETCHECK,0L,0L);                  \
   EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL),TRUE);                                         \
   EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME),BST_CHECKED == is3DDataArity ? TRUE : FALSE);\
   EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_CONTOUR),BST_CHECKED == is3DDataArity ? TRUE : FALSE);  \
   EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_PIE),BST_CHECKED == is3DDataArity ? FALSE : TRUE);      \
   EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_SURFACE),BST_CHECKED == is3DDataArity ? TRUE : FALSE);  \
   EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS),BST_CHECKED == is3DDataArity ? TRUE : FALSE);   \
\
   SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL),BM_SETCHECK,BST_CHECKED == is3DDataArity ? p -> default3DPlotType & gcPlotTypeNatural : p -> default2DPlotType & gcPlotTypeNatural,0L);       \
   SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME),BM_SETCHECK,BST_CHECKED == is3DDataArity ? p -> default3DPlotType & gcPlotTypeWireFrame : p -> default2DPlotType & gcPlotTypeWireFrame,0L);   \
   SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_CONTOUR),BM_SETCHECK,BST_CHECKED == is3DDataArity ? p -> default3DPlotType & gcPlotTypeContour : p -> default2DPlotType & gcPlotTypeContour,0L);       \
   SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_PIE),BM_SETCHECK,BST_CHECKED == is3DDataArity ? p -> default3DPlotType & gcPlotTypePie : p -> default2DPlotType & gcPlotTypePie,0L);               \
   SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS),BM_SETCHECK,BST_CHECKED == is3DDataArity ? p -> default3DPlotType & gcPlotTypeStacks : p -> default2DPlotType & gcPlotTypeStacks,0L);         \
\
   }

#define GET_SUBSTYLES   \
   { \
   long pt = 0L;  \
   if ( BST_CHECKED == SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_NATURAL),BM_GETCHECK,0L,0L) ) pt |= (long)gcPlotTypeNatural;       \
   if ( BST_CHECKED == SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_WIREFRAME),BM_GETCHECK,0L,0L) ) pt |= (long)gcPlotTypeWireFrame;   \
   if ( BST_CHECKED == SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_CONTOUR),BM_GETCHECK,0L,0L) ) pt |= (long)gcPlotTypeContour;       \
   if ( BST_CHECKED == SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_PIE),BM_GETCHECK,0L,0L) ) pt |= (long)gcPlotTypePie;               \
   if ( BST_CHECKED == SendMessage(GetDlgItem(hwnd,IDDI_GRAPHIC_SUB_STYLE_STACKS),BM_GETCHECK,0L,0L) ) pt |= (long)gcPlotTypeStacks;         \
\
   if ( BST_CHECKED == SendMessage(GetDlgItem(hwnd,IDMI_GRAPHIC_VIEW_3D),BM_GETCHECK,0L,0L) ) \
      p -> default3DPlotType = pt;     \
   else                                \
      p -> default2DPlotType = pt;     \
\
   }


   LRESULT CALLBACK G::plotHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   G *p = (G *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = (PROPSHEETPAGE *)lParam;

      p = (G *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      p -> hwndPlotSettings = hwnd;

      char szTemp[128];
      sprintf(szTemp,"There are %ld plots on this graphic",p -> plotList.Count());
      SetWindowText(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_COUNT),szTemp);

      RECT rect;
      GetClientRect(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SAMPLE),&rect);
      MapWindowPoints(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SAMPLE),hwnd,reinterpret_cast<POINT *>(&rect),2);
      HWND hwndTemp = CreateWindowEx(WS_EX_CLIENTEDGE,"G-plotSettingsGraphic","",WS_VISIBLE | WS_CHILD,
                                       rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,hwnd,NULL,hModule,(void *)p);
   
      DestroyWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SAMPLE));

      SetWindowLongPtr(hwndTemp,GWLP_ID,IDDI_GRAPHIC_STYLE_SAMPLE);

      LVCOLUMN lvColumn;
      LVITEM lvItem;
      GetWindowRect(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_LIST),&rect);
      memset(&lvColumn,0,sizeof(LVCOLUMN));
      lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
      lvColumn.fmt = LVCFMT_LEFT;
      lvColumn.cx = rect.right - rect.left - 4;
      lvColumn.pszText = "Plot";
      SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn);
      IPlot *pIPlot = NULL;
      while ( pIPlot = p -> plotList.GetNext(pIPlot) ) {
         memset(&lvItem,0,sizeof(LVITEM));
         long n;
         BSTR bstrExpression;
         char szTemp[128];
         pIPlot -> get_Name(&bstrExpression);
         if ( 0 == wcslen(bstrExpression) ) 
            sprintf(szTemp,"Plot #%d",p -> plotList.ID(pIPlot));
         else         
            WideCharToMultiByte(CP_ACP,0,bstrExpression,-1,szTemp,64,0,0);
         SysFreeString(bstrExpression);
         lvItem.mask = LVIF_TEXT | LVIF_PARAM;
         lvItem.iItem = 32768;
         lvItem.state = LVIS_SELECTED;
         lvItem.stateMask = -1;
         lvItem.pszText = szTemp;
         lvItem.lParam = reinterpret_cast<long>(pIPlot);
         SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_INSERTITEM,0L,(LPARAM)&lvItem);
      }

      EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_EDIT),FALSE);

      char szInstructions[1024];

      LoadString(hModule,IDDI_GRAPHIC_PLOTS_INSTRUCTIONS,szInstructions,1024);

      SetDlgItemText(hwnd,IDDI_GRAPHIC_PLOTS_INSTRUCTIONS,szInstructions);

      SendMessage(GetDlgItem(hwnd,IDMI_GRAPHIC_VIEW_2D),BM_SETCHECK,(WPARAM)BST_CHECKED,0L);

      SET_SUBSTYLES

      }
      return LRESULT(FALSE);
 
   case WM_NOTIFY:
      switch ( wParam ) {
      case IDDI_GRAPHIC_PLOTS_LIST: {
         NMHDR *pHeader = (NMHDR *)lParam;
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEMCOUNT,0L,0L);
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         EnableWindow(GetDlgItem(hwnd,IDDI_GRAPHIC_PLOTS_EDIT),itemCount && selectedCount ? TRUE : FALSE);
         if ( pHeader -> code == LVN_ITEMCHANGED ) 
            InvalidateRect(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SAMPLE),NULL,TRUE);
         }
         break;
      }
      break;

   case WM_COMMAND:

      switch ( LOWORD(wParam) ) {

      case IDMI_GRAPHIC_VIEW_3D: {
         long rv = SendMessage((HWND)lParam,BM_GETCHECK,0L,0L);
         if ( rv == BST_CHECKED ) 
            p -> defaultPlotView = gcPlotView3D;
         else
            p -> defaultPlotView = gcPlotView2D;
         }
         SET_SUBSTYLES
         break;

      case IDMI_GRAPHIC_VIEW_2D: {
         long rv = SendMessage((HWND)lParam,BM_GETCHECK,0L,0L);
         if ( rv == BST_CHECKED ) 
            p -> defaultPlotView = gcPlotView2D;
         else
            p -> defaultPlotView = gcPlotView3D;
         SET_SUBSTYLES
         }
         break;

      case IDDI_GRAPHIC_SUB_STYLE_NATURAL:
      case IDDI_GRAPHIC_SUB_STYLE_WIREFRAME:
      case IDDI_GRAPHIC_SUB_STYLE_CONTOUR:
      case IDDI_GRAPHIC_SUB_STYLE_PIE:
      case IDDI_GRAPHIC_SUB_STYLE_STACKS:
         GET_SUBSTYLES
         break;


      case IDDI_GRAPHIC_PLOTS_EDIT: {
         long selectedCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETSELECTEDCOUNT,0L,0L);
         long itemCount = SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEMCOUNT,0L,0L);
         LVITEM lvItem;
         if ( ! itemCount ) {
            MessageBox(GetParent(hwnd),"There aren't any plots to edit.","Note",MB_OK);
            break;
         }
         if ( ! selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select one of the plots, then select Edit.","Note",MB_OK);
            break;
         }
         if ( 1 < selectedCount ) {
            MessageBox(GetParent(hwnd),"Please select only one of the plots, then select Edit.","Note",MB_OK);
            break;
         }
         for ( int k = 0; k < itemCount; k++ ) {
            memset(&lvItem,0,sizeof(LVITEM));
            lvItem.iItem = k;
            lvItem.mask = LVIF_STATE | LVIF_PARAM;
            lvItem.stateMask = LVIS_SELECTED;
            SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_GETITEM,0L,(LPARAM)&lvItem);
            if ( lvItem.state && LVIS_SELECTED ) {
               IPlot * pIPlot = (IPlot *)lvItem.lParam;
               IUnknown* pIUnknown;
               pIPlot -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
               p -> pIGProperties -> ShowProperties(hwnd,pIUnknown);
               pIUnknown -> Release();
               char szTemp[MAX_PROPERTY_SIZE];
               BSTR bstr = NULL;
               pIPlot -> get_Name(&bstr);
               if ( 0 < wcslen(bstr) ) {
                  WideCharToMultiByte(CP_ACP,0,bstr,MAX_PROPERTY_SIZE,szTemp,MAX_PROPERTY_SIZE,0,0);
                  lvItem.mask = LVIF_TEXT;
                  lvItem.pszText = szTemp;
                  SendDlgItemMessage(hwnd,IDDI_GRAPHIC_PLOTS_LIST,LVM_SETITEM,0L,(LPARAM)&lvItem);
               }
               break;
            }
         }
         }
         break;

      }

      InvalidateRect(GetDlgItem(hwnd,IDDI_GRAPHIC_STYLE_SAMPLE),NULL,TRUE);

      break;
 
   default:
      break;
   }
 
   return LRESULT(FALSE);
   }
