/*

                       Copyright (c) 1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>
#include <CommCtrl.h>

#include "general.h"
#include "Graphic_resource.h"
#include "gmessage.h"

#include "vlist.h"

#include "DataSet.h"

#include "List.cpp"

   HRESULT DataSet::SavePrep() {
   return S_OK;
   }


   HRESULT DataSet::InitNew() {
   return Loaded();
   }
 
 
   HRESULT DataSet::Loaded() {
   return S_OK;
   }
 
 
   HRESULT DataSet::Saved() {
   return S_OK;
   }

 
   HRESULT DataSet::IsDirty() {
   return S_FALSE;
   }


   HRESULT DataSet::GetClassID(BYTE *pCLSID) {
   *(CLSID*)pCLSID = CLSID_DataSet;
   return S_OK;
   }


