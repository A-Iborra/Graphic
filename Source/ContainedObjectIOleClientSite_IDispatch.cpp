/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>
#include <stdio.h>

#include "utils.h"
#include "Graphic_resource.h"

#include "Graphic.h"

#include "GraphicControl_i.h"
#include "List.cpp"


   long __stdcall ContainedObject::_IOleClientSite::_IDispatch::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      *ppv = static_cast<IUnknown*>(this);
   else

   if ( IID_IDispatch == riid ) 
      *ppv = static_cast<IDispatch*>(this);
   else

      return pParent -> QueryInterface(riid,ppv);

   reinterpret_cast<IUnknown*>(*ppv) -> AddRef();

   return S_OK;
   }
   unsigned long __stdcall ContainedObject::_IOleClientSite::_IDispatch::AddRef() {
   return pParent -> AddRef();
   }
   unsigned long __stdcall ContainedObject::_IOleClientSite::_IDispatch::Release() {
   return pParent -> Release();
   }


   long __stdcall ContainedObject::_IOleClientSite::_IDispatch::GetTypeInfoCount(UINT *i) { 
   *i = 1; 
   return S_OK; 
   } 
  
 
   long __stdcall ContainedObject::_IOleClientSite::_IDispatch::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   *pptinfo = pITypeInfo_IG;
   pITypeInfo_IG -> AddRef();
   return S_OK; 
   } 
  
 
   long __stdcall ContainedObject::_IOleClientSite::_IDispatch::GetIDsOfNames(REFIID riid,OLECHAR **rgszNames,UINT cNames,LCID lcid,DISPID *rgdispid) { 
   return DispGetIDsOfNames(pITypeInfo_IG, rgszNames, cNames, rgdispid); 
   } 
 
       
   long __stdcall ContainedObject::_IOleClientSite::_IDispatch::Invoke(DISPID dispidMember,
                            REFIID riid,
                            LCID lcid,
                            WORD wFlags, 
                            DISPPARAMS *pdispparams,
                            VARIANT *pvarResult,
                            EXCEPINFO *pexcepinfo, 
                            UINT *puArgErr) { 

   if ( DISPATCH_PROPERTYGET != wFlags ) 
      return DISP_E_MEMBERNOTFOUND;

   switch ( dispidMember ) {

   case DISPID_AMBIENT_USERMODE: {

      VARIANT varUserMode;

      DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};

      varUserMode.vt = VT_BOOL;

      VariantClear(&varUserMode);

      IDispatch *pIDispatch = NULL;

      pParent -> pParent -> pParent -> QueryInterface(IID_IDispatch,(void **)&pIDispatch);

      pIDispatch -> Invoke(DISPID_AMBIENT_USERMODE,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_PROPERTYGET,&dispparamsNoArgs,&varUserMode,NULL,NULL);

      pIDispatch -> Release();

      pvarResult -> vt = VT_BOOL;

      if ( varUserMode.bVal )
         pvarResult -> boolVal = varUserMode.bVal ? TRUE : FALSE;

      return S_OK;

      }
      break;
   case DISPID_AMBIENT_BACKCOLOR:
      pvarResult -> vt = VT_I4;
      pvarResult -> lVal = GetSysColor(COLOR_BTNFACE);
      break;
   case DISPID_AMBIENT_FORECOLOR:
      pvarResult -> vt = VT_I4;
      pvarResult -> lVal = GetSysColor(COLOR_BTNTEXT);
      break;
   default:
      return DISP_E_MEMBERNOTFOUND;
   }

   return S_OK;
   } 
 
