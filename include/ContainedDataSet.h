
#pragma once

#include "ContainedObject.h"
#include "DataSet_i.h"

   class ContainedDataSet : 
      public ContainedObject,
      public IDataSetEvents {

   public:

      ContainedDataSet(G *pParent,long plotID,HWND hwndTab,IDataSet *pIDataSet,IUnknown* pIUnknownObject,REFIID riidEventsInterface);

      IDataSet *pDataSet() { return pIDataSet; };

      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();

      STDMETHOD(Clear)();
      STDMETHOD(Started)(long cntExpectedResults);
      STDMETHOD(TakeValues)(long iterationNo,long valueCount,SAFEARRAY **ppNamesArrayOfStrings,SAFEARRAY **ppValuesArrayOfDoubles);
      STDMETHOD(Finished)();


  private:

     long plotID;

     IDataSet *pIDataSet;

  };

