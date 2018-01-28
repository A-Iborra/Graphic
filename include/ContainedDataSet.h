// Copyright 2017 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "ContainedObject.h"
#include "DataSet_i.h"

   class ContainedDataSet : 
      public ContainedObject,
      public IDataSetEvents {

   public:

      ContainedDataSet(G *pParent,ULONG_PTR plotID,HWND hwndTab,IDataSet *pIDataSet,IUnknown* pIUnknownObject,REFIID riidEventsInterface);

      IDataSet *pDataSet() { return pIDataSet; };

      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();

      STDMETHOD(Clear)();
      STDMETHOD(Started)(long cntExpectedResults);
      STDMETHOD(TakeValues)(long iterationNo,long valueCount,SAFEARRAY **ppNamesArrayOfStrings,SAFEARRAY **ppValuesArrayOfDoubles);
      STDMETHOD(Finished)();


  private:

     ULONG_PTR plotID;

     IDataSet *pIDataSet;

  };

