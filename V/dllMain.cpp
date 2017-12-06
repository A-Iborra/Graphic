/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <stdio.h>

#include "resource.h"
#include "utils.h"

#include "V.h"

#include "Variable_i.c"
#include "Evaluator_i.c"
#ifdef EMBEDDED_PROPERTIES
#include "Properties_i.h"
#endif

   BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *) {
   switch ( flag ) {
   case DLL_PROCESS_ATTACH: {

      gsVariables_hModule = reinterpret_cast<HMODULE>(module);

      GetModuleFileName(gsVariables_hModule,gsVariables_szModuleName,1024);

      gsVariables_wstrModuleName = SysAllocStringLen(NULL,strlen(gsVariables_szModuleName) + 1);

      MultiByteToWideChar(CP_ACP, 0, gsVariables_szModuleName, -1, gsVariables_wstrModuleName, strlen(gsVariables_szModuleName) + 1);  

      }
      break;
 
   case DLL_PROCESS_DETACH:
      SysFreeString(gsVariables_wstrModuleName);
      break;
 
   default:
      break;
   }
   return TRUE;
   }
 
