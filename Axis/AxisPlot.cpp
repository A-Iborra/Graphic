// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "axis.h"

#include "utils.h"

#include "list.cpp"

   char *nextLabel(char **pszLabels);

   int Axis::preparePlot() {

   IText *pText = NULL;
   while ( pText = textList.GetFirst() ) {
      pText -> Release();
      textList.Remove(pText);
   }
 
   if ( isHidden )
      return 0;

   PlotViews plotView;
 
   pParentPropertyPlotView -> get_longValue((long*)&plotView);

   if ( plotView != gcPlotView3D && type == 'Z' )
      return 0;
 
   long nTicks,tickNumber,tickStyle,tickLengthUnits;
   double axisPrecision,usedTickLength,gridSpace;
   double progress,step,cosPhi,cosTheta,sinPhi,sinTheta;
   double min,max,primaryOtherMin,primaryOtherMax,division;
   double secondaryOtherMin,secondaryOtherMax;

   double maxError,foundError,answer;
   static double piover2 = 0.0;
   DataPoint dp,movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};
   int power = 0,precision = 0;

   short originAtMinpoint,endpointAtMaxpoint;

   if ( 0.0 == piover2 )
      piover2 = 2.0 * atan(1.0);

   propertyAxisPrecision -> get_doubleValue(&axisPrecision);

   maxError = axisPrecision / 100.0;
 
   pIPlot -> PrepareForData();

   IDataSet* pIDataSetPlot;
   boolean bIsIncluded;

   pIPlot -> get_IDataSet(&pIDataSetPlot);

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
   while ( *pszTemp ) 
      evalConsume(pIEvaluator,pszTemp);

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

   theta = radiansFromXY(uvDirection.x,uvDirection.y);

   cosTheta = cos(theta);
   sinTheta = sin(theta);

   if ( 0.0 == uvDirection.x && 0.0 == uvDirection.y ) {
      phi = piover2;
      cosPhi = 0.0;
      sinPhi = 1.0;
      theta = 0.0;
      cosTheta = 0.0;
      sinTheta = 0.0;
   } else {
      phi = atan(uvDirection.z/sqrt(uvDirection.x*uvDirection.x + uvDirection.y*uvDirection.y));
      cosPhi = cos(phi);
      sinPhi = sin(phi);
   }

   switch ( type ) {
   case 'X':
      min = origin.x;
      max = endPoint.x;
      primaryOtherMin = minPoint.y;
      primaryOtherMax = maxPoint.y;
      secondaryOtherMin = minPoint.z;
      secondaryOtherMax = maxPoint.z;
      if ( determinesDomain ) {
         pIDataSetDomain -> put_minX(origin.x);
         pIDataSetDomain -> put_maxX(endPoint.x);
      }
      break;
 
   case 'Y':
      min = origin.y;
      max = endPoint.y;
      primaryOtherMin = minPoint.x;
      primaryOtherMax = maxPoint.x;
      secondaryOtherMin = minPoint.z;
      secondaryOtherMax = maxPoint.z;
      if ( determinesDomain ) {
         pIDataSetDomain -> put_minY(origin.y);
         pIDataSetDomain -> put_maxY(endPoint.y);
      }
      break;
 
   case 'Z':
      min = origin.z;
      max = endPoint.z;
      primaryOtherMin = minPoint.x;
      primaryOtherMax = maxPoint.x;
      secondaryOtherMin = minPoint.y;
      secondaryOtherMax = maxPoint.y;
      if ( determinesDomain ) {
         pIDataSetDomain -> put_minZ(origin.z);
         pIDataSetDomain -> put_maxZ(endPoint.z);
      }
      break;
 
   }          

   if ( min == max ) 
      max = 1.1 * min;

   if ( max == min ) {
      DataPoint dp = {0.0,0.0,0.0};
      pIPlot -> ResetData();
      pIPlot -> TakeDataPoint(&dp);
      pText = NULL;
      while ( pText = textList.GetFirst() ) {
         textList.Remove(pText);
         pText -> Release();
      }
      return 0;
   }

   if ( tickCount ) {
      division = (max - min) / (double)tickCount;
   } else
      division = (max - min);
 
   if ( (max - min) / division < 0.0 ) 
      division = -division;
 
   if ( gridLinesPerTick )
      gridSpace = division / gridLinesPerTick;

   usedTickLength = tickLength;

   propertyTickLengthUnits -> get_longValue(&tickLengthUnits);

   switch ( tickLengthUnits ) {

   case UNIT_PERCENT:
      usedTickLength /= 100.0;
      break;

   case UNIT_PIXEL: {

      DataPoint dpConvert1,dpConvert2,dpConvert3,dpConvert4;
      dpConvert1.x = 0.0;
      dpConvert1.y = 0.0;
      dpConvert1.z = 0.0;
      dpConvert2.x = 0.0;
      dpConvert2.y = 0.0;
      dpConvert2.z = 0.0;

      switch ( type ) {
      case 'X':
         dpConvert2.y = usedTickLength;
         break;

      case 'Y': 
         dpConvert2.x = usedTickLength;
         break;

      case 'Z': 
         dpConvert2.x = usedTickLength;
         break;

      }

      pIOpenGLImplementation -> WindowToData(&dpConvert1,&dpConvert3);
      pIOpenGLImplementation -> WindowToData(&dpConvert2,&dpConvert4);

      switch ( type ) {
      case 'X':
         usedTickLength = dpConvert4.y - dpConvert3.y;
         break;

      case 'Y': 
         usedTickLength = dpConvert4.x - dpConvert3.x;
         break;

      case 'Z': 
         usedTickLength = dpConvert4.x - dpConvert3.x;
         break;

      }

      }
      break;

   default:
      break;
 
   }

   propertyTickStraddleStyle -> get_longValue(&tickStyle);
 
   double secondaryTickAbove,secondaryTickBelow;

   if ( tickStyle == TICK_STRADDLE_BOTH) {
      double dblVal;
      propertyTickStraddleAbove -> get_doubleValue(&dblVal);
      tickAbove = (primaryOtherMax - primaryOtherMin) * usedTickLength * dblVal / 100.0;
      tickBelow = (primaryOtherMax - primaryOtherMin) * usedTickLength * (100.0 - dblVal) / 100.0;
      secondaryTickAbove = (secondaryOtherMax - secondaryOtherMin) * usedTickLength * dblVal / 100.0;
      secondaryTickBelow = (secondaryOtherMax - secondaryOtherMin) * usedTickLength * (100.0 - dblVal) / 100.0;
   } else {
      if ( tickStyle == TICK_STRADDLE_ABOVE) {
         tickAbove = (primaryOtherMax - primaryOtherMin) * usedTickLength;
         tickBelow = 0.0;
         secondaryTickAbove = (secondaryOtherMax - secondaryOtherMin) * usedTickLength;
         secondaryTickBelow = 0.0;
      } else {
         if ( tickStyle == TICK_STRADDLE_BELOW ) {
            tickAbove = 0.0;
            tickBelow = (primaryOtherMax - primaryOtherMin) * usedTickLength;
            secondaryTickAbove = 0.0;
            secondaryTickBelow = (secondaryOtherMax - secondaryOtherMin) * usedTickLength;
         } else {
            tickAbove = 0.0;
            tickBelow = 0.0;
         }
      }
   }
 
   if ( UNIT_PIXEL == tickLengthUnits ) {
      tickAbove /= (primaryOtherMax - primaryOtherMin);
      tickBelow /= (primaryOtherMax - primaryOtherMin);
      secondaryTickAbove /= (secondaryOtherMax - secondaryOtherMin);
      secondaryTickBelow /= (secondaryOtherMax - secondaryOtherMin);
   }
 
   nTicks = (long)((max - min) / division);
   if ( nTicks > 50 ) {
      nTicks = 50;
      division = (max - min) / 50.0;
   }

   char *pszOriginalLabels = (char *)NULL,*pszLabels = (char *)NULL,*pszNextLabel = (char *)NULL;
   long cbLabels = 0;
   BSTR bstrLabels = NULL;
   bstrLabels = SysAllocStringLen(NULL,1024);
   pRepresentativeText -> get_Text(&bstrLabels);
   if ( 0 < wcslen(bstrLabels) ) {
      pszLabels = new char[cbLabels = ((DWORD)wcslen(bstrLabels) + 1)];
      pszOriginalLabels = pszLabels;
      memset(pszLabels,0,cbLabels);
      memset(pszOriginalLabels,0,cbLabels);
      WideCharToMultiByte(CP_ACP,0,bstrLabels,-1,pszLabels,cbLabels,0,0);
      strcpy(pszOriginalLabels,pszLabels);
   }

   if ( bstrLabels )
      SysFreeString(bstrLabels);

   pRepresentativeText -> put_Size(tickLabelSize);
   pRepresentativeText -> put_SizeUnits((TextSizeUnits)tickLabelSizeUnits);

   pIPlot -> TakeDataPoint(&origin);
   pIPlot -> TakeDataPoint(&endPoint);

   tickNumber = 0;

   switch ( type ) {
 
   case 'X':

      if ( 0.0 == cosTheta )
         break;

      if ( ! drawTickLabels && ! tickCount && ! gridLinesPerTick )
         break;

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

            if ( ticksOnAllNormalPlanes ) {
               pIPlot -> TakeDataPoint(&movePoint);
               DataPoint dp2;
               dp2.x = xAtAxis;
               dp2.y = yAtAxis;
               dp2.z = origin.z + secondaryTickAbove;
               pIPlot -> TakeDataPoint(&dp2);
               dp2.z = origin.z - secondaryTickBelow;
               pIPlot -> TakeDataPoint(&dp2);
            }

         }
 
         if ( drawTickLabels ) {
 
            dp.x = xAtAxis + tickBelow * cosPhi * sinTheta;
            dp.y = yAtAxis - tickBelow * cosPhi * cosTheta;

            if ( ! ( pText = textList.Get(tickNumber) ) ) {
               CoCreateInstance(CLSID_Text,pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IText,reinterpret_cast<void **>(&pText));
               textList.Add(pText,NULL,tickNumber);
            }

            pText -> AdviseGSGraphicServices(reinterpret_cast<void *>(pIGSGraphicServices));

            pText -> Initialize(pIOpenGLImplementation,pIEvaluator,pIDataSetDomain,
                                 pParentPropertyXFloor,pParentPropertyXCeiling,
                                 pParentPropertyYFloor,pParentPropertyYCeiling,
                                 pParentPropertyZFloor,pParentPropertyZCeiling,
                                 NULL,NULL,pWhenChangedCallback,pWhenChangedCallbackArg,whenChangedCallbackCookie);

            pText -> AdviseGSGraphicServices(pIGSGraphicServices);

            pText -> CopyFrom(pRepresentativeText);

            BSTR bstrAxisValue = NULL;
            if ( pszOriginalLabels ) {
               pszNextLabel = nextLabel(&pszLabels);
               if ( ! pszNextLabel ) 
                  break;
               long n = (DWORD)strlen(pszNextLabel) + 1;
               bstrAxisValue = SysAllocStringLen(NULL,n);
               MultiByteToWideChar(CP_ACP,0,pszNextLabel,-1,bstrAxisValue,n);
            } else
               bstrAxisValue = axisValue(xAtAxis,max - min,&precision,maxError,&foundError,&answer,FALSE,&power);

            pText -> put_Text(bstrAxisValue);
            pText -> TextColorProperty(currentPropertyTickLabelColor);
            pText -> put_PositionX(dp.x);
            pText -> put_PositionY(dp.y);
            pText -> put_PositionZ(dp.z);

            SysFreeString(bstrAxisValue);

         }

         if ( gridLinesPerTick && tickNumber < nTicks + 1 ) {

            double oldProgress = progress;

            DataPoint dpClipPlaneLowerLeft = {min(origin.x,endPoint.x),min(primaryOtherMin,primaryOtherMax),0.0};
            DataPoint dpClipPlaneUpperRight = {max(origin.x,endPoint.x),max(primaryOtherMin,primaryOtherMax),0.0};

            for ( long gridNumber = 0; gridNumber <= gridLinesPerTick; gridNumber++ ) {

               DataPoint dpStart = {0.0,0.0,dp.z};
               DataPoint dpEnd = {0.0,0.0,dp.z};

               dpStart.y = primaryOtherMin;
               dpStart.x = xAtAxis + (yAtAxis - dpStart.y) * sinTheta / cosTheta;
               dpEnd.y = primaryOtherMax;
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
         dp.y = primaryOtherMax;
         pIPlot -> TakeDataPoint(&dp);
         dp.x = origin.x + progress * cosPhi * cosTheta;
         pIPlot -> TakeDataPoint(&dp);

         pIPlot -> TakeDataPoint(&movePoint);
         dp.y = primaryOtherMin;
         pIPlot -> TakeDataPoint(&dp);
         dp.x = origin.x;
         pIPlot -> TakeDataPoint(&dp);
      }

      break;
 
   case 'Y':

      if ( 0.0 == sinTheta )
         break;

      if ( ! drawTickLabels && ! tickCount && ! gridLinesPerTick )
         break;
 
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
            dp.x = xAtAxis + tickAbove * cosPhi * sinTheta;
            dp.y = yAtAxis - tickAbove * cosPhi * cosTheta;
            pIPlot -> TakeDataPoint(&dp);
            dp.x = xAtAxis - tickBelow * cosPhi * sinTheta;
            dp.y = yAtAxis + tickBelow * cosPhi * cosTheta;
            pIPlot -> TakeDataPoint(&dp);

            if ( ticksOnAllNormalPlanes ) {
               pIPlot -> TakeDataPoint(&movePoint);
               DataPoint dp2;
               dp2.x = xAtAxis;
               dp2.y = yAtAxis;
               dp2.z = origin.z + secondaryTickAbove;
               pIPlot -> TakeDataPoint(&dp2);
               dp2.z = origin.z - secondaryTickBelow;
               pIPlot -> TakeDataPoint(&dp2);
            }

         }

         if ( drawTickLabels ) {
 
            dp.x = xAtAxis - 1.10 * tickBelow * cosPhi * sinTheta;
            dp.y = yAtAxis + 1.10 * tickBelow * cosPhi * cosTheta;
 
            if ( ! ( pText = textList.Get(tickNumber) ) ) {
               CoCreateInstance(CLSID_Text,pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IText,reinterpret_cast<void **>(&pText));
               textList.Add(pText,NULL,tickNumber);
            }

            pText -> AdviseGSGraphicServices(reinterpret_cast<void *>(pIGSGraphicServices));

            pText -> Initialize(pIOpenGLImplementation,pIEvaluator,pIDataSetDomain,
                                 pParentPropertyXFloor,pParentPropertyXCeiling,
                                 pParentPropertyYFloor,pParentPropertyYCeiling,
                                 pParentPropertyZFloor,pParentPropertyZCeiling,
                                 NULL,NULL,pWhenChangedCallback,pWhenChangedCallbackArg,whenChangedCallbackCookie);

            pText -> CopyFrom(pRepresentativeText);

            BSTR bstrAxisValue;
            if ( pszOriginalLabels ) {
               pszNextLabel = nextLabel(&pszLabels);
               if ( ! pszNextLabel ) 
                  break;
               long n = (DWORD)strlen(pszNextLabel) + 1;
               bstrAxisValue = SysAllocStringLen(NULL,n);
               MultiByteToWideChar(CP_ACP,0,pszNextLabel,-1,bstrAxisValue,n);
            } else
               bstrAxisValue = axisValue(yAtAxis,max - min,&precision,maxError,&foundError,&answer,FALSE,&power);

            pText -> put_Text(bstrAxisValue);
            pText -> TextColorProperty(propertyTickLabelColor);
            pText -> put_PositionX(dp.x);
            pText -> put_PositionY(dp.y);
            pText -> put_PositionZ(dp.z);

            SysFreeString(bstrAxisValue);
 
         }

         if ( tickNumber < nTicks + 1 && gridLinesPerTick ) {

            double oldProgress = progress;

            DataPoint dpClipPlaneLowerLeft = {min(primaryOtherMin,primaryOtherMax),min(origin.y,endPoint.y),0.0};
            DataPoint dpClipPlaneUpperRight = {max(primaryOtherMin,primaryOtherMax),max(origin.y,endPoint.y),0.0};

            for ( long gridNumber = 0; gridNumber <= gridLinesPerTick; gridNumber++ ) {

               DataPoint dpStart = {0.0,0.0,dp.z};
               DataPoint dpEnd = {0.0,0.0,dp.z};

               dpStart.x = primaryOtherMin;
               dpStart.y = yAtAxis + (xAtAxis - dpStart.x) * cosTheta / sinTheta;
               dpEnd.x = primaryOtherMax;
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
         dp.x = primaryOtherMax;
         dp.y = origin.y;
         pIPlot -> TakeDataPoint(&dp);
         dp.y = origin.y + progress * cosPhi * sinTheta;
         pIPlot -> TakeDataPoint(&dp);
         pIPlot -> TakeDataPoint(&movePoint);
         dp.x = primaryOtherMin;
         pIPlot -> TakeDataPoint(&dp);
         dp.y = origin.y;            
         pIPlot -> TakeDataPoint(&dp);
      }

      break;
 
   case 'Z':
 
      if ( drawTickLabels || tickCount || gridLinesPerTick ) {

         step = division / sinPhi;
         gridSpace = gridSpace / sinPhi;

         progress = origin.z - step;

         for ( tickNumber = 1; tickNumber <= nTicks + 1; tickNumber++ ) {

            progress += step;

            double xAtAxis = origin.x;
            double yAtAxis = origin.y;

            if ( tickCount ) {

               pIPlot -> TakeDataPoint(&movePoint);

               dp.x = xAtAxis + tickBelow;
               dp.y = yAtAxis;
               dp.z = progress;
               pIPlot -> TakeDataPoint(&dp);

               dp.x = xAtAxis - tickAbove;
               dp.y = yAtAxis;
               pIPlot -> TakeDataPoint(&dp);

               if ( ticksOnAllNormalPlanes ) {
                  pIPlot -> TakeDataPoint(&movePoint);
                  dp.x = xAtAxis;
                  dp.y = yAtAxis + secondaryTickBelow;
                  pIPlot -> TakeDataPoint(&dp);
                  dp.x = xAtAxis;
                  dp.y = yAtAxis - secondaryTickAbove;
                  pIPlot -> TakeDataPoint(&dp);
               }

            }

            if ( drawTickLabels ) {
  
               dp.x = xAtAxis + 2.0 * tickBelow;
               dp.y = yAtAxis;
               dp.z = progress;
 
               if ( ! ( pText = textList.Get(tickNumber) ) ) {
                  CoCreateInstance(CLSID_Text,pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IText,reinterpret_cast<void **>(&pText));
                  textList.Add(pText,NULL,tickNumber);
               }

               pText -> AdviseGSGraphicServices(reinterpret_cast<void *>(pIGSGraphicServices));

               pText -> Initialize(pIOpenGLImplementation,pIEvaluator,pIDataSetDomain,
                                       pParentPropertyXFloor,pParentPropertyXCeiling,
                                       pParentPropertyYFloor,pParentPropertyYCeiling,
                                       pParentPropertyZFloor,pParentPropertyZCeiling,
                                       NULL,NULL,
                                       pWhenChangedCallback,pWhenChangedCallbackArg,whenChangedCallbackCookie);

               pText -> CopyFrom(pRepresentativeText);

               BSTR bstrAxisValue;
               if ( pszOriginalLabels ) {
                  pszNextLabel = nextLabel(&pszLabels);
                  if ( ! pszNextLabel )
                     break;
                  long n = (DWORD)strlen(pszNextLabel) + 1;
                  bstrAxisValue = SysAllocStringLen(NULL,n);
                  MultiByteToWideChar(CP_ACP,0,pszNextLabel,-1,bstrAxisValue,n);
               } else
                  bstrAxisValue = axisValue(progress,max - min,&precision,maxError,&foundError,&answer,FALSE,&power);

               pText -> put_Text(bstrAxisValue);
               pText -> TextColorProperty(propertyTickLabelColor);
               SysFreeString(bstrAxisValue);
               pText -> put_PositionX(dp.x);
               pText -> put_PositionY(dp.y);
               pText -> put_PositionZ(dp.z);
 
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
      if ( pText = textList.Get(k) ) {
         textList.Remove(pText);
         pText -> Release();
      }
   }

   if ( bIsIncluded )
      pIDataSetDomain -> IncludeDomain(pIDataSetPlot);

   if ( pszOriginalLabels ) 
      delete [] pszOriginalLabels;

   return 0;
   }


   char *nextLabel(char **ppszLabels) {

   char *pszLabels = *ppszLabels;
   char *pszNextLabel = strtok(pszLabels,",");

   if ( ! pszNextLabel ) return (char *)NULL;

   long n = (DWORD)strlen(pszNextLabel) - 1;

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

         n = (DWORD)strlen(pszNextLabel) - 1;

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