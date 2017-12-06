/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <comcat.h>
#include <stdio.h>
#include <time.h>

#include "Graphic_resource.h"

#include "Function.h"
#include "VList.h"

#include "Function_i.c"
#include "Variable_i.c"

#ifdef EMBEDDED_PROPERTIES
#include "Evaluator_i.h"
#include "Properties_i.h"
#else
#include "Evaluator_i.c"
#include "Properties_i.c"
#endif

   HMODULE hModule;
   char szModuleName[1024];
   BSTR wstrModuleName;
   ITypeInfo *pITypeInfo_IGSFunctioNater;
   ITypeInfo *pITypeInfo_IGSFunctioNaterEvents;
   ITypeInfo* pIEvaluatorEventsTypeInfo;

   unsigned short wsVersionMajor = 1;
   unsigned short wsVersionMinor = 0;
 
   STDAPI GSystemsVariablesDllGetClassObject(HMODULE hModule,REFCLSID clsid,REFIID riid,void** ppObject);

#ifdef FUNCTION_SAMPLE

   bool trialExpired = FALSE;

#endif

   BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *) {

   switch(flag) {

   case DLL_PROCESS_ATTACH: {

      hModule = reinterpret_cast<HMODULE>(module);

      memset(szModuleName,0,1024);

      GetModuleFileName(hModule,szModuleName,1024);

      ITypeLib *ptLib;
      {
      char szLibraryName[1024];
      memset(szLibraryName,0,1024);
      sprintf(szLibraryName,"%s\\%d",szModuleName,FUNCTION_TYPELIB_ID);

      BSTR bstrLibraryName = SysAllocStringLen(NULL,strlen(szLibraryName) + 1);

      MultiByteToWideChar(CP_ACP,0,szLibraryName,-1,bstrLibraryName,strlen(szLibraryName) + 1);  
 
      LoadTypeLib(bstrLibraryName,&ptLib);
      SysFreeString(bstrLibraryName);
      }

      ptLib -> GetTypeInfoOfGuid(IID_IGSFunctioNater,&pITypeInfo_IGSFunctioNater);
      ptLib -> GetTypeInfoOfGuid(DIID_IGSFunctioNaterEvents,&pITypeInfo_IGSFunctioNaterEvents);

      {
      char szEvaluatorLib[1026];
      sprintf(szEvaluatorLib,"%s\\%d",szModuleName,EVALUATOR_TYPELIB_ID);
      BSTR bstrEvaluatorLib = SysAllocStringLen(NULL,strlen(szEvaluatorLib) + 1);
      memset(bstrEvaluatorLib,0,(strlen(szEvaluatorLib) + 1) * sizeof(OLECHAR));
      MultiByteToWideChar(CP_ACP,0,szEvaluatorLib,-1,bstrEvaluatorLib,strlen(szEvaluatorLib) + 1);

      LoadTypeLib(bstrEvaluatorLib,&ptLib);
      ptLib -> GetTypeInfoOfGuid(IID_IEvaluatorEvents,&pIEvaluatorEventsTypeInfo);
      SysFreeString(bstrEvaluatorLib);
      }

      wstrModuleName = SysAllocStringLen(NULL,strlen(szModuleName) + 1);
      memset(wstrModuleName,0,(strlen(szModuleName) + 1) * sizeof(OLECHAR));
      MultiByteToWideChar(CP_ACP, 0, szModuleName, -1, wstrModuleName, strlen(szModuleName) + 1);  

#ifdef FUNCTION_SAMPLE

      HKEY hKey;
      char szInstallTime[32];
      DWORD dwSize = 32;
      SYSTEMTIME sysTime,installTime;

      memset(&sysTime,0,sizeof(SYSTEMTIME));
      GetSystemTime(&sysTime);

      if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_CLASSES_ROOT,"CLSID\\{685223E6-5CF9-4d06-B744-DA4A9A777E68}",0,KEY_QUERY_VALUE,&hKey) ) {

         RegOpenKeyEx(hKey,"InProcServer",0,KEY_QUERY_VALUE,&hKey);

         RegQueryValueEx(hKey,NULL,0,NULL,(BYTE*)szInstallTime,&dwSize);
         
         szInstallTime[3] = '\0';
         szInstallTime[6] = '\0';
         szInstallTime[11] = '\0';
         installTime.wMonth = (WORD)atol(szInstallTime + 1);
         installTime.wDay = (WORD)atol(szInstallTime + 4);
         installTime.wYear = (WORD)atol(szInstallTime + 7);

         while ( installTime.wYear < sysTime.wYear ) {
            sysTime.wYear--;
            sysTime.wMonth += 12;
         }

         if ( installTime.wYear > sysTime.wYear || installTime.wMonth > sysTime.wMonth ) {
            trialExpired = true;
            break;
         }

         long dayOfYearInstalled = installTime.wDay + (installTime.wMonth - 1) * 30;
         long dayOfYearNow = sysTime.wDay + (sysTime.wMonth - 1) * 30;

         if ( dayOfYearNow - dayOfYearInstalled > 31 ) trialExpired = true;

         RegCloseKey(hKey);

      }
 
#endif

      }
      break;
 
   case DLL_PROCESS_DETACH:
 
      SysFreeString(wstrModuleName);

      break;
 
   default:
      break;
   }
   return TRUE;
   }
 
 
 
   class FunctionFactory : public IClassFactory {
   public:
 
      HRESULT STDMETHODCALLTYPE QueryInterface(const struct _GUID &,void **);
      unsigned long __stdcall AddRef();
      unsigned long __stdcall Release();
      HRESULT STDMETHODCALLTYPE CreateInstance(struct IUnknown *,const struct _GUID &,void **);
      HRESULT STDMETHODCALLTYPE LockServer(int);
 
      FunctionFactory() : refCount(0) {};
      ~FunctionFactory() {};
 
   private:
 
      int refCount;
 
   } functionFactory;
 
 

   STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject) {
   *ppObject = NULL;
#ifdef FUNCTION_SAMPLE
   if ( rclsid != CLSID_GSystemFunctioNaterSample ) {
      return CLASS_E_CLASSNOTAVAILABLE;
   }
#else
   if ( rclsid !=  CLSID_GSystemFunctioNater)
      return CLASS_E_CLASSNOTAVAILABLE;
#endif
   return functionFactory.QueryInterface(riid,ppObject);
   }
 
 
 
   STDAPI DllRegisterServer() {

   HKEY hKey;                             
   DWORD dwDisposition;

#ifdef FUNCTION_SAMPLE
                        
   RegCreateKeyEx(HKEY_CLASSES_ROOT,"CLSID\\{685223E6-5CF9-4d06-B744-DA4A9A777E68}",0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,NULL,&hKey,&dwDisposition);

   if ( REG_CREATED_NEW_KEY == dwDisposition ) {

      RegCreateKeyEx(hKey,"InProcServer",0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,NULL,&hKey,&dwDisposition);

      char szTime[128];
      SYSTEMTIME sysTime;
      memset(&sysTime,0,sizeof(SYSTEMTIME));
      GetSystemTime(&sysTime);
      memset(szTime,0,sizeof(szTime));
      sprintf(szTime,"/%02d/%02d/%02d",sysTime.wMonth,sysTime.wDay,sysTime.wYear);
      RegSetValueEx(hKey,NULL,0,REG_SZ,(BYTE*)szTime,strlen(szTime));
   }

   RegCloseKey(hKey);

#else

   RegCreateKeyEx(HKEY_CLASSES_ROOT,"CLSID\\{685223E6-5CF9-4d06-B744-DA4A9A777E68}",0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,NULL,&hKey,&dwDisposition);

   RegDeleteKey(hKey,"InProcServer");

   RegDeleteKey(HKEY_CLASSES_ROOT,"CLSID\\{685223E6-5CF9-4d06-B744-DA4A9A777E68}");

#endif

   utilsDllRegisterTypeLib(wstrModuleName);

   GUID categoryIDs[6];
   categoryIDs[0] = CATID_PersistsToFile;
   categoryIDs[1] = CATID_PersistsToStorage;
   categoryIDs[2] = CATID_PersistsToStream;
   categoryIDs[3] = CATID_PersistsToStreamInit;
   categoryIDs[4] = CATID_PersistsToPropertyBag;
   categoryIDs[5] = CATID_NULL;

#ifdef FUNCTION_SAMPLE
   return utilsDllRegisterObject(CLSID_GSystemFunctioNaterSample,LIBID_FunctioNaterSample,hModule,
                                  szModuleName,
                                  "GSystems FunctioNater Sample Object",
                                  "GSystem.FunctioNaterSampleControl",
                                  "GSystem.FunctioNaterSampleControl.1",
                                  categoryIDs,
                                  IDOCXBITMAP_FUNCTION,oleMiscStatus,
                                  false,true,true);
#else
   return utilsDllRegisterObject(CLSID_GSystemFunctioNater,LIBID_FunctioNater,hModule,
                                  szModuleName,
                                  "GSystems FunctioNater Object",
                                  "GSystem.FunctioNaterControl",
                                  "GSystem.FunctioNaterControl.1",
                                  categoryIDs,
                                  IDOCXBITMAP_FUNCTION,oleMiscStatus,
                                  true,true,true);
#endif
   }
 
 
   STDAPI DllUnregisterServer() {
#ifdef FUNCTION_SAMPLE
   utilsDllUnregisterObject(CLSID_GSystemFunctioNaterSample,"GSystem.FunctionNaterSampleControl","GSystem.FunctioNaterSampleControl.1");
   return utilsDllUnregisterTypeLib(wstrModuleName,LIBID_FunctioNaterSample,wsVersionMajor,wsVersionMinor);
#else
   utilsDllUnregisterObject(CLSID_GSystemFunctioNater,"GSystem.FunctioNaterControl","GSystem.FunctioNaterControl.1");
   return utilsDllUnregisterTypeLib(wstrModuleName,LIBID_FunctioNater,wsVersionMajor,wsVersionMinor);
#endif
   }
 
 
   HRESULT STDMETHODCALLTYPE FunctionFactory::QueryInterface(const struct _GUID &iid,void **ppv) {
   *ppv = NULL; 
   if ( iid != IID_IUnknown && iid != IID_IClassFactory && iid != IID_IGSFunctioNater ) 
      return E_NOINTERFACE; 
   *ppv = this; 
   AddRef(); 
   return S_OK; 
   }
   unsigned long __stdcall FunctionFactory::AddRef() {return ++refCount;}
   unsigned long __stdcall FunctionFactory::Release() {return --refCount;}
 
 
   HRESULT STDMETHODCALLTYPE FunctionFactory::CreateInstance(struct IUnknown *pUnkOuter,const struct _GUID &riid,void **ppv) {

#ifdef FUNCTION_SAMPLE
#ifdef FUNCTION_EVALUATION
   if ( trialExpired ) {
      MessageBox(NULL,"The GSystems FunctioNater Control Sample trial period of 30 days has expired.\n\n"
                        "Thank you sincerely for evaluating this product.","Note",MB_OK | MB_ICONEXCLAMATION);
      return CLASS_E_CLASSNOTAVAILABLE;
   }
#endif
#endif

   HRESULT hr = S_OK;
   *ppv = NULL;
 
   Function *f = new Function(pUnkOuter);
   hr = f -> QueryInterface(riid,ppv);
 
   if ( ! ppv ) 
      delete f;
 
   return hr;
   }
 
 
   HRESULT STDMETHODCALLTYPE FunctionFactory::LockServer(int) {
   return 0;
   }
