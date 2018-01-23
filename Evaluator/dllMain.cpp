// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Evaluator.h"

#include "Evaluator_i.c"
#include "Variable_i.c"

#include "Properties_i.c"

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


