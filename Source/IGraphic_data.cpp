// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include "utils.h"

#include "GraphicControl_i.h"
#include "List.cpp"


   STDMETHODIMP G::put_Floor(double newfloorZ) {
   floorZ = newfloorZ;
   return S_OK;
   }
   STDMETHODIMP G::get_Floor(double *pfloorZ) {
   if ( ! pfloorZ ) return E_POINTER;
   *pfloorZ = floorZ;
   return S_OK;
   }


   STDMETHODIMP G::put_Ceiling(double newCeilingZ) {
   ceilingZ = newCeilingZ;
   return S_OK;
   }
   STDMETHODIMP G::get_Ceiling(double *pCeilingZ) {
   if ( ! pCeilingZ ) return E_POINTER;
   *pCeilingZ = ceilingZ;
   return S_OK;
   }


   STDMETHODIMP G::ClearAllData() {
   clearData();
   return S_OK;
   }


   STDMETHODIMP G::PrepareForData(long plotID) {

   IPlot *pPlot = NULL;
   if ( plotID || 0 == plotList.Count() ) {
      if ( ! plotID ) 
         plotID = plotList.Count() + 1;

      pPlot = plotList.Get(plotID);
      if ( ! pPlot )
         pPlot = newPlot(plotID);

      pPlot -> PrepareForData();
      pPlot -> put_AutoPlotViewDetection(autoPlotViewDetection);

   } else {

      while ( pPlot = plotList.GetNext(pPlot) ) {
         pPlot -> PrepareForData();
         pPlot -> put_AutoPlotViewDetection(autoPlotViewDetection);
      }

   }
 
   if ( 0 == plotID ) //|| autoClear )
      clearData();
 
   return S_OK;
   }
 
  
   STDMETHODIMP G::TakeDataString(BSTR inputBstr,long plotID) {

   DataPoint dpLast;

   IPlot *pPlot = (IPlot *)NULL;
 
   if ( plotID ) {
      pPlot = plotList.Get(plotID);
      if ( ! pPlot )
         pPlot = newPlot(plotID);
      pPlot -> TakeDataString(inputBstr);
      pPlot -> LastPoint(&dpLast);
      pIDataSetMaster -> ResetLimits(&dpLast);
   } else {
      while ( pPlot = plotList.GetNext(pPlot) ) {
         if ( SUCCEEDED(pPlot -> TakeDataString(inputBstr)) ) {
            pPlot -> LastPoint(&dpLast);
            pIDataSetMaster -> ResetLimits(&dpLast);
         }
      }
   }
   return S_OK;
   }
 

   STDMETHODIMP G::TakeDataArray(SAFEARRAY **ppArray,long plotID) {

   DataPoint dpLast;
   double *pData;

   SafeArrayAccessData(*ppArray,reinterpret_cast<void **>(&pData));
   DataPoint pt = {pData[0],pData[1],pData[2]};

   IPlot *pPlot = (IPlot *)NULL;
 
   if ( plotID ) {
      pPlot = plotList.Get(plotID);
      if ( ! pPlot )
         pPlot = newPlot(plotID);
      pPlot -> TakeDataPoint(&pt);//TakeDataString(inputBstr);
      pPlot -> LastPoint(&dpLast);
      pIDataSetMaster -> ResetLimits(&dpLast);
   } else {
      while ( pPlot = plotList.GetNext(pPlot) ) {
         if ( SUCCEEDED(pPlot -> TakeDataPoint(&pt)) ) { //String(inputBstr)) ) {
            pPlot -> LastPoint(&dpLast);
            pIDataSetMaster -> ResetLimits(&dpLast);
         }
      }
   }
   return S_OK;
   }

 
   STDMETHODIMP G::TakeFile(BSTR fileName,long plotID) {
   FILE *f;
   char szTemp[256];
   BSTR bstrString;
   DataPoint dpLast;
 
   strcpy(szTemp,convertToSZAscii(fileName));
   f = fopen(szTemp,"r");
 
   if ( ! f ) return S_OK;
 
   if ( plotID != 0 ) {
      IPlot *pPlot = (IPlot *)NULL;
      if ( (pPlot = plotList.Get(plotID)) == (IPlot *)NULL ) {
         pPlot = newPlot(plotID);
      }
   }
 
   Beep(2000,100);
 
   while ( fgets(szTemp,256,f) != 0 ) {
      bstrString = bstrFromStr(szTemp);
      if ( plotID != 0 ) {
         plotList.Get(plotID) -> TakeDataString(bstrString);
         plotList.Get(plotID) -> LastPoint(&dpLast);
         pIDataSetMaster -> ResetLimits(&dpLast);
      } else {
         IPlot *p = (IPlot *)NULL;
         while ( p = plotList.GetNext(p) ) {
            p -> TakeDataString(bstrString);
            p -> LastPoint(&dpLast);
            pIDataSetMaster -> ResetLimits(&dpLast);
         }
      }
      freeBstrFromStr(bstrString);
   }
 
   Beep(1000,100);
 
   return S_OK;
   }
 
 
   STDMETHODIMP G::TakeDataSet(long dsi,long plotID) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP G::FinishedWithData(long plotID) {
   return S_OK;
   }
 
 
   STDMETHODIMP G::PrepareForDataSets() {
 
   stop();
 
   IPlot *pPlot = (IPlot *)NULL;
   while ( pPlot = plotList.GetFirst() ) {
      plotList.Remove(pPlot);
      pPlot -> Release();
   }
 
   pIDataSetMaster -> ReSet();
 
   return S_OK;
   }
 
 
