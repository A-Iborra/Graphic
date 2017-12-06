/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "utils.h"
#include "Graphic_resource.h"

#include "Plot.h"

   extern HMODULE hModule;


   HRESULT Plot::BeforeAllPropertyPages() {
   pIProperties -> Push();
   pIProperties -> Push();
   return S_OK;
   }


   HRESULT Plot::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {

   *pCntPages = 3;
   long index;

   SAFEARRAYBOUND rgsaBound;
   rgsaBound.cElements = *pCntPages;
   rgsaBound.lLbound = 0;

   SafeArrayRedim(*thePageNames,&rgsaBound);
   SafeArrayRedim(*theHelpDirs,&rgsaBound);
   rgsaBound.cElements = *pCntPages * 2;
   SafeArrayRedim(*pSize,&rgsaBound);

   index = 0; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Dimension"));
   index = 1; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Type"));
   index = 2; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Color"));

   index = 0; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index = 1; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index = 2; SafeArrayPutElement(*theHelpDirs,&index,NULL);

   long cx = 400,cy = 400;

   index = 0; SafeArrayPutElement(*pSize,&index,&cx);
   index = 1; SafeArrayPutElement(*pSize,&index,&cy);
   index = 2; SafeArrayPutElement(*pSize,&index,&cx);
   index = 3; SafeArrayPutElement(*pSize,&index,&cy);
   index = 4; SafeArrayPutElement(*pSize,&index,&cx);
   index = 5; SafeArrayPutElement(*pSize,&index,&cy);

   return S_OK;
   }


   HRESULT Plot::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {

   return E_NOTIMPL;
#if 0
   if ( ! pHwnd ) return E_POINTER;

   if ( ! hwndParent ) return E_POINTER;

   switch ( pageNumber ) {

   case 0: {

      DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_PLOT_DIMENSION),RT_DIALOG));
      hwndDimensionSettings = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)dimensionHandler,(long)(void *)this);

      if ( hwndParentWindow ) {
         RECT rectParent;
         RECT rectThis;
         long cx,cy;
         GetWindowRect(hwndParentWindow,&rectParent);
         GetWindowRect(hwndDimensionSettings,&rectThis);
         rectPropertiesPosition.left += rectParent.left;
         rectPropertiesPosition.top += rectParent.top;
         rectPropertiesPosition.left = max(0,rectPropertiesPosition.left);
         rectPropertiesPosition.top = max(0,rectPropertiesPosition.top);
         cx = GetSystemMetrics(SM_CXSCREEN);
         cy = GetSystemMetrics(SM_CYSCREEN);
         rectPropertiesPosition.left = min(rectPropertiesPosition.left,cx - (rectThis.right - rectThis.left));
         rectPropertiesPosition.top = min(rectPropertiesPosition.top,cy - (rectThis.bottom - rectThis.top));
         SetWindowPos(GetParent((HWND)hwndParent),HWND_TOP,max(0,rectPropertiesPosition.left),max(0,rectPropertiesPosition.top),0,0,SWP_NOSIZE);
      }

      PlotViews parentPlotView;
      pOwnerPropertyPlotView -> get_longValue((long*)&parentPlotView);

      if ( gcPlotView3D == parentPlotView ) 
         SetDlgItemText(hwndDimensionSettings,IDDI_PLOT_DIMENSION_OVERRIDE_OWNER,"Override the owner's 3-D transformation settings?");
      else
         SetDlgItemText(hwndDimensionSettings,IDDI_PLOT_DIMENSION_OVERRIDE_OWNER,"Override the owner's 2-D transformation settings?");

      SendMessage(GetDlgItem(hwndDimensionSettings,IDDI_PLOT_DIMENSION_OVERRIDE_OWNER),BM_SETCHECK,overrideOwnerView ? BST_CHECKED : BST_UNCHECKED,0L);

      if ( overrideOwnerView ) {
         SendMessage(GetDlgItem(hwndDimensionSettings,IDDI_PLOT_DIMENSION_3D),BM_SETCHECK,plotView == gcPlotView3D ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwndDimensionSettings,IDDI_PLOT_DIMENSION_2D),BM_SETCHECK,plotView == gcPlotView3D ? BST_UNCHECKED : BST_CHECKED,0L);
         EnableWindow(GetDlgItem(hwndDimensionSettings,IDDI_PLOT_DIMENSION_3D),TRUE);
         EnableWindow(GetDlgItem(hwndDimensionSettings,IDDI_PLOT_DIMENSION_2D),TRUE);
      } else {
         pOwnerPropertyPlotView -> get_longValue((long*)&parentPlotView);
         SendMessage(GetDlgItem(hwndDimensionSettings,IDDI_PLOT_DIMENSION_3D),BM_SETCHECK,parentPlotView == gcPlotView3D ? BST_CHECKED : BST_UNCHECKED,0L);
         SendMessage(GetDlgItem(hwndDimensionSettings,IDDI_PLOT_DIMENSION_2D),BM_SETCHECK,parentPlotView == gcPlotView3D ? BST_UNCHECKED : BST_CHECKED,0L);
         EnableWindow(GetDlgItem(hwndDimensionSettings,IDDI_PLOT_DIMENSION_3D),FALSE);
         EnableWindow(GetDlgItem(hwndDimensionSettings,IDDI_PLOT_DIMENSION_2D),FALSE);
      }

      *pHwnd = hwndDimensionSettings;

      }
      break;

   case 1: {

      DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_PLOT_TYPE),RT_DIALOG));
      hwndTypeSettings = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)typeHandler,(long)(void *)this);

      PlotTypes pType;

      if ( overrideOwnerType ) {
         pType = (PlotTypes)plotType;
         EnableWindow(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_OVERRIDE_OWNER),pOwnerPropertyPlotType ? TRUE : FALSE);
      } else
         pOwnerPropertyPlotType -> get_longValue(reinterpret_cast<long*>(&pType));

      propertyOverrideOwnerType -> setWindowChecked(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_OVERRIDE_OWNER));

      SendMessage(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_NATURAL),BM_SETCHECK,(long)gcPlotTypeNatural & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_CONTOUR),BM_SETCHECK,(long)gcPlotTypeContour & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_PIE),BM_SETCHECK,(long)gcPlotTypePie & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_STACKS),BM_SETCHECK,(long)gcPlotTypeStacks & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_SURFACE),BM_SETCHECK,(long)gcPlotTypeSurface & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_WIREFRAME),BM_SETCHECK,(long)gcPlotTypeWireFrame & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_QUADS),BM_SETCHECK,(long)gcPlotTypeQuads & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);
      SendMessage(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_TRIANGLES),BM_SETCHECK,(long)gcPlotTypeTriangles & (long)pType ? BST_CHECKED : BST_UNCHECKED,0L);

      EnableWindow(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_NATURAL),overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_CONTOUR),overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_PIE),overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_STACKS),overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_SURFACE),overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_WIREFRAME),overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_QUADS),overrideOwnerType ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTypeSettings,IDDI_PLOT_TYPE_TRIANGLES),overrideOwnerType ? TRUE : FALSE);

      *pHwnd = hwndTypeSettings;
      }
      break;

   case 2: {

      DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_PLOT_COLOR),RT_DIALOG));
      hwndColorSettings = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)colorHandler,(long)(void *)this);

      *pHwnd = hwndColorSettings;
      }
      break;

   }

   return S_OK;
#endif

   }


   HRESULT Plot::Apply() {
   pIProperties -> Discard();
   pIProperties -> Push();
   Erase();
   Draw();
   return S_OK;
   }


   HRESULT Plot::IsPageDirty(long hwndPage,BOOL* isDirty) {
   pIProperties -> Compare((short*)isDirty);
   return S_OK;
   }


   HRESULT Plot::Help(BSTR bstrHelpDir) {
   return  S_OK;
   }


   HRESULT Plot::TranslateAccelerator(long,long* pResult) {
   *pResult = S_FALSE;
   return S_OK;
   }


   HRESULT Plot::AfterAllPropertyPages(BOOL userCanceled) {
   if ( userCanceled ) {
      pIProperties -> Pop();
      pIProperties -> Pop();
   } else {
      pIProperties -> Discard();
      pIProperties -> Discard();
   }
   Draw();
   return S_OK;
   }



   HRESULT Plot::DestroyPropertyPage(long pageNumber) {

   switch ( pageNumber ) {

   case 0:
      if ( hwndParentWindow ) {
         RECT rectParent;  
         GetWindowRect(hwndParentWindow,&rectParent);
         GetWindowRect(GetParent(GetParent(hwndDimensionSettings)),&rectPropertiesPosition);
         rectPropertiesPosition.left -= rectParent.left;
         rectPropertiesPosition.right -= rectParent.left;
         rectPropertiesPosition.top -= rectParent.top;
         rectPropertiesPosition.bottom -= rectParent.bottom;
      }
      SetParent(hwndSample,hwndObjectWindow);
      DestroyWindow(hwndDimensionSettings);
      hwndDimensionSettings = NULL;
      break;

   case 1: 
      SetParent(hwndSample,hwndObjectWindow);
      DestroyWindow(hwndTypeSettings);
      hwndTypeSettings = NULL;
      break;

   case 2: 
      DestroyWindow(hwndColorSettings);
      hwndColorSettings = NULL;
      break;

   }

   return S_OK;
   }


   HRESULT Plot::GetPropertySheetHeader(void *pv) {

   if ( ! pv )
      return E_POINTER;

   PROPSHEETHEADER *pHeader = reinterpret_cast<PROPSHEETHEADER *>(pv);

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;
   pHeader -> hInstance = hModule;
   pHeader -> pszIcon = NULL;
   pHeader -> pszCaption = " Properties";
   pHeader -> pfnCallback = NULL;

   return S_OK;

   }


   HRESULT Plot::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 3;
   return S_OK;
   }


   HRESULT Plot::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_PLOT_DIMENSION);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)dimensionHandler;
   pPropSheetPages[0].pszTitle = "Dimensions";
   pPropSheetPages[0].lParam = (LPARAM)this;
   pPropSheetPages[0].pfnCallback = NULL;

   pPropSheetPages[1].dwFlags = PSP_USETITLE;
   pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[1].hInstance = hModule;
   pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_PLOT_TYPE);
   pPropSheetPages[1].pfnDlgProc = (DLGPROC)typeHandler;
   pPropSheetPages[1].pszTitle = "Plot Type";
   pPropSheetPages[1].lParam = (LPARAM)this;
   pPropSheetPages[1].pfnCallback = NULL;

   pPropSheetPages[2].dwFlags = PSP_USETITLE;
   pPropSheetPages[2].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[2].hInstance = hModule;
   pPropSheetPages[2].pszTemplate = MAKEINTRESOURCE(IDDIALOG_PLOT_COLOR);
   pPropSheetPages[2].pfnDlgProc = (DLGPROC)colorHandler;
   pPropSheetPages[2].pszTitle = "Color";
   pPropSheetPages[2].lParam = (LPARAM)this;
   pPropSheetPages[2].pfnCallback = NULL;

   return S_OK;
   }

