/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <math.h>

#include "utils.h"

#include "plot.h"

   int Plot::balls() {
   DataArity dataArity;
   pIDataSet -> get_DataArity(&dataArity);
   if ( dataArity == DATA_ARITY_2D ) 
      return balls2D();
   else
      return balls3D();
   }


   int Plot::balls2D() {
   double minx,maxx,miny,maxy,minz,maxz;
   long segmentID;
   DataPoint homePoint,dp;
   DataList *dl;

   get_SegmentID(&segmentID);
   
   pIOpenGLImplementation -> BeginSolids(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);

   pIDataSet -> get_minX(&minx);
   pIDataSet -> get_maxX(&maxx);
   pIDataSet -> get_minY(&miny);
   pIDataSet -> get_maxY(&maxy);
   pIDataSet -> get_minZ(&minz);
   pIDataSet -> get_maxZ(&maxz);

   double xWidth = (maxx - minx) * defaultSolidSizeInPercentDomain / 100.0;
   double zWidth = (maxz - minz) * defaultSolidSizeInPercentDomain / 100.0;
   double floor = miny;
   
   dl = (DataList *)NULL;
   pIDataSet -> get(dl,&homePoint,&dl);
   while ( dl ) {
      pIOpenGLImplementation -> Vertex(&dp);
      pIDataSet -> get(dl,&homePoint,&dl);
   }

   pIOpenGLImplementation -> EndSolids(segmentID);

   return 0;
   }


   int Plot::balls3D() {

   DECLARE_PLANE

   double minx,maxx,miny,maxy,minz,maxz;
   long segmentID;
   DataList *dl;

   get_SegmentID(&segmentID);
   
   pIOpenGLImplementation -> BeginSolids(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);

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

   static double twoPi = 0.0;
   static double dTheta,dThetaBase,dPhi,piOver2;

   if ( 0.0 == twoPi ) {
      piOver2 = 2.0 * atan(1.0);
      twoPi = 4.0 * piOver2;
      dThetaBase = twoPi / 10.0;
      dPhi = piOver2 / 4.0;
   }

   dl = (DataList *)NULL;
   pIDataSet -> get(dl,&homePoint,&dl);
   while ( dl ) {

      for ( phi = 0.0; phi <= piOver2; phi += dPhi ) {

dTheta = dThetaBase * cos(phi);

         for ( theta = 0.0; theta <= twoPi; theta += dTheta ) {

            firstPoint = homePoint;
            firstPoint.z += 0.0;
            firstPoint.x += xRadius * cos(phi) * cos(theta);
            firstPoint.y += xRadius * cos(phi) * sin(theta) / yScaleFactor;

            secondPoint = homePoint;
            secondPoint.z += 0.0;
            secondPoint.x += xRadius * cos(phi) * cos(theta + dTheta);
            secondPoint.y += xRadius * cos(phi) * sin(theta + dTheta) / yScaleFactor;

            thirdPoint = homePoint;
            thirdPoint.z += xRadius * sin(phi + dPhi) / zScaleFactor;
            thirdPoint.x += xRadius * cos(phi + dPhi) * cos(theta + dTheta);
            thirdPoint.y += xRadius * cos(phi + dPhi) * sin(theta + dTheta) / yScaleFactor;

            fourthPoint = homePoint;
            fourthPoint.z += xRadius * sin(phi + dPhi) / zScaleFactor;
            fourthPoint.x += xRadius * cos(phi + dPhi) * cos(theta);
            fourthPoint.y += xRadius * cos(phi + dPhi) * sin(theta) / yScaleFactor;

            DRAW_PLANE

if ( fabs(dTheta) < 0.001 ) break;

         }
      }
      
      pIDataSet -> get(dl,&homePoint,&dl);

   }

   pIOpenGLImplementation -> EndSolids(segmentID);

   return 0;
   }