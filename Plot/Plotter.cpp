/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "utils.h"

#include "plot.h"


   unsigned int __stdcall Plot::plotter(void *pObject) {
 
   Plot* pThis = (Plot *)pObject;
 
   long okToPlot,segmentID;

   long plotTypes;

   for (  int plotIndex = pThis -> currentPlotCount - 1; plotIndex > -1; plotIndex-- ) {
 
      Plot *p = static_cast<Plot *>(pThis -> pIPlots[plotIndex]);
 
      p -> get_OkToPlot(&okToPlot);

      if ( ! okToPlot )
         continue;

      p -> Create(&segmentID);

      if ( p -> overrideOwnerView ) {
         //p -> propertyPlotView -> get_longValue((long *)&view);
         p -> pIOpenGLImplementation -> Push();
         p -> pIOpenGLImplementation -> SetUp(p -> pIDataSet,p -> propertyPlotView,NULL,NULL,NULL);//p -> pOwnerPropertyTheta,p -> pOwnerPropertyPhi,p -> pOwnerPropertySpin);
      } //else      
         //p -> pOwnerPropertyPlotView -> get_longValue((long *)&view);

      if ( p -> overrideOwnerType || ( NULL == p -> pOwnerProperty2DPlotType ) ) 
         p -> property2DPlotType -> get_longValue(&plotTypes);
      else
         p -> pOwnerProperty2DPlotType -> get_longValue(&plotTypes);

      if ( ! ( gcPlotTypeNone == (gc2DPlotTypes)plotTypes ) ) {

         for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> pPair : plotType2DProviderInstances ) {

            if ( ! ( NULL == pPair.second ) ) 
               break;

            if ( ! ( plotTypes & pPair.first ) )
               continue;

            switch ( (long)pPair.first ) {
        
            case gcPlotTypeStacks:
               p -> stacks();
               break;

            case gcPlotTypeBalls:
               p -> balls();
               break;
    
            case gcPlotTypeBlocks:
               p -> blocks();
               break;
    
            case gcPlotTypeNatural:
               p -> BasePlot::Draw();
               break;

            }

            for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> pPair : plotType2DProviderInstances ) {

               if ( NULL == pPair.second ) 
                  continue;

               if ( ! ( plotTypes & pPair.first ) )
                  continue;

               pPair.second -> Execute(plotType2DInstanceNumber[pPair.first],segmentID,(void *)p,(void *)p -> pIOpenGLImplementation,(void *)p -> pIDataSet);

            }

         }

      }

      if ( p -> overrideOwnerType || ( NULL == p -> pOwnerProperty3DPlotType ) ) 
         p -> property3DPlotType -> get_longValue(&plotTypes);
      else
         p -> pOwnerProperty3DPlotType -> get_longValue(&plotTypes);

      if ( ( gcPlotType3DNone == (gc3DPlotTypes)plotTypes ) ) 
         continue;

      for ( std::pair<gc3DPlotTypes,IGSystemPlotType *> pPair : plotType3DProviderInstances ) {

         if ( ! ( plotTypes & pPair.first ) )
            continue;

         pPair.second -> Execute(plotType3DInstanceNumber[pPair.first],segmentID,(void *)p,(void *)p -> pIOpenGLImplementation,(void *)p -> pIDataSet);

      }

   }

 
   for ( int k = 0; k < pThis -> currentPlotCount; k++ ) 
      (pThis -> pIPlots[k]) -> Release();
 
   CoTaskMemFree(pThis -> pIPlots);

   pThis -> currentPlotCount = 0;
 
   return TRUE;
  }
