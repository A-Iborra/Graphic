// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "PlotTypes.h"
#include "utils.h"

   void PlotTypes::surface(commonProperties *pProperties,long segmentID) {
   
   DataPoint v[5];
   DataList *dlTemp,*dlNext,*dl;
   DataPoint firstPoint,secondPoint,thirdPoint,fourthPoint;
   int endOfData,kAccross;

   double xProd0[3],xProd1[3],xProd2[3],avgNormal[5][4];

   pIOpenGLImplementation -> BeginSurface(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);
   
   kAccross = 0;
   endOfData = FALSE;
    
   dl = (DataList *)NULL;

   pIDataSet -> get(dl,&firstPoint,&dl);

   pIOpenGLImplementation -> BeginOpenGLMode(GL_QUADS);
      
   while ( dl ) {
         
      dlNext = dl;

      pIDataSet -> get(dlNext,&secondPoint,&dlTemp);

      if ( ! dlTemp ) break;
         
      if ( secondPoint.x != firstPoint.x ) {
         kAccross = 0;
         pIDataSet -> get(dl,&firstPoint,&dl);
         continue;
      }
         
      fourthPoint = secondPoint;
         
      while ( secondPoint.x == firstPoint.x ) {
         pIDataSet -> get(dlNext,&secondPoint,&dlNext);
         if ( ! dlNext ) {
            endOfData = TRUE;
            break;
         }
      }
   
      if ( endOfData ) break;
         
      for ( int j = 0; j < kAccross; j++ ) {
         if ( ! dlNext ) {
            endOfData = TRUE;
            break;
         }
         pIDataSet -> get(dlNext,&secondPoint,&dlNext);
      }
   
      if ( endOfData ) break;
    
      pIDataSet -> get(dlNext,&thirdPoint,&dlTemp);
         
      v[0] = firstPoint;
      v[1] = secondPoint;
      v[2] = thirdPoint;
      v[3] = fourthPoint;
   
      xProd0[0] = v[1].x - v[0].x; // x-product of the vector from 0->1 with 0->3
      xProd0[1] = v[1].y - v[0].y;
      xProd0[2] = v[1].z - v[0].z;
      xProd1[0] = v[3].x - v[0].x;
      xProd1[1] = v[3].y - v[0].y;
      xProd1[2] = v[3].z - v[0].z;

      VxV(xProd0,xProd1,xProd2);
      unitVector(xProd2,avgNormal[0]);
    
      xProd0[0] = v[2].x - v[1].x; // x-product of the vector from 1->2 with 1->0
      xProd0[1] = v[2].y - v[1].y;
      xProd0[2] = v[2].z - v[1].z;
      xProd1[0] = v[0].x - v[1].x;
      xProd1[1] = v[0].y - v[1].y;
      xProd1[2] = v[0].z - v[1].z;

      VxV(xProd0,xProd1,xProd2);
      unitVector(xProd2,avgNormal[1]);
    
      xProd0[0] = v[3].x - v[2].x; // x-product of the vector from 2->3 with 2->1
      xProd0[1] = v[3].y - v[2].y;
      xProd0[2] = v[3].z - v[2].z;
      xProd1[0] = v[1].x - v[2].x;
      xProd1[1] = v[1].y - v[2].y;
      xProd1[2] = v[1].z - v[2].z;

      VxV(xProd0,xProd1,xProd2);
      unitVector(xProd2,avgNormal[2]);
    
      xProd0[0] = v[0].x - v[3].x; // x-product of the vector from 3->0 with 3->2
      xProd0[1] = v[0].y - v[3].y;
      xProd0[2] = v[0].z - v[3].z;
      xProd1[0] = v[2].x - v[3].x;
      xProd1[1] = v[2].y - v[3].y;
      xProd1[2] = v[2].z - v[3].z;

      VxV(xProd0,xProd1,xProd2);
      unitVector(xProd2,avgNormal[3]);

      avgNormal[4][0] = 0.0;
      avgNormal[4][1] = 0.0;
      avgNormal[4][2] = 0.0;
      avgNormal[4][3] = 0.0;

      for ( int vk = 0; vk < 4; vk++ ) {
         avgNormal[4][0] += avgNormal[vk][0];
         avgNormal[4][1] += avgNormal[vk][1];
         avgNormal[4][2] += avgNormal[vk][2];
         avgNormal[4][3] += avgNormal[vk][3];
      }

      pIOpenGLImplementation -> Normal3dv(avgNormal[4]);

      for ( int vk = 0; vk < 4; vk++ ) 
         pIOpenGLImplementation -> Vertex(&v[vk]);

      kAccross++;
   
      pIDataSet -> get(dl,&firstPoint,&dl);
    
   }

   pIOpenGLImplementation -> CloseSegment(segmentID,TRUE);   

   return;
   }
