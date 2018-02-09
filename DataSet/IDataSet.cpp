// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "dataset.h"

#include "general.h"
#include "utils.h"

#include "list.cpp"


   HRESULT DataSet::ReSet() {

   DataList *t;

   while ( topData != (DataList *)NULL ) {
      t = topData -> previous;
      delete topData;
      topData = t;
   }

   firstData = NULL;
   topData = NULL;
   xMin = DBL_MAX;
   yMin = DBL_MAX;
   zMin = DBL_MAX;
   xMax = -DBL_MAX;
   yMax = -DBL_MAX;
   zMax = -DBL_MAX;

   if ( gdiData ) {
      delete [] gdiData;
      gdiData = NULL;
   }

   for ( IDataSet *p : otherDomains )
      p -> ReSet();

   countX = 0L;
   countY = 0L;
   countZ = 0L;

   return S_OK;
   }
 
   HRESULT DataSet::put_Name(BSTR name) {
   WideCharToMultiByte(CP_ACP,0,name,-1,szName,64,0,0);
   return S_OK;
   }


   HRESULT DataSet::get_Name(BSTR *pName) {
   if ( ! pName ) 
      return E_POINTER;
   *pName = SysAllocStringLen(NULL,(DWORD)strlen(szName));
   MultiByteToWideChar(CP_ACP,0,szName,-1,*pName,64);
   return S_OK;
   }

   HRESULT DataSet::put_DataSource(BSTR dataSource) {
   WideCharToMultiByte(CP_ACP,0,dataSource,-1,szDataSource,64,0,0);
   return S_OK;
   }


   HRESULT DataSet::get_DataSource(BSTR *pDataSource) {
   if ( ! pDataSource ) 
      return E_POINTER;
   *pDataSource = SysAllocStringLen(NULL,(DWORD)strlen(szDataSource));
   MultiByteToWideChar(CP_ACP,0,szDataSource,-1,*pDataSource,MAX_PATH);
   return S_OK;
   }


   HRESULT DataSet::put_IsFunctionSource(VARIANT_BOOL isf) {
   isFunction = isf == VARIANT_TRUE ? true : false;
   return S_OK;
   }


   HRESULT DataSet::get_IsFunctionSource(VARIANT_BOOL *pResponse) {
   if ( ! pResponse )
      return E_POINTER;
   *pResponse = isFunction ? VARIANT_TRUE : VARIANT_FALSE;
   return S_OK;
   }


   HRESULT DataSet::put_IFunction(void *pIFun) {
   pIFunction = (IGSFunctioNater *)pIFun;
   return S_OK;
   }


   HRESULT DataSet::get_IFunction(void **ppIFun) {
   if ( ! ppIFun )
      return E_POINTER;
   *ppIFun = (void *)pIFunction;
   return S_OK;
   }


   HRESULT DataSet::put_IPlot(void *pIP) {
   if ( pIPlot )
      pIPlot -> Release();
   pIPlot = (IPlot *)pIP;
   if ( pIPlot )
      pIPlot -> AddRef();
   if ( pPropertyPlots ) {
      pIGProperties -> Remove(L"plots");
      pPropertyPlots -> Release();
      pPropertyPlots = NULL;
   }
   return S_OK;
   }


   HRESULT DataSet::get_IPlot(void **ppIP) {
   if ( ! ppIP )
      return E_POINTER;
   *ppIP = (void *)pIPlot;
   return S_OK;
   }

   STDMETHODIMP DataSet::Initialize(void* pvIDataSet_Domain,void *pvIOpenGLImplementation,IEvaluator *piev,
                                          IGProperty* pIPropertyLineColor,IGProperty* pIPropertyLineWeight,
                                          IGProperty *parentPropertyPlotView,
                                          IGProperty *parentPropertyDefault2DPlotSubType,
                                          IGProperty *parentPropertyDefault3DPlotSubType,
                                          IGProperty *parentPropertyBackgroundColor,
                                          IGProperty *parentPropertyXFloor,IGProperty *parentPropertyXCeiling,
                                          IGProperty *parentPropertyYFloor,IGProperty *parentPropertyYCeiling,
                                          IGProperty *parentPropertyZFloor,IGProperty *parentPropertyZCeiling,
                                          void (__stdcall *pCallback)(void *,ULONG_PTR),void *pArg,ULONG_PTR callbackCookie) {

   IDataSet *pds = NULL;

   QueryInterface(IID_IDataSet,reinterpret_cast<void **>(&pds));
   pIPlot -> put_IDataSet(pds);
   pds -> Release();

   pWhenChangedCallback = pCallback;
   pWhenChangedCallbackArg = pArg;
   whenChangedCallbackCookie = callbackCookie;

   if ( pIOpenGLImplementation )
      pIOpenGLImplementation -> Release();

   pIOpenGLImplementation = (IOpenGLImplementation *)pvIOpenGLImplementation;

   if ( pIEvaluator )
      pIEvaluator -> Release();

   pIEvaluator = piev;

   HRESULT rc = pIPlot -> Initialize((IDataSet *)pvIDataSet_Domain,(IOpenGLImplementation *)pvIOpenGLImplementation,
                                       pIEvaluator,pIPropertyLineColor,pIPropertyLineWeight,parentPropertyPlotView,parentPropertyDefault2DPlotSubType,parentPropertyDefault3DPlotSubType,
                                       parentPropertyBackgroundColor,
                                       parentPropertyXFloor,parentPropertyXCeiling,
                                       parentPropertyYFloor,parentPropertyYCeiling,
                                       parentPropertyZFloor,parentPropertyZCeiling,
                                       pCallback,pArg,callbackCookie);

   return rc;
   }

   STDMETHODIMP DataSet::put_OnChangeCallback(void (__stdcall *pCallback)(void *,ULONG_PTR),void *pArg,ULONG_PTR callbackCookie) {
   pWhenChangedCallback = pCallback;
   pWhenChangedCallbackArg = pArg;
   whenChangedCallbackCookie = callbackCookie;
   return S_OK;
   }


   HRESULT DataSet::get_maxX(double *getVal) {

   if ( ! getVal ) 
      return E_POINTER;

   double otherMax,returnedMax = xMax;

   for ( IDataSet *p : otherDomains ) {
      p -> get_maxX(&otherMax);
      returnedMax = max(returnedMax,otherMax);
   }

   if ( pPropertyXCeiling ) {
      if ( -DBL_MAX == returnedMax )
         pPropertyXCeiling -> get_doubleValue(&returnedMax);
      IGProperty* pKeep = pPropertyXFloor;
      pPropertyXFloor = NULL;
      double tempMin;
      get_minX(&tempMin);
      if ( tempMin == returnedMax )
         pPropertyXCeiling -> get_doubleValue(&returnedMax);
      pPropertyXFloor = pKeep;
   }

   *getVal = returnedMax;

   return S_OK;
   }

   HRESULT DataSet::put_maxX(double v) {
   xMax = v;
   for ( IDataSet *p : otherDomains )
      p -> put_maxX(v);
   return S_OK;
   }
 

   HRESULT DataSet::get_maxY(double *getVal) {

   if ( ! getVal ) 
      return E_POINTER;

   double otherMax,returnedMax = yMax;

   for ( IDataSet *p : otherDomains ) {
      p -> get_maxY(&otherMax);
      returnedMax = max(returnedMax,otherMax);
   }

   if ( pPropertyYCeiling ) {
      if ( -DBL_MAX == returnedMax )
         pPropertyYCeiling -> get_doubleValue(&returnedMax);
      IGProperty* pKeep = pPropertyYFloor;
      pPropertyYFloor = NULL;
      double tempMin;
      get_minY(&tempMin);
      if ( tempMin == returnedMax )
         pPropertyYCeiling -> get_doubleValue(&returnedMax);
      pPropertyYFloor = pKeep;
   }

   *getVal = returnedMax;

   return S_OK;
   }

   HRESULT DataSet::put_maxY(double v) {
   yMax = v;
   for ( IDataSet *p : otherDomains )
      p -> put_maxY(v);
   return S_OK;
   }
 
 
   HRESULT DataSet::get_maxZ(double *getVal) {

   if ( ! getVal ) 
      return E_POINTER;

   double otherMax,returnedMax = zMax;

   for ( IDataSet *p : otherDomains ) {
      p -> get_maxZ(&otherMax);
      returnedMax = max(returnedMax,otherMax);
   }

   if ( pPropertyZCeiling ) {
      if ( -DBL_MAX == returnedMax )
         pPropertyZCeiling -> get_doubleValue(&returnedMax);
      IGProperty* pKeep = pPropertyZFloor;
      pPropertyZFloor = NULL;
      double tempMin;
      get_minZ(&tempMin);
      if ( tempMin == returnedMax )
         pPropertyZCeiling -> get_doubleValue(&returnedMax);
      pPropertyZFloor = pKeep;
   }

   *getVal = returnedMax;

   return S_OK;
   }

   HRESULT DataSet::put_maxZ(double v) {
   zMax = v;
   for ( IDataSet *p : otherDomains )
      p -> put_maxZ(v);
   return S_OK;
   }
 
 
   HRESULT DataSet::get_minX(double *getVal) {

   if ( ! getVal ) 
      return E_POINTER;

   double otherMin,returnedMin = xMin;

   for ( IDataSet *p : otherDomains ) {
      p -> get_minX(&otherMin);
      returnedMin = min(returnedMin,otherMin);
   }

   if ( pPropertyXFloor ) {
      if ( DBL_MAX == returnedMin )
         pPropertyXFloor -> get_doubleValue(&returnedMin);
      IGProperty *pKeep = pPropertyXCeiling;
      pPropertyXCeiling = NULL;
      double tempMax;
      get_maxX(&tempMax);
      if ( tempMax == returnedMin )
         pPropertyXFloor -> get_doubleValue(&returnedMin);
      pPropertyXCeiling = pKeep;
   }

   *getVal = returnedMin;

   return S_OK;
   }

   HRESULT DataSet::put_minX(double v) {
   xMin = v;
   for ( IDataSet *p : otherDomains )
      p -> put_minX(v);
   return S_OK;
   }

 
   HRESULT DataSet::get_minY(double *getVal) {

   if ( ! getVal ) 
      return E_POINTER;

   double otherMin,returnedMin = yMin;

   for ( IDataSet *p : otherDomains ) {
      p -> get_minY(&otherMin);
      returnedMin = min(returnedMin,otherMin);
   }

   if ( pPropertyYFloor ) {
      if ( DBL_MAX == returnedMin )
         pPropertyYFloor -> get_doubleValue(&returnedMin);
      IGProperty *pKeep = pPropertyYCeiling;
      pPropertyYCeiling = NULL;
      double tempMax;
      get_maxY(&tempMax);
      if ( tempMax == returnedMin )
         pPropertyYFloor -> get_doubleValue(&returnedMin);
      pPropertyYCeiling = pKeep;
   }

   *getVal = returnedMin;

   return S_OK;
   }

   HRESULT DataSet::put_minY(double v) {
   yMin = v;
   for ( IDataSet *p : otherDomains )
      p -> put_minY(v);
   return S_OK;
   }
 
 
   HRESULT DataSet::get_minZ(double *getVal) {
   if ( ! getVal ) 
      return E_POINTER;

   double otherMin,returnedMin = zMin;

   for ( IDataSet *p : otherDomains ) {
      p -> get_minZ(&otherMin);
      returnedMin = min(returnedMin,otherMin);
   }

   if ( pPropertyZFloor ) {
      if ( DBL_MAX == returnedMin )
         pPropertyZFloor -> get_doubleValue(&returnedMin);
      IGProperty *pKeep = pPropertyZCeiling;
      pPropertyZCeiling = NULL;
      double tempMax;
      get_maxZ(&tempMax);
      if ( tempMax == returnedMin )
         pPropertyZFloor -> get_doubleValue(&returnedMin);
      pPropertyZCeiling = pKeep;
   }

   *getVal = returnedMin;

   return S_OK;
   }

   HRESULT DataSet::put_minZ(double v) {
   zMin = v;
   for ( IDataSet *p : otherDomains )
      p -> put_minZ(v);
   return S_OK;
   }
 
   HRESULT DataSet::put_XFloor(IGProperty *pf) {
   pPropertyXFloor = pf;
   return S_OK;
   }

   
   HRESULT DataSet::put_XCeiling(IGProperty *pc) {
   pPropertyXCeiling = pc;
   return S_OK;
   }

   HRESULT DataSet::put_YFloor(IGProperty *pf) {
   pPropertyYFloor = pf;
   return S_OK;
   }
   
   HRESULT DataSet::put_YCeiling(IGProperty *pc) {
   pPropertyYCeiling = pc;
   return S_OK;
   }

   HRESULT DataSet::put_ZFloor(IGProperty *pf) {
   pPropertyZFloor = pf;
   return S_OK;
   }

   
   HRESULT DataSet::put_ZCeiling(IGProperty *pc) {
   pPropertyZCeiling = pc;
   return S_OK;
   }

 
   HRESULT DataSet::get_top(DataList **getList) { 
   if ( ! getList ) return E_POINTER;
   *getList = topData; 
   return S_OK; 
   }

 
   HRESULT DataSet::get_bottom(DataList **getList) {
   if ( ! getList ) return E_POINTER;
   *getList = firstData;
   return S_OK; 
   }
 

   HRESULT DataSet::get_countPoints(long* pCount) {
   if ( ! pCount ) return E_POINTER;
   *pCount = 0;
   if ( ! firstData ) return S_OK;
   DataList* p = firstData;
   long k = 0;
   while ( p ) {
      k++;
      p = p -> next;
   }
   *pCount = k;
   return S_OK;
   }


   enum DataArity DataSet::DataArity() {
   if ( 0 < countZ )
      return DATA_ARITY_3D;
   if ( 0 < countY )
      return DATA_ARITY_2D;
   if ( 0 < countX )
      return DATA_ARITY_1D;
   return DATA_ARITY_UNKNOWN;
   }


   HRESULT DataSet::pushString(char *valueString) {
   char *s;
   DataPoint currentPoint;
   char data[BIG_STRING_SIZE];
 
   strcpy(data,(char *)valueString);
 
   s = strtok(data," ");
   if ( s ) {
      currentPoint.x = atof(s);
      s = strtok((char *)NULL," ");
      if ( s ) {
         currentPoint.y = atof(s);
         s = strtok((char *)NULL," ");
         if ( s ) {
            currentPoint.z = atof(s);
            countZ++;
         } else  {
            currentPoint.z = -DBL_MAX;
            countY++;
         }
      } else {
         currentPoint.y = -DBL_MAX;
         currentPoint.z = -DBL_MAX;
         countX++;
      }
   } else {
      currentPoint.x = -DBL_MAX;
      currentPoint.y = -DBL_MAX;
      currentPoint.z = -DBL_MAX;
   }
 
   pushDataPoint(&currentPoint);
 
   return S_OK;
   }
 
 
   HRESULT DataSet::pushDataPoint(DataPoint *data,void (__stdcall *pAction)(void *,void *),void *pvArg1,void *pvArg2) {

   DataList *p = new DataList;

   memcpy(p -> colorRGB,&currentColor,sizeof(currentColor));

   p -> pvAction = (void *)pAction;
   p -> pvArg1 = pvArg1;
   p -> pvArg2 = pvArg2;

   p -> previous = topData;

   p -> next = (DataList *)NULL;

   if ( topData )
      topData -> next = p;
   else {
      firstData = p;
      firstData -> previous = (DataList *)NULL;
      firstData -> next = (DataList *)NULL;
   }

   topData = p;
   topData -> data.x = data -> x;
   topData -> data.y = data -> y;
   topData -> data.z = data -> z;

   if ( ! ( data -> x == -DBL_MAX ) )
      resetLimits(*data);

   if ( ! ( 0.0 == data -> x ) && ! ( -DBL_MAX == data -> x ) )
      countX++;

   if ( ! ( 0.0 == data -> y ) && ! ( -DBL_MAX == data -> y ) )
      countY++;

   if ( ! ( 0.0 == data -> z ) && ! ( -DBL_MAX == data -> z ) )
      countZ++;

   return S_OK;
   }
 
 
   HRESULT DataSet::popDataPoint(DataPoint *data) {

   if ( ! data ) 
      return E_POINTER;

   if ( ! topData ) 
      return E_UNEXPECTED;

   DataList *t = topData -> previous;

   data -> x = topData -> data.x;
   data -> y = topData -> data.y;
   data -> z = topData -> data.z;

   if ( ! ( 0.0 == data -> x ) && ! ( -DBL_MAX == data -> x ) )
      countX--;

   if ( ! ( 0.0 == data -> y ) && ! ( -DBL_MAX == data -> y ) )
      countY--;

   if ( ! ( 0.0 == data -> z ) && ! ( -DBL_MAX == data -> z ) )
      countZ--;

   delete topData;
   topData = t;
   if ( topData ) 
      topData -> next = (DataList *)NULL;

   return S_OK;
   }
 
 
   HRESULT DataSet::peek(DataList *afterItem,DataList **getItem) {
   if ( afterItem  )
      *getItem = afterItem -> next;
   else
      *getItem = firstData;
   return S_OK;
   }
 
 
   HRESULT DataSet::peekInt(int item,DataList **getItem) {
   int k;
   DataList *p = firstData;
   for ( k = 0; k < item; k++ ) 
      if ( ! (p = p -> next) ) {
         *getItem = reinterpret_cast<DataList *>(NULL);
         return S_OK;
      }
   *getItem = p;
   return S_OK;
   }
 
 
   HRESULT DataSet::peekGDI(DataList *afterItem,DataList **getItem) {
   if ( afterItem  )
      *getItem = afterItem -> next;
   else
      *getItem = gdiData;
   return S_OK;
   }
 
   HRESULT DataSet::get(DataList *afterItem,DataPoint *d,DataList **nextItem) {
   if ( afterItem == (DataList *)NULL )
      afterItem = firstData;
   if ( ! afterItem ) {
      *nextItem = NULL;
      return S_OK;
   }
   d -> x = afterItem -> data.x;
   d -> y = afterItem -> data.y;
   d -> z = afterItem -> data.z;
   *nextItem = afterItem -> next;
   return S_OK;
   }
 
 
   HRESULT DataSet::set(DataList *pItem,DataPoint *pData) {

   if ( ! ( 0.0 == pItem -> data.x ) && ! ( -DBL_MAX == pItem -> data.x ) )
      countX--;

   if ( ! ( 0.0 == pItem -> data.y ) && ! ( -DBL_MAX == pItem -> data.y ) )
      countY--;

   if ( ! ( 0.0 == pItem -> data.z ) && ! ( -DBL_MAX == pItem -> data.z ) )
      countZ--;

   pItem -> data.x = pData -> x;
   pItem -> data.y = pData -> y;
   pItem -> data.z = pData -> z;

   if ( ! ( 0.0 == pItem -> data.x ) && ! ( -DBL_MAX == pItem -> data.x ) )
      countX++;

   if ( ! ( 0.0 == pItem -> data.y ) && ! ( -DBL_MAX == pItem -> data.y ) )
      countY++;

   if ( ! ( 0.0 == pItem -> data.z ) && ! ( -DBL_MAX == pItem -> data.z ) )
      countZ++;

   return S_OK;
   }
 
 
   HRESULT DataSet::addXYZ(double *x,double *y,double *z) {

   DataList *t = new DataList;

   memcpy(t -> colorRGB,&currentColor,sizeof(currentColor));

   t -> previous = topData;
   t -> next = (DataList *)NULL;
   if ( topData )
      topData -> next = t;
   else {
      firstData = t;
      firstData -> previous = (DataList *)NULL;
      firstData -> next = (DataList *)NULL;
   }

   topData = t;

   if ( x ) {
      topData -> data.x = *x;
      countX++;
   } else
      topData -> data.x = 0.0;

   if ( y )  {
      topData -> data.y = *y;
      countY++;
   } else
      topData -> data.y = 0.0;

   if ( z ) {
      topData -> data.z = *z;
      countZ++;
   } else
      topData -> data.z = 0.0;

   if ( x && *x != -DBL_MAX ) resetLimits
      (topData -> data);
   
   return S_OK;
   }


   HRESULT DataSet::addMovePoint() {
   DataList *t = new DataList;
   memcpy(t -> colorRGB,&currentColor,sizeof(currentColor));
   t -> previous = topData;
   t -> next = (DataList *)NULL;
   if ( topData )
      topData -> next = t;
   else {
      firstData = t;
      firstData -> previous = (DataList *)NULL;
      firstData -> next = (DataList *)NULL;
   }
   topData = t;
   topData -> data.x = -DBL_MAX;
   topData -> data.y = -DBL_MAX;
   topData -> data.z = -DBL_MAX;
   return S_OK;
   }


   HRESULT DataSet::insert(DataList *insertBefore,DataPoint *d) {

   DataList *p = new DataList;

   memcpy(p -> colorRGB,&currentColor,sizeof(currentColor));
 
   p -> data.x = d -> x;
   p -> data.y = d -> y;
   p -> data.z = d -> z;
   p -> previous = insertBefore -> previous;
   p -> next = insertBefore;
 
   if ( insertBefore -> previous ) {
      insertBefore -> previous -> next = p;
      insertBefore -> previous = p;
   }
 
   if ( insertBefore == firstData )
      firstData = p;
   
   return S_OK;
   }
 
 
   HRESULT DataSet::makeUnitSize() {
   DataPoint dp;
   ReSet();
   dp.x = 0.0;
   dp.y = 0.0;
   dp.z = 0.0;
   pushDataPoint(&dp);
   dp.x = 1.0;
   dp.y = 1.0;
   dp.z = 1.0;
   pushDataPoint(&dp);
   return S_OK;
   }
 

   HRESULT DataSet::ResetLimits(DataPoint *newPoint) {

   if ( ! newPoint ) 
      return E_POINTER;

   if ( newPoint -> x != DBL_MAX ) xMin = min(newPoint -> x,xMin);
   if ( newPoint -> y != DBL_MAX ) yMin = min(newPoint -> y,yMin);
   if ( newPoint -> z != DBL_MAX ) zMin = min(newPoint -> z,zMin);
   if ( newPoint -> x != -DBL_MAX ) xMax = max(newPoint -> x,xMax);
   if ( newPoint -> y != -DBL_MAX ) yMax = max(newPoint -> y,yMax);
   if ( newPoint -> z != -DBL_MAX ) zMax = max(newPoint -> z,zMax);

   for ( IDataSet *p : otherDomains )
      p -> ResetLimits(newPoint);

#if 1
   char szNumber[6][64];

   if ( -DBL_MAX == xMin || DBL_MAX == xMin )
      sprintf(szNumber[0],"INF");
   else
      sprintf(szNumber[0],"%lf",xMin);

   if ( DBL_MAX == xMax || -DBL_MAX == xMax )
      sprintf(szNumber[1],"INF");
   else
      sprintf(szNumber[1],"%lf",xMax);

   if ( -DBL_MAX == yMin || DBL_MAX == yMin )
      sprintf(szNumber[2],"INF");
   else
      sprintf(szNumber[2],"%lf",yMin);

   if ( DBL_MAX == yMax || -DBL_MAX == yMax )
      sprintf(szNumber[3],"INF");
   else
      sprintf(szNumber[3],"%lf",yMax);

   if ( -DBL_MAX == zMin || DBL_MAX == zMin)
      sprintf(szNumber[4],"INF");
   else
      sprintf(szNumber[4],"%lf",zMin);

   if ( DBL_MAX == zMax || -DBL_MAX == zMax )
      sprintf(szNumber[5],"INF");
   else
      sprintf(szNumber[5],"%lf",zMax);

   sprintf(szEquations,
            "xMin = %s,yMin = %s zMin = %s xMax = %s yMax = %s zMax = %s "
            "minX = %s,minY = %s minZ = %s maxX = %s maxY = %s maxZ = %s",
                     szNumber[0],szNumber[2],szNumber[4],szNumber[1],szNumber[3],szNumber[5],
                     szNumber[0],szNumber[2],szNumber[4],szNumber[1],szNumber[3],szNumber[5]);

#endif
   return S_OK;
   }

 
   HRESULT DataSet::SetDomain(DataPoint *pMinPoint,DataPoint *pMaxPoint) {
   if ( ! pMinPoint ) return E_POINTER;
   if ( ! pMaxPoint ) return E_POINTER;
   xMin = pMinPoint -> x;
   yMin = pMinPoint -> y;
   zMin = pMinPoint -> z;
   xMax = pMaxPoint -> x;
   yMax = pMaxPoint -> y;
   zMax = pMaxPoint -> z;
#if 0
   IDataSet* p = (IDataSet *)NULL;
   while ( p = otherDomains.GetNext(p) ) 
      p -> SetDomain(pMinPoint,pMaxPoint);
#endif
   return S_OK;
   }


   HRESULT DataSet::SetDomainXYZ(double *pxmin,double *pymin,double *pzmin,double *pxmax,double *pymax,double *pzmax) {
   if ( pxmin )
      xMin = *pxmin;
   else
      xMin = 0.0;
   if ( pymin ) 
      yMin = *pymin;
   else
      yMin = 0.0;
   if ( pzmin ) 
      zMin = *pzmin;
   else
      zMin = 0.0;
   if ( pxmax ) 
      xMax = *pxmax;
   else
      xMax = 0.0;
   if ( pymax ) 
      yMax = *pymax;
   else
      yMax = 0.0;
   if ( pzmax ) 
      zMax = *pzmax;
   else
      zMax = 0.0;
#if 0
   IDataSet* p = (IDataSet *)NULL;
   while ( p = otherDomains.GetNext(p) ) 
      p -> SetDomainXYZ(pxmin,pymin,pzmin,pxmax,pymax,pzmax);
#endif
   return S_OK;
   }



   HRESULT DataSet::GetDomain(DataPoint *pMinPoint,DataPoint *pMaxPoint) {

   if ( ! pMinPoint )
      return E_POINTER;

   if ( ! pMaxPoint ) 
      return E_POINTER;

   pMinPoint -> x = xMin;
   pMinPoint -> y = yMin;
   pMinPoint -> z = zMin;
   pMaxPoint -> x = xMax;
   pMaxPoint -> y = yMax;
   pMaxPoint -> z = zMax;

   if ( pMinPoint -> x == pMaxPoint -> x ) {
      if ( pPropertyXFloor ) 
         pPropertyXFloor -> get_doubleValue(&pMinPoint -> x);
      if ( pPropertyXCeiling ) 
         pPropertyXCeiling -> get_doubleValue(&pMaxPoint -> x);
   }

   if ( pMinPoint -> y == pMaxPoint -> y ) {
      if ( pPropertyYFloor ) 
         pPropertyYFloor -> get_doubleValue(&pMinPoint -> y);
      if ( pPropertyYCeiling ) 
         pPropertyYCeiling -> get_doubleValue(&pMaxPoint -> y);
   }

   if ( pMinPoint -> z == pMaxPoint -> z ) {
      if ( pPropertyZFloor ) 
         pPropertyZFloor -> get_doubleValue(&pMinPoint -> z);
      if ( pPropertyZCeiling ) 
         pPropertyZCeiling -> get_doubleValue(&pMaxPoint -> z);
   }

   return S_OK;
   }


   HRESULT DataSet::IncludeDomain(IDataSet* pOther) {

   for ( IDataSet *p : otherDomains ) 
      if ( p == pOther )
         return S_OK;

   otherDomains.push_back(pOther);

   pOther -> AddRef();

   return S_OK;
   }


   HRESULT DataSet::get_IsIncludedDomain(IDataSet* pOther,boolean *pIs) {
   if ( ! pIs ) return E_POINTER;
   *pIs = true;
   for ( IDataSet *p : otherDomains ) 
      if ( p == pOther )
         return S_OK;
   *pIs = false;
   return S_OK;
   }


   HRESULT DataSet::RemoveIncludedDomain(IDataSet* pOther) {
   bool found = false;
   for ( IDataSet *p : otherDomains ) {
      if ( p == pOther ) {
         found = true;
         break;
      }
   }

   if ( ! found )
      return E_UNEXPECTED;

   pOther -> Release();

   otherDomains.remove(pOther);

   return S_OK;
   }


   HRESULT DataSet::SetColorRGB(double *pred,double *pblue,double *pgreen) {
   if ( pred ) currentColor[0] = *pred;
   if ( pblue ) currentColor[1] = *pblue;
   if ( pgreen ) currentColor[2] = *pgreen;
   return S_OK;
   }


   HRESULT DataSet::GetFirstNaturalPoint(DataPoint *pDataPoint) {

   if ( ! pDataPoint )
      return E_POINTER;

   DataList *p = firstData;

   while ( p ) {

      if ( p -> data.x == DBL_MAX || p -> data.x == -DBL_MAX ||
           p -> data.y == DBL_MAX || p -> data.y == -DBL_MAX ||
           p -> data.z == DBL_MAX || p -> data.z == -DBL_MAX ) {
         p = p -> next;
         continue;
      }

      pDataPoint -> x = p -> data.x;
      pDataPoint -> y = p -> data.y;
      pDataPoint -> z = p -> data.z;

      return S_OK;
   }

   pDataPoint -> x = DBL_MAX;
   pDataPoint -> y = DBL_MAX;
   pDataPoint -> z = DBL_MAX;

   return S_OK;
   }


   HRESULT DataSet::Scale(DataPoint* f) {

   if ( ! f ) return E_POINTER;

   if ( xMin == DBL_MAX && xMax == -DBL_MAX &&
        yMin == DBL_MAX && yMax == -DBL_MAX &&
        zMin == DBL_MAX && zMax == -DBL_MAX ) return E_UNEXPECTED;

   if ( f -> x == 1.0 && f -> y == 1.0 && f -> z == 1.0 ) return S_OK;

   DataList *p = firstData;
   if ( ! p ) return E_UNEXPECTED;

   xMin = DBL_MAX;
   xMax = -DBL_MAX;
   yMin = DBL_MAX;
   yMax = -DBL_MAX;
   zMin = DBL_MAX;
   zMax = -DBL_MAX;

   while ( p ) {

      if ( p -> data.x != DBL_MAX && p -> data.x != -DBL_MAX ) {
         p -> data.x *= f -> x;
         xMin = min(xMin,p -> data.x);
         xMax = max(xMax,p -> data.x);
      }

      if ( p -> data.y != DBL_MAX && p -> data.y != -DBL_MAX ) {
         p -> data.y *= f -> y;
         yMin = min(yMin,p -> data.y);
         yMax = max(yMax,p -> data.y);
      }


      if ( p -> data.z != DBL_MAX && p -> data.z != -DBL_MAX ) {
         p -> data.z *= f -> z;
         zMin = min(zMin,p -> data.z);
         zMax = max(zMax,p -> data.z);
      }

      p = p -> next;
   }

   for ( IDataSet *pDataSet : otherDomains )
      pDataSet -> Scale(f);

   return S_OK;
   }


   HRESULT DataSet::Translate(DataPoint* f) {

   if ( ! f ) return E_POINTER;

   if ( xMin == DBL_MAX && xMax == -DBL_MAX &&
        yMin == DBL_MAX && yMax == -DBL_MAX &&
        zMin == DBL_MAX && zMax == -DBL_MAX ) return E_UNEXPECTED;

   if ( f -> x == 0.0 && f -> y == 0.0 && f -> z == 0.0 ) return S_OK;

   DataList *p = firstData;
   if ( ! p ) 
      return E_UNEXPECTED;

   xMin = DBL_MAX;
   xMax = -DBL_MAX;
   yMin = DBL_MAX;
   yMax = -DBL_MAX;
   zMin = DBL_MAX;
   zMax = -DBL_MAX;

   while ( p ) {

      if ( p -> data.x != DBL_MAX && p -> data.x != -DBL_MAX ) {
         p -> data.x += f -> x;
         xMin = min(xMin,p -> data.x);
         xMax = max(xMax,p -> data.x);
      }

      if ( p -> data.y != DBL_MAX && p -> data.y != -DBL_MAX ) {
         p -> data.y += f -> y;
         yMin = min(yMin,p -> data.y);
         yMax = max(yMax,p -> data.y);
      }

      if ( p -> data.z != DBL_MAX && p -> data.z != -DBL_MAX ) {
         p -> data.z += f -> z;
         zMin = min(zMin,p -> data.z);
         zMax = max(zMax,p -> data.z);
      }

      p = p -> next;

   }

   for ( IDataSet *pDataSet : otherDomains )
      pDataSet -> Translate(f);

   return S_OK;
   }


   HRESULT DataSet::TranslateGDI(POINT* f) {

   if ( ! gdiData ) 
      return E_UNEXPECTED;

   DataList *p = gdiData;

   while ( p ) {

      if ( p -> data.x != DBL_MAX && p -> data.x != -DBL_MAX ) 
         p -> data.x += (double)f -> x;

      if ( p -> data.y != DBL_MAX && p -> data.y != -DBL_MAX ) 
         p -> data.y += (double)f -> y;

      //if ( p -> data.z != DBL_MAX && p -> data.z != -DBL_MAX ) 
      //   p -> data.z += (double)f -> z;

      p = p -> next;
   }

   for ( IDataSet *pDataSet : otherDomains )
      pDataSet -> TranslateGDI(f);

   return S_OK;
   }


   HRESULT DataSet::Rotate(char axis,double angle) {

   if ( ! firstData ) 
      return E_UNEXPECTED;

   if ( xMin == DBL_MAX && xMax == -DBL_MAX &&
        yMin == DBL_MAX && yMax == -DBL_MAX &&
        zMin == DBL_MAX && zMax == -DBL_MAX )
      return E_UNEXPECTED;

   DataPoint dpCenter{-(xMax + xMin) / 2.0,-(yMax + yMin) / 2.0,-(zMax + zMin) / 2.0};

   Translate(&dpCenter);

   double rMatrix[9];

   rotateMatrix(axis,angle,rMatrix);

   DataList *p = firstData;

   while ( p ) {

      if ( p -> data.x == DBL_MAX || p -> data.x == -DBL_MAX ||
           p -> data.y == DBL_MAX || p -> data.y == -DBL_MAX ||
           p -> data.z == DBL_MAX || p -> data.z == -DBL_MAX ) {
         p = p -> next;
         continue;
      }

      MxPoint(rMatrix,&p -> data,&p -> data);

      p = p -> next;
   }

   dpCenter.x = -dpCenter.x;//(xMax + xMin) / 2.0;
   dpCenter.y = -dpCenter.y;//(yMax + yMin) / 2.0;
   dpCenter.z = -dpCenter.z;//(zMax + zMin) / 2.0;

   Translate(&dpCenter);

   for ( IDataSet *pDataSet : otherDomains )
      pDataSet -> Rotate(axis,angle);

   return S_OK;
   }


   HRESULT DataSet::RotateVector(double angle,DataPoint *vector) {

   if ( ! firstData ) return E_UNEXPECTED;

   if ( xMin == DBL_MAX && xMax == -DBL_MAX &&
        yMin == DBL_MAX && yMax == -DBL_MAX &&
        zMin == DBL_MAX && zMax == -DBL_MAX ) return E_UNEXPECTED;

   double rMatrix[9];

   double v[] = {vector -> x,vector -> y,vector -> z};

   rotateMatrixVector(angle,v,rMatrix);

   DataList *p = firstData;

   xMin = DBL_MAX;
   yMin = DBL_MAX;
   zMin = DBL_MAX;
   xMax = -DBL_MAX;
   yMax = -DBL_MAX;
   zMax = -DBL_MAX;

   while ( p ) {

      if ( p -> data.x == DBL_MAX || p -> data.x == -DBL_MAX ||
           p -> data.y == DBL_MAX || p -> data.y == -DBL_MAX ||
           p -> data.z == DBL_MAX || p -> data.z == -DBL_MAX ) {
         p = p -> next;
         continue;
      }

      MxPoint(rMatrix,&p -> data,&p -> data);

      xMin = min(xMin,p -> data.x);
      yMin = min(yMin,p -> data.y);
      zMin = min(zMin,p -> data.z);
      xMax = max(xMax,p -> data.x);
      yMax = max(yMax,p -> data.y);
      zMax = max(zMax,p -> data.z);

      p = p -> next;
   }

   for ( IDataSet *pDataSet : otherDomains )
      pDataSet -> RotateVector(angle,vector);

   return S_OK;
   }


   HRESULT DataSet::Shear(char axis,double ratio) {

   if ( ! firstData ) return E_UNEXPECTED;

   if ( xMin == DBL_MAX && xMax == -DBL_MAX &&
        yMin == DBL_MAX && yMax == -DBL_MAX &&
        zMin == DBL_MAX && zMax == -DBL_MAX ) return E_UNEXPECTED;

   double sMatrix[9];

   shearMatrix(axis,ratio,sMatrix);

   DataList *p = firstData;
/*
   xMin = DBL_MAX;
   yMin = DBL_MAX;
   zMin = DBL_MAX;
   xMax = -DBL_MAX;
   yMax = -DBL_MAX;
   zMax = -DBL_MAX;
*/
   while ( p ) {

      if ( p -> data.x == DBL_MAX || p -> data.x == -DBL_MAX ||
           p -> data.y == DBL_MAX || p -> data.y == -DBL_MAX ||
           p -> data.z == DBL_MAX || p -> data.z == -DBL_MAX ) {
         p = p -> next;
         continue;
      }

      MxPoint(sMatrix,&p -> data,&p -> data);
/*
         xMin = min(xMin,p -> data.x);
         yMin = min(yMin,p -> data.y);
         zMin = min(zMin,p -> data.z);
         xMax = max(xMax,p -> data.x);
         yMax = max(yMax,p -> data.y);
         zMax = max(zMax,p -> data.z);
*/
      p = p -> next;
   }

   for ( IDataSet *pDataSet : otherDomains )
      pDataSet -> Shear(axis,ratio);

   return S_OK;
   }


   HRESULT DataSet::Move(DataPoint* f) {
   DataPoint dp = {f -> x - xMin,f -> y - yMin,f -> z - zMin};
   return Translate(&dp);
   }


   HRESULT DataSet::GenerateBoundingBox(IDataSet* pIDataSetBoundingBox) {

   double otherXMax,otherXMin,otherYMax,otherYMin,otherZMax,otherZMin;

   pIDataSetBoundingBox -> ReSet();

   pIDataSetBoundingBox -> pushDataPoint(&movePoint);

   get_minX(&otherXMin);
   get_maxX(&otherXMax);
   get_minY(&otherYMin);
   get_maxY(&otherYMax);
   get_minZ(&otherZMin);
   get_maxZ(&otherZMax);

   DataPoint dp = {otherXMin,otherYMin,otherZMin};

   pIDataSetBoundingBox -> pushDataPoint(&dp);

   dp.x = otherXMax;
   pIDataSetBoundingBox -> pushDataPoint(&dp);

   dp.y = otherYMax;
   pIDataSetBoundingBox -> pushDataPoint(&dp);

   dp.x = otherXMin;
   pIDataSetBoundingBox -> pushDataPoint(&dp);

   dp.y = otherYMin;
   pIDataSetBoundingBox -> pushDataPoint(&dp);

   return S_OK;
   }


   HRESULT DataSet::GenerateGDICoordinates() {

   if ( xMin == DBL_MAX && xMax == -DBL_MAX &&
        yMin == DBL_MAX && yMax == -DBL_MAX &&
        zMin == DBL_MAX && zMax == -DBL_MAX ) 
      return E_UNEXPECTED;

   long cp;

   get_countPoints(&cp);

   if ( ! cp ) 
      return E_UNEXPECTED;
   
   if ( gdiData ) 
      delete [] gdiData;

   gdiData = new DataList[cp];

   memset(gdiData,0,cp * sizeof(DataList));

   DataList *p = gdiData;

   for ( long k = 0; k < cp; k++ ) {
      p -> colorRGB[0] = currentColor[0];
      p -> colorRGB[1] = currentColor[1];
      p -> colorRGB[2] = currentColor[2];
      p -> next = p + 1;
      if ( k < cp - 1 )
         p -> next -> previous = p;
      p = p -> next;
   }

   gdiData[cp - 1].next = NULL;

   pIOpenGLImplementation -> DataListToWindow(firstData,gdiData);

   return S_OK;
   }


   HRESULT DataSet::ConvertGDIToWorld() {

   DataList *p = firstData;

   xMin = DBL_MAX;
   yMin = DBL_MAX;
   zMin = DBL_MAX;
   xMax = -DBL_MAX;
   yMax = -DBL_MAX;
   zMax = -DBL_MAX;

   while ( p ) {

      if ( p -> data.x == DBL_MAX || p -> data.x == -DBL_MAX ||
           p -> data.y == DBL_MAX || p -> data.y == -DBL_MAX ||
           p -> data.z == DBL_MAX || p -> data.z == -DBL_MAX ) {
         p = p -> next;
         continue;
      }

      pIOpenGLImplementation -> WindowToData(&p -> data,&p -> data);

//      MxPoint(sMatrix,&p -> data,&p -> data);

      xMin = min(xMin,p -> data.x);
      yMin = min(yMin,p -> data.y);
      zMin = min(zMin,p -> data.z);
      xMax = max(xMax,p -> data.x);
      yMax = max(yMax,p -> data.y);
      zMax = max(zMax,p -> data.z);

      p = p -> next;
  
    }

   return S_OK;
   }


   HRESULT DataSet::SaveAsGDICoordinates() {

   if ( xMin == DBL_MAX && xMax == -DBL_MAX &&
        yMin == DBL_MAX && yMax == -DBL_MAX &&
        zMin == DBL_MAX && zMax == -DBL_MAX ) 
      return E_UNEXPECTED;

   long cp;

   get_countPoints(&cp);

   if ( ! cp ) 
      return E_UNEXPECTED;
   
   if ( gdiData ) 
      delete [] gdiData;

   gdiData = new DataList[cp];

   memset(gdiData,0,cp * sizeof(DataList));

   DataList *p = gdiData;
   DataList *pSource = firstData;
   for ( long k = 0; k < cp; k++ ) {
      p -> data.x = pSource -> data.x;
      p -> data.y = pSource -> data.y;
      p -> data.z = pSource -> data.z;
      p -> colorRGB[0] = pSource -> colorRGB[0];
      p -> colorRGB[1] = pSource -> colorRGB[1];
      p -> colorRGB[2] = pSource -> colorRGB[2];
      p -> next = p + 1;
      if ( k < cp - 1 )
         p -> next -> previous = p;
      p = p -> next;
      pSource = pSource -> next;
   }

   gdiData[cp - 1].next = NULL;

   return S_OK;
   }

   HRESULT DataSet::PushExtents() {

   extents *pExtents = new extents();

   if ( xMin == xMax && xMin != DBL_MAX && xMin != -DBL_MAX ) {
      xMin -= 0.01 * xMax;
      xMax += 0.01 * xMax;
   }
   if ( yMin == yMax && yMin != DBL_MAX && yMin != -DBL_MAX ) {
      yMin -= 0.01 * yMax;
      yMax += 0.01 * yMax;
   }
   if ( zMin == zMax && zMin != DBL_MAX && zMin != -DBL_MAX ) {
      zMin -= 0.01 * zMax;
      zMax += 0.01 * zMax;
   }
   if ( xMin == xMax ) xMax = 1.0;
   if ( yMin == yMax ) yMax = 1.0;
   if ( zMin == zMax ) zMax = 1.0;
   pExtents -> xMin = xMin;
   pExtents -> yMin = yMin;
   pExtents -> zMin = zMin;
   pExtents -> xMax = xMax;
   pExtents -> yMax = yMax;
   pExtents -> zMax = zMax;
   extentsStack.push_back(pExtents);
   return S_OK;
   }

   HRESULT DataSet::PopExtents() {
   if ( 0 == extentsStack.size() )
      return E_UNEXPECTED;
   extents *pExtents = extentsStack.back();
   if ( ! pExtents ) 
      return E_UNEXPECTED;
   xMin = pExtents -> xMin;
   yMin = pExtents -> yMin;
   zMin = pExtents -> zMin;
   xMax = pExtents -> xMax;
   yMax = pExtents -> yMax;
   zMax = pExtents -> zMax;
   extentsStack.pop_back();
   delete pExtents;
   return S_OK;
   }


   HRESULT DataSet::Evaluate(BSTR expression,double *result) {
   if ( ! result ) 
      return E_POINTER;
   if ( ! pIEvaluator ) {
      CoCreateInstance(CLSID_Evaluator,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IEvaluator,reinterpret_cast<void **>(&pIEvaluator));
   }

//   BSTR bstrEquations = SysAllocStringLen(NULL,strlen(szEquations) + 1);
//   MultiByteToWideChar(CP_ACP,0,szEquations,-1,bstrEquations,strlen(szEquations) + 1);

   char *pszTemp = new char[strlen(szEquations) + 1];
// ??   memset(pszTemp,0,(strlen(szEquations) + 1) * sizeof(char));
   strcpy(pszTemp,szEquations);
   while ( *pszTemp ) evalConsume(pIEvaluator,pszTemp);
   delete [] pszTemp;

   pIEvaluator -> get_Eval(expression,result);

   return S_OK;
   }


   HRESULT DataSet::GetDomainGDI(DataPoint *pMinPoint,DataPoint *pMaxPoint) {

   if ( ! pMinPoint ) return E_POINTER;
   if ( ! pMaxPoint ) return E_POINTER;

   pMinPoint -> x = DBL_MAX;
   pMinPoint -> y = DBL_MAX;
   pMinPoint -> z = DBL_MAX;
   pMaxPoint -> x = -DBL_MAX;
   pMaxPoint -> y = -DBL_MAX;
   pMaxPoint -> z = -DBL_MAX;

   if ( ! gdiData )
      return E_UNEXPECTED;

   long cp;
   get_countPoints(&cp);

   if ( ! cp ) 
      return E_UNEXPECTED;

   DataList *p = gdiData;
   for ( long k = 0; k < cp; k++ ) {
      if ( -DBL_MAX == p -> data.x ) {
         p = p -> next;
         continue;
      }
      pMinPoint -> x = min(pMinPoint -> x, p -> data.x);
      pMinPoint -> y = min(pMinPoint -> y, p -> data.y);
      pMinPoint -> z = min(pMinPoint -> z, p -> data.z);
      pMaxPoint -> x = max(pMaxPoint -> x, p -> data.x);
      pMaxPoint -> y = max(pMaxPoint -> y, p -> data.y);
      pMaxPoint -> z = max(pMaxPoint -> z, p -> data.z);
      p = p -> next;
   }

   for ( IDataSet *pDataSet : otherDomains ) 
      pDataSet -> GetDomainGDI(pMinPoint,pMaxPoint);

   return S_OK;
   }



   STDMETHODIMP DataSet::Start() {

   long countPoints = 0;

   get_countPoints(&countPoints);

   if ( 0 == countPoints ) {

      if ( szDataSource[0] && szSpreadsheetName[0] && szCellRange[0]) 
         loadExcelCellRange(NULL,NULL,szDataSource,szSpreadsheetName,szCellRange);
      else if ( szDataSource[0] && szNamedRange[0] )
         loadExcelNamedRange(NULL,NULL,szDataSource,szNamedRange);
      else
         return E_UNEXPECTED;

      get_countPoints(&countPoints);

      if ( 0 == countPoints )
         return E_UNEXPECTED;

   }

   fire_Clear();

   fire_Started(countPoints);

   fire_Finished();

   return S_OK;
   }

   STDMETHODIMP DataSet::AdviseGSystemStatusBar(IGSystemStatusBar* p) {
   if ( pIPlot )
      pIPlot -> AdviseGSystemStatusBar(p);
   if ( ! p ) {
      if ( ! pIGSystemStatusBar ) 
         return E_UNEXPECTED;
      pIGSystemStatusBar = NULL;
   }
   pIGSystemStatusBar = p;
   return S_OK;
   }


   STDMETHODIMP DataSet::AdviseGSGraphicServices(void *pvIGSGraphicServices) {
   if ( pIPlot )
      pIPlot -> AdviseGSGraphicServices(pvIGSGraphicServices);
   if ( ! pvIGSGraphicServices ) {
      if ( ! pIGSGraphicServices ) return E_UNEXPECTED;
      pIGSGraphicServices = NULL;
   }
   pIGSGraphicServices = (IGSGraphicServices *)pvIGSGraphicServices;
   return S_OK;
   }