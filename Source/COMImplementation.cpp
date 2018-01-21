// Copyright 2017 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "Graphic_resource.h"

#include "utils.h"

#include "GraphicControl_i.h"

#include "plot_i.c"
#include "axis_i.c"
#include "ViewSet_i.c"
#include "Properties_i.c"
#include "OpenGLImplementation_i.c"
#include "GraphicControl_i.c"
#include "Text_i.c"
#include "DataSet_i.c"
#include "Function_i.c"
#include "GSystem_i.c"

#include "Variable_i.h"
#include "Evaluator_i.h"
#include "Evaluator_i.c"

   G *pStaticObject = NULL;

   class GFactory : public IClassFactory {
   public:
 
      STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();
      STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
      STDMETHOD (LockServer)(BOOL fLock);
 
      GFactory(REFCLSID mg) : myGuid(mg) {};
      ~GFactory() {};

   private:

      GUID myGuid;
 
   };
 
   HMODULE hModule;
   char szModuleName[1024];
   BSTR bstrModuleName;

   ITypeInfo* pITypeInfo_CLSID_G;
   ITypeInfo* pITypeInfo_IG;
   ITypeInfo* pITypeInfo_IGSFunctioNaterEvents;

   unsigned short wsVersionMajor = 1;
   unsigned short wsVersionMinor = 0;
 
   long oleMisc = OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST | OLEMISC_INSIDEOUT | 
                            OLEMISC_CANTLINKINSIDE | OLEMISC_RECOMPOSEONRESIZE | OLEMISC_ALWAYSRUN;
 
   STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject);
 
   BOOL WINAPI DllMain(HANDLE module,ULONG flag,void *res) {
 
   switch(flag) {

   case DLL_PROCESS_ATTACH: {

      hModule = reinterpret_cast<HMODULE>(module);

      GetModuleFileName(hModule,szModuleName,1024);
      bstrModuleName = SysAllocStringLen(NULL,strlen(szModuleName) + 1);
      memset(bstrModuleName,0,(strlen(szModuleName) + 1) * sizeof(OLECHAR));
      MultiByteToWideChar(CP_ACP,0,szModuleName,-1,bstrModuleName,strlen(szModuleName));
 
      ITypeLib *ptLib;
      LoadTypeLib(bstrModuleName,&ptLib);
 
      HRESULT hr = ptLib -> GetTypeInfoOfGuid(CLSID_GSystemGraphic,&pITypeInfo_CLSID_G);

      hr = ptLib -> GetTypeInfoOfGuid(IID_IGSGraphic,&pITypeInfo_IG);

      {
      char szLibrary[1026];
      sprintf(szLibrary,"%s\\%ld",szModuleName,(long)FUNCTION_TYPELIB_ID);
      BSTR bstrLibrary = SysAllocStringLen(NULL,256);
      memset(bstrLibrary,0,strlen(szLibrary) + 1);
      MultiByteToWideChar(CP_ACP,0,szLibrary,-1,bstrLibrary,strlen(szLibrary) + 1);

      LoadTypeLib(bstrLibrary,&ptLib);
      hr = ptLib -> GetTypeInfoOfGuid(DIID_IGSFunctioNaterEvents,&pITypeInfo_IGSFunctioNaterEvents);

      SysFreeString(bstrLibrary);
      }

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
 
char OBJECT_DESCRIPTION[][128] = {"GSystemGraphic Position And Size Properties",
                                    "GSystemGraphic Style Properties",
                                    "GSystemGraphic Background Properties",
                                    "GSystemGraphic Text Properties",
                                    "GSystemGraphic Lighting Properties",
                                    "GSystemGraphic Axis Properties",
                                    "GSystemGraphic Plot Properties",
                                    "GSystemGraphic DataSet Properties",
                                    "GSystemGraphic Function Properties"};
 
char OBJECT_NAME[][128] = {"GSystemGraphic.PropertiesPosSize",
                                 "GSystemGraphic.PropertiesStyle",
                                 "GSystemGraphic.PropertiesBackground",
                                 "GSystemGraphic.PropertiesText",
                                 "GSystemGraphic.PropertiesLighting",
                                 "GSystemGraphic.PropertiesAxis",
                                 "GSystemGraphic.PropertiesPlot",
                                 "GSystemGraphic.PropertiesDataSets",
                                 "GSystemGraphic.PropertiesFunctions"};

char OBJECT_NAME_V[][128] = {"GSystemGraphic.PropertiesPosSize.1",
                                 "GSystemGraphic.PropertiesStyle.1",
                                 "GSystemGraphic.PropertiesBackground.1",
                                 "GSystemGraphic.PropertiesText.1",
                                 "GSystemGraphic.PropertiesLighting.1",
                                 "GSystemGraphic.PropertiesAxis.1",
                                 "GSystemGraphic.PropertiesPlot.1",
                                 "GSystemGraphic.PropertiesDataSets.1",
                                 "GSystemGraphic.PropertiesFunctions.1"};

CLSID OBJECT_CLSID[] = {CLSID_GSystemGraphicPropertiesPosSize,
                                 CLSID_GSystemGraphicPropertiesStyle,
                                 CLSID_GSystemGraphicPropertiesBackground,
                                 CLSID_GSystemGraphicPropertiesText,
                                 CLSID_GSystemGraphicPropertiesLighting,
                                 CLSID_GSystemGraphicPropertiesAxis,
                                 CLSID_GSystemGraphicPropertiesPlot,
                                 CLSID_GSystemGraphicPropertiesDataSets,
                                 CLSID_GSystemGraphicPropertiesFunctions,
                                 GUID_NULL};

   static GFactory gFactory(CLSID_GSystemGraphic);
   static GFactory propertyPagePosSize(OBJECT_CLSID[0]);
   static GFactory propertyPageStyle(OBJECT_CLSID[1]);
   static GFactory propertyPageBackground(OBJECT_CLSID[2]);
   static GFactory propertyPageText(OBJECT_CLSID[3]);
   static GFactory propertyPageLighting(OBJECT_CLSID[4]);
   static GFactory propertyPageAxis(OBJECT_CLSID[5]);
   static GFactory propertyPagePlot(OBJECT_CLSID[6]);
   static GFactory propertyPageDataSets(OBJECT_CLSID[7]);
   static GFactory propertyPageFunctions(OBJECT_CLSID[8]);
 
   STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppObject) {

   *ppObject = NULL;

   if ( rclsid == CLSID_GSystemGraphic ) 
      return gFactory.QueryInterface(riid,ppObject);

   if ( rclsid == OBJECT_CLSID[0] )
      return propertyPagePosSize.QueryInterface(riid,ppObject);

   if ( rclsid == OBJECT_CLSID[1] )
      return propertyPageStyle.QueryInterface(riid,ppObject);

   if ( rclsid == OBJECT_CLSID[2] )
      return propertyPageBackground.QueryInterface(riid,ppObject);

   if ( rclsid == OBJECT_CLSID[3] )
      return propertyPageText.QueryInterface(riid,ppObject);

   if ( rclsid == OBJECT_CLSID[4] )
      return propertyPageLighting.QueryInterface(riid,ppObject);

   if ( rclsid == OBJECT_CLSID[5] )
      return propertyPageAxis.QueryInterface(riid,ppObject);

   if ( rclsid == OBJECT_CLSID[6] )
      return propertyPagePlot.QueryInterface(riid,ppObject);

   if ( rclsid == OBJECT_CLSID[7] )
      return propertyPageDataSets.QueryInterface(riid,ppObject);

   if ( rclsid == OBJECT_CLSID[8] )
      return propertyPageFunctions.QueryInterface(riid,ppObject);

   return E_NOINTERFACE;
   }

   STDAPI DllRegisterServer() {

   utilsDllRegisterTypeLib(bstrModuleName);

   {
   char szLibrary[1026];
   sprintf(szLibrary,"%s\\%ld",szModuleName,(long)GSYSTEMS_TYPELIB_ID);
   BSTR bstrLibrary = SysAllocStringLen(NULL,256);
   memset(bstrLibrary,0,strlen(szLibrary) + 1);
   MultiByteToWideChar(CP_ACP,0,szLibrary,-1,bstrLibrary,strlen(szLibrary) + 1);
   utilsDllRegisterTypeLib(bstrLibrary);
   utilsDllRegisterObject(CLSID_GSystems,LIBID_GSystems,hModule,
                                  szModuleName,
                                  "GSystems Common Definitions Type Library",
                                  "GSystem.CommonDefinitions",
                                  "GSystem.CommonDefinitions.1",
                                  (CATID *)NULL,0,oleMisc,false,false,true);
   }

   for ( long objIndex = 0; 1; objIndex++ ) {

      if ( GUID_NULL == OBJECT_CLSID[objIndex] )
         break;

      utilsDllRegisterObject(OBJECT_CLSID[objIndex],LIBID_Graphic,hModule,
                                  szModuleName,
                                  OBJECT_DESCRIPTION[objIndex],
                                  OBJECT_NAME[objIndex],
                                  OBJECT_NAME_V[objIndex],
                                  (CATID *)NULL,0,0,false,false,false);
   }

   return utilsDllRegisterObject(CLSID_GSystemGraphic,LIBID_Graphic,hModule,
                                  szModuleName,
                                  "GSystems Graphic Object",
                                  "GSystem.Graphic",
                                  "GSystem.Graphic.1",
                                  (CATID *)NULL,IDOCXBITMAP_GRAPHIC,oleMisc,true,true,true);
   }
 
 
   STDAPI DllUnregisterServer() {

   for ( long objIndex = 0; 1; objIndex++ ) {

      if ( GUID_NULL == OBJECT_CLSID[objIndex] )
         break;

      utilsDllUnregisterObject(OBJECT_CLSID[objIndex],OBJECT_NAME[objIndex],OBJECT_NAME_V[objIndex]);
   }

   utilsDllUnregisterObject(CLSID_GSystemGraphic,"GSystem.Graphic","GSystem.Graphic.1");

   return utilsDllUnregisterTypeLib(bstrModuleName,LIBID_Graphic,wsVersionMajor,wsVersionMinor);
   }
 
 
   long __stdcall GFactory::QueryInterface(REFIID iid, void **ppv) { 
   *ppv = NULL; 
   if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
      *ppv = this; 
   else 
      return E_NOINTERFACE; 
   AddRef(); 
   return S_OK; 
   } 

   unsigned long __stdcall GFactory::AddRef() { return 1; }
   unsigned long __stdcall GFactory::Release() { return 1; }
 
 
   HRESULT STDMETHODCALLTYPE GFactory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 

   *ppv = NULL; 

#if 1

   if ( ! pStaticObject )
      pStaticObject = new G(punkOuter);

   if ( CLSID_GSystemGraphic == myGuid ) 
      return pStaticObject -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[0] == myGuid )
      return pStaticObject -> PropertyPage(0) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[1] == myGuid )
      return pStaticObject -> PropertyPage(1) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[2] == myGuid )
      return pStaticObject -> PropertyPage(2) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[3] == myGuid )
      return pStaticObject -> PropertyPage(3) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[4] == myGuid )
      return pStaticObject -> PropertyPage(4) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[5] == myGuid )
      return pStaticObject -> PropertyPage(5) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[6] == myGuid )
      return pStaticObject -> PropertyPage(6) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[8] == myGuid )
      return pStaticObject -> PropertyPage(7) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[7] == myGuid )
      return pStaticObject -> PropertyPage(8) -> QueryInterface(riid,ppv);

   delete pStaticObject;

   pStaticObject = NULL;

#else

   G *pG = new G(punkOuter);

   if ( CLSID_GSystemGraphic == myGuid ) 
      return pG -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[0] == myGuid )
      return pG -> PropertyPage(0) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[1] == myGuid )
      return pG -> PropertyPage(1) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[2] == myGuid )
      return pG -> PropertyPage(2) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[3] == myGuid )
      return pG -> PropertyPage(3) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[4] == myGuid )
      return pG -> PropertyPage(4) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[5] == myGuid )
      return pG -> PropertyPage(5) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[6] == myGuid )
      return pG -> PropertyPage(6) -> QueryInterface(riid,ppv);

   if ( OBJECT_CLSID[7] == myGuid )
      return pG -> PropertyPage(7) -> QueryInterface(riid,ppv);

   delete pG;
#endif

   return E_NOINTERFACE;
   } 
 
 
   long __stdcall GFactory::LockServer(int fLock) { 
   return S_OK; 
   } 
