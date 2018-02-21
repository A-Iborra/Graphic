// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"
#include <stdio.h>

   long __stdcall Text::_IGPropertiesClient::QueryInterface(REFIID riid,void **ppv) {
   return pParent -> QueryInterface(riid,ppv);
   }
   unsigned long __stdcall Text::_IGPropertiesClient::AddRef() { return 1; }
   unsigned long __stdcall Text::_IGPropertiesClient::Release() { return 1; }
 
   HRESULT Text::_IGPropertiesClient::SavePrep() {
   return S_OK;
   }


   HRESULT Text::_IGPropertiesClient::InitNew() {

   float fvBlack[] = {CLR_BLACK};
   float fvWhite[] = {CLR_WHITE};

   pParent -> propertyFormat -> put_longValue((long)TEXT_FORMAT_NONE);

   strcpy(pParent -> szFace,DEFAULT_FONT);

   pParent -> propertyPositionX -> put_stringValue(L"0.0");
   pParent -> propertyPositionY -> put_stringValue(L"0.0");
   pParent -> propertyPositionZ -> put_stringValue(L"0.0");

   pParent -> propertyCoordinatePlane -> put_longValue(CoordinatePlane_XY);

   pParent -> propertyPlaneHeight -> put_doubleValue(0.0);
 
   pParent -> propertyTextColor -> put_binaryValue(sizeof(fvBlack),(BYTE*)fvBlack);

   pParent -> propertyLineWeight -> put_doubleValue(0.0);
 
   pParent -> fontSize = 12.0;

   pParent -> propertySizeUnits -> put_longValue(TEXT_SIZE_POINTS);

   pParent -> partOfWorldDomain = TRUE;

   pParent -> partOfMainGraphic = TRUE;

   pParent -> rotation = 0.0;

   return Loaded();
   }
 
   HRESULT Text::_IGPropertiesClient::Loaded() {
   return S_OK;
   }
 
   HRESULT Text::_IGPropertiesClient::Saved() {
   return S_OK;
   }


   HRESULT Text::_IGPropertiesClient::IsDirty() {
   return S_FALSE;
   }


   HRESULT Text::_IGPropertiesClient::GetClassID(BYTE *pCLSID) {
   *(CLSID*)pCLSID = CLSID_Text;
   return S_OK;
   }