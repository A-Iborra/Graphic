/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "utils.h"

#include "plot.h"


   unsigned int __stdcall Plot::plotter(void *pObject) {
 
   Plot* pThis = (Plot *)pObject;
 
   DataPoint v[5];
   DataList *dlTemp,*dlNext,*dl;
   DataPoint firstPoint,secondPoint,thirdPoint,fourthPoint;
   int endOfData,kAccross;
   long okToPlot,segmentID;
   PlotViews view;
   long plotTypes;
   long allTypes[] = {(long)gcPlotTypeNatural,(long)gcPlotTypeSurface,(long)gcPlotTypeWireFrame,
                        (long)gcPlotTypeStacks,(long)gcPlotTypeBlocks,(long)gcPlotTypeBalls,
                        (long)gcPlotTypePie,(long)gcPlotTypeContour,
                        (long)gcPlotTypeQuads,(long)gcPlotTypeTriangles,0};

   for (  int plotIndex = pThis -> currentPlotCount - 1; plotIndex > -1; plotIndex-- ) {
 
      Plot *p = static_cast<Plot *>(pThis -> pIPlots[plotIndex]);
 
      p -> get_OkToPlot(&okToPlot);

      if ( ! okToPlot )
         continue;

      p -> Create(&segmentID);

      if ( p -> overrideOwnerType ) 
         p -> propertyPlotType -> get_longValue(&plotTypes);
      else
         p -> pOwnerProperty2DPlotType -> get_longValue(&plotTypes);

      if ( p -> overrideOwnerView ) {
         p -> propertyPlotView -> get_longValue((long *)&view);
      //   p -> pIOpenGLImplementation -> Push();
      //   p -> pIOpenGLImplementation -> SetUp(p -> pIDataSet,p -> propertyPlotView,p -> pOwnerPropertyTheta,p -> pOwnerPropertyPhi,p -> pOwnerPropertySpin);
      } else      
         p -> pOwnerPropertyPlotView -> get_longValue((long *)&view);

      for ( int typeIndex = 0; allTypes[typeIndex]; typeIndex++ ) {

         PlotTypes type = (PlotTypes)(plotTypes & allTypes[typeIndex]);

         switch ( type ) {
    
         case gcPlotTypeSurface: {
    
            double xProd0[3],xProd1[3],xProd2[3],avgNormal[4][3];
    
            p -> pIOpenGLImplementation -> BeginSurface(segmentID,p -> propertyTopSurfaceColor,p -> propertyBottomSurfaceColor);
   
            kAccross = 0;
            endOfData = FALSE;
    
            dl = (DataList *)NULL;
            p -> pIDataSet -> get(dl,&firstPoint,&dl);
   
            while ( dl ) {
         
               dlNext = dl;
               p -> pIDataSet -> get(dlNext,&secondPoint,&dlTemp);
               if ( ! dlTemp ) break;
         
               if ( secondPoint.x != firstPoint.x ) {
                  kAccross = 0;
                  p -> pIDataSet -> get(dl,&firstPoint,&dl);
                  continue;
               }
         
               fourthPoint = secondPoint;
         
               while ( secondPoint.x == firstPoint.x ) {
                  p -> pIDataSet -> get(dlNext,&secondPoint,&dlNext);
                  if ( !dlNext ) {
                     endOfData = TRUE;
                     break;
                  }
               }
   
               if ( endOfData ) break;
         
               for ( int j = 0; j < kAccross; j++ ) {
                  if ( !dlNext ) {
                     endOfData = TRUE;
                     break;
                  }
                  p -> pIDataSet -> get(dlNext,&secondPoint,&dlNext);
               }
   
               if ( endOfData ) break;
    
               p -> pIDataSet -> get(dlNext,&thirdPoint,&dlTemp);
         
               v[0] = firstPoint;
               v[1] = secondPoint;
               v[2] = thirdPoint;
               v[3] = fourthPoint;
   
               xProd0[0] = v[1].x - v[0].x;
               xProd0[1] = v[1].y - v[0].y;
               xProd0[2] = v[1].z - v[0].z;
               xProd1[0] = v[3].x - v[0].x;
               xProd1[1] = v[3].y - v[0].y;
               xProd1[2] = v[3].z - v[0].z;
               VxV(xProd0,xProd1,xProd2);
               unitVector(xProd2,xProd0);
               avgNormal[0][0] = xProd0[0];
               avgNormal[0][1] = xProd0[1];
               avgNormal[0][2] = xProd0[2];
    
               xProd0[0] = v[2].x - v[1].x;
               xProd0[1] = v[2].y - v[1].y;
               xProd0[2] = v[2].z - v[1].z;
               xProd1[0] = v[0].x - v[1].x;
               xProd1[1] = v[0].y - v[1].y;
               xProd1[2] = v[0].z - v[1].z;
               VxV(xProd0,xProd1,xProd2);
               unitVector(xProd2,xProd0);
               avgNormal[1][0] = xProd0[0];
               avgNormal[1][1] = xProd0[1];
               avgNormal[1][2] = xProd0[2];
    
               xProd0[0] = v[3].x - v[2].x;
               xProd0[1] = v[3].y - v[2].y;
               xProd0[2] = v[3].z - v[2].z;
               xProd1[0] = v[1].x - v[2].x;
               xProd1[1] = v[1].y - v[2].y;
               xProd1[2] = v[1].z - v[2].z;
               VxV(xProd0,xProd1,xProd2);
               unitVector(xProd2,xProd0);
               avgNormal[2][0] = xProd0[0];
               avgNormal[2][1] = xProd0[1];
               avgNormal[2][2] = xProd0[2];
    
               xProd0[0] = v[0].x - v[3].x;
               xProd0[1] = v[0].y - v[3].y;
               xProd0[2] = v[0].z - v[3].z;
               xProd1[0] = v[2].x - v[3].x;
               xProd1[1] = v[2].y - v[3].y;
               xProd1[2] = v[2].z - v[3].z;
               VxV(xProd0,xProd1,xProd2);
               unitVector(xProd2,xProd0);
               avgNormal[3][0] = xProd0[0];
               avgNormal[3][1] = xProd0[1];
               avgNormal[3][2] = xProd0[2];
    
               for ( int vk = 0; vk < 4; vk++ ) {
                  p -> pIOpenGLImplementation -> Normal3dv(avgNormal[vk]);
                  p -> pIOpenGLImplementation -> Vertex(&v[vk]);
               }
    
               kAccross++;
   
               p -> pIDataSet -> get(dl,&firstPoint,&dl);
    
            }
    
            p -> pIOpenGLImplementation -> EndSurface(segmentID);
   
            break;
            }
       
         case gcPlotTypeWireFrame: {
   
            p -> pIOpenGLImplementation -> BeginWireframe(segmentID,p -> propertyLineColor,p -> propertyLineWeight);
    
            kAccross = 0;
            endOfData = FALSE;
    
            dl = (DataList *)NULL;
            p -> pIDataSet -> get(dl,&firstPoint,&dl);
            while ( dl ) {
       
               dlNext = dl;
               p -> pIDataSet -> get(dlNext,&secondPoint,&dlTemp);
               if ( !dlTemp ) break;
       
               if ( secondPoint.x != firstPoint.x ) {
                  kAccross = 0;
                  p -> pIDataSet -> get(dl,&firstPoint,&dl);
                  continue;
               }
       
               fourthPoint = secondPoint;
       
               while ( secondPoint.x == firstPoint.x ) {
                  p -> pIDataSet -> get(dlNext,&secondPoint,&dlNext);
                  if ( !dlNext ) {
                     endOfData = TRUE;
                     break;
                  }
               }
               if ( endOfData ) break;
       
               for ( int j = 0; j < kAccross; j++ ) {
                  if ( !dlNext ) {
                     endOfData = TRUE;
                     break;
                  }
                  p -> pIDataSet -> get(dlNext,&secondPoint,&dlNext);
               }
               if ( endOfData ) break;
    
               p -> pIDataSet -> get(dlNext,&thirdPoint,&dlTemp);
       
               v[0] = firstPoint;
               v[1] = secondPoint;
               v[2] = thirdPoint;
               v[3] = fourthPoint;
       
               for ( int vk = 0; vk < 4; vk++ ) p -> pIOpenGLImplementation -> Vertex(&v[vk]);
       
               kAccross++;
       
               p -> pIDataSet -> get(dl,&firstPoint,&dl);
    
            }
       
            p -> pIOpenGLImplementation -> EndWireframe(segmentID);
    
            }
            break;
    
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

      }

      //if ( p -> overrideOwnerView ) 
      //   p -> pIOpenGLImplementation -> Pop();
 
   }
 
   for ( int k = 0; k < pThis -> currentPlotCount; k++ ) 
      (pThis -> pIPlots[k]) -> Release();
 
   CoTaskMemFree(pThis -> pIPlots);

   pThis -> currentPlotCount = 0;
 
   return TRUE;
  }
