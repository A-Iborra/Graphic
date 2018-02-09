// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include <map>

#include "Graphic_resource.h"

#include "utils.h"

#include "List.cpp"

//#define DO_THREAD_RENDERING

   int G::render(ULONG_PTR sourceID) {

   if ( ! pIOpenGLImplementation )
      return 0;

   if ( ! Canvas() ) 
      return 0;

   if ( ! IsWindowVisible(Canvas()) )
      return 0;

   //if ( renderThreadList.Count() > 0 ) 
   //   return 0;

   stop();

   currentPlotSourceID = sourceID;
 
#ifdef DO_THREAD_RENDERING
   unsigned int threadAddr;
   HANDLE *pt = new HANDLE(NULL);
   *pt = (HANDLE)_beginthreadex(NULL,0,G::threadRender,(void*)this,0,&threadAddr);
   renderThreadList.Add(pt);
#else
   threadRender((void *)this);
#endif

   return 0;
   }


   unsigned __stdcall G::threadRender(void *pThis) {

#ifdef DO_THREAD_RENDERING
   CoInitializeEx(NULL,COINIT_MULTITHREADED);
#endif

   G *p = (G *)pThis;

   p -> rendering = true;

   p -> pIDataSetMaster -> ReSet();

   p -> visiblePlotList.Empty();

   if ( -1 == p -> currentPlotSourceID ) {

      if ( p -> hwndDataSourcesTab ) {

         long index = (long)SendMessage(p -> hwndDataSourcesTab,TCM_GETCURSEL,0L,0L);

         if ( -1 < index ) {

            char szTemp[32];

            TCITEM tcItem = {0};

            tcItem.mask = TCIF_TEXT;
            tcItem.pszText = szTemp;
            tcItem.cchTextMax = 32;

            SendMessage(p -> hwndDataSourcesTab,TCM_GETITEM,(WPARAM)index,(LPARAM)&tcItem);

            if ( 0 == strcmp(szTemp,"Functions") ) {

               index = (long)SendMessage(p -> hwndDataSourcesFunctions,TCM_GETCURSEL,0L,0L);

               tcItem.mask = TCIF_PARAM;

               SendMessage(p -> hwndDataSourcesFunctions,TCM_GETITEM,(WPARAM)index,(LPARAM)&tcItem);

               ContainedFunction *pContainedFunction = (ContainedFunction *)tcItem.lParam;

               if ( pContainedFunction ) {

                  IDataSet *pIDataSet = NULL;
         
                  pContainedFunction -> pFunction() -> get_IDataSet((void **)&pIDataSet);

                  if ( pIDataSet ) {

                     long countPoints = 0L;

                     pIDataSet -> get_countPoints(&countPoints);

                     if ( 0 == countPoints ) {

                        pContainedFunction -> pFunction() -> Start();

                        return 0;

                     }

                  }

                  IPlot *pIPlot = NULL;

                  pContainedFunction -> pFunction() -> get_IPlot((void **)&pIPlot);

                  p -> currentPlotSourceID = p -> plotList.ID(pIPlot);

               }

            } else if ( 0 == strcmp(szTemp,"DataSets") ) {

               index = (long)SendMessage(p -> hwndDataSourcesDataSets,TCM_GETCURSEL,0L,0L);

               tcItem.mask = TCIF_PARAM;

               SendMessage(p -> hwndDataSourcesDataSets,TCM_GETITEM,(WPARAM)index,(LPARAM)&tcItem);

               ContainedDataSet *pContainedDataSet = (ContainedDataSet *)tcItem.lParam;

               if ( pContainedDataSet ) {

                  long countPoints = 0L;

                  pContainedDataSet -> pDataSet() -> get_countPoints(&countPoints);

                  if ( 0 == countPoints ) {

                     pContainedDataSet -> pDataSet() -> Start();

                     return 0;
                  }

                  IPlot *pIPlot = NULL;

                  pContainedDataSet -> pDataSet() -> get_IPlot((void **)&pIPlot);

                  p -> currentPlotSourceID = p -> plotList.ID(pIPlot);

               }

            }

         }

      }

   }

   IPlot *pIPlot = (IPlot *)NULL;

   long allowedPlotCount = 0L;

   while ( pIPlot = p -> plotList.GetNext(pIPlot) ) {

      long okayToPlot = false;

      if ( p -> plotList.ID(pIPlot) == p -> currentPlotSourceID )
         okayToPlot = TRUE;
      else
         okayToPlot = ( ! p -> autoClear || p -> currentPlotSourceID == 0 );

      IDataSet *pIDataSet = NULL;

      pIPlot -> get_IDataSet(&pIDataSet);

      if ( okayToPlot ) {
         DataPoint minPoint,maxPoint;
         pIDataSet -> GetDomain(&minPoint,&maxPoint);
         p -> pIDataSetMaster -> ResetLimits(&minPoint);
         p -> pIDataSetMaster -> ResetLimits(&maxPoint);
      }

      long countPoints = 0;

      pIDataSet -> get_countPoints(&countPoints);

      if ( 0 == countPoints )
         okayToPlot = 0L;

      pIPlot -> put_OkToPlot(okayToPlot);

      if ( okayToPlot ) {
         allowedPlotCount++;
         p -> visiblePlotList.Add(pIPlot);
      }

   }

   if ( 0 == allowedPlotCount ) {
      DataPoint dpMin = {0.0,0.0,0.0};
      DataPoint dpMax = {1.0,1.0,1.0};
      p -> pIDataSetMaster -> pushDataPoint(&dpMin);
      p -> pIDataSetMaster -> pushDataPoint(&dpMax);
   }

   p -> pIOpenGLImplementation -> SetUp(p -> pIDataSetMaster);

   IAxis *ax = NULL;

   std::map<DataPoint *,DataPoint *> axiisDomains;

   while ( ax = p -> axisList.GetNext(ax) ) {

      char type;
      ax -> get_Type(&type);
      if ( p -> plotView != gcPlotView3D && 'Z' == type )
         continue;

      IDataSet *pIDataSet;

      ax -> get_DataSet(&pIDataSet);

      DataPoint *pMin = new DataPoint();
      DataPoint *pMax = new DataPoint();

      pIDataSet -> GetDomain(pMin,pMax);

      ax -> PrepData();

      pIDataSet -> GetDomain(pMin,pMax);

      axiisDomains[pMin] = pMax;

   }

   for ( std::pair<DataPoint *,DataPoint *> pair : axiisDomains ) {
      p -> pIDataSetMaster -> ResetLimits(pair.first);
      p -> pIDataSetMaster -> ResetLimits(pair.second);
      delete pair.second;
      delete pair.first;
   }

   // I can now use text in the world domain to adjust the ultimate world domain extents
#if 0
   IText *pIText = NULL;
   while ( pIText = p -> textList.GetNext(pIText) ) {
      pIText -> PrepData();
      pIText -> Draw();
   } 
#endif

   p -> pIOpenGLImplementation -> SetUp(p -> pIDataSetMaster);

   p -> ActivateLighting();

   p -> erase(); 

   ax = NULL;
   while ( ax = p -> axisList.GetNext(ax) )
      ax -> Draw();

   IPlot **ppIPlots = NULL;
   if ( 0 < p -> plotList.Count() ) {
      ppIPlots = reinterpret_cast<IPlot **>(CoTaskMemAlloc(p -> plotList.Count() * sizeof(IPlot *)));
      int n = p -> plotList.Count();
      for ( int k = 0; k < n; k++ )
         p -> plotList.GetByIndex(k) -> QueryInterface(IID_IPlot,(void**)(&ppIPlots[k]));
      p -> plotList.GetFirst() -> Plotter(n,&ppIPlots);
      for ( int k = 0; k < p -> plotList.Count(); k++ )
         p -> plotList.GetByIndex(k) -> DrawOpenGLText();
   }

   //IText *pIText = NULL;
   //while ( pIText = p -> textList.GetNext(pIText) ) {
   //   boolean doOpenGLRendering = FALSE;
   //   pIText -> get_TextRenderOpenGL(&doOpenGLRendering);
   //   if ( ! doOpenGLRendering ) 
   //      continue;
   //   pIText -> PrepData();
   //   pIText -> Draw();
   //} 

   p -> pIOpenGLImplementation -> Finalize();

   ax = NULL;
   while ( ax = p -> axisList.GetNext(ax) )
      ax -> DrawLabels();

   IText *pIText = NULL;
   while ( pIText = p -> textList.GetNext(pIText) ) {
      pIText -> PrepData();
      pIText -> Draw();
   } 

   if ( 0 < p -> plotList.Count() ) {
      for ( int k = 0; k < p -> plotList.Count(); k++ )
         p -> plotList.GetByIndex(k) -> DrawGDIText();
   }

   if ( p -> showStatusBar ) {

      char szText[MAX_PROPERTY_SIZE];

      char *pszExistingText = new char[MAX_PROPERTY_SIZE];

      p -> get_StatusText(0,&pszExistingText);

      if ( 0 == strcmp("Extents ",pszExistingText) || 2 > strlen(pszExistingText) ) {

         DataPoint dp[2];
         p -> pIDataSetMaster -> GetDomain(&dp[0],&dp[1]);

         if ( ! ( dp[0].x == -DBL_MAX ) && ! ( dp[0].y == -DBL_MAX ) && ! ( dp[0].z == -DBL_MAX ) &&
               ! ( dp[1].x == DBL_MAX ) && ! ( dp[1].y == DBL_MAX ) && ! ( dp[1].z == DBL_MAX ) ) {
            sprintf(szText,"Extents [ (%lf,%lf,%lf) <-> (%lf,%lf,%lf) ]",dp[0].x,dp[0].y,dp[0].z,dp[1].x,dp[1].y,dp[1].z);
            p -> put_StatusText(0,szText);
         } else
            p -> put_StatusText(0,"Extents: infinite");
   
      }

      delete [] pszExistingText;

   }

   HANDLE *pt = p -> renderThreadList.GetLast();
   p -> renderThreadList.Remove(pt);
   delete pt;

   p -> rendering = false;

#ifdef DO_THREAD_RENDERING
   CoUninitialize();
#endif

   return 0;
   }
 
 
   int G::stop() {
   HANDLE *pt;
   while ( pt = renderThreadList.GetLast() ) {
      TerminateThread(*pt,0);
      WaitForSingleObject(*pt,INFINITE);
      renderThreadList.Remove(pt);
      pIDataSetMaster -> PopExtents();
      delete pt;
   }
   rendering = false;
   return TRUE;
   }
 
 
   int G::wait() {
   HANDLE *pt;
   while ( pt = renderThreadList.GetLast() ) {
      WaitForSingleObject(*pt,INFINITE);
   }
   return TRUE;
   }

   int G::redraw() {
   if ( rendering ) return 1;
   IAxis *ia = reinterpret_cast<IAxis *>(NULL);
   while ( ia = axisList.GetNext(ia) ) ia -> Redraw();
   IPlot *ap = reinterpret_cast<IPlot *>(NULL);
   while ( ap = plotList.GetNext(ap) ) ap -> Redraw();
   IText *it = reinterpret_cast<IText *>(NULL);
   while ( it = textList.GetNext(it) ) it -> Redraw();
   return 0;
   }
 
 
   void G::setColorBackground() {
   return;
   }
 
 
   int G::erase() {
   if ( pIOpenGLImplementation ) 
      pIOpenGLImplementation -> Erase(propertyBackgroundColor);
   else
      eraseBackground();
   eraseGraphicCursor();
   return TRUE;
   }


   int G::stubDraw() {

   if ( ! pIOpenGLImplementation )
      return 0;

//   erase();
 
/* 
   pIOpenGLImplementation -> SetUp(pIDataSetMaster);
   pIOpenGLImplementation -> SetLighting(
                  ppPropertyLightOn,
                  ppPropertyAmbientLight,
                  ppPropertyDiffuseLight,
                  ppPropertySpecularLight,
                  ppPropertyLightPos,
                  propertyCountLights,propertyShinyness);
*/ 
   IPlot *pPlot = (IPlot*)NULL;
   while ( pPlot = plotList.GetNext(pPlot) )
      pPlot -> StubPlot();
 
Sleep(1000);
   return TRUE;
   }
