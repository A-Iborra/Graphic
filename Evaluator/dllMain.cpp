/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Evaluator.h"

#include "Evaluator_i.c"
#include "Variable_i.c"

#ifdef EMBEDDED_PROPERTIES
#include "Properties_i.h"
#else
#include "Properties_i.c"
#endif

   extern HMODULE gsEvaluator_hModule;
   extern char gsEvaluator_szModuleName[];
   extern BSTR gsEvaluator_wstrModuleName;
   extern ITypeInfo *pITypeInfo_IEvaluator;

   BOOL WINAPI gsEvaluator_dllAttach(HANDLE module);
   BOOL WINAPI gsEvaluator_dllDetach();

   BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *) {
   switch(flag) {
   case DLL_PROCESS_ATTACH: 
      gsEvaluator_dllAttach(module);
      break;

   case DLL_PROCESS_DETACH:
      gsEvaluator_dllDetach();
      break;

   default:
      break;
   }
   return TRUE;
   }


