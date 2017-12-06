/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <math.h>

#include "utils.h"

#include "axis.h"

#include "list.cpp"

   char *nextLabel(char **pszLabels);

   int Axis::preparePlot() {

   IText *t;
   PlotViews plotView;
 
   pParentPropertyPlotView -> get_longValue((long*)&plotView);

   if ( plotView != gcPlotView3D && type == 'Z' ) {
#if 0
      DataPoint dp = {0.0,0.0,0.0};
      pIPlot -> ResetData();
      pIPlot -> TakeDataPoint(&dp);
      t = NULL;
      while ( t = textList.GetFirst() ) {
         textList.Remove(t);
         t -> Release();
      }
#endif
      return 0;
   }
 
   long nTicks,tickNumber,tickStyle,tickLengthUnits;
   double d,usedTickLength,gridSpace;
   double progress,step,cosPhi,cosTheta,sinPhi,sinTheta;
   double min,max,otherMin,otherMax,division;
   double maxError,foundError,answer;
   static double piover2 = 0.0;
   DataPoint dp,movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};
   int power = 0,precision = 0;
   DataPoint directionX = {1.0,0.0,0.0},directionY = {0.0,1.0,0.0};
   short originAtMinpoint,endpointAtMaxpoint;

   SAFEARRAYBOUND rgsa;
   double *pDirectionForwardData,*pDirectionUpData;
   rgsa.cElements = 3;
   rgsa.lLbound = 0;
   SAFEARRAY *pDirectionForward = SafeArrayCreate(VT_R8,1,&rgsa);
   SAFEARRAY *pDirectionUp = SafeArrayCreate(VT_R8,1,&rgsa);
   SafeArrayAccessData(pDirectionForward,reinterpret_cast<void **>(&pDirectionForwardData));
   SafeArrayAccessData(pDirectionUp,reinterpret_cast<void **>(&pDirectionUpData));

   if ( 0.0 == piover2 )
      piover2 = 2.0 * atan(1.0);

   propertyAxisPrecision -> get_doubleValue(&d);
   maxError = d / 100.0;
 
   pIPlot -> PrepareForData();

   IDataSet* pIDataSetPlot;
   boolean bIsIncluded;

   pIPlot -> get_DataSet(&pIDataSetPlot);
   pIDataSetDomain -> get_IsIncludedDomain(pIDataSetPlot,&bIsIncluded);
   if ( bIsIncluded )
      pIDataSetDomain -> RemoveIncludedDomain(pIDataSetPlot);

   pIDataSetDomain -> GetDomain(&minPoint,&maxPoint);
   if ( -DBL_MAX == minPoint.x || DBL_MAX == minPoint.x )
      return 0;
  
   char szVariable[1024];

   sprintf(szVariable,"Ox = %lf",minPoint.x);
   eval(pIEvaluator,szVariable);

   sprintf(szVariable,"Oy = %lf",minPoint.y);
   eval(pIEvaluator,szVariable);

   sprintf(szVariable,"Oz = %lf",minPoint.z);
   eval(pIEvaluator,szVariable);

   sprintf(szVariable,"Mx = %lf",maxPoint.x);
   eval(pIEvaluator,szVariable);

   sprintf(szVariable,"My = %lf",maxPoint.y);
   eval(pIEvaluator,szVariable);

   sprintf(szVariable,"Mz = %lf",maxPoint.z);
   eval(pIEvaluator,szVariable);

   char szAll[] = {"OX=Ox ox=Ox oX=Ox OY=Oy oy=Oy oY=Oy OZ=Oz oz=Oz oZ=Oz MX=Mx mx=Mx mX=Mx MY=My my=My mY=My MZ=Mz mz=Mz mZ=Mz "
                     "MinX=Ox MinY=Oy MinZ=Oz MaxX=Mx MaxY=My MaxZ=Mz"};
   char *pszTemp = szAll;
   while ( *pszTemp ) evalConsume(pIEvaluator,pszTemp);

   propertyOriginAtMinpoint -> get_boolValue(&originAtMinpoint);

   if ( ! originAtMinpoint ) {

      char *pszx,*pszy,*pszz;
      long n;

      propertyOriginXValue -> get_size(&n);
      pszx = new char[n + 1];
      propertyOriginXValue -> get_szValue(pszx);

      propertyOriginYValue -> get_size(&n);
      pszy = new char[n + 1];
      propertyOriginYValue -> get_szValue(pszy);

      propertyOriginZValue -> get_size(&n);
      pszz = new char[n + 1];
      propertyOriginZValue -> get_szValue(pszz);

      origin.x = eval(pIEvaluator,pszx);
      origin.y = eval(pIEvaluator,pszy);
      origin.z = eval(pIEvaluator,pszz);

      delete [] pszx;
      delete [] pszy;
      delete [] pszz;

   } else {

      origin.x = minPoint.x;
      origin.y = minPoint.y;
      origin.z = minPoint.z;
   }

   propertyEndpointAtMaxpoint -> get_boolValue(&endpointAtMaxpoint);

   if ( ! endpointAtMaxpoint ) {

      char *pszx,*pszy,*pszz;
      long n;

      propertyEndpointXValue -> get_size(&n);
      pszx = new char[n + 1];
      propertyEndpointXValue -> get_szValue(pszx);

      propertyEndpointYValue -> get_size(&n);
      pszy = new char[n + 1];
      propertyEndpointYValue -> get_szValue(pszy);

      propertyEndpointZValue -> get_size(&n);
      pszz = new char[n + 1];
      propertyEndpointZValue -> get_szValue(pszz);

      endPoint.x = eval(pIEvaluator,pszx);
      endPoint.y = eval(pIEvaluator,pszy);
      endPoint.z = eval(pIEvaluator,pszz);

      delete [] pszx;
      delete [] pszy;
      delete [] pszz;

   } else {

      switch ( type ) {
      case 'X':
         endPoint.x = maxPoint.x;
         endPoint.y = minPoint.y;
         endPoint.z = minPoint.z;
         break;
      case 'Y':
         endPoint.x = minPoint.x;
         endPoint.y = maxPoint.y;
         endPoint.z = minPoint.z;
         break;
      case 'Z':
         endPoint.x = minPoint.x;
         endPoint.y = minPoint.y;
         endPoint.z = maxPoint.z;
      }

   }
 
   uvDirection.x = endPoint.x - origin.x;
   uvDirection.y = endPoint.y - origin.y;
   uvDirection.z = endPoint.z - origin.z;

   unitPoint(&uvDirection,&uvDirection);

   pDirectionForwardData[0] = uvDirection.x;
   pDirectionForwardData[1] = uvDirection.y;
   pDirectionForwardData[2] = uvDirection.z;

   if ( 0.0 != uvDirection.x ) {

      if ( 1.0 == uvDirection.x ) {

         theta = 1.0;
         cosTheta = 1.0;
         sinTheta = 0.0;

      } else {

         theta = atan(uvDirection.y / uvDirection.x);

         if ( uvDirection.x < 0.0 && uvDirection.y > 0.0 ) {
            theta = 2.0 * piover2 + theta;
         } else {
            if ( uvDirection.x < 0.0 && uvDirection.y < 0.0 )
               theta = 2.0 * piover2 - theta;
            else {
               if ( uvDirection.x > 0.0 && uvDirection. y < 0.0 )
                  theta += 4.0 * piover2;
            }
         }

         cosTheta = cos(theta);
         sinTheta = sin(theta);
      }

   } else {

      theta = piover2;
      cosTheta = 0.0;
      sinTheta = 1.0;

   }

   pDirectionUpData[0] = uvDirection.x * cos(theta) - uvDirection.y * sin(theta);   // This is just a simple rotation about 'Z'
   pDirectionUpData[1] = uvDirection.x * sin(theta) + uvDirection.y * cos(theta);   // which may not always be correct
   pDirectionUpData[2] = uvDirection.z;

   if ( uvDirection.x == 0.0 && uvDirection.y == 0.0 ) {
      phi = piover2;
      cosPhi = 0.0;
      sinPhi = 1.0;
   } else {
      phi = atan(uvDirection.z/sqrt(uvDirection.x*uvDirection.x + uvDirection.y*uvDirection.y));
      cosPhi = cos(phi);
      sinPhi = sin(phi);
   }

   switch ( type ) {
   case 'X':
      min = origin.x;
      max = endPoint.x;
      otherMin = minPoint.y;
      otherMax = maxPoint.y;
      if ( determinesDomain ) {
         pIDataSetDomain -> put_minX(origin.x);
         pIDataSetDomain -> put_maxX(endPoint.x);
      }
      break;
 
   case 'Y':
      min = origin.y;
      max = endPoint.y;
      otherMin = minPoint.x;
      otherMax = maxPoint.x;
      if ( determinesDomain ) {
         pIDataSetDomain -> put_minY(origin.y);
         pIDataSetDomain -> put_maxY(endPoint.y);
      }
      break;
 
   case 'Z':
      min = origin.z;
      max = endPoint.z;
      otherMin = origin.x;
      otherMax = endPoint.x;
      if ( determinesDomain ) {
         pIDataSetDomain -> put_minZ(origin.z);
         pIDataSetDomain -> put_maxZ(endPoint.z);
      }
      break;
 
   }          

   if ( min == max ) max = 1.1 * min;

   if ( max == min ) {
      DataPoint dp = {0.0,0.0,0.0};
      pIPlot -> ResetData();
      pIPlot -> TakeDataPoint(&dp);
      t = NULL;
      while ( t = textList.GetFirst() ) {
         textList.Remove(t);
         t -> Release();
      }
      return 0;
   }

   if ( tickCount ) {
      division = (max - min) / (double)tickCount;
   } else
      division = (max - min);
 
   if ( (max - min) / division < 0.0 ) division = -division;
 
   if ( gridLinesPerTick )
      gridSpace = division / gridLinesPerTick;

   usedTickLength = tickLength;
   propertyTickLengthUnits -> get_longValue(&tickLengthUnits);
   switch ( tickLengthUnits ) {
   case UNIT_PERCENT:
      usedTickLength /= 100.0;
      break;
 
   default:
      break;
 
   }
 
   propertyTickStraddleStyle -> get_longValue(&tickStyle);
 
   if ( tickStyle == TICK_STRADDLE_BOTH) {
      propertyTickStraddleAbove -> get_doubleValue(&d);
      tickAbove = (otherMax - otherMin) * usedTickLength * d / 100.0;
      tickBelow = (otherMax - otherMin) * usedTickLength * (100.0 - d) / 100.0;
   } else {
      if ( tickStyle == TICK_STRADDLE_ABOVE) {
         tickAbove = (otherMax - otherMin) * usedTickLength;
         tickBelow = 0.0;
      } else {
         if ( tickStyle == TICK_STRADDLE_BELOW ) {
            tickAbove = 0.0;
            tickBelow = (otherMax - otherMin) * usedTickLength;
         } else {
            tickAbove = 0.0;
            tickBelow = 0.0;
         }
      }
   }
 
   if ( tickLengthUnits == TICK_UNITS_DATA ) {
      tickAbove /= (otherMax - otherMin);
      tickBelow /= (otherMax - otherMin);
   }
 
   nTicks = (long)((max - min) / division);
   if ( nTicks > 50 ) {
      nTicks = 50;
      division = (max - min) / 50.0;
   }

   t = NULL;
   while ( t = textList.GetFirst() ) {
      t -> Release();
      textList.Remove(t);
   }
 
   char *pszOriginalLabels = (char *)NULL,*pszLabels = (char *)NULL,*pszNextLabel = (char *)NULL;
   long cbLabels = 0;
   BSTR bstrLabels = NULL;
   bstrLabels = SysAllocStringLen(NULL,1024);
   pRepresentativeText -> get_Text(&bstrLabels);
   if ( wcslen(bstrLabels) > 0 ) {
      pszLabels = new char[cbLabels = (wcslen(bstrLabels) + 1)];
      pszOriginalLabels = pszLabels;
      memset(pszLabels,0,cbLabels);
      memset(pszOriginalLabels,0,cbLabels);
      WideCharToMultiByte(CP_ACP,0,bstrLabels,-1,pszLabels,cbLabels,0,0);
      strcpy(pszOriginalLabels,pszLabels);
   }
   if ( bstrLabels )
      SysFreeString(bstrLabels);

   pIPlot -> TakeDataPoint(&origin);
   pIPlot -> TakeDataPoint(&endPoint);

   tickNumber = 0;

   switch ( type ) {
 
   case 'X':

      if ( cosTheta != 0.0 && ( drawTickLabels || tickCount || gridLinesPerTick ) ) {
 
         step = division / cosPhi / cosTheta;
         gridSpace = gridSpace / cosPhi / cosTheta;

         dp.z = origin.z;

         progress = -step;

         for ( tickNumber = 1; tickNumber <= nTicks + 1; tickNumber++ ) {
 
            progress += step;

            double xAtAxis = origin.x + progress * cosPhi * cosTheta;
            double yAtAxis = origin.y + progress * cosPhi * sinTheta;

            if ( tickCount ) {
               pIPlot -> TakeDataPoint(&movePoint);
               dp.x = xAtAxis + tickBelow * cosPhi * sinTheta;
               dp.y = yAtAxis - tickBelow * cosPhi * cosTheta;
               pIPlot -> TakeDataPoint(&dp);
               dp.x = xAtAxis - tickAbove * cosPhi * sinTheta;
               dp.y = yAtAxis + tickAbove * cosPhi * cosTheta;
               pIPlot -> TakeDataPoint(&dp);
            }
 
            if ( drawTickLabels ) {
 
               dp.x = xAtAxis + (tickBelow/* + usedTickLength/10.0*/) * cosPhi * sinTheta; // <-- It is not clear why this "10%" was being used              
               dp.y = yAtAxis - (tickBelow/* + usedTickLength/10.0*/) * cosPhi * cosTheta; //     but it was causing problems when domain width < 1, or small
 
               if ( ! ( t = textList.Get(tickNumber) ) ) {
                  CoCreateInstance(CLSID_Text,pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IText,reinterpret_cast<void **>(&t));
                  textList.Add(t,NULL,tickNumber);
               }
 
               t -> Initialize(hwndOwner,pIOpenGLImplementation,pIEvaluator,pIDataSetDomain,pParentPropertyFloor,pParentPropertyCeiling,pParentPropertyOpenGLText,NULL,NULL);

               t -> CopyFrom(pRepresentativeText);

               BSTR bstrAxisValue = NULL;
               if ( pszOriginalLabels ) {
                  pszNextLabel = nextLabel(&pszLabels);
                  if ( ! pszNextLabel ) break;
                  long n = strlen(pszNextLabel) + 1;
                  bstrAxisValue = SysAllocStringLen(NULL,n);
                  MultiByteToWideChar(CP_ACP,0,pszNextLabel,-1,bstrAxisValue,n);
               } else {
                  bstrAxisValue = axisValue(xAtAxis,max - min,&precision,maxError,&foundError,&answer,FALSE,&power);
               }

               t -> put_Text(bstrAxisValue);
               t -> TextColorProperty(propertyTickLabelColor);
               t -> put_PositionX(dp.x);
               t -> put_PositionY(dp.y);
               t -> put_PositionZ(dp.z);
               t -> put_CoordinatePlane(CoordinatePlane_XY);
               t -> put_Format(TEXT_FORMAT_CENTER | TEXT_COORDINATES_FROM_TOP);
               t -> put_PartOfWorldDomain(TRUE);
               t -> put_DirectionForward(pDirectionForward);
               t -> put_DirectionUp(pDirectionUp);

               SysFreeString(bstrAxisValue);

            }
 
            if ( gridLinesPerTick && tickNumber < nTicks + 1 ) {

               double oldProgress = progress;

               DataPoint dpClipPlaneLowerLeft = {min(origin.x,endPoint.x),min(otherMin,otherMax),0.0};
               DataPoint dpClipPlaneUpperRight = {max(origin.x,endPoint.x),max(otherMin,otherMax),0.0};

               for ( long gridNumber = 0; gridNumber <= gridLinesPerTick; gridNumber++ ) {

                  DataPoint dpStart = {0.0,0.0,dp.z};
                  DataPoint dpEnd = {0.0,0.0,dp.z};

                  dpStart.y = otherMin;
                  dpStart.x = xAtAxis + (yAtAxis - dpStart.y) * sinTheta / cosTheta;
                  dpEnd.y = otherMax;
                  dpEnd.x = xAtAxis + (yAtAxis - dpEnd.y) * sinTheta / cosTheta;

                  if ( S_OK == pIOpenGLImplementation -> ClipPlane(&dpStart,&dpEnd,&dpClipPlaneLowerLeft,&dpClipPlaneUpperRight) ) {
                     pIPlot -> TakeDataPoint(&movePoint);
                     pIPlot -> TakeDataPoint(&dpStart);
                     pIPlot -> TakeDataPoint(&dpEnd);
                  }

                  progress += gridSpace;

                  xAtAxis = origin.x + progress * cosPhi * cosTheta;
                  yAtAxis = origin.y + progress * cosPhi * sinTheta;

               }

               progress = oldProgress;

            }

         }

         if ( gridLinesPerTick ) {
            pIPlot -> TakeDataPoint(&movePoint);
            dp.x = origin.x;
            dp.y = otherMax;
            pIPlot -> TakeDataPoint(&dp);
            dp.x = origin.x + progress * cosPhi * cosTheta;
            pIPlot -> TakeDataPoint(&dp);

            pIPlot -> TakeDataPoint(&movePoint);
            dp.y = otherMin;
            pIPlot -> TakeDataPoint(&dp);
            dp.x = origin.x;
            pIPlot -> TakeDataPoint(&dp);
         }

      }

      break;
 
   case 'Y':

      if ( sinTheta != 0.0 && (drawTickLabels || tickCount || gridLinesPerTick) ) {
 
         step = division / cosPhi / sinTheta;
         gridSpace = gridSpace / cosPhi / sinTheta;

         dp.z = origin.z;

         progress = -step;

         for ( tickNumber = 1; tickNumber <= nTicks + 1; tickNumber++ ) {
 
            progress += step;
 
            double xAtAxis = origin.x + progress * cosPhi * cosTheta;
            double yAtAxis = origin.y + progress * cosPhi * sinTheta;

            if ( tickCount ) {
               pIPlot -> TakeDataPoint(&movePoint);
               dp.x = xAtAxis + tickBelow * cosPhi * sinTheta;
               dp.y = yAtAxis - tickBelow * cosPhi * cosTheta;
               pIPlot -> TakeDataPoint(&dp);
               dp.x = xAtAxis - tickAbove * cosPhi * sinTheta;
               dp.y = yAtAxis + tickAbove * cosPhi * cosTheta;
               pIPlot -> TakeDataPoint(&dp);
            }
    
            if ( drawTickLabels ) {
 
               dp.x = xAtAxis - (tickAbove/* + usedTickLength/10.0*/) * cosPhi * sinTheta; // <-- It is not clear why this "10%" was being used
               dp.y = yAtAxis - (tickAbove/* + usedTickLength/10.0*/) * cosPhi * cosTheta; //     but it was causing problems when domain width < 1, or small
 
               if ( ! ( t = textList.Get(tickNumber) ) ) {
                  CoCreateInstance(CLSID_Text,
                                   pIUnknownOuter,
                                   CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                                   IID_IText,
                                   reinterpret_cast<void **>(&t));
                  textList.Add(t,NULL,tickNumber);
               }
 
               t -> Initialize(hwndOwner,pIOpenGLImplementation,pIEvaluator,pIDataSetDomain,pParentPropertyFloor,pParentPropertyCeiling,pParentPropertyOpenGLText,NULL,NULL);

               t -> CopyFrom(pRepresentativeText);

               BSTR bstrAxisValue;
               if ( pszOriginalLabels ) {
                  pszNextLabel = nextLabel(&pszLabels);
                  if ( ! pszNextLabel ) break;
                  long n = strlen(pszNextLabel) + 1;
                  bstrAxisValue = SysAllocStringLen(NULL,n);
                  MultiByteToWideChar(CP_ACP,0,pszNextLabel,-1,bstrAxisValue,n);
               } else {
                  bstrAxisValue = axisValue(yAtAxis,max - min,&precision,maxError,&foundError,&answer,FALSE,&power);
               }

               t -> put_Text(bstrAxisValue);
               t -> TextColorProperty(propertyTickLabelColor);

               t -> put_PositionX(dp.x);
               t -> put_PositionY(dp.y);
               t -> put_PositionZ(dp.z);
               t -> put_CoordinatePlane(CoordinatePlane_YX);
               t -> put_Format(TEXT_FORMAT_CENTER | TEXT_COORDINATES_FROM_BOTTOM);
               t -> put_PartOfWorldDomain(TRUE);
               t -> put_DirectionForward(pDirectionForward);
               t -> put_DirectionUp(pDirectionUp);

               SysFreeString(bstrAxisValue);
 
            }
 
            if ( tickNumber < nTicks + 1 && gridLinesPerTick ) {

               double oldProgress = progress;

               DataPoint dpClipPlaneLowerLeft = {min(otherMin,otherMax),min(origin.y,endPoint.y),0.0};
               DataPoint dpClipPlaneUpperRight = {max(otherMin,otherMax),max(origin.y,endPoint.y),0.0};

               for ( long gridNumber = 0; gridNumber <= gridLinesPerTick; gridNumber++ ) {

                  DataPoint dpStart = {0.0,0.0,dp.z};
                  DataPoint dpEnd = {0.0,0.0,dp.z};

                  dpStart.x = otherMin;
                  dpStart.y = yAtAxis + (xAtAxis - dpStart.x) * cosTheta / sinTheta;
                  dpEnd.x = otherMax;
                  dpEnd.y = yAtAxis + (xAtAxis - dpEnd.x) * cosTheta / sinTheta;

                  if ( S_OK == pIOpenGLImplementation -> ClipPlane(&dpStart,&dpEnd,&dpClipPlaneLowerLeft,&dpClipPlaneUpperRight) ) {
                     pIPlot -> TakeDataPoint(&movePoint);
                     pIPlot -> TakeDataPoint(&dpStart);
                     pIPlot -> TakeDataPoint(&dpEnd);
                  }

                  progress += gridSpace;

                  xAtAxis = origin.x + progress * cosPhi * cosTheta;
                  yAtAxis = origin.y + progress * cosPhi * sinTheta;

               }

               progress = oldProgress;

            }
 
         }

         if ( gridLinesPerTick ) {
            pIPlot -> TakeDataPoint(&movePoint);
            dp.x = otherMax;
            dp.y = origin.y;
            pIPlot -> TakeDataPoint(&dp);
            dp.y = origin.y + progress * cosPhi * sinTheta;
            pIPlot -> TakeDataPoint(&dp);
            pIPlot -> TakeDataPoint(&movePoint);
            dp.x = otherMin;
            pIPlot -> TakeDataPoint(&dp);
            dp.y = origin.y;            
            pIPlot -> TakeDataPoint(&dp);
         }

      }
 
      break;
 
   case 'Z':
 
      if ( drawTickLabels || tickCount || gridLinesPerTick ) {

         step = division / sinPhi;
         gridSpace = gridSpace / sinPhi;

         progress = -step;

         for ( tickNumber = 1; tickNumber <= nTicks + 1; tickNumber++ ) {

            progress += step;

            if ( tickCount ) {
               pIPlot -> TakeDataPoint(&movePoint);
               dp.x = origin.x + progress * cosPhi * cosTheta - tickBelow;
               dp.y = origin.y + progress * cosPhi * sinTheta;
               dp.z = origin.z + progress * sinPhi;
               pIPlot -> TakeDataPoint(&dp);
               dp.x += tickBelow + tickAbove;
               pIPlot -> TakeDataPoint(&dp);
               pIPlot -> TakeDataPoint(&movePoint);
               dp.x -= tickAbove;
               dp.y -= tickBelow;
               pIPlot -> TakeDataPoint(&dp);
               dp.y += tickBelow + tickAbove;
               pIPlot -> TakeDataPoint(&dp);
            }
       
            if ( drawTickLabels ) {
  
               dp.x = origin.x + progress * cosPhi * cosTheta;
               dp.y = origin.y + progress * cosPhi * sinTheta;
               dp.z = origin.z + progress * sinPhi;
 
               if ( ! ( t = textList.Get(tickNumber) ) ) {
                  CoCreateInstance(CLSID_Text,
                                   pIUnknownOuter,
                                   CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                                   IID_IText,
                                   reinterpret_cast<void **>(&t));
                  textList.Add(t,NULL,tickNumber);
               }
 
               t -> Initialize(hwndOwner,pIOpenGLImplementation,pIEvaluator,pIDataSetDomain,pParentPropertyFloor,pParentPropertyCeiling,pParentPropertyOpenGLText,NULL,NULL);

               t -> CopyFrom(pRepresentativeText);

               BSTR bstrAxisValue;
               if ( pszOriginalLabels ) {
                  pszNextLabel = nextLabel(&pszLabels);
                  if ( ! pszNextLabel ) break;
                  long n = strlen(pszNextLabel) + 1;
                  bstrAxisValue = SysAllocStringLen(NULL,n);
                  MultiByteToWideChar(CP_ACP,0,pszNextLabel,-1,bstrAxisValue,n);
               } else {
                  bstrAxisValue = axisValue(progress,max - min,&precision,maxError,&foundError,&answer,FALSE,&power);
               }

               t -> put_Text(bstrAxisValue);
               t -> TextColorProperty(propertyTickLabelColor);
               SysFreeString(bstrAxisValue);
               t -> put_PositionX(dp.x);
               t -> put_PositionY(dp.y);
               t -> put_PositionZ(dp.z);
               t -> put_CoordinatePlane(CoordinatePlane_XY);
               t -> put_Format(TEXT_FORMAT_RIGHT);
               t -> put_PartOfWorldDomain(TRUE);
               t -> put_DirectionForward(pDirectionForward);
               t -> put_DirectionUp(pDirectionUp);
 
            }

            if ( tickNumber < nTicks + 1 && gridLinesPerTick ) for ( long gridNumber = 0; gridNumber <= gridLinesPerTick; gridNumber++ ) {

               pIPlot -> TakeDataPoint(&movePoint);

               dp.x = minPoint.x;
               dp.y = origin.y;
               dp.z = origin.z + (progress + (double)gridNumber * gridSpace ) * sinPhi;

               pIPlot -> TakeDataPoint(&dp);

               dp.x = maxPoint.x;
               pIPlot -> TakeDataPoint(&dp);

               pIPlot -> TakeDataPoint(&movePoint);

               dp.x = origin.x;
               dp.y = minPoint.y;

               pIPlot -> TakeDataPoint(&dp);

               dp.y = maxPoint.y;

               pIPlot -> TakeDataPoint(&dp);
            }

         }

         if ( gridLinesPerTick ) {

            pIPlot -> TakeDataPoint(&movePoint);
            dp.x = maxPoint.x;
            dp.y = origin.y;
            dp.z = origin.z;
            pIPlot -> TakeDataPoint(&dp);
            dp.z = origin.y + progress * sinPhi;
            pIPlot -> TakeDataPoint(&dp);

            pIPlot -> TakeDataPoint(&movePoint);
            dp.x = origin.x;
            dp.y = maxPoint.y;
            dp.z = origin.z;
            pIPlot -> TakeDataPoint(&dp);
            dp.z = origin.z + progress * sinPhi;
            pIPlot -> TakeDataPoint(&dp);

         }

      }
 
      break;

   }
 
   for ( int k = tickNumber + 1; k <= textList.Count(); k++ ) {
      if ( t = textList.Get(k) ) {
         textList.Remove(t);
         t -> Release();
      }
   }

#if 1
   if ( bIsIncluded )
      pIDataSetDomain -> IncludeDomain(pIDataSetPlot);
#endif

   if ( pszOriginalLabels ) 
      delete [] pszOriginalLabels;

   SafeArrayDestroy(pDirectionForward);

   return 0;
   }


   char *nextLabel(char **ppszLabels) {

   char *pszLabels = *ppszLabels;
   char *pszNextLabel = strtok(pszLabels,",");

   if ( ! pszNextLabel ) return (char *)NULL;

   long n = strlen(pszNextLabel) - 1;

   pszLabels = pszNextLabel + n + 2;

   if ( pszNextLabel[n] == '\\' ) {

      pszLabels = pszNextLabel;
      char *pszNextLabelKeep = pszNextLabel;

      List<long> commaPositions;

      while ( pszNextLabel[n] == '\\' ) {

         commaPositions.Add(new long(n));

         char *pszTemp = new char[strlen(pszNextLabel + n + 2)];

         memset(pszTemp,0,strlen(pszNextLabel + n + 2));

         memcpy(pszTemp,pszNextLabel + n + 2,strlen(pszNextLabel + n + 2));

         memcpy(pszNextLabel + n + 1,pszTemp,strlen(pszNextLabel + n + 2));

         pszNextLabel[n + 1 + strlen(pszNextLabel + n + 2)] = '\0';

         delete [] pszTemp;

         pszNextLabel[n] = ' ';

         pszNextLabel = strtok(pszLabels,",");

         n = strlen(pszNextLabel) - 1;

         pszLabels = pszNextLabel + n + 2;

      }
   
      pszNextLabel = pszNextLabelKeep;

      long *pn = (long *)NULL;
      while ( pn = commaPositions.GetFirst() ) {
         pszNextLabel[*pn] = ',';
         commaPositions.Remove(pn);
         delete pn;
      }

   }

   *ppszLabels = pszLabels;
   return pszNextLabel;
   }