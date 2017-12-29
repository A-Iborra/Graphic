/*

                       Copyright (c) 1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <CommCtrl.h>
#include <stdio.h>

#include "Graphic_Resource.h"
#include "utils.h"

#include "V.h"
#include "Properties_i.h"
#include "Variable_i.h"


   HRESULT V::SavePrep() {

   if ( ! hwndDialog )
      return S_OK;

   pIProperty_minValue -> getWindowItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_MIN);
   pIProperty_stepCount -> getWindowItemText(hwndDialog,IDDI_VARIABLE_STEPS);
   pIProperty_stepExpression -> getWindowItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_EXPRESSION);
   pIProperty_maxValue -> getWindowItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_MAX);
   pIProperty_valueExpression -> getWindowItemText(hwndDialog,IDDI_VARIABLE_VALUE);

   return S_OK;
   }


   HRESULT V::InitNew() {

   memset(minValue,0,sizeof(minValue));
   memset(maxValue,0,sizeof(maxValue));
   memset(stepCount,0,sizeof(stepCount));
   memset(stepExpression,0,sizeof(stepExpression));
   memset(valueExpression,0,sizeof(valueExpression));

   strcpy(minValue,"-1");
   strcpy(maxValue,"1");
   strcpy(stepCount,"10");

   isIndependentVariable = TRUE;

   return Loaded();
   }
  
  
   HRESULT V::Loaded() {

   getDomain(hwndDialog);

   if ( hwndTabControl ) {
      TC_ITEM tie;
      memset(&tie,0,sizeof(TC_ITEM));
      tie.mask = TCIF_TEXT; 
      tie.pszText = name;
      tie.cchTextMax = strlen(name);
      SendMessage(hwndTabControl,TCM_SETITEM,(WPARAM)tabIndex,(LPARAM)&tie);
   }

   return S_OK;
   }
  
  
   HRESULT V::Saved() {
   return S_OK;
   }
  
  
   HRESULT V::IsDirty() {
   return S_OK;
   }


   HRESULT V::GetClassID(BYTE *pCLSID) {
   *(CLSID*)pCLSID = CLSID_Variable;
   return S_OK;
   }