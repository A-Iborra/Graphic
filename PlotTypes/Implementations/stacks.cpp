// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "PlotTypes.h"

#include "utils.h"

#define DRAW_PLANE                  \
   v[0] = firstPoint;               \
   v[1] = secondPoint;              \
   v[2] = thirdPoint;               \
   v[3] = fourthPoint;              \
                                    \
   xProd0[0] = v[1].x - v[0].x;     \
   xProd0[1] = v[1].y - v[0].y;     \
   xProd0[2] = v[1].z - v[0].z;     \
   xProd1[0] = v[3].x - v[0].x;     \
   xProd1[1] = v[3].y - v[0].y;     \
   xProd1[2] = v[3].z - v[0].z;     \
   VxV(xProd0,xProd1,xProd2);       \
   unitVector(xProd2,xProd0);       \
   avgNormal[0] = xProd0[0];        \
   avgNormal[1] = xProd0[1];        \
   avgNormal[2] = xProd0[2];        \
                                    \
   xProd0[0] = v[2].x - v[1].x;     \
   xProd0[1] = v[2].y - v[1].y;     \
   xProd0[2] = v[2].z - v[1].z;     \
   xProd1[0] = v[0].x - v[1].x;     \
   xProd1[1] = v[0].y - v[1].y;     \
   xProd1[2] = v[0].z - v[1].z;     \
   VxV(xProd0,xProd1,xProd2);       \
   unitVector(xProd2,xProd0);       \
   avgNormal[0] += xProd0[0];       \
   avgNormal[1] += xProd0[1];       \
   avgNormal[2] += xProd0[2];       \
                                    \
   xProd0[0] = v[3].x - v[2].x;     \
   xProd0[1] = v[3].y - v[2].y;     \
   xProd0[2] = v[3].z - v[2].z;     \
   xProd1[0] = v[1].x - v[2].x;     \
   xProd1[1] = v[1].y - v[2].y;     \
   xProd1[2] = v[1].z - v[2].z;     \
   VxV(xProd0,xProd1,xProd2);       \
   unitVector(xProd2,xProd0);       \
   avgNormal[0] += xProd0[0];       \
   avgNormal[1] += xProd0[1];       \
   avgNormal[2] += xProd0[2];       \
                                    \
   xProd0[0] = v[1].x - v[3].x;     \
   xProd0[1] = v[1].y - v[3].y;     \
   xProd0[2] = v[1].z - v[3].z;     \
   xProd1[0] = v[2].x - v[3].x;     \
   xProd1[1] = v[2].y - v[3].y;     \
   xProd1[2] = v[2].z - v[3].z;     \
   VxV(xProd0,xProd1,xProd2);       \
   unitVector(xProd2,xProd0);       \
   avgNormal[0] += xProd0[0];       \
   avgNormal[1] += xProd0[1];       \
   avgNormal[2] += xProd0[2];       \
                                    \
   avgNormal[0] /= 4.0;             \
   avgNormal[1] /= 4.0;             \
   avgNormal[2] /= 4.0;             \
                                    \
   pIOpenGLImplementation -> Normal3dv(avgNormal);                          \
                                                                            \
   for ( vk = 0; vk < 4; vk++ ) pIOpenGLImplementation -> Vertex(&v[vk]);   \

   void PlotTypes::stacks(commonProperties *pProperties,long segmentID) {

   DataPoint homePoint,firstPoint,secondPoint,thirdPoint,fourthPoint,v[4]; 
   double xProd0[3],xProd1[3],xProd2[3],avgNormal[3];
   int vk;

   double minx,maxx,miny,maxy,minz,maxz;
   BOOL endOfData;
   DataList *dl;
   
   pIOpenGLImplementation -> BeginSolids(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);

   pIDataSet -> get_minX(&minx);
   pIDataSet -> get_maxX(&maxx);
   pIDataSet -> get_minY(&miny);
   pIDataSet -> get_maxY(&maxy);
   pIDataSet -> get_minZ(&minz);
   pIDataSet -> get_maxZ(&maxz);

   double xWidth = (maxx - minx) / 50.0;
   double yWidth = (maxy - miny) / 50.0;
   double floor = minz;
   
   endOfData = FALSE;
   dl = (DataList *)NULL;
   pIDataSet -> get(dl,&homePoint,&dl);
   while ( dl ) {
   
/* Back facing on the X-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y - yWidth;
      firstPoint.z = floor;
   
      secondPoint.x = homePoint.x + xWidth;
      secondPoint.y = firstPoint.y;
      secondPoint.z = firstPoint.z;
   
      thirdPoint.x = secondPoint.x;
      thirdPoint.y = secondPoint.y;
      thirdPoint.z = homePoint.z;
   
      fourthPoint.x = firstPoint.x;
      fourthPoint.y = firstPoint.y;
      fourthPoint.z = homePoint.z;
    
      DRAW_PLANE
    
/* Front facing on the Y-Z plane */

      firstPoint = secondPoint;
    
      secondPoint.y = homePoint.y + yWidth;
    
      thirdPoint = secondPoint;
      thirdPoint.z = homePoint.z;
    
      fourthPoint = firstPoint;
      fourthPoint.z = homePoint.z;
    
      DRAW_PLANE
    
/* Front facing on the X-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y + yWidth;
      firstPoint.z = floor;
    
      secondPoint.x = homePoint.x - xWidth;
      secondPoint.y = homePoint.y + yWidth;
      secondPoint.z = homePoint.z;

      thirdPoint = secondPoint;

      thirdPoint.x = homePoint.x + xWidth;
    
      fourthPoint = thirdPoint;
      fourthPoint.z = floor;
    
      DRAW_PLANE
    
/* Back facing on the Y-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y + yWidth;
      firstPoint.z = floor;
    
      secondPoint.x = firstPoint.x;
      secondPoint.y = homePoint.y - yWidth;
      secondPoint.z = firstPoint.z;
    
      thirdPoint.x = secondPoint.x;
      thirdPoint.y = secondPoint.y;
      thirdPoint.z = homePoint.z;
    
      fourthPoint.x = firstPoint.x;
      fourthPoint.y = firstPoint.y;
      fourthPoint.z = homePoint.z;
    
      DRAW_PLANE
    
/* Up facing on the X-Y plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y - yWidth;
      firstPoint.z = homePoint.z;
    
      secondPoint.x = homePoint.x + xWidth;
      secondPoint.y = homePoint.y - yWidth;
      secondPoint.z = homePoint.z;
    
      thirdPoint.x = homePoint.x + xWidth;
      thirdPoint.y = homePoint.y + yWidth;
      thirdPoint.z = homePoint.z;
    
      fourthPoint.x = homePoint.x - xWidth;
      fourthPoint.y = homePoint.y + yWidth;
      fourthPoint.z = homePoint.z;
    
      DRAW_PLANE
    
      pIDataSet -> get(dl,&homePoint,&dl);
    
   }
    
   pIOpenGLImplementation -> EndSolids(segmentID);

   return;
   }