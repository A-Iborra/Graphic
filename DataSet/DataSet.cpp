/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <olectl.h>
#include <CommCtrl.h>

#include <stdio.h>
#include <string.h>

#include "general.h"
#include "utils.h"
#include "list.cpp"

#include "dataset.h"

   DataPoint movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};
 
   long oleMiscStatus = OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST | OLEMISC_INSIDEOUT | 
                           OLEMISC_CANTLINKINSIDE | OLEMISC_RECOMPOSEONRESIZE | OLEMISC_ALWAYSRUN;

   DataSet::DataSet(IUnknown *pUnk) : 

     pWhenChangedCallback(NULL),
     pWhenChangedCallbackArg(NULL),

     pUnknownOuter(pUnk),
     otherDomains() {

   firstData = NULL;
   topData = NULL;
   gdiData = NULL;
   xMax = -DBL_MAX;
   yMax = -DBL_MAX;
   zMax = -DBL_MAX;
   xMin = DBL_MAX;
   yMin = DBL_MAX;
   zMin = DBL_MAX;

   pBoundingBox = new boundingBox(&xMin,&yMin,&zMin,&xMax,&yMax,&zMax);

   memset(szName,0,sizeof(szName));
   memset(szDataSource,0,sizeof(szDataSource));
   memset(szNamedRange,0,sizeof(szNamedRange));
   memset(szSpreadsheetName,0,sizeof(szSpreadsheetName));
   memset(szEquations,0,sizeof(szEquations));

   pIOleInPlaceActiveObject = new _IOleInPlaceActiveObject(this);

   pIConnectionPoint = new _IConnectionPoint(this,IID_IDataSetEvents);

   IUnknown* pIUnknownThis;

   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));

   CoCreateInstance(CLSID_InnoVisioNateProperties,pIUnknownThis,CLSCTX_INPROC_SERVER,IID_IUnknown,reinterpret_cast<void **>(&pIUnknownProperties));

   pIUnknownProperties -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&pIGProperties));

   pIUnknownThis -> Release();

   IGPropertiesClient *pIPropertiesClient = NULL;

   QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void **>(&pIPropertiesClient));

   pIGProperties -> Advise(pIPropertiesClient);

   pIPropertiesClient -> Release();

   pIGProperties -> Add(L"state",&pPropertyState);

   pPropertyState -> directAccess(TYPE_BINARY,&propertiesStart,offsetof(DataSet,propertiesEnd) - offsetof(DataSet,propertiesStart));

   CLSIDFromString(L"Excel.Application",&CLSID_excel);

   return;
   }
 

   DataSet::~DataSet() {

   ReSet();

   for ( IDataSet *pOther : otherDomains ) 
      pOther -> Release();
   otherDomains.clear();

   for ( extents *pExtents : extentsStack ) 
      delete pExtents;
   extentsStack.clear();

   delete pBoundingBox;

   if ( pIEvaluator )
      pIEvaluator -> Release();

   if ( pIUnknownProperties )
      pIUnknownProperties -> Release();

   if ( pIOleInPlaceSite ) 
      pIOleInPlaceSite -> Release();

   if ( pIOleClientSite ) 
      pIOleClientSite -> Release();

   if ( pIPropertyNotifySink ) 
      pIPropertyNotifySink -> Release();

   if ( pAdviseSink ) 
      pAdviseSink -> Release();

   if ( pIGProperties ) 
      pIGProperties -> Release();

   if ( enumConnectionPoints ) 
      delete enumConnectionPoints;

   delete pIConnectionPoint;

   return;
   }
 

   int DataSet::initWindows() {

   HWND hwndOwner = NULL;
 
   HRESULT hr = pIOleInPlaceSite -> GetWindow(&hwndOwner);

   if ( ! hwndOwner )
      return 0;

   DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_DATASET_SPEC),RT_DIALOG));

   hwndSpecDialog = CreateDialogIndirectParam(hModule,dt,hwndOwner,(DLGPROC)dataSetDialogHandler,(LPARAM)this);

   return 0;
   }


   int DataSet::resetLimits(const DataPoint& d) {

   int rv = ( (xMax < d.x) || (yMax < d.y) || (zMax < d.z) ||
              (xMin > d.x) || (yMin > d.y) || (zMin > d.z) ) ? TRUE : FALSE;

   xMax = (d.x > xMax) ? d.x : xMax;
   yMax = (d.y > yMax) ? d.y : yMax;
   zMax = (d.z > zMax) ? d.z : zMax;
   xMin = (d.x < xMin) ? d.x : xMin;
   yMin = (d.y < yMin) ? d.y : yMin;
   zMin = (d.z < zMin) ? d.z : zMin;

#if 1
   char szNumber[6][64];

   if ( -DBL_MAX == xMin || DBL_MAX == xMin/* || fabs(xMin - DBL_MAX) > 10.0 */)
      sprintf(szNumber[0],"INF");
   else
      sprintf(szNumber[0],"%lf",xMin);

   if ( DBL_MAX == xMax || -DBL_MAX == xMax/* || fabs(xMax - DBL_MAX) > 10.0 */)
      sprintf(szNumber[1],"INF");
   else
      sprintf(szNumber[1],"%lf",xMax);

   if ( -DBL_MAX == yMin || DBL_MAX == yMin/* || fabs(yMin - DBL_MAX) > 10.0 */)
      sprintf(szNumber[2],"INF");
   else
      sprintf(szNumber[2],"%lf",yMin);

   if ( DBL_MAX == yMax || -DBL_MAX == yMax/* || fabs(yMax - DBL_MAX) > 10.0 */)
      sprintf(szNumber[3],"INF");
   else
      sprintf(szNumber[3],"%lf",yMax);

   if ( -DBL_MAX == zMin || DBL_MAX == zMin/* || fabs(zMin - DBL_MAX) > 10.0 */)
      sprintf(szNumber[4],"INF");
   else
      sprintf(szNumber[4],"%lf",zMin);

   if ( DBL_MAX == zMax || -DBL_MAX == zMax/* || fabs(zMax - DBL_MAX) > 10.0 */)
      sprintf(szNumber[5],"INF");
   else
      sprintf(szNumber[5],"%lf",zMax);

   sprintf(szEquations,
            "xMin = %s,yMin = %s zMin = %s xMax = %s yMax = %s zMax = %s "
            "minX = %s,minY = %s minZ = %s maxX = %s maxY = %s maxZ = %s",
                     szNumber[0],szNumber[2],szNumber[4],szNumber[1],szNumber[3],szNumber[5],
                     szNumber[0],szNumber[2],szNumber[4],szNumber[1],szNumber[3],szNumber[5]);
#endif
   return rv;
   }
 
 
   int DataSet::resizeBounds() {
   adjustRange(&xMin,&xMax);
   adjustRange(&yMin,&yMax);
   adjustRange(&zMin,&zMax);
   return TRUE;
   }