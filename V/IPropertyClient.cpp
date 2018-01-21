// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "V.h"

#include "Graphic_resource.h"

   HRESULT V::Changed(IGProperty *pProp) {
   if ( ! hwndDialog ) 
      return S_OK;
   if ( pProp == pIProperty_stepCount ) 
      pIProperty_stepCount -> setWindowItemText(hwndDialog,IDDI_VARIABLE_STEPS);
   if ( pProp == pIProperty_stepExpression ) 
      pIProperty_stepExpression -> setWindowItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_EXPRESSION);
   if ( pProp == pIProperty_minValue ) 
      pIProperty_minValue -> setWindowItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_MIN);
   if ( pProp == pIProperty_maxValue ) 
      pIProperty_maxValue -> setWindowItemText(hwndDialog,IDDI_VARIABLE_DOMAIN_MAX);
   if ( pProp == pIProperty_valueExpression ) 
      pIProperty_valueExpression -> setWindowItemText(hwndDialog,IDDI_VARIABLE_VALUE);
   return S_OK;
   }