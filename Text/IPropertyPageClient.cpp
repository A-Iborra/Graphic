/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "Graphic_resource.h"
#include "utils.h"

#include "Text.h"


   extern HMODULE hModule;

   HRESULT Text::BeforeAllPropertyPages() {
   pIProperties -> Push();
   pIProperties -> Push();
   RECT rect;
   BYTE *pb = (BYTE *)&rect;
   propertyPropertiesPosition -> get_binaryValue(sizeof(RECT),(BYTE**)&pb);
   SetWindowPos(hwndObjectWindow,HWND_TOP,rect.left,rect.top,0,0,SWP_NOSIZE);
   return S_OK;
   }


   HRESULT Text::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {

   long index;

   *pCntPages = showContentPropertyPage ? 3 : 2;

   SAFEARRAYBOUND rgsaBound;
   rgsaBound.cElements = *pCntPages;
   rgsaBound.lLbound = 0;

   SafeArrayRedim(*thePageNames,&rgsaBound);
   SafeArrayRedim(*theHelpDirs,&rgsaBound);

   rgsaBound.cElements = 2 * (*pCntPages);
   SafeArrayRedim(*pSize,&rgsaBound);

   index = 0; 
   if ( showContentPropertyPage ) {
      SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Content"));
      index++;
   }
   SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Font"));
   index++;
   SafeArrayPutElement(*thePageNames,&index,SysAllocString(L"Orientation"));

   index = 0; 
   if ( showContentPropertyPage ) {
      SafeArrayPutElement(*theHelpDirs,&index,NULL);
      index++;
   }
   SafeArrayPutElement(*theHelpDirs,&index,NULL);
   index++;
   SafeArrayPutElement(*theHelpDirs,&index,NULL);

   long cx = 340,cy = 306;

   index = 0; SafeArrayPutElement(*pSize,&index,&cx);
   index = 1; SafeArrayPutElement(*pSize,&index,&cy);
   index = 2; SafeArrayPutElement(*pSize,&index,&cx);
   index = 3; SafeArrayPutElement(*pSize,&index,&cy);
   if ( showContentPropertyPage ) {
      index = 4; SafeArrayPutElement(*pSize,&index,&cx);
      index = 5; SafeArrayPutElement(*pSize,&index,&cy);
   }

   return S_OK;
   }


   HRESULT Text::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND* pHwnd) {

   if ( ! pHwnd ) return E_POINTER;

   if ( ! hwndParent ) return E_POINTER;

   long mappedPageNumber = pageNumber + 1;

   if ( showContentPropertyPage )
      mappedPageNumber = pageNumber;

   switch ( mappedPageNumber ) {

   case 0: {

      DLGTEMPLATE* dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_TEXT_CONTENT),RT_DIALOG));
      hwndContent = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)contentHandler,reinterpret_cast<long>(this));

//FIXME      propertyText -> setWindowItemText((long)hwndContent,IDDI_TEXT_TEXT);

      long n;
      propertyDescription -> get_size(&n);
      if ( n > 1 ) {
         char *pszDescription = new char[n + 1];
         propertyDescription -> get_szValue(pszDescription);
         SetWindowText(GetDlgItem(hwndContent,IDDI_TEXT_DESCRIPTION),pszDescription);
         RECT rect;

         GetClientRect(GetDlgItem(hwndContent,IDDI_TEXT_DESCRIPTION),&rect);
         SetWindowPos(GetDlgItem(hwndContent,IDDI_TEXT_DESCRIPTION),HWND_TOP,rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top + 16,0L);

         GetClientRect(GetDlgItem(hwndContent,IDDI_TEXT_TEXT),&rect);
         SetWindowPos(GetDlgItem(hwndContent,IDDI_TEXT_TEXT),HWND_TOP,rect.left,rect.top + 16,rect.right - rect.left,rect.bottom - rect.top - 16,0L);

         delete [] pszDescription;
      }

      *pHwnd = hwndContent;
      }
      break;

   case 1: {

      DLGTEMPLATE* dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_TEXT_STYLE),RT_DIALOG));
      hwndStyle = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)styleHandler,reinterpret_cast<long>(this));
   
      HWND hwndListBox = GetDlgItem(hwndStyle,IDDI_TEXT_FONTLIST);
   
      HDC hdc = GetDC(0);
      LOGFONT logFont;
    
      memset(&logFont,0,sizeof(LOGFONT));
    
      logFont.lfCharSet = DEFAULT_CHARSET;

      logFont.lfFaceName[0] = '\0';
    
      EnumFontFamiliesEx(hdc,&logFont,fontListEnumerator,reinterpret_cast<long>(hwndListBox),0L);
   
      DeleteDC(hdc); 
   
      long n = strlen(szFace);
      char *pszFontName = new char[n + 1];
      char *pszFontStyle;
      memset(pszFontName,0,(n + 1) * sizeof(char));
      strcpy(pszFontName,szFace);
   
      n = SendMessage(hwndListBox,CB_FINDSTRINGEXACT,-1L,(LPARAM)pszFontName);
      SendMessage(hwndListBox,CB_SETCURSEL,(WPARAM)n,0L);
   
      SendMessage(hwndStyle,WM_COMMAND,MAKEWPARAM(IDDI_TEXT_FONTLIST,CBN_SELCHANGE),(LPARAM)GetDlgItem(hwndStyle,IDDI_TEXT_FONTLIST));
   
      delete [] pszFontName;
   
      propertyFaceStyle -> get_size(&n);
      if ( n ) {
         pszFontStyle = new char[n + 1];
         propertyFaceStyle -> get_szValue(pszFontStyle);
      } else {
         pszFontStyle = new char[32];
         sprintf(pszFontStyle,"Regular");
      }
   
      n = SendMessage(GetDlgItem(hwndStyle,IDDI_TEXT_FONTSTYLELIST),CB_FINDSTRINGEXACT,-1L,reinterpret_cast<LPARAM>(pszFontStyle));
      if ( n == CB_ERR ) {
         propertyFaceStyle -> put_szValue("Regular");
         n = SendMessage(GetDlgItem(hwndStyle,IDDI_TEXT_FONTSTYLELIST),CB_SELECTSTRING,-1L,reinterpret_cast<LPARAM>("Regular"));
      }
      SendMessage(GetDlgItem(hwndStyle,IDDI_TEXT_FONTSTYLELIST),CB_SETCURSEL,n,0L);
   
      delete [] pszFontStyle;
   
      char szTemp[32];
      long fontSizeUnits;
      sprintf(szTemp,"%5.1f",fontSize);
      SetDlgItemText(hwndStyle,IDDI_TEXT_FONTSIZE,szTemp);
   
      long k = 0;
      char unitsArray[][32] = TEXT_UNITS_ARRAY;
      SendMessage(GetDlgItem(hwndStyle,IDDI_TEXT_FONTSIZEUNITSLIST),CB_RESETCONTENT,0L,0L);
      while ( *unitsArray[k] ) 
         SendMessage(GetDlgItem(hwndStyle,IDDI_TEXT_FONTSIZEUNITSLIST),CB_ADDSTRING,0,(LPARAM)unitsArray[k++]);
   
      propertySizeUnits -> get_longValue(&fontSizeUnits);
      SendMessage(GetDlgItem(hwndStyle,IDDI_TEXT_FONTSIZEUNITSLIST),CB_SETCURSEL,fontSizeUnits,0L);
   
      *pHwnd = hwndStyle;
      }
      break;

   case 2: {

      DLGTEMPLATE* dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_TEXT_ORIENTATION),RT_DIALOG));
      hwndOrientation = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)orientationHandler,NULL);
 
      switch ( coordinatePlane ) {
      case CoordinatePlane_XY:
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_XYPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_XZ:
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_XZPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_YX:
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_YXPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_YZ:
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_YZPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_ZX:
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_ZXPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_ZY:
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_ZYPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      case CoordinatePlane_screen:
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_SCREENPLANE,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         break;
      }

      if ( flipVertical ) 
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_FLIP_TB,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);

      if ( flipHorizontal ) 
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_FLIP_LR,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);

      SendDlgItemMessage(hwndOrientation,IDDI_TEXT_PLANEHEIGHT,TBM_SETSEL,(WPARAM)FALSE,MAKELPARAM(0,100));
      SendDlgItemMessage(hwndOrientation,IDDI_TEXT_PLANEHEIGHT,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)static_cast<long>(100.0 * (1.0 - planeHeight)));

      char szTemp[32];
      sprintf(szTemp,"%5.2f",dpStart.x);
      SetDlgItemText(hwndOrientation,IDDI_TEXT_XCOORDINATE,szTemp);
      sprintf(szTemp,"%5.2f",dpStart.y);
      SetDlgItemText(hwndOrientation,IDDI_TEXT_YCOORDINATE,szTemp);
      sprintf(szTemp,"%5.2f",dpStart.y);
      SetDlgItemText(hwndOrientation,IDDI_TEXT_ZCOORDINATE,szTemp);

      EnableWindow(GetDlgItem(hwndOrientation,IDDI_TEXT_XCOORDINATE),enablePositionSettings);
      EnableWindow(GetDlgItem(hwndOrientation,IDDI_TEXT_YCOORDINATE),enablePositionSettings);
      EnableWindow(GetDlgItem(hwndOrientation,IDDI_TEXT_ZCOORDINATE),enablePositionSettings);

      TextFormat format;
      propertyFormat -> get_longValue(reinterpret_cast<long*>(&format));

      if ( format & TEXT_COORDINATES_FROM_TOP ) 
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_FORMAT_FROM_TOP,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
      else {
         if ( format & TEXT_COORDINATES_FROM_CENTER )
            SendDlgItemMessage(hwndOrientation,IDDI_TEXT_FORMAT_FROM_CENTER,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         else
            SendDlgItemMessage(hwndOrientation,IDDI_TEXT_FORMAT_FROM_BOTTOM,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
      }
      if ( format & TEXT_FORMAT_CENTER )
         SendDlgItemMessage(hwndOrientation,IDDI_TEXT_FORMAT_CENTER,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
      else {
         if ( format & TEXT_FORMAT_RIGHT )
            SendDlgItemMessage(hwndOrientation,IDDI_TEXT_FORMAT_RIGHT,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
         else
            SendDlgItemMessage(hwndOrientation,IDDI_TEXT_FORMAT_LEFT,BM_SETCHECK,(WPARAM)BST_CHECKED,0L);
      }

      *pHwnd = hwndOrientation;

      SetWindowLong(hwndOrientation,GWL_USERDATA,(long)this);

      }
      break;

   }

   return S_OK;
   }


   HRESULT Text::Apply() {
   Erase();
   pIProperties -> Discard();
   pIProperties -> Push();
   PrepData();
   Draw();
   return S_OK;
   }


   HRESULT Text::IsPageDirty(long hwndPage,BOOL* isDirty) {
   pIProperties -> Compare((short*)isDirty);
   if ( IsWindowVisible(hwndOrientation) ) {
      Erase();
      PrepData();
      Draw();
   }
   return S_OK;
   }


   HRESULT Text::Help(BSTR bstrHelpDir) {
   return  S_OK;
   }


   HRESULT Text::TranslateAccelerator(long,long* pResult) {
   *pResult = S_FALSE;
   return S_OK;
   }


   HRESULT Text::AfterAllPropertyPages(BOOL userCanceled) {
   Erase();
   if ( userCanceled ) {
      pIProperties -> Pop();
      pIProperties -> Pop();
      PrepData();
      Draw();
   } else {
      pIProperties -> Discard();
      pIProperties -> Discard();
      PrepData();
      Draw();
   }
   return S_OK;
   }



   HRESULT Text::DestroyPropertyPage(long pageNumber) {

   switch ( pageNumber ) {

   case 0:
   case 1: 
      if ( showContentPropertyPage && pageNumber == 0 ) {
         DestroyWindow(hwndContent);
      } else if ( ( pageNumber == 0 && ! showContentPropertyPage ) || ( pageNumber == 1 && showContentPropertyPage ) ) {
         DestroyWindow(hwndStyle);
      }
      break;

   case 2:

      DestroyWindow(hwndOrientation);
      break;

   }

   return S_OK;
   }


   HRESULT Text::GetPropertySheetHeader(void *pv) {
   
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


   HRESULT Text::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 3;
   return S_OK;
   }


   HRESULT Text::GetPropertySheets(void *pPages) {

//IDDIALOG_TEXT_CONTENT
//IDDIALOG_TEXT_STYLE
//IDDIALOG_TEXT_ORIENTATION
//
   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDDIALOG_TEXT_CONTENT);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)Text::contentHandler;
   pPropSheetPages[0].pszTitle = "Content";
   pPropSheetPages[0].lParam = (LPARAM)this;
   pPropSheetPages[0].pfnCallback = NULL;

   pPropSheetPages[1].dwFlags = PSP_USETITLE;
   pPropSheetPages[1].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[1].hInstance = hModule;
   pPropSheetPages[1].pszTemplate = MAKEINTRESOURCE(IDDIALOG_TEXT_STYLE);
   pPropSheetPages[1].pfnDlgProc = (DLGPROC)Text::styleHandler;
   pPropSheetPages[1].pszTitle = "Style";
   pPropSheetPages[1].lParam = (LPARAM)this;
   pPropSheetPages[1].pfnCallback = NULL;

   pPropSheetPages[2].dwFlags = PSP_USETITLE;
   pPropSheetPages[2].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[2].hInstance = hModule;
   pPropSheetPages[2].pszTemplate = MAKEINTRESOURCE(IDDIALOG_TEXT_ORIENTATION);
   pPropSheetPages[2].pfnDlgProc = (DLGPROC)Text::orientationHandler;
   pPropSheetPages[2].pszTitle = "Orientation";
   pPropSheetPages[2].lParam = (LPARAM)this;
   pPropSheetPages[2].pfnCallback = NULL;

   return S_OK;

   }

