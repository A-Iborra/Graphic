
#include "ObjectInOffice.h"

#include <stdio.h>

#include "Utils.h"

#include "ObjectInOffice_i.c"

   unsigned short wsVersionMajor = 1;
   unsigned short wsVersionMinor = 0;

   BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *res) {
 
   switch(flag) {

   case DLL_PROCESS_ATTACH: {

      hModule = reinterpret_cast<HMODULE>(module);

      GetModuleFileName(hModule,szModuleName,1024);
      bstrModuleName = SysAllocStringLen(NULL,(DWORD)strlen(szModuleName) + 1);
      memset(bstrModuleName,0,(strlen(szModuleName) + 1) * sizeof(OLECHAR));
      MultiByteToWideChar(CP_ACP,0,szModuleName,-1,bstrModuleName,(DWORD)strlen(szModuleName));
 
      //ITypeLib *ptLib;
      //LoadTypeLib(bstrModuleName,&ptLib);
 
      //HRESULT hr = ptLib -> GetTypeInfoOfGuid(CLSID_ObjectInOffice,&pITypeInfo_CLSID_G);

      //hr = ptLib -> GetTypeInfoOfGuid(IID_IObjectInOffice,&pITypeInfo_IG);

      //{
      //char szLibrary[1026];
      //sprintf(szLibrary,"%s\\%ld",szModuleName,(long)FUNCTION_TYPELIB_ID);
      //BSTR bstrLibrary = SysAllocStringLen(NULL,256);
      //memset(bstrLibrary,0,strlen(szLibrary) + 1);
      //MultiByteToWideChar(CP_ACP,0,szLibrary,-1,bstrLibrary,strlen(szLibrary) + 1);

      //LoadTypeLib(bstrLibrary,&ptLib);
      //hr = ptLib -> GetTypeInfoOfGuid(DIID_IGSFunctioNaterEvents,&pITypeInfo_IGSFunctioNaterEvents);

      //SysFreeString(bstrLibrary);
      //}

      }
      break;
 
   case DLL_PROCESS_DETACH:

      SysFreeString(bstrModuleName);

      break;
 
   default:
      break;
   }
 
   return TRUE;
   }

   class factory : public IClassFactory {
   public:
 
      STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();
      STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
      STDMETHOD (LockServer)(BOOL fLock);
 
      factory() {};
      ~factory() {};
 
   } theFactory;


   STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppObject) {
   *ppObject = NULL;
   if ( rclsid == CLSID_ObjectInOffice ) 
      return theFactory.QueryInterface(riid,ppObject);
   return E_NOINTERFACE;
   }


   STDAPI DllRegisterServer() {
   utilsDllRegisterTypeLib(bstrModuleName);
   return utilsDllRegisterObject(CLSID_ObjectInOffice,LIBID_ObjectInOffice,hModule,szModuleName,"InnoVisioNate Office Object","InnoVisioNate.Office","InnoVisioNate.Office.1",
                                                   (CATID *)NULL,0,oleMisc,false,false,true);//IDOCXBITMAP_GRAPHIC,oleMisc,true,true,true);
   }


   STDAPI DllUnregisterServer() {
   utilsDllUnregisterObject(CLSID_ObjectInOffice,"InnoVisioNate.Office","InnoVisioNate.Office.1");
   return utilsDllUnregisterTypeLib(bstrModuleName,LIBID_ObjectInOffice,wsVersionMajor,wsVersionMinor);
   }


   long __stdcall factory::QueryInterface(REFIID iid, void **ppv) { 
   *ppv = NULL; 
   if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
      *ppv = this; 
   else 
      return E_NOINTERFACE; 
   AddRef(); 
   return S_OK; 
   } 

   unsigned long __stdcall factory::AddRef() { return 1; }
   unsigned long __stdcall factory::Release() { return 1; }
 
 
   HRESULT STDMETHODCALLTYPE factory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 

   *ppv = NULL; 

   if ( ! pObjectInOffice )
      pObjectInOffice = new ObjectInOffice(punkOuter);

   HRESULT rc = pObjectInOffice -> QueryInterface(riid,ppv);

   if ( ! ( S_OK == rc )  ) {
      delete pObjectInOffice;
      pObjectInOffice = NULL;
   }

   return rc;
   } 
 
 
   long __stdcall factory::LockServer(int fLock) { 
   return S_OK; 
   } 