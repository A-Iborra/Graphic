
#include "PlotTypes.h"

#include <math.h>

#include "utils.h"


   void PlotTypes::balls(long segmentID) {

   DataPoint v[5];
   double avgNormal[5][4];
   DataPoint homePoint,firstPoint,secondPoint;//,thirdPoint,fourthPoint;
   //double xProd0[3],xProd1[3],xProd2[3];
   double minx,maxx,miny,maxy,minz,maxz;
   DataList *dl;

   pIOpenGLImplementation -> BeginSurface(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);

   pIOpenGLImplementation -> EndOpenGLMode();

   pIDataSet -> get_minX(&minx);
   pIDataSet -> get_maxX(&maxx);
   pIDataSet -> get_minY(&miny);
   pIDataSet -> get_maxY(&maxy);
   pIDataSet -> get_minZ(&minz);
   pIDataSet -> get_maxZ(&maxz);

   double xRadius = (maxx - minx) * defaultSolidSizeInPercentDomain / 100.0;
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

   dl = (DataList *)NULL;

   pIDataSet -> get(dl,&homePoint,&dl);

   while ( dl ) {

      pIOpenGLImplementation -> BeginOpenGLMode(GL_TRIANGLE_STRIP);//QUADS);

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

            //thirdPoint = homePoint;
            //thirdPoint.x += xRadius * cos(nextTheta) * cos(nextPhi);
            //thirdPoint.y += xRadius * sin(nextTheta) * cos(nextPhi) / yScaleFactor;
            //thirdPoint.z += xRadius * sin(nextPhi) / zScaleFactor;

            //fourthPoint = homePoint;
            //fourthPoint.x += xRadius * cos(theta) * cos(nextPhi);
            //fourthPoint.y += xRadius * sin(theta) * cos(nextPhi) / yScaleFactor;
            //fourthPoint.z += xRadius * sin(nextPhi) / zScaleFactor;

            v[0] = firstPoint;
            v[1] = secondPoint;
            //v[2] = thirdPoint;
            //v[3] = fourthPoint;

#if 1
            avgNormal[0][0] = firstPoint.x;
            avgNormal[0][1] = firstPoint.y;
            avgNormal[0][2] = firstPoint.z;
#else
            xProd0[0] = v[1].x - v[0].x; // x-product of the vector from 0->1 with 0->3
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
    
            xProd0[0] = v[2].x - v[1].x; // x-product of the vector from 1->2 with 1->0
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
    
            xProd0[0] = v[3].x - v[2].x; // x-product of the vector from 2->3 with 2->1
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
    
            xProd0[0] = v[0].x - v[3].x; // x-product of the vector from 3->0 with 3->2
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
#endif

            pIOpenGLImplementation -> Normal3dv(avgNormal[0]);
            for ( int vk = 0; vk < 2; vk++ ) {
               pIOpenGLImplementation -> Vertex(&v[vk]);
            }

         }

      }

      pIDataSet -> get(dl,&homePoint,&dl);

      pIOpenGLImplementation -> EndOpenGLMode();

   }

   pIOpenGLImplementation -> BeginOpenGLMode(GL_QUADS);

   pIOpenGLImplementation -> CloseSegment(segmentID,TRUE);
    
   return;
   }