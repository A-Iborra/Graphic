// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"

   static DataPoint movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};
   static POINT ptPoints[1024];
   static int contourPointsCount[32];
   static long pointCount;
   static long contourIndex;

   static POINT ptMinGDI{4096,4096};
   static POINT ptMaxGDI{-4096,-4096};

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

   int oldPolyFillMode = SetPolyFillMode(hdc,ALTERNATE);

   float fv[4];
   BYTE *pb = (BYTE *)fv;

   propertyTextColor -> get_binaryValue(4 * sizeof(long),(BYTE **)&pb);

   BYTE vb[3];

   vb[0] = (BYTE)(255.0f*fv[0]);
   vb[1] = (BYTE)(255.0f*fv[1]);
   vb[2] = (BYTE)(255.0f*fv[2]);

   HGDIOBJ oldBrush = SelectObject(hdc,(HBRUSH)CreateSolidBrush(RGB(vb[0],vb[1],vb[2])));

   HGDIOBJ oldPen = SelectObject(hdc,(HPEN)CreatePen(PS_SOLID,1,RGB(vb[0],vb[1],vb[2])));

   void (__stdcall *pActionFunction)(void *,void *) = NULL;

   DataList *pItem = NULL;

   pIDataSet -> peek(pItem,&pItem);

   bool nextIsMove = false;

   while ( pItem ) {

      DataPoint dpGDI{0.0,0.0,0.0};

      if ( -DBL_MAX == pItem -> data.x ) {

         if ( ! ( NULL == pItem -> pvAction ) ) {
            pActionFunction = (void (__stdcall *)(void *,void *))pItem -> pvAction;
            pActionFunction(pItem -> pvArg1,pItem -> pvArg2);
         }

         nextIsMove = true;

         pIDataSet -> peek(pItem,&pItem);

         if ( ! pItem )
            break;

         continue;

      }

      if ( coordinatePlane == CoordinatePlane_screen ) {
         ptPoints[pointCount].x = (long)pItem -> data.x;
         ptPoints[pointCount].y = (long)pItem -> data.y;
      } else {
         pIOpenGLImplementation -> DataToWindow(&pItem -> data,UNIT_PIXEL,&dpGDI);
         ptPoints[pointCount].x = (long)dpGDI.x;
         ptPoints[pointCount].y = (long)dpGDI.y;
      }

      contourPointsCount[contourIndex]++;

      ptMinGDI.x = min(ptPoints[pointCount].x,ptMinGDI.x);
      ptMinGDI.y = min(ptPoints[pointCount].y,ptMinGDI.y);
      ptMaxGDI.x = max(ptPoints[pointCount].x,ptMaxGDI.x);
      ptMaxGDI.y = max(ptPoints[pointCount].y,ptMaxGDI.y);

      pointCount++;

      pIDataSet -> peek(pItem,&pItem);

   }

   SetPolyFillMode(hdc,oldPolyFillMode);

   DeleteObject(SelectObject(hdc,oldBrush));

   DeleteObject(SelectObject(hdc,oldPen));

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

   OUTLINETEXTMETRIC outlineTextMetric = {0};

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
         if ( 0 == outlineTextMetric.otmSize ) {
            outlineTextMetric.otmSize = sizeof(OUTLINETEXTMETRIC);
            GetOutlineTextMetrics(hdc,outlineTextMetric.otmSize,&outlineTextMetric);
         }
         startPoint.y -= outlineTextMetric.otmTextMetrics.tmHeight;
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
 
      pIDataSet -> pushDataPoint(&movePoint,renderTextNewContour,(void *)this,(void *)NULL);

      dp.x = xText + Ax;
      dp.y = yText + Ay;
      pIDataSet -> pushDataPoint(&dp);

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

               Ax = x;
               Ay = y;

            }

            }
            break;

         default:
            Beep(2000,100);
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

      pIDataSet -> pushDataPoint(&dp);

      pIDataSet -> pushDataPoint(&movePoint,renderTextEndContour,(void *)this,(void *)NULL);
 
      pPolygonHeader = reinterpret_cast<TTPOLYGONHEADER *>(pPolyCurve);

   }

   pIDataSet -> pushDataPoint(&movePoint,renderTextEndGlyph,(void *)this,NULL);
 
   delete [] b;
 
   return 0;
   }

   void Text::renderTextNewGlyph(void *pvThis,void *pvArg) {
   pointCount = 0;
   contourIndex = -1;
   contourPointsCount[0] = 0;
   memset(ptPoints,0,sizeof(ptPoints));
   memset(contourPointsCount,0,sizeof(contourPointsCount));
   ptMinGDI.x = 4096;
   ptMinGDI.y = 4096;
   ptMaxGDI.x = -4096;
   ptMaxGDI.y = -4096;
   return;
   }

   void Text::renderTextNewContour(void *pvThis,void *pvArg) {
   contourIndex++;
   return;
   }

   void Text::renderTextEndContour(void *pvThis,void *pvArg) {
   return;
   }

   void Text::renderTextEndGlyph(void *pvThis,void *pvArg) {

   RECT rcBounds;
   rcBounds.left = ptMinGDI.x;
   rcBounds.top = ptMinGDI.y;
   rcBounds.right = ptMaxGDI.x;
   rcBounds.bottom = ptMaxGDI.y;

//HPEN hPen = CreatePen(PS_SOLID,1,RGB(255,0,0));
//HGDIOBJ oldPen = SelectObject(hdc,hPen);
//MoveToEx(hdc,rcBounds.left,rcBounds.top,NULL);
//LineTo(hdc,rcBounds.right,rcBounds.top);
//LineTo(hdc,rcBounds.right,rcBounds.bottom);
//LineTo(hdc,rcBounds.left,rcBounds.bottom);
//LineTo(hdc,rcBounds.left,rcBounds.top);
//DeleteObject(SelectObject(hdc,oldPen));

   if ( S_OK == ((Text *)pvThis) -> pIGSGraphicServices -> AnyOpenGLHere(&rcBounds) )
      return;

   PolyPolygon(hdc,(LPPOINT)ptPoints,contourPointsCount,contourIndex + 1);

   return;
   }