
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


   HRESULT PlotTypes::Execute(long itemNumber,long segmentID,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet) {
   (*executePrep)(pvIPlot,pvIOpenGLImplementation,pvIDataSet);
   (*itemExecute[itemNumber])(segmentID,pvIPlot,pvIOpenGLImplementation,pvIDataSet);
   return S_OK;
   }


   HRESULT PlotTypes::ShowProperties(long itemNumber,HWND hwndParent,REFIID instanceGuid,void (__stdcall *pCallback)(void *,ULONG_PTR),void *pvArg,ULONG_PTR cookie) {
   IUnknown *pIUnknown = NULL;
   itemPropertyPageClients[itemNumber] -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
   pIGProperties -> ShowProperties(hwndParent,pIUnknown);
   pIUnknown -> Release();
   if ( pCallback ) 
      pCallback(pvArg,cookie);
   return S_OK;
   }