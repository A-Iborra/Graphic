// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Windows.h>
#include <map>
#include <functional>
#include <CommCtrl.h>
#include <gl\gl.h>

#include "resource.h"
#include "GSystem_i.h"
#include "Properties_i.h"
#include "Variable_i.h"
#include "Evaluator_i.h"
#include "DataSet_i.h"
#include "OpenGLImplementation_i.h"
#include "Plot_i.h"

#include "PlotTypes_i.h"

#include "utils.h"
#include "utilities.h"

#define ITEM_NATURAL       1
#define ITEM_WRITEFRAME    2
#define ITEM_SURFACE       3
#define ITEM_HISTORGRAM    4
#define ITEM_BLOCKS        5
#define ITEM_BALLS         6

   struct blocksProperties {
      bool keepCubic;
      double sizeInPercentDomain[3]{10.0,10.0,10.0};
   };

   struct commonProperties {
      double defaultSolidSizeInPercentDomain{10.0};
      void *pvAdditionalProperties{NULL};
      long additionalpropertiesSize{0};
   };


   class PlotTypes : public IGSystemPlotType {

   public:

      PlotTypes();
      ~PlotTypes();

      // IGSystemPlotType

      STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();

      STDMETHOD(get_Count)(long *pCountProvided);
      STDMETHOD(get_Name)(long item,BSTR *pBstr);
      STDMETHOD(get_Is3DOnly)(long item,VARIANT_BOOL *);
      STDMETHOD(get_UsesMaterialShading)(long item,VARIANT_BOOL *);
      STDMETHOD(get_HasProperties)(long item,VARIANT_BOOL *);

      STDMETHOD(InitNew)(long item,REFIID instanceGUID);
      STDMETHOD(Save)(long item,REFIID instanceGUID,void *pvIStream);
      STDMETHOD(Load)(long item,REFIID instanceGUID,void *pvIStream);

      STDMETHOD(Execute)(long item,REFIID instanceGUID,long segmentId,void *pvIPlot,void *pvIOpenGLImplementation,void *pvIDataSet);
      STDMETHOD(ShowProperties)(long item,HWND hwndParent,REFIID instanceGuid,void (__stdcall *)(void *,ULONG_PTR),void *,ULONG_PTR);

      // IPropertyPageClient

      class _IGPropertyPageClient : public IGPropertyPageClient {

      public:

         _IGPropertyPageClient(PlotTypes *pp,long dialogIdentifier,WNDPROC dialogProc,char *dialogTitle);
         ~_IGPropertyPageClient();

         STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

      private:

         STDMETHOD(BeforeAllPropertyPages)();
         STDMETHOD(GetPropertyPagesInfo)(long* countPages,SAFEARRAY** stringDescriptions,SAFEARRAY** stringHelpDirs,SAFEARRAY** pSize);
         STDMETHOD(CreatePropertyPage)(long,HWND,RECT*,BOOL,HWND* hwndPropertyPage);
         STDMETHOD(Apply)();
         STDMETHOD(IsPageDirty)(long,BOOL*);
         STDMETHOD(Help)(BSTR bstrHelpDir);
         STDMETHOD(TranslateAccelerator)(long,long*);
         STDMETHOD(AfterAllPropertyPages)(BOOL);
         STDMETHOD(DestroyPropertyPage)(long);

         STDMETHOD(GetPropertySheetHeader)(void *pHeader);
         STDMETHOD(get_PropertyPageCount)(long *pCount);
         STDMETHOD(GetPropertySheets)(void *pSheets);

         PlotTypes *pParent{NULL};

         long dialogID;
         char szDialogTitle[128];
         WNDPROC pDialogProc{NULL};
         long refCount{0};

      };

      void natural(commonProperties *pProperties,long segmentID);

      void surface(commonProperties *pProperties,long segmentID);
      void wireFrame(commonProperties *pProperties,long segmentID);

      void stacks(commonProperties *pProperties,long segmentID);

      void blocks(commonProperties *pProperties,long segmentID);

      void balls(commonProperties *pProperties,long segmentID);

      void normals(commonProperties *pProperties,long segmentID);

   private:

      IPlot *pIPlot{NULL};
      IDataSet *pIDataSet{NULL};
      IOpenGLImplementation *pIOpenGLImplementation{NULL};

      IGProperty *propertyTopSurfaceColor{NULL};
      IGProperty *propertyBottomSurfaceColor{NULL};
      IGProperty *propertyLineColor{NULL};
      IGProperty *propertyLineWeight{NULL};

      IGProperties *pIGProperties{NULL};

      long refCount{0};

      commonProperties *findProperties(long itemNumber,REFIID);

      std::function<void(void *,void *,void *)> *executePrep;

      std::map<long,BSTR> itemName;
      std::map<long,bool> itemIs3DOnly;
      std::map<long,bool> itemUseMaterialShading;
      std::map<long,bool> itemHasProperties;
      std::map<long,std::function<void(long,commonProperties *,void *,void *,void *)> *> itemExecute;

      std::map<long,_IGPropertyPageClient *> itemPropertyPageClients;

      std::map<long,commonProperties *> allocatedInstancePropertiesStorage;

      IGProperty *propertyAllBinary{NULL};

      commonProperties properties;

      commonProperties *pActiveProperties{NULL};

      static LRESULT APIENTRY propertiesHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT APIENTRY blocksPropertiesHandler(HWND,UINT,WPARAM,LPARAM);

   };

#ifdef DEFINE_DATA

HMODULE hModule = NULL;

char szModuleName[MAX_PATH];

#else

extern HMODULE hModule;

extern char szModuleName[];

#endif
