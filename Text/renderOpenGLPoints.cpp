// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"

   static DataPoint movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};
   static POINT ptPoints[1024];
   static int contourPointsCount[32];
   static long pointCount;
   static long contourIndex;

   static HDC hdc;

   static double mapX(FIXED fx) {
   long lx = *(LONG *)&fx;
   return ((double)(lx) / 65536.0);
   }
 
   static double mapY(FIXED fy) {
   long ly = *(LONG *)&fy;
   return ((double)(ly) / 65536.0);
   }
 
   static FIXED doubleToFixed(double d) {
   long x;
   x = static_cast<long>(d * 65536L);
   return *(FIXED *)&x;
   }
 

   void Text::renderOpenGLPoints() {

   if ( ! propertyContent -> pointer() )
      return;

   hdc = pIOpenGLImplementation -> TargetDC();

   int oldPolyFillMode = SetPolyFillMode(hdc, ALTERNATE);

   float fv[4];
   BYTE *pb = (BYTE *)fv;

   propertyTextColor -> get_binaryValue(4 * sizeof(long),(BYTE **)&pb);

   BYTE vb[3];
   COLORREF cr;
   vb[0] = (BYTE)(255.0f*fv[0]);
   vb[1] = (BYTE)(255.0f*fv[1]);
   vb[2] = (BYTE)(255.0f*fv[2]);

   HGDIOBJ oldBrush = SelectObject(hdc,(HBRUSH)CreateSolidBrush(RGB(vb[0],vb[1],vb[2])));

   void (__stdcall *pActionFunction)(void *,void *) = NULL;

   DataList *pItem = NULL;

   pIDataSet -> peek(pItem,&pItem);

   while ( pItem ) {

      DataPoint dpGDI{0.0,0.0,0.0};

      if ( -DBL_MAX == pItem -> data.x ) {

         while ( pItem && -DBL_MAX == pItem -> data.x ) {
            if ( ! ( NULL == pItem -> pvAction ) ) {
               pActionFunction = (void (__stdcall *)(void *,void *))pItem -> pvAction;
               pActionFunction(pItem -> pvArg1,pItem -> pvArg2);
            }
            pIDataSet -> peek(pItem,&pItem);
         }

         if ( ! pItem )
            break;

         pIOpenGLImplementation -> DataToWindow(&pItem -> data,UNIT_PIXEL,&dpGDI);

         ptPoints[pointCount].x = dpGDI.x;
         ptPoints[pointCount].y = dpGDI.y;

         pointCount++;

         if ( ! ( NULL == pItem -> pvAction ) ) {
            pActionFunction = (void (__stdcall *)(void *,void *))pItem -> pvAction;
            pActionFunction(pItem -> pvArg1,pItem -> pvArg2);
         }

      }

      pIDataSet -> peek(pItem,&pItem);

      if ( ! pItem )
         break;

      if ( ! ( -DBL_MAX == pItem -> data.x ) ) {

         pIOpenGLImplementation -> DataToWindow(&pItem -> data,UNIT_PIXEL,&dpGDI);

         ptPoints[pointCount].x = dpGDI.x;
         ptPoints[pointCount].y = dpGDI.y;

         pointCount++;
      }

      if ( ! ( NULL == pItem -> pvAction ) ) {
         pActionFunction = (void (__stdcall *)(void *,void *))pItem -> pvAction;
         pActionFunction(pItem -> pvArg1,pItem -> pvArg2);
      }

   }

   SetPolyFillMode(hdc,oldPolyFillMode);

   DeleteObject(SelectObject(hdc,oldBrush));

   return;
   }

 
 
   long Text::renderText(HDC hdcp,char* theText) {

   HDC hdc = hdcp;

   if ( ! hdc ) 
      hdc = pIOpenGLImplementation -> TargetDC();

   if ( ! hFont ) {
      if ( ! szFace[0] )
         strcpy(szFace,DEFAULT_FONT);
      memset(&logicalFont,0,sizeof(LOGFONT));
      logicalFont.lfHeight = -MulDiv((long)fontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
      logicalFont.lfQuality = PROOF_QUALITY;
      logicalFont.lfWeight = FW_NORMAL;
      strcpy(logicalFont.lfFaceName,szFace);
      logicalFont.lfCharSet = DEFAULT_CHARSET;
      hFont = CreateFontIndirect(&logicalFont);
      hOriginalFont = (HFONT)SelectObject(hdc,hFont);
   }

   TEXTMETRIC fontMetrics;

   memset(&fontMetrics,0,sizeof(TEXTMETRIC));

   GetTextMetrics(hdc,&fontMetrics);

   int charWidth;
   char *pC;
   char *text;

   DataPoint startPoint = {0.0,0.0,0.0};

   pIDataSet -> ReSet();

   if ( ! theText ) {
      if ( ! propertyContent -> pointer() )
         return 0;
      long n = (DWORD)strlen((char *)propertyContent -> pointer()) + 1;
      text = new char[n];
      memset(text,0,n * sizeof(char));
      strcpy(text,(char *)propertyContent -> pointer());
   } else {
      long n = (DWORD)strlen(theText);
      text = new char[n + 1];
      strcpy(text,theText);
   }

   pC = text;
   while ( *pC && *pC > 0 ) {
      if ( (char)0x0A == *pC || (char)0x0D == *pC || '\n' == *pC ) {
         if ( *pC == (char)0x0D ) {
            pC++;
            if ( *pC == (char)0x0A ) 
               pC++;
         } else {
            if ( *pC == (char)0x0A ) {
               pC++;
               if ( *pC == (char)0X0D ) 
                  pC++;
            } else {
               pC++;
            }
         }
         startPoint.x = 0;
         startPoint.y -= fontMetrics.tmHeight;
      } else {
         renderGlyph(hdc,*pC,startPoint);
         GetCharWidth32(hdc,*pC,*pC,&charWidth);
         startPoint.x += charWidth;
         pC++;
      }
   }

   delete [] text;

   return 1;
   }
 
 
   long Text::renderGlyph(HDC hdc,char c,DataPoint& dpStart) {

   static MAT2 matrix = {0,0,0,0};
   GLYPHMETRICS glyphMetrics;
   long cbPolygon,cbCharacter;
   TTPOLYGONHEADER *pPolygonHeader;
   TTPOLYCURVE *pPolyCurve;
   double Ax,Ay,x,y,xText,yText,zText;
   long n;
   BYTE *b,*bInner;
   DataPoint dp;
 
   if ( 0 == matrix.eM11.value ) {
      matrix.eM11 = doubleToFixed(1.0);
      matrix.eM12 = doubleToFixed(0.0);
      matrix.eM21 = doubleToFixed(0.0);
      matrix.eM22 = doubleToFixed(1.0);
   }

   xText = dpStart.x;
   yText = dpStart.y;
   zText = dpStart.z;

   cbCharacter = GetGlyphOutline(hdc,c,GGO_NATIVE,&glyphMetrics,0,NULL,&matrix);

   if ( 0 > cbCharacter ) 
      return 0;

   b = new BYTE[cbCharacter];

   cbCharacter = GetGlyphOutline(hdc,c,GGO_NATIVE,&glyphMetrics,cbCharacter,b,&matrix);
 
   pPolygonHeader = reinterpret_cast<TTPOLYGONHEADER *>(b);
 
   dp.z = zText;

   pIDataSet -> pushDataPoint(&movePoint,renderTextNewGlyph,(void *)this,(void *)NULL);
 
   while ( 0 < cbCharacter ) {
 
      pPolyCurve = reinterpret_cast<TTPOLYCURVE *>(reinterpret_cast<BYTE *>(pPolygonHeader) + sizeof(TTPOLYGONHEADER));
 
      cbPolygon = (long)pPolygonHeader -> cb - sizeof(TTPOLYGONHEADER);
 
      Ax = mapX(pPolygonHeader -> pfxStart.x);
      Ay = mapY(pPolygonHeader -> pfxStart.y);
 
      dp.x = xText + Ax;
      dp.y = yText + Ay;
      pIDataSet -> pushDataPoint(&dp,renderTextNewContour,(void *)this,(void *)NULL);
 
      while ( 0 < cbPolygon ) {
 
         switch ( pPolyCurve -> wType ) {

         case TT_PRIM_LINE:
            for ( long k = 0; k < pPolyCurve -> cpfx; k++ ) {
               Ax = mapX(pPolyCurve -> apfx[k].x);
               Ay = mapY(pPolyCurve -> apfx[k].y);
               dp.x = xText + Ax;
               dp.y = yText + Ay;
               pIDataSet -> pushDataPoint(&dp);
            }
            break;
 
         case TT_PRIM_QSPLINE: {

            double Bx,Cx,By,Cy;
 
            dp.x = xText + Ax;
            dp.y = yText + Ay;

            pIDataSet -> pushDataPoint(&dp);

            for ( long k = 0; k < pPolyCurve -> cpfx - 1; k++ ) {

               Bx = mapX(pPolyCurve -> apfx[k].x);
               By = mapY(pPolyCurve -> apfx[k].y);

               if ( k < pPolyCurve -> cpfx - 2 ) {
                  Cx = (Bx + mapX(pPolyCurve -> apfx[k + 1].x)) / 2.0;
                  Cy = (By + mapY(pPolyCurve -> apfx[k + 1].y)) / 2.0;
               } else {
                  Cx = mapX(pPolyCurve -> apfx[k + 1].x);
                  Cy = mapY(pPolyCurve -> apfx[k + 1].y);
               }

               for ( double t = 0.0; t <= 1.0; t += 0.25 ) {
                  x = (Ax - 2.0*Bx + Cx) * t*t + (2.0 * Bx - 2.0 * Ax) * t + Ax;
                  y = (Ay - 2.0*By + Cy) * t*t + (2.0 * By - 2.0 * Ay) * t + Ay;
                  dp.x = xText + x;
                  dp.y = yText + y;
                  pIDataSet -> pushDataPoint(&dp);
               }

               Ax = Cx;
               Ay = Cy;

            }
 
            }
            break;

         default:
            Beep(2000,100);
#if 0
// The Cubic form ?
        //B(t) = (1-t)**3 p0 + 3(1 - t)**2 t P1 + 3(1-t)t**2 P2 + t**3 P3

        x = (1-t)*(1-t)*(1-t)*p0x + 3*(1-t)*(1-t)*t*p1x + 3*(1-t)*t*t*p2x + t*t*t*p3x;
        y = (1-t)*(1-t)*(1-t)*p0y + 3*(1-t)*(1-t)*t*p1y + 3*(1-t)*t*t*p2y + t*t*t*p3y

// Also

/**
 * Performs deCasteljau's algorithm for a bezier curve defined by the given control points.
 *
 * A cubic for example requires four points. So it should get at least an array of 8 values
 *
 * @param controlpoints (x,y) coord list of the Bezier curve.
 * @param returnArray Array to store the solved points. (can be null)
 * @param t Amount through the curve we are looking at.
 * @return returnArray
 */
public static float[] deCasteljau(float[] controlpoints, float[] returnArray, float t) {
    int m = controlpoints.length;
    int sizeRequired = (m/2) * ((m/2) + 1);
    if (returnArray == null) returnArray = new float[sizeRequired];
    if (sizeRequired > returnArray.length) returnArray = Arrays.copyOf(controlpoints, sizeRequired); //insure capacity
    else System.arraycopy(controlpoints,0,returnArray,0,controlpoints.length);
    int index = m; //start after the control points.
    int skip = m-2; //skip if first compare is the last control point.
    for (int i = 0, s = returnArray.length - 2; i < s; i+=2) {
        if (i == skip) {
            m = m - 2;
            skip += m;
            continue;
        }
        returnArray[index++] = (t * (returnArray[i + 2] - returnArray[i])) + returnArray[i];
        returnArray[index++] = (t * (returnArray[i + 3] - returnArray[i + 1])) + returnArray[i + 1];
    }
    return returnArray;
}
#endif
            break;

         }
 
         bInner = reinterpret_cast<BYTE *>(pPolyCurve);
         n = sizeof(TTPOLYCURVE) + sizeof(POINTFX) * (pPolyCurve -> cpfx - 1);
         bInner += n;
         pPolyCurve = reinterpret_cast<TTPOLYCURVE *>(bInner);
         cbPolygon -= n;

      }
 
      dp.x = xText + mapX(pPolygonHeader -> pfxStart.x);
      dp.y = yText + mapY(pPolygonHeader -> pfxStart.y);

      cbCharacter -= pPolygonHeader -> cb;
 
      if ( cbCharacter )
         pIDataSet -> pushDataPoint(&dp,renderTextEndContour,(void *)this,(void *)NULL);
      else
         pIDataSet -> pushDataPoint(&dp,renderTextEndGlyph,(void *)this,NULL);
 
      pPolygonHeader = reinterpret_cast<TTPOLYGONHEADER *>(pPolyCurve);

   }
 
   delete [] b;
 
   return 0;
   }

   void Text::renderTextNewGlyph(void *pvThis,void *pvArg) {
   pointCount = 0;
   contourIndex = -1;
   contourPointsCount[0] = 0;
   return;
   }

   void Text::renderTextNewContour(void *pvThis,void *pvArg) {
   contourIndex++;
   return;
   }

   void Text::renderTextEndContour(void *pvThis,void *pvArg) {
   contourPointsCount[contourIndex] = pointCount;
   if ( 0 < contourIndex )
      contourPointsCount[contourIndex] -= contourPointsCount[contourIndex - 1];
   return;
   }

   void Text::renderTextEndGlyph(void *pvThis,void *pvArg) {
   renderTextEndContour(pvThis,pvArg);
   PolyPolygon (hdc, (LPPOINT)ptPoints,contourPointsCount,contourIndex + 1);
   return;
   }