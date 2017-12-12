/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "general.h"
#include "utils.h"
#include "list.cpp"

#include "dataset.h"

   DataPoint movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};
 
   DataSet::DataSet(IUnknown *pUnk) : 
   pUnknownOuter(pUnk),
   pIEvaluator(NULL),
   otherDomains(),
   refCount(0),
   pBoundingBox(NULL),
   dataArity(DATA_ARITY_UNKNOWN),
   pPropertyFloor(NULL),
   pPropertyCeiling(NULL)

   {
   firstData = NULL;
   topData = NULL;
   gdiData = NULL;
   xMax = -DBL_MAX;
   yMax = -DBL_MAX;
   zMax = -DBL_MAX;
   xMin = DBL_MAX;
   yMin = DBL_MAX;
   zMin = DBL_MAX;
   pBoundingBox = new boundingBox(&xMin,&yMin,&zMin,&xMax,&yMax,&zMax);
   currentColor[0] = currentColor[1] = currentColor[2] = 0.0;
   memset(szEquations,0,sizeof(szEquations));
   return;
   }
 

   DataSet::~DataSet() {
   ReSet();
   //IDataSet* pOther;
   for ( IDataSet *pOther : otherDomains ) //while ( pOther = otherDomains.GetLast() ) {
      pOther -> Release();
      //otherDomains.Remove(pOther);
   //}
   otherDomains.clear();
   //extents *pExtents = extentsStack.GetLast();
   for ( extents *pExtents : extentsStack ) //while ( pExtents ) {
      //extentsStack.Remove(pExtents);
      delete pExtents;
   //}
   extentsStack.clear();
   delete pBoundingBox;
   if ( pIEvaluator )
      pIEvaluator -> Release();
   return;
   }
 

   int DataSet::resetLimits(const DataPoint& d) {

   int rv = ( (xMax < d.x) || (yMax < d.y) || (zMax < d.z) ||
              (xMin > d.x) || (yMin > d.y) || (zMin > d.z) ) ? TRUE : FALSE;

   xMax = (d.x > xMax) ? d.x : xMax;
   yMax = (d.y > yMax) ? d.y : yMax;
   zMax = (d.z > zMax) ? d.z : zMax;
   xMin = (d.x < xMin) ? d.x : xMin;
   yMin = (d.y < yMin) ? d.y : yMin;
   zMin = (d.z < zMin) ? d.z : zMin;

#if 1
   char szNumber[6][64];

   if ( -DBL_MAX == xMin || DBL_MAX == xMin/* || fabs(xMin - DBL_MAX) > 10.0 */)
      sprintf(szNumber[0],"INF");
   else
      sprintf(szNumber[0],"%lf",xMin);

   if ( DBL_MAX == xMax || -DBL_MAX == xMax/* || fabs(xMax - DBL_MAX) > 10.0 */)
      sprintf(szNumber[1],"INF");
   else
      sprintf(szNumber[1],"%lf",xMax);

   if ( -DBL_MAX == yMin || DBL_MAX == yMin/* || fabs(yMin - DBL_MAX) > 10.0 */)
      sprintf(szNumber[2],"INF");
   else
      sprintf(szNumber[2],"%lf",yMin);

   if ( DBL_MAX == yMax || -DBL_MAX == yMax/* || fabs(yMax - DBL_MAX) > 10.0 */)
      sprintf(szNumber[3],"INF");
   else
      sprintf(szNumber[3],"%lf",yMax);

   if ( -DBL_MAX == zMin || DBL_MAX == zMin/* || fabs(zMin - DBL_MAX) > 10.0 */)
      sprintf(szNumber[4],"INF");
   else
      sprintf(szNumber[4],"%lf",zMin);

   if ( DBL_MAX == zMax || -DBL_MAX == zMax/* || fabs(zMax - DBL_MAX) > 10.0 */)
      sprintf(szNumber[5],"INF");
   else
      sprintf(szNumber[5],"%lf",zMax);

   sprintf(szEquations,
            "xMin = %s,yMin = %s zMin = %s xMax = %s yMax = %s zMax = %s "
            "minX = %s,minY = %s minZ = %s maxX = %s maxY = %s maxZ = %s",
                     szNumber[0],szNumber[2],szNumber[4],szNumber[1],szNumber[3],szNumber[5],
                     szNumber[0],szNumber[2],szNumber[4],szNumber[1],szNumber[3],szNumber[5]);
#endif
   return rv;
   }
 
 
   int DataSet::resizeBounds() {
   adjustRange(&xMin,&xMax);
   adjustRange(&yMin,&yMax);
   adjustRange(&zMin,&zMax);
   return TRUE;
   }