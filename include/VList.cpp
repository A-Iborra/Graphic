/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "general.h"
#include "properties_i.h"

#include "VList.h"
#include "list.cpp"

   IClassFactory* VList::pIClassFactory_variables = NULL;

   VList::VList(IEvaluator* piev) : hwndParent(0),hwndTabControl(0),pIEvaluator(piev) { };

   VList::~VList() {
      IVariable *v = (IVariable *)NULL;
      while ( v = GetFirst() ) Remove(v,NULL);
   };


   void VList::SetHwnds(HWND hwndP,HWND hwndTC) {
   long ti;
   IVariable* v = (IVariable *)NULL;
   hwndParent = hwndP;
   hwndTabControl = hwndTC;
   while ( v = GetNext(v) ) {
      v -> SetHwnds(hwndParent,hwndTabControl);
      v -> get_TabIndex(&ti);
      ID(v,ti);
   }
   return;
   };


   IVariable* VList::Add(char *varName) {
   IVariable* v;
   long ti;

   pIClassFactory_variables -> CreateInstance(NULL,IID_IVariable,reinterpret_cast<void**>(&v));

   v -> put_IEvaluator(pIEvaluator);

   if ( varName ) {
      BSTR bstrValue = SysAllocStringLen(NULL,strlen(varName) + 1);
      MultiByteToWideChar(CP_ACP,0,varName,-1,bstrValue,1024);
      v -> put_Name(bstrValue);
      SysFreeString(bstrValue);
   }

   if ( hwndParent ) {
      v -> SetHwnds(hwndParent,hwndTabControl);
      v -> get_TabIndex(&ti);
   } else
      ti = 0;

   ID(v,ti);

   List<IVariable>::Add(v,varName,ti);

   return v;
   };


   IVariable* VList::Copy(IVariable* vToCopy) {
   IVariable* v;
   long ti;
   char szName[MAX_PROPERTY_SIZE];
   pIClassFactory_variables -> CreateInstance(NULL,IID_IVariable,reinterpret_cast<void**>(&v));
   v -> CopyFrom(vToCopy);
   v -> put_IEvaluator(pIEvaluator);
   if ( hwndParent ) {
      v -> SetHwnds(hwndParent,hwndTabControl);
      v -> get_TabIndex(&ti);
   } else
      ti = 0;

   BSTR bstrValue;
   memset(szName,0,sizeof(szName));
   v -> get_Name(&bstrValue);
   WideCharToMultiByte(CP_ACP,0,bstrValue,-1,szName,MAX_PROPERTY_SIZE,0,0);
   SysFreeString(bstrValue);

   List<IVariable>::Add(v,szName,ti);
   ID(v,ti);
   return v;
   };


   int VList::Remove(IVariable* removeItem,IVariable** ppCurrentActiveVariable) {

   IVariable *v = (IVariable *)NULL;
   if ( ppCurrentActiveVariable ) 
      while ( v = GetNext(v) ) {
         if ( v == *ppCurrentActiveVariable ) {
            if ( v == removeItem )
               *ppCurrentActiveVariable = (IVariable *)NULL;
            break;
         }
      }

   int rc = List<IVariable>::Remove(removeItem);

   if ( ! removeItem ) return rc;

   long ti;
   long oldTab;
   removeItem -> get_TabIndex(&oldTab);
   removeItem -> Release();
   v = (IVariable *)NULL;
   while ( v = GetNext(v) ) {
      if ( ID(v) > oldTab ) {
         v -> get_TabIndex(&ti);
         v -> put_TabIndex(ti - 1); 
         ID(v,ti - 1);
      }
   }

   return rc;
   };