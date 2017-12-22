/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>
#include <CommCtrl.h>

#include "Graphic_resource.h"

#include "DataSet.h"
#include "list.cpp"

   LRESULT EXPENTRY DataSet::dataSetDialogHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   DataSet *p = (DataSet *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {
   case WM_INITDIALOG: {

      p = (DataSet *)lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);

      SetDlgItemText(hwnd,IDDI_DATASET_SPEC_NAME,p -> szName);
      SetDlgItemText(hwnd,IDDI_DATASET_SPEC_DATASOURCE,p -> szDataSource);

      }
      return LRESULT(1);

   case WM_DESTROY:
      break;

   case WM_PAINT: {

      if ( ! p ) break;

      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      IDispatch* pIDispatch = (IDispatch *)NULL;

      HFONT oldFont = NULL;
      HFONT ambientFont = NULL;

      if ( p -> pIOleClientSite && SUCCEEDED(p -> pIOleClientSite -> QueryInterface(IID_IDispatch,reinterpret_cast<void**>(&pIDispatch))) ) {

         HBRUSH hb;
	      DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
         VARIANT varBackColor;
         VARIANT varFont;
         VARIANT varForeColor;
         VARIANT varUserMode;

         memset(&varBackColor,0,sizeof(VARIANT));
         memset(&varFont,0,sizeof(VARIANT));
         memset(&varForeColor,0,sizeof(VARIANT));

         varUserMode.vt = VT_BOOL;

         VariantClear(&varBackColor);
         VariantClear(&varFont);
         VariantClear(&varForeColor);
         VariantClear(&varUserMode);

         pIDispatch -> Invoke(DISPID_AMBIENT_BACKCOLOR,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varBackColor,NULL,NULL);
         pIDispatch -> Invoke(DISPID_AMBIENT_FORECOLOR,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varForeColor,NULL,NULL);
         pIDispatch -> Invoke(DISPID_AMBIENT_FONT,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varFont,NULL,NULL);
         pIDispatch -> Invoke(DISPID_AMBIENT_USERMODE,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varUserMode,NULL,NULL);

         pIDispatch -> Release();

         OleTranslateColor(varBackColor.lVal,NULL,&p -> backgroundColor);
         OleTranslateColor(varForeColor.lVal,NULL,&p -> foregroundColor);

         p -> isDesignMode = ! varUserMode.bVal;

         if ( varFont.pdispVal ) {

            IFont* pIFont;
            varFont.pdispVal -> QueryInterface(IID_IFont,reinterpret_cast<void**>(&pIFont));

            pIFont -> get_hFont(&ambientFont);
            oldFont = (HFONT)SelectObject(ps.hdc,ambientFont);
            SelectObject(ps.hdc,oldFont);

            pIFont -> Release();
            varFont.pdispVal -> Release();
         }
        
         hb = CreateSolidBrush(p -> backgroundColor);
         SelectObject(ps.hdc,hb);
         FillRect(ps.hdc,&ps.rcPaint,hb);
         DeleteObject(hb);

      } else {

         FillRect(ps.hdc,&ps.rcPaint,(HBRUSH)(COLOR_BTNFACE + 1));
         p -> backgroundColor = GetSysColor(COLOR_BTNFACE);
         p -> foregroundColor = GetSysColor(COLOR_BTNTEXT);

      }

      //EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_PLOT_PROPERTIES),p -> pIPlot ? TRUE : FALSE);

      EndPaint(hwnd,&ps);
      }
      return LRESULT(TRUE);
      break;

   //case WM_SETTEXT: {
   //   long id = GetWindowLong(hwnd,GWL_ID);
   //   switch ( id ) {
   //   case IDDI_FUNCTION_RESULT_LABEL:
   //      if ( p -> oldResultLabelHandler ) {
   //         CallWindowProc(p -> oldResultLabelHandler,hwnd,msg,wParam,lParam);
   //         InvalidateRect(hwnd,NULL,TRUE);
   //      }
   //      break;
   //   case IDDI_FUNCTION_EXPRESSION_LABEL:
   //      if ( p -> oldExpressionLabelHandler ) {
   //         CallWindowProc(p -> oldExpressionLabelHandler,hwnd,msg,wParam,lParam);
   //         InvalidateRect(hwnd,NULL,TRUE);
   //      }
   //      break;
   //   }
   //   }
   //   break;

   //case WM_GETTEXT: {
   //   long id = GetWindowLong(hwnd,GWL_ID);
   //   switch ( id ) {
   //   case IDDI_FUNCTION_RESULT_LABEL: 
   //   case IDDI_FUNCTION_EXPRESSION_LABEL: {
   //      long nSize,nAvailable = (long)wParam;
   //      IGProperty* pProp;
   //      if ( id == IDDI_FUNCTION_RESULT_LABEL )
   //         pProp = p -> pIPropertyResultsLabel;
   //      else
   //         pProp = p -> pIPropertyExpressionLabel;
   //      pProp -> get_size(&nSize);
   //      char *pszText = new char[nSize + 1];
   //      pProp -> get_szValue(pszText);
   //      strncpy((char*)lParam,pszText,nAvailable);
   //      delete [] pszText;
   //      }
   //      break;
   //   }
   //   }
   //   break;

   case WM_COMMAND: {
      if ( ! p ) break; 
      WORD controlID = LOWORD(wParam);
      WORD notifyCode = HIWORD(wParam);
      switch ( controlID ) {
#if 0
      case IDDI_FUNCTION_EQUATION_ENTRY:
         switch ( notifyCode ) {
         case EN_CHANGE:
            if ( ! p -> enteringData && ! p -> stopAllProcessing ) {
               if ( p -> defineFunction() ) 
                  EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_START),TRUE);
            }
            return LRESULT(FALSE);
 
         default:
            return LRESULT(FALSE);
         }
#endif
 
      case IDDI_DATASET_SPEC_PROPERTIES: {
         IUnknown* pIUnknown;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> pIGProperties -> ShowProperties(hwnd,pIUnknown);
         pIUnknown -> Release();
#if 0
         p -> resize(p -> containerSize.cx);
         if ( p -> pWhenChangedCallback )
            p -> pWhenChangedCallback(p -> pWhenChangedCallbackArg);
#endif
         }
         break;

#if 0
      case IDDI_FUNCTION_START:
         p -> Start();
         return LRESULT(TRUE);
 
      case IDDI_FUNCTION_PAUSE:
         p -> Pause();
         return LRESULT(TRUE);
 
      case IDDI_FUNCTION_RESUME:
         p -> Resume();
         return LRESULT(TRUE);
 
      case IDDI_FUNCTION_STOP:
         p -> fire_Stopped();
         p -> Stop();
         return LRESULT(TRUE);

      case IDDI_FUNCTION_PLOT_PROPERTIES: {
         if ( p -> pIPlot )
            p -> pIPlot -> EditProperties();
         }
         return LRESULT(TRUE);
#endif

      case IDDI_DATASET_SPEC_GETDATASOURCE: {
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

         SetDlgItemText(hwnd,IDDI_DATASET_SPEC_DATASOURCE,p -> szDataSource);

         }
         break;   
      }
      }
      break;

   default:
      break;
   }

   return 0;
   }