
#include "PlotTypes.h"

   HRESULT PlotTypes::get_Count(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = itemName.size();
   return S_OK;
   }

   HRESULT PlotTypes::get_Name(long itemNumber,BSTR *pBstr) {
   if ( ! pBstr )
      return E_POINTER;
   *pBstr = itemName[itemNumber];
   return S_OK;
   }

   HRESULT PlotTypes::get_Is3DOnly(long itemNumber,VARIANT_BOOL *pAnswer) {
   if ( ! pAnswer )
      return E_POINTER;
   *pAnswer = itemIs3DOnly[itemNumber] ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
   }

   HRESULT PlotTypes::get_UsesMaterialShading(long itemNumber,VARIANT_BOOL *pAnswer) {
   if ( ! pAnswer )
      return E_POINTER;
   *pAnswer = itemUseMaterialShading[itemNumber] ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
   }

   HRESULT PlotTypes::get_HasProperties(long itemNumber,VARIANT_BOOL *pAnswer) {
   if ( ! pAnswer )
      return E_POINTER;
   *pAnswer = itemHasProperties[itemNumber] ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
   }


   HRESULT PlotTypes::InitNew(long itemNumber,REFIID instanceGUID) {

   BSTR bstrCLSID;

   StringFromCLSID(instanceGUID,&bstrCLSID);

   long key = HashCode(bstrCLSID);

   if ( allocatedInstancePropertiesStorage.find(key) == allocatedInstancePropertiesStorage.end() ) {

      commonProperties *pStorage = new commonProperties();

      memcpy(pStorage,&properties,sizeof(commonProperties));

      switch ( itemNumber ) {
      case ITEM_BLOCKS: {
         pStorage -> pvAdditionalProperties = (void *)new blocksProperties();
         pStorage -> additionalpropertiesSize = sizeof(blocksProperties);
         break;

      default:
         break;
      }

      }

      allocatedInstancePropertiesStorage[key] = pStorage;

   }

   CoTaskMemFree(bstrCLSID);

   return S_OK;
   }


   HRESULT PlotTypes::Save(long itemNumber,REFIID instanceGUID,void *pvIStream) {

   BSTR bstrCLSID;

   StringFromCLSID(instanceGUID,&bstrCLSID);
   
   long key = HashCode(bstrCLSID);

   if ( allocatedInstancePropertiesStorage.find(key) == allocatedInstancePropertiesStorage.end() ) 
      InitNew(itemNumber,instanceGUID);

   CoTaskMemFree(bstrCLSID);

   void *pStorage = allocatedInstancePropertiesStorage[key];

   ULONG sizeStorage = sizeof(commonProperties);

   IStream *pIStream = (IStream *)pvIStream;

   ULONG cbWritten;

   pIStream -> Write(pStorage,sizeStorage,&cbWritten);

   if ( 0 < allocatedInstancePropertiesStorage[key] -> additionalpropertiesSize ) 
      pIStream -> Write(allocatedInstancePropertiesStorage[key] -> pvAdditionalProperties,allocatedInstancePropertiesStorage[key] -> additionalpropertiesSize,&cbWritten);

   return S_OK;
   }


   HRESULT PlotTypes::Load(long itemNumber,REFIID instanceGUID,void *pvIStream) {

   BSTR bstrCLSID;

   StringFromCLSID(instanceGUID,&bstrCLSID);

   long key = HashCode(bstrCLSID);

   if ( allocatedInstancePropertiesStorage.find(key) == allocatedInstancePropertiesStorage.end() ) 
      InitNew(itemNumber,instanceGUID);

   void *pStorage = allocatedInstancePropertiesStorage[key];

   ULONG sizeStorage = sizeof(commonProperties);

   IStream *pIStream = (IStream *)pvIStream;

   ULONG cbRead;

   pIStream -> Read(pStorage,sizeStorage,&cbRead);

#if 0
   if ( allocatedInstancePropertiesStorage.find(key) == allocatedInstancePropertiesStorage.end() ) {
      commonProperties *pStorage = new commonProperties();
      memcpy(pStorage,&properties,sizeof(commonProperties));
      allocatedInstancePropertiesStorage[key] = pStorage;
   }
#endif

#if 1
   if ( 0 < allocatedInstancePropertiesStorage[key] -> additionalpropertiesSize ) {
      allocatedInstancePropertiesStorage[key] -> pvAdditionalProperties = new BYTE[allocatedInstancePropertiesStorage[key] -> additionalpropertiesSize ];
      pIStream -> Read(allocatedInstancePropertiesStorage[key] -> pvAdditionalProperties,allocatedInstancePropertiesStorage[key] -> additionalpropertiesSize,&cbRead);
   }
#endif

   CoTaskMemFree(bstrCLSID);

   return S_OK;
   }


   HRESULT PlotTypes::Execute(long itemNumber,REFIID instanceGUID,long segmentID,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) {
   (*executePrep)(pvIPlot,pvIOpenGLImplementation,pvIDataSet);
   (*itemExecute[itemNumber])(segmentID,findProperties(itemNumber,instanceGUID),pvIPlot,pvIOpenGLImplementation,pvIDataSet);
   return S_OK;
   }


   HRESULT PlotTypes::ShowProperties(long itemNumber,HWND hwndParent,REFIID instanceGuid,void (__stdcall *pCallback)(void *,ULONG_PTR),void *pvArg,ULONG_PTR cookie) {

   pActiveProperties = findProperties(itemNumber,instanceGuid);

   IUnknown *pIUnknown = NULL;
   itemPropertyPageClients[itemNumber] -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
   pIGProperties -> ShowProperties(hwndParent,pIUnknown);
   pIUnknown -> Release();

   if ( pCallback ) 
      pCallback(pvArg,cookie);

   return S_OK;
   }