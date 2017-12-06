/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <process.h>
#include <commctrl.h>

#include "Graphic_resource.h"

#include "utils.h"
#include "Graphic.h"

#include "List.cpp"

   int G::render(long sourceID) {

   if ( ! pIOpenGLImplementation )
      return 0;

   if ( ! hwndGraphic ) 
      return 0;

   if ( ! IsWindowVisible(hwndGraphic) )
      return 0;

   if ( renderThreadList.Count() > 0 ) 
      return 0;

   stop();

   currentPlotSourceID = sourceID;

   unsigned int threadAddr;
 
#if 0
   HANDLE *pt = new HANDLE(NULL);
   *pt = (HANDLE)_beginthreadex(NULL,0,G::threadRender,(void*)this,0,&threadAddr);
   renderThreadList.Add(pt);
#else
   threadRender((void *)this);
#endif

   return 0;
   }


   unsigned __stdcall G::threadRender(void *pThis) {

#if 0
   CoInitializeEx(NULL,COINIT_MULTITHREADED);
#endif

   G *p = (G *)pThis;

   p -> rendering = true;

   p -> pIDataSetMaster -> ReSet();

   IPlot *pPlot = (IPlot *)NULL;

   while ( pPlot = p -> plotList.GetNext(pPlot) ) {

      long okayToPlot = false;

      if ( p -> plotList.ID(pPlot) == p -> currentPlotSourceID )
         okayToPlot = TRUE;
      else
         okayToPlot = ( ! p -> autoClear || p -> currentPlotSourceID == 0 );

      pPlot -> put_OkToPlot(okayToPlot);

      if ( okayToPlot ) {
         DataPoint minPoint,maxPoint;
         IDataSet *pIDataSet;
         pPlot -> get_DataSet(&pIDataSet);
         pIDataSet -> GetDomain(&minPoint,&maxPoint);
         p -> pIDataSetMaster -> ResetLimits(&minPoint);
         p -> pIDataSetMaster -> ResetLimits(&maxPoint);
      }

   }

   if ( 0 == p -> plotList.Count() ) {
      DataPoint dpMin = {0.0,0.0,0.0};
      DataPoint dpMax = {1.0,1.0,1.0};
      p -> pIDataSetMaster -> pushDataPoint(&dpMin);
      p -> pIDataSetMaster -> pushDataPoint(&dpMax);
   }

   p -> pIOpenGLImplementation -> SetUp(p -> pIDataSetMaster);

   IAxis *ax = NULL;
   while ( ax = p -> axisList.GetNext(ax) ) {
      char type;
      ax -> get_Type(&type);
      if ( p -> plotView != gcPlotView3D && 'Z' == type )
         continue;
      ax -> PrepData();
      DataPoint minPoint,maxPoint;
      IDataSet *pIDataSet;
      ax -> get_DataSet(&pIDataSet);
      pIDataSet -> GetDomain(&minPoint,&maxPoint);
      p -> pIDataSetMaster -> ResetLimits(&minPoint);
      p -> pIDataSetMaster -> ResetLimits(&maxPoint);
   }

   p -> pIOpenGLImplementation -> SetUp(p -> pIDataSetMaster);
 
   p -> pIOpenGLImplementation -> SetLighting(
                  p -> ppPropertyLightOn,
                  p -> ppPropertyAmbientLight,
                  p -> ppPropertyDiffuseLight,
                  p -> ppPropertySpecularLight,
                  p -> ppPropertyLightPos,
                  p -> propertyCountLights,p -> propertyShinyness);
 
   p -> erase(); 

   ax = NULL;
   while ( ax = p -> axisList.GetNext(ax) )
      ax -> Draw();

   if ( p -> plotList.Count() > 0 ) {
      IPlot** pIPlots = reinterpret_cast<IPlot **>(CoTaskMemAlloc(p -> plotList.Count() * sizeof(IPlot*)));
      int n = p -> plotList.Count();
      for ( int k = 0; k < n; k++ )
         p -> plotList.GetByIndex(k) -> QueryInterface(IID_IPlot,(void**)(&pIPlots[k]));
      p -> plotList.GetFirst() -> Plotter(n,&pIPlots);
   }

   ax = NULL;
   while ( ax = p -> axisList.GetNext(ax) )
      ax -> DrawLabels();

   IText *t = NULL;
   while ( t = p -> textList.GetNext(t) ) {
      t -> PrepData();
      t -> Draw();
   } 

   p -> pIOpenGLImplementation -> Flush();

   if ( p -> showStatusBar ) {
      char szText[MAX_PROPERTY_SIZE];
      DataPoint dp[2];
      p -> pIDataSetMaster -> GetDomain(&dp[0],&dp[1]);
      if ( dp[0].x != DBL_MAX ) {
         sprintf(szText,"Extents [ (%lf,%lf,%lf) <-> (%lf,%lf,%lf) ]",dp[0].x,dp[0].y,dp[0].z,dp[1].x,dp[1].y,dp[1].z);
         p -> put_StatusText(0,szText);
      } else
         p -> put_StatusText(0,"Extents: infinite");
   }

//   p -> pIOpenGLImplementation -> SetUp(p -> pIDataSetMaster);

   HANDLE *pt = p -> renderThreadList.GetLast();
   p -> renderThreadList.Remove(pt);
   delete pt;

   p -> rendering = false;

#if 0
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
   if ( pIOpenGLImplementation ) {
      pIOpenGLImplementation -> Erase(propertyBackgroundColor);
      pIOpenGLImplementation -> Flush();
   }
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
