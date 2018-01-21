// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "PlotTypes.h"

   void PlotTypes::wireFrame(commonProperties *pProperties,long segmentID) {

   DataPoint v[5];
   DataList *dlTemp,*dlNext,*dl;
   DataPoint firstPoint,secondPoint,thirdPoint,fourthPoint;
   int endOfData,kAccross;
   
   pIOpenGLImplementation -> BeginWireframe(segmentID,propertyLineColor,propertyLineWeight);

   kAccross = 0;
   endOfData = FALSE;    
    
   dl = (DataList *)NULL;

   pIDataSet -> get(dl,&firstPoint,&dl);

   while ( dl ) {
       
      dlNext = dl;

      pIDataSet -> get(dlNext,&secondPoint,&dlTemp);

      if ( ! dlTemp ) 
         break;
       
      if ( ! ( secondPoint.x == firstPoint.x ) ) {
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
       
      for ( int vk = 0; vk < 4; vk++ ) 
         pIOpenGLImplementation -> Vertex(&v[vk]);

      kAccross++;
       
      pIDataSet -> get(dl,&firstPoint,&dl);
    
   }
       
   pIOpenGLImplementation -> EndWireframe(segmentID);
    
   return;
   }
