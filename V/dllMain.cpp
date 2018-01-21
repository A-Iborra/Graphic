// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "V.h"

#include "utils.h"

#include "Variable_i.c"
#include "Evaluator_i.c"


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
 
