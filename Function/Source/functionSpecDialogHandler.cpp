// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Function.h"

#include <olectl.h>
#include <CommCtrl.h>

#include "Graphic_resource.h"
#include "list.cpp"

   LRESULT EXPENTRY Function::functionDialogHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Function *p = (Function *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {
   case WM_INITDIALOG: {
      p = (Function *)lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)p);
      }
      return LRESULT(1);

   case WM_DESTROY:
      if ( ! p ) break;
      p -> stopAllProcessing = true;
      if ( p -> oldResultLabelHandler ) 
         SetWindowLongPtr(GetDlgItem(hwnd,IDDI_FUNCTION_RESULT_LABEL),GWLP_WNDPROC,(ULONG_PTR)(p -> oldResultLabelHandler));
      if ( p -> oldExpressionLabelHandler )
         SetWindowLongPtr(GetDlgItem(hwnd,IDDI_FUNCTION_EXPRESSION_LABEL),GWLP_WNDPROC,(ULONG_PTR)(p -> oldExpressionLabelHandler));
      if ( p -> oldExpressionHandler )
         SetWindowLongPtr(GetDlgItem(hwnd,IDDI_FUNCTION_EQUATION_ENTRY),GWLP_WNDPROC,(ULONG_PTR)(p -> oldExpressionHandler));
      break;

   case WM_MOVE:
      p -> rectDialog.left = LOWORD(lParam);
      p -> rectDialog.top = HIWORD(lParam);
      break;

   case WM_PAINT: {

      if ( ! p ) break;

      if ( p -> currentShowingVariable ) {
         HWND hwndV;
         p -> currentShowingVariable -> get_HWND(&hwndV);
         ShowWindow(hwndV,SW_SHOW);
      }

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

         p -> userMode = varUserMode.bVal;

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

      if ( ! p -> anyVisible() && ! p -> userMode ) {
         ps.rcPaint.left += 8;
         ps.rcPaint.top += 8;
         ps.rcPaint.bottom -= 8;
         SetBkColor(ps.hdc,p -> backgroundColor);
         SetTextColor(ps.hdc,p -> foregroundColor);
         DrawText(ps.hdc, "No UI components are visible, set properties to change. The control won't be visible at runtime in this state.", -1, &ps.rcPaint, DT_VCENTER | DT_WORDBREAK); 
      } else {
         long nID = (long)GetWindowLongPtr(hwnd,GWLP_ID);
         if ( IDDI_FUNCTION_RESULT_LABEL == nID || IDDI_FUNCTION_EXPRESSION_LABEL == nID ) {
            RECT rect;
            char *pszText;
            long n;
            IGProperty* pProp;
            switch ( nID ) {
            case IDDI_FUNCTION_RESULT_LABEL:
               pProp = p -> pIPropertyResultsLabel;
               break;
            case IDDI_FUNCTION_EXPRESSION_LABEL:
               pProp = p -> pIPropertyExpressionLabel;
               break;
            }
            pProp -> get_size(&n);
            pszText = new char[n + 1];
            pProp -> get_szValue(pszText);
            SetBkColor(ps.hdc,p -> backgroundColor);
            SetTextColor(ps.hdc,p -> foregroundColor);
            if ( ambientFont ) 
               oldFont = (HFONT)SelectObject(ps.hdc,ambientFont);
            else
               oldFont = (HFONT)SelectObject(ps.hdc,(HFONT)SendMessage(GetParent(hwnd),WM_GETFONT,0L,0L));
            GetClientRect(hwnd,&rect);
            DrawText(ps.hdc,pszText,-1,&rect,DT_WORDBREAK | DT_INTERNAL | DT_EDITCONTROL); 
            if ( oldFont ) 
               SelectObject(ps.hdc,oldFont);
            delete [] pszText;
         }
      }

      EndPaint(hwnd,&ps);
      }
      return LRESULT(TRUE);
      break;

   case WM_SETTEXT: {
      long id = GetWindowLong(hwnd,GWL_ID);
      switch ( id ) {
      case IDDI_FUNCTION_RESULT_LABEL:
         if ( p -> oldResultLabelHandler ) {
            CallWindowProc(p -> oldResultLabelHandler,hwnd,msg,wParam,lParam);
            InvalidateRect(hwnd,NULL,TRUE);
         }
         break;
      case IDDI_FUNCTION_EXPRESSION_LABEL:
         if ( p -> oldExpressionLabelHandler ) {
            CallWindowProc(p -> oldExpressionLabelHandler,hwnd,msg,wParam,lParam);
            InvalidateRect(hwnd,NULL,TRUE);
         }
         break;
      }
      }
      break;

   case WM_GETTEXT: {
      long id = GetWindowLong(hwnd,GWL_ID);
      switch ( id ) {
      case IDDI_FUNCTION_RESULT_LABEL: 
      case IDDI_FUNCTION_EXPRESSION_LABEL: {
         long nSize,nAvailable = (long)wParam;
         IGProperty* pProp;
         if ( id == IDDI_FUNCTION_RESULT_LABEL )
            pProp = p -> pIPropertyResultsLabel;
         else
            pProp = p -> pIPropertyExpressionLabel;
         pProp -> get_size(&nSize);
         char *pszText = new char[nSize + 1];
         pProp -> get_szValue(pszText);
         strncpy((char*)lParam,pszText,nAvailable);
         delete [] pszText;
         }
         break;
      }
      }
      break;

   case WM_COMMAND: {
      if ( ! p ) break; 
      WORD controlID = LOWORD(wParam);
      WORD notifyCode = HIWORD(wParam);
      switch ( controlID ) {
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
 
      case IDDI_FUNCTION_PROPERTIES: {
         IUnknown* pIUnknown;
         p -> QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknown));
         p -> iProperties -> ShowProperties(hwnd,pIUnknown);
         pIUnknown -> Release();
         //p -> resize();
         if ( p -> pWhenChangedCallback )
            p -> pWhenChangedCallback(p -> pWhenChangedCallbackArg,p -> whenChangedCallbackCookie);
         }
         break;

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

      }
      }
      break;

   default:
      break;
   }

   return 0;
   }
 
 
   LRESULT EXPENTRY Function::functionVariablesHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   Function *p = (Function *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
 
   switch (msg) {
   case WM_INITDIALOG:
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(ULONG_PTR)lParam);
      return LRESULT(1);

   case WM_NOTIFY: {
      if ( ! p ) break;
      NMHDR *pn = (NMHDR *)lParam;
      TCITEM tcItem;
      char szText[64];
      long k = (long)SendMessage(pn -> hwndFrom,TCM_GETCURSEL,0,0);
      memset(&tcItem,0,sizeof(TCITEM));
      tcItem.mask = TCIF_TEXT;
      tcItem.pszText = szText;
      tcItem.cchTextMax = 64;
      SendMessage(pn -> hwndFrom,TCM_GETITEM,(int)k,(LPARAM)&tcItem);
      switch ( pn -> code ) {
      case TCN_SELCHANGING:
         p -> currentShowingVariable = p -> pVariableList -> Get(szText);
         if ( ! p -> currentShowingVariable ) 
            p -> currentShowingVariable = p -> pManuallyAddedVariables -> Get(szText);
         if ( p -> currentShowingVariable ) {
            HWND hwndV;
            p -> currentShowingVariable -> get_HWND(&hwndV);
            EnableWindow(hwndV,FALSE);
            ShowWindow(hwndV,SW_HIDE);
            p -> currentShowingVariable = NULL;
         } else {
            ShowWindow(p -> hwndErrors,SW_HIDE);
            ShowWindow(GetDlgItem(p -> hwndVariablesTab,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),SW_HIDE);
         }
         return LRESULT(0);
 
      case TCN_SELCHANGE:
         p -> currentShowingVariable = p -> pVariableList -> Get(szText);
         if ( ! p -> currentShowingVariable ) 
            p -> currentShowingVariable = p -> pManuallyAddedVariables -> Get(szText);
         if ( p -> currentShowingVariable ) {
            HWND hwndV;
            p -> currentShowingVariable -> get_HWND(&hwndV);
            ShowWindow(hwndV,SW_SHOW);
            EnableWindow(hwndV,TRUE);
            ShowWindow(p -> hwndErrors,SW_HIDE);
            ShowWindow(GetDlgItem(p -> hwndVariablesTab,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),SW_HIDE);
         } else {
            if ( k == 0 ) 
               ShowWindow(GetDlgItem(p -> hwndVariablesTab,IDDI_FUNCTION_VARIABLES_DOMAIN_BOX),SW_SHOW);
            else
               ShowWindow(p -> hwndErrors,SW_SHOW);
         }
         return LRESULT(0);
 
      default:
         break;
      }
      }
      break;
 
   case WM_COMMAND: {

      if ( ! p ) break; 
      WORD controlID = LOWORD(wParam);
      WORD notifyCode = HIWORD(wParam);
      switch ( controlID ) {

      case IDDI_FUNCTION_ERRORS_CLEAR:
         SendMessage(GetDlgItem(hwnd,IDDI_FUNCTION_ERRORS_BOX),LB_RESETCONTENT,0l,0l);
         EnableWindow(GetDlgItem(hwnd,IDDI_FUNCTION_ERRORS_CLEAR),FALSE);
         return LRESULT(FALSE);

      default:
         break;
      }
   
      }
      break;

   }
 
   return 0;
   }


   LRESULT EXPENTRY Function::expressionHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
   Function *p = reinterpret_cast<Function *>(GetWindowLongPtr(hwnd,GWLP_USERDATA));
   switch ( msg ) {
   case WM_CHAR: {
      short keyCode = (short)wParam;
      if ( keyCode != 0x0D && keyCode != 0x09 )
         p -> enteringData = true;
      else {
         p -> enteringData = false;
         SendMessage(GetParent(hwnd),WM_COMMAND,MAKEWPARAM(IDDI_FUNCTION_EQUATION_ENTRY,EN_CHANGE),0L);
      }
      }
      break;

   }
   return CallWindowProc(p -> oldExpressionHandler,hwnd,msg,wParam,lParam);
   }