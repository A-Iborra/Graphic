// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "PlotTypes.h"

#include <math.h>

#include "utils.h"

   void PlotTypes::balls(commonProperties *pProperties,long segmentID) {

   DataPoint v[5];
   double avgNormal[5][4];
   DataPoint homePoint,firstPoint,secondPoint;
   double minx,maxx,miny,maxy,minz,maxz;

   pIOpenGLImplementation -> BeginSurface(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);

   pIDataSet -> get_minX(&minx);
   pIDataSet -> get_maxX(&maxx);
   pIDataSet -> get_minY(&miny);
   pIDataSet -> get_maxY(&maxy);
   pIDataSet -> get_minZ(&minz);
   pIDataSet -> get_maxZ(&maxz);

   double xRadius = (maxx - minx) * pProperties -> defaultSolidSizeInPercentDomain / 100.0;
   double zScaleFactor = (maxx - minx) / (maxz - minz);
   double yScaleFactor = (maxx - minx) / (maxy - miny);
   double theta, phi;

   static double twoPi = 0.0,pi = 0.0;
   static double dTheta,dThetaBase,dPhi,piOver2;

   if ( 0.0 == twoPi ) {
      piOver2 = 2.0 * atan(1.0);
      pi = 2.0 * piOver2;
      twoPi = 2.0 * pi;
      dTheta = twoPi / 60.0;
      dPhi = pi / 60.0;
   }

   DataList *pItem = NULL;

   pIDataSet -> peek(pItem,&pItem);

   while ( pItem ) {

      homePoint = pItem -> data;

      pIOpenGLImplementation -> BeginOpenGLMode(GL_TRIANGLE_STRIP);

      for ( theta = 0.0; theta <= twoPi; theta += dTheta ) {

         double nextTheta = theta + dTheta;

         double cosTheta = cos(theta);
         double cosNextTheta = cos(theta + dTheta);
         double sinTheta = sin(theta);
         double sinNextTheta = sin(theta + dTheta);

         for ( phi = -piOver2; phi < piOver2; phi += dPhi ) {

            double nextPhi = phi + dPhi;

            firstPoint = homePoint;
            firstPoint.x += xRadius * cosTheta * cos(phi);
            firstPoint.y += xRadius * sinTheta * cos(phi) / yScaleFactor;
            firstPoint.z += xRadius * sin(phi) / zScaleFactor;

            secondPoint = homePoint;
            secondPoint.x += xRadius * cosNextTheta * cos(phi);
            secondPoint.y += xRadius * sinNextTheta * cos(phi) / yScaleFactor;
            secondPoint.z += xRadius * sin(phi) / zScaleFactor;

            v[0] = firstPoint;
            v[1] = secondPoint;

            avgNormal[0][0] = firstPoint.x;
            avgNormal[0][1] = firstPoint.y;
            avgNormal[0][2] = firstPoint.z;

            pIOpenGLImplementation -> Normal3dv(avgNormal[0]);
            for ( int vk = 0; vk < 2; vk++ ) {
               pIOpenGLImplementation -> Vertex(&v[vk]);
            }

         }

      }

      pIOpenGLImplementation -> EndOpenGLMode();

      pIDataSet -> peek(pItem,&pItem);

   }

   pIOpenGLImplementation -> BeginOpenGLMode(GL_QUADS);

   pIOpenGLImplementation -> CloseSegment(segmentID,TRUE);
    
   return;
   }