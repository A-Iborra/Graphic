/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "utils.h"
#include "Graphic_resource.h"
#include "Axis.h"


   extern HMODULE hModule;


   HRESULT Axis::BeforeAllPropertyPages() {
   pIGProperties -> Push();
   pIGProperties -> Push();
   return S_OK;
   }


   HRESULT Axis::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {

#if 1
   return E_NOTIMPL;
#else
   *pCntPages = 4;
   long index;

   SAFEARRAYBOUND rgsaBound;
   rgsaBound.cElements = *pCntPages;
   rgsaBound.lLbound = 0;

   SafeArrayRedim(*thePageNames,&rgsaBound);
   SafeArrayRedim(*theHelpDirs,&rgsaBound);
   rgsaBound.cElements = *pCntPages * 2;
   SafeArrayRedim(*pSize,&rgsaBound);

   index = 0; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Markings"));
   index = 1; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Position"));
   index = 2; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Labelling"));
   index = 3; SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Color"));

   index = 0; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index = 1; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index = 2; SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index = 3; SafeArrayPutElement(*theHelpDirs,&index,NULL);

   long cx = 350,cy = 320;

   index = 0; SafeArrayPutElement(*pSize,&index,&cx);
   index = 1; SafeArrayPutElement(*pSize,&index,&cy);
   index = 2; SafeArrayPutElement(*pSize,&index,&cx);
   index = 3; SafeArrayPutElement(*pSize,&index,&cy);
   index = 4; SafeArrayPutElement(*pSize,&index,&cx);
   index = 5; SafeArrayPutElement(*pSize,&index,&cy);
   index = 6; SafeArrayPutElement(*pSize,&index,&cx);
   index = 7; SafeArrayPutElement(*pSize,&index,&cy);

   return S_OK;
#endif
   }


   HRESULT Axis::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {

#if 1
   return E_NOTIMPL;
#else
   if ( ! pHwnd ) return E_POINTER;

   if ( ! hwndParent ) return E_POINTER;

   PROPSHEETPAGE propSheetPage = {0};

   propSheetPage.lParam = (LPARAM)this;

   switch ( pageNumber ) {

   case 0: {

      char szTemp[32];
      long v;

      DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_AXIS_STYLE),RT_DIALOG));
      hwndStyleSettings = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)styleHandler,(LPARAM)&propSheetPage);

      propertyLineWeight -> setWindowItemText(hwndStyleSettings,IDDI_AXIS_STYLE_LINEWEIGHT);

      propertyGridLinesPerTick -> setWindowItemText(hwndStyleSettings,IDDI_AXIS_STYLE_GRIDSPERTICK);

      sprintf(szTemp,"%6.2lf",tickLength);
      SetWindowText(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKLENGTH),szTemp);

      propertyTickLengthUnits -> get_longValue(&v);

      SendMessage(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKLENGTH_UNITS),CB_GETLBTEXT,v,(LPARAM)szTemp);
      SendMessage(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKLENGTH_UNITS),CB_SELECTSTRING,0,(LPARAM)szTemp);

      propertyTickCount -> setWindowItemText(hwndStyleSettings,IDDI_AXIS_STYLE_TICKCOUNT);

      propertyTickStraddleStyle -> get_longValue(&v);

      switch ( v ) {
      case TICK_STRADDLE_ABOVE:
         SendMessage(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKSABOVE),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
         break;
      case TICK_STRADDLE_BOTH:
         SendMessage(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKSACCROSS),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
         ShowWindow(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_MSG_PCNTABOVE),SW_SHOW);
         ShowWindow(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_PCNTABOVE),SW_SHOW);
         break;
      case TICK_STRADDLE_BELOW:
         SendMessage(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKSBELOW),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
         break;
      }

      sprintf(szTemp,"%5.1lf",tickPctAbove);
      SetWindowText(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_PCNTABOVE),szTemp);

      EnableWindow(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKLENGTH),tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKLENGTH_UNITS),tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKSABOVE),tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKSACCROSS),tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_TICKSBELOW),tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_PCNTABOVE),tickCount ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndStyleSettings,IDDI_AXIS_STYLE_GRIDSPERTICK),tickCount ? TRUE : FALSE);

      *pHwnd = hwndStyleSettings;

      if ( hwndOwner ) {
         RECT rectParent;
         RECT rectThis;
         long cx,cy;
         GetWindowRect(hwndOwner,&rectParent);
         GetWindowRect(hwndStyleSettings,&rectThis);
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

      }
      break;

   case 1: {

      short b;

      DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_AXIS_POSITION),RT_DIALOG));
      hwndPositionSettings = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)positionHandler,(LPARAM)&propSheetPage);

      propertyOriginAtMinpoint -> setWindowItemChecked(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_MINPOINT);
      propertyOriginXValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_X);
      propertyOriginYValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_Y);
      propertyOriginZValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_Z);

      propertyOriginAtMinpoint -> get_boolValue(&b);
      EnableWindow(GetDlgItem(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_X),b ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_Y),b ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_Z),b ? FALSE : TRUE);
      propertyOriginXValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_X);
      propertyOriginYValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_Y);
      propertyOriginZValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ORIGIN_Z);

      propertyEndpointAtMaxpoint -> setWindowItemChecked(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_MAXPOINT);
      propertyEndpointXValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_X);
      propertyEndpointYValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_Y);
      propertyEndpointZValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_Z);

      propertyEndpointAtMaxpoint -> get_boolValue(&b);
      EnableWindow(GetDlgItem(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_X),b ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_Y),b ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_Z),b ? FALSE : TRUE);
      propertyEndpointXValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_X);
      propertyEndpointYValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_Y);
      propertyEndpointZValue -> setWindowItemText(hwndPositionSettings,IDDI_AXIS_POSITION_ENDPOINT_Z);

      RECT rect;  
      GetWindowRect((HWND)hwndParent,&rect);
      SetWindowPos(GetDlgItem(hwndPositionSettings,IDDI_AXIS_POSITION_NOTES),HWND_TOP,0,0,rect.right - rect.left - 40,64,SWP_NOMOVE);

      propertyDeterminesDomain -> setWindowItemChecked(hwndPositionSettings,IDDI_AXIS_STYLE_DETERMINESDOMAIN);
      switch ( type ) {
      case 'X':
         SetWindowText(GetDlgItem(hwndPositionSettings,IDDI_AXIS_STYLE_DETERMINESDOMAIN),"These values determine the X domain");
         break;
      case 'Y':
         SetWindowText(GetDlgItem(hwndPositionSettings,IDDI_AXIS_STYLE_DETERMINESDOMAIN),"These values determine the Y domain");
         break;
      case 'Z':
         SetWindowText(GetDlgItem(hwndPositionSettings,IDDI_AXIS_STYLE_DETERMINESDOMAIN),"These values determine the Z domain");
         break;
      default:
         SetWindowText(GetDlgItem(hwndPositionSettings,IDDI_AXIS_STYLE_DETERMINESDOMAIN),"These values determine the axis domain");
         break;
      }

      *pHwnd = hwndPositionSettings;
      }
      break;

   case 2: {

      long v;
      double d;
      char szTemp[32];

      DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_AXIS_TEXT),RT_DIALOG));
      hwndTextSettings = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)textHandler,(LPARAM)&propSheetPage);

      if ( doAutoPrecision ) {
         SendMessage(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_AUTOPRECISION),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
      } else {
         SendMessage(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_SPECIFICPRECISION),BM_SETCHECK,(WPARAM)BST_CHECKED,0);
      }

      propertyAxisPrecision -> get_doubleValue(&d);
      sprintf(szTemp,"%8.3f",d);
      SetDlgItemText(hwndTextSettings,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE,szTemp);

      propertyLabel -> setWindowText(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABELTEXT));
      long n;
      propertyLabel -> get_size(&n);

      propertyDrawLabel -> put_boolValue(1 < n ? TRUE : FALSE);

      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_POSITION_NATURAL),drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_POSITION_X),drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_POSITION_Y),drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_POSITION_Z),drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_ANGLE_PERPENDICULAR),drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_ANGLE_PARALLEL),drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_ANGLE_ISOTHER),drawAxisLabel ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER),drawAxisLabel ? TRUE : FALSE);

      propertyLabelPositionNatural -> setWindowItemChecked(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_POSITION_NATURAL);
      propertyLabelAnglePerpendicular -> setWindowItemChecked(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_ANGLE_PERPENDICULAR);
      propertyLabelAngleParallel -> setWindowItemChecked(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_ANGLE_PARALLEL);
      propertyLabelAngleOther -> setWindowItemChecked(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_ANGLE_ISOTHER);
      propertyLabelAngleOtherValue -> setWindowItemText(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER);

      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_ANGLE_OTHER),labelAngleOther ? TRUE : FALSE);

      propertyLabelPositionNatural -> setWindowItemChecked(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_POSITION_NATURAL);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_POSITION_X),labelPositionNatural ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_POSITION_Y),labelPositionNatural ? FALSE : TRUE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_LABEL_POSITION_Z),labelPositionNatural ? FALSE : TRUE);

      propertyTickLabelSize -> get_doubleValue(&d);
      sprintf(szTemp,"%6.1f",d);
      SetDlgItemText(hwndTextSettings,IDDI_AXIS_TEXT_TICK_LABEL_SIZE,szTemp);
      propertyTickLabelSizeUnits -> get_longValue(&v);
      SendMessage(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS),CB_GETLBTEXT,v,(LPARAM)szTemp);
      SendMessage(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS),CB_SELECTSTRING,0,(LPARAM)szTemp);

      propertyDrawText -> setWindowItemChecked(hwndTextSettings,IDDI_AXIS_TEXT_DISPLAYTICKLABELS);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_SPECIFICPRECISION),drawTickLabels ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_AUTOPRECISION),drawTickLabels ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_AXISTICKLABELPRECISION_VALUE),drawTickLabels && ! doAutoPrecision ? TRUE : FALSE);

      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_TICK_LABEL_SIZE),drawTickLabels ? TRUE : FALSE);
      EnableWindow(GetDlgItem(hwndTextSettings,IDDI_AXIS_TEXT_TICK_LABEL_SIZE_UNITS),drawTickLabels ? TRUE : FALSE);

      *pHwnd = hwndTextSettings;

      }
      break;

   case 3: {

      if ( axisTickColorTrackLineColor ) {
         currentPropertyTickLabelColor = propertyLineColor;
      } else {
         currentPropertyTickLabelColor = propertyTickLabelColor;
      }

      if ( axisLabelColorTrackLineColor ) {
         currentPropertyAxisLabelColor = propertyLineColor;
      } else {
         currentPropertyAxisLabelColor = propertyLabelColor;
      }

      DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_AXIS_COLOR),RT_DIALOG));
      hwndColorSettings = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)colorHandler,(LPARAM)&propSheetPage);

      *pHwnd = hwndColorSettings;
      }
      break;

   }

   return S_OK;
#endif
   }


   HRESULT Axis::Apply() {
#if 1
   return E_NOTIMPL;
#else
   pIGProperties -> Discard();
   pIGProperties -> Push();
   if ( currentPropertyTickLabelColor ) {
      if ( axisTickColorTrackLineColor ) {
         currentPropertyTickLabelColor -> copyTo(propertyTickLabelColor);
      }
   }
   if ( currentPropertyAxisLabelColor ) {
      if ( axisLabelColorTrackLineColor ) {
         currentPropertyAxisLabelColor -> copyTo(propertyLabelColor);
      }
   }
   Erase();
   PrepData();
   PrepText();
   Draw();
   return S_OK;
#endif
   }


   HRESULT Axis::IsPageDirty(long hwndPage,BOOL* isDirty) {
   pIGProperties -> Compare((short*)isDirty);
   return S_OK;
   }


   HRESULT Axis::Help(BSTR bstrHelpDir) {
   return  S_OK;
   }


   HRESULT Axis::TranslateAccelerator(long,long* pResult) {
   *pResult = S_FALSE;
   return S_OK;
   }


   HRESULT Axis::AfterAllPropertyPages(BOOL userCanceled) {
   if ( userCanceled ) {
      pIGProperties -> Pop();
      pIGProperties -> Pop();
   } else {
      pIGProperties -> Discard();
      pIGProperties -> Discard();
   }
   Draw();
   return S_OK;
   }



   HRESULT Axis::DestroyPropertyPage(long pageNumber) {
#if 1
Beep(2000,100);
   return E_NOTIMPL;
#else

   switch ( pageNumber ) {

   case 0:
      if ( hwndOwner ) {
         RECT rectParent;  
         GetWindowRect(hwndOwner,&rectParent);
         GetWindowRect(GetParent(GetParent(hwndStyleSettings)),&rectPropertiesPosition);
         rectPropertiesPosition.left -= rectParent.left;
         rectPropertiesPosition.right -= rectParent.left;
         rectPropertiesPosition.top -= rectParent.top;
         rectPropertiesPosition.bottom -= rectParent.bottom;
      }
      DestroyWindow(hwndStyleSettings);
      break;

   case 1: 
      DestroyWindow(hwndPositionSettings);
      break;

   case 2: 
      DestroyWindow(hwndTextSettings);
      break;

   case 3:
      DestroyWindow(hwndColorSettings);
      currentPropertyTickLabelColor = NULL;
      currentPropertyAxisLabelColor = NULL;
      break;

   }

   return S_OK;
#endif
   }


   HRESULT Axis::GetPropertySheetHeader(void *pv) {
   
   if ( ! pv )
      return E_POINTER;

   PROPSHEETHEADER *pHeader = reinterpret_cast<PROPSHEETHEADER *>(pv);

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;
   pHeader -> hInstance = hModule;
   pHeader -> pszIcon = NULL;

   switch ( type ) {
   case 'X':
      pHeader -> pszCaption = " X-Axis Properties";
      break;
   case 'Y':
      pHeader -> pszCaption = " Y-Axis Properties";
      break;
   case 'Z':
      pHeader -> pszCaption = " Z-Axis Properties";
      break;
   }

   pHeader -> pfnCallback = NULL;

   return S_OK;
   }


   HRESULT Axis::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 4;
   return S_OK;
   }


   HRESULT Axis::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_AXIS_STYLE);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)styleHandler;
   pPropSheetPages[0].pszTitle = "Style";
   pPropSheetPages[0].lParam = (LPARAM)this;
   pPropSheetPages[0].pfnCallback = NULL;

   pPropSheetPages[1].dwFlags = PSP_USETITLE;
   pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[1].hInstance = hModule;
   pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_AXIS_POSITION);
   pPropSheetPages[1].pfnDlgProc = (DLGPROC)positionHandler;
   pPropSheetPages[1].pszTitle = "Position";
   pPropSheetPages[1].lParam = (LPARAM)this;
   pPropSheetPages[1].pfnCallback = NULL;

   pPropSheetPages[2].dwFlags = PSP_USETITLE;
   pPropSheetPages[2].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[2].hInstance = hModule;
   pPropSheetPages[2].pszTemplate = MAKEINTRESOURCE(IDDIALOG_AXIS_TEXT);
   pPropSheetPages[2].pfnDlgProc = (DLGPROC)textHandler;
   pPropSheetPages[2].pszTitle = "Text";
   pPropSheetPages[2].lParam = (LPARAM)this;
   pPropSheetPages[2].pfnCallback = NULL;

   //if ( axisTickColorTrackLineColor )
   //   currentPropertyTickLabelColor = propertyLineColor;
   //else 
   //   currentPropertyTickLabelColor = propertyTickLabelColor;

   //if ( axisLabelColorTrackLineColor )
   //   currentPropertyAxisLabelColor = propertyLineColor;
   //else
   //   currentPropertyAxisLabelColor = propertyLabelColor;

   pPropSheetPages[3].dwFlags = PSP_USETITLE;
   pPropSheetPages[3].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[3].hInstance = hModule;
   pPropSheetPages[3].pszTemplate = MAKEINTRESOURCE(IDDIALOG_AXIS_COLOR);
   pPropSheetPages[3].pfnDlgProc = (DLGPROC)colorHandler;
   pPropSheetPages[3].pszTitle = "Color";
   pPropSheetPages[3].lParam = (LPARAM)this;
   pPropSheetPages[3].pfnCallback = NULL;

   return S_OK;
   }

