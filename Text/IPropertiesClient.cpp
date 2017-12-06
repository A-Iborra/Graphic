/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>

#include "General.h"

#include "Text.h"


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

//   pParent -> propertyText -> put_szValue("");

   pParent -> propertyFormat -> put_longValue((long)TEXT_FORMAT_NONE);

   strcpy(pParent -> szFace,DEFAULT_FONT);

   pParent -> propertyPositionX -> put_stringValue(L"0.0");
   pParent -> propertyPositionY -> put_stringValue(L"0.0");
   pParent -> propertyPositionZ -> put_stringValue(L"0.0");

   pParent -> directionForward.x = 1.0;
   pParent -> directionForward.y = 0.0;
   pParent -> directionForward.z = 0.0;
   pParent -> directionUp.x = 0.0;
   pParent -> directionUp.y = 1.0;
   pParent -> directionUp.z = 0.0;

   pParent -> propertyCoordinatePlane -> put_longValue(CoordinatePlane_XY);

   pParent -> propertyPlaneHeight -> put_doubleValue(0.0);
 
   pParent -> propertyTextColor -> put_binaryValue(sizeof(fvBlack),(BYTE*)fvBlack);

   pParent -> propertyLineWeight -> put_doubleValue(0.0);
 
   pParent -> fontSize = 12.0;

   pParent -> propertySizeUnits -> put_longValue(TEXT_SIZE_POINTS);

#ifndef USE_OPENGL_RENDERING
   pParent -> partOfWorldDomain = FALSE;
#else
   pParent -> partOfWorldDomain = TRUE;
#endif
   pParent -> partOfMainGraphic = TRUE;

   return Loaded();
   }
 
   HRESULT Text::_IGPropertiesClient::Loaded() {
   pParent -> put_PartOfWorldDomain(pParent -> partOfWorldDomain);
#ifndef USE_OPENGL_RENDERING
   pParent -> put_PartOfWorldDomain(FALSE);
#endif
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