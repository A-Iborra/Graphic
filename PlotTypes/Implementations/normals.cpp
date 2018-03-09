// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "PlotTypes.h"
#include "utils.h"

   void PlotTypes::normals(commonProperties *pProperties,long segmentID) {
   
   DataPoint v[5];
   DataList *pDLTemp,*pDLNext,*pDL;
   DataPoint firstPoint,secondPoint,thirdPoint,fourthPoint;
   int endOfData,kAccross;

   DataPoint xProd0,xProd1,avgNormal[4];

   pIOpenGLImplementation -> BeginSurface(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);
   
   kAccross = 0;
   endOfData = FALSE;
    
   pDL = (DataList *)NULL;

   pIDataSet -> get(pDL,&firstPoint,&pDL);

   pIOpenGLImplementation -> BeginOpenGLMode(GL_LINES);
      
   while ( pDL ) {
         
      pDLNext = pDL;

      pIDataSet -> get(pDLNext,&secondPoint,&pDLTemp);

      if ( ! pDLTemp ) 
         break;
         
      if ( ! ( secondPoint.x == firstPoint.x ) ) {
         kAccross = 0;
         pIDataSet -> get(pDL,&firstPoint,&pDL);
         continue;
      }
         
      fourthPoint = secondPoint;
         
      while ( secondPoint.x == firstPoint.x ) {
         pIDataSet -> get(pDLNext,&secondPoint,&pDLNext);
         if ( ! pDLNext ) {
            endOfData = TRUE;
            break;
         }
      }
   
      if ( endOfData ) break;
         
      for ( int j = 0; j < kAccross; j++ ) {
         if ( ! pDLNext ) {
            endOfData = TRUE;
            break;
         }
         pIDataSet -> get(pDLNext,&secondPoint,&pDLNext);
      }
   
      if ( endOfData ) break;
    
      pIDataSet -> get(pDLNext,&thirdPoint,&pDLTemp);
         
      v[0] = firstPoint;
      v[1] = secondPoint;
      v[2] = thirdPoint;
      v[3] = fourthPoint;

      // x-product of the vector from 0->1 with 0->3

      VminusV(&v[1],&v[0],&xProd0);
      VminusV(&v[3],&v[0],&xProd1);

      VxV(&xProd0,&xProd1,&avgNormal[0]);
      unitVector(&avgNormal[0],&avgNormal[0]);
    
      // x-product of the vector from 1->2 with 1->0

      VminusV(&v[2],&v[1],&xProd0);
      VminusV(&v[0],&v[1],&xProd1);

      VxV(&xProd0,&xProd1,&avgNormal[1]);
      unitVector(&avgNormal[1],&avgNormal[1]);

      // x-product of the vector from 2->3 with 2->1

      VminusV(&v[3],&v[2],&xProd0);
      VminusV(&v[1],&v[2],&xProd1);

      VxV(&xProd0,&xProd1,&avgNormal[2]);
      unitVector(&avgNormal[2],&avgNormal[2]);
    
      // x-product of the vector from 3->0 with 3->2

      VminusV(&v[0],&v[3],&xProd0);
      VminusV(&v[2],&v[3],&xProd1);

      VxV(&xProd0,&xProd1,&avgNormal[3]);
      unitVector(&avgNormal[3],&avgNormal[3]);

      for ( int vk = 0; vk < 4; vk++ ) {
         pIOpenGLImplementation -> Vertex(&v[vk]);
         VplusV(&v[vk],&avgNormal[vk],&avgNormal[vk]);
         pIOpenGLImplementation -> Vertex(&avgNormal[vk]);
      }

      kAccross++;
   
      pIDataSet -> get(pDL,&firstPoint,&pDL);
    
   }

   pIOpenGLImplementation -> CloseSegment(segmentID,TRUE);   

   return;
   }
