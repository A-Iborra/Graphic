/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "utils.h"

#include "Text.h"

#define EPSILON 1.0E-6

   static DataPoint movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};
   static double piover2 = 0.0;
 
   static double mapX(FIXED fx) {
   long lx = *(LONG *)&fx;
   return ((double)(lx) / 65536.0);
   }
 
   static double mapY(FIXED fy) {
   long ly = *(LONG *)&fy;
   return ((double)(ly) / 65536.0);
   }
 
   static FIXED doubleToFixed(double d) {
   long l;
   l = static_cast<long>(d * 65536L);
   return *(FIXED *)&l;
   }
 
 
   HRESULT Text::PrepData() {

   if ( ! pszText )
      return E_FAIL;

   if ( 0.0 == piover2 )
      piover2 = 2.0 * atan(1.0);

   HDC hdc = NULL;
   if ( doOpenGLRendering )
      pIOpenGLImplementation -> get_HDC(&hdc);
   else
      hdc = GetDC(hwndOwner);

   DataPoint dpMin,dpMax;
   double worldXMin,worldXMax,worldYMin,worldYMax,worldZMin,worldZMax;
   
//   pIOpenGLImplementation -> GetExtents(&worldXMin,&worldYMin,&worldZMin,&worldXMax,&worldYMax,&worldZMax);
   pIDataSetWorld -> GetDomain(&dpMin,&dpMax);
   worldXMin = dpMin.x;
   worldXMax = dpMax.x;
   worldYMin = dpMin.y;
   worldYMax = dpMax.y;
   worldZMin = dpMin.z;
   worldZMax = dpMax.z;

   if ( DBL_MAX == worldXMin && -DBL_MAX == worldXMax )
      return 0;

   double toTextDomainScaleX,toTextDomainScaleY,toTextDomainScaleZ;
   double textDomainMinX,textDomainMinY,textDomainMinZ;
   double textDomainMaxX,textDomainMaxY,textDomainMaxZ;
   double textMinX,textMaxX,textMinY,textMaxY,textMinZ,textMaxZ;
   int viewPort[4];

   pIOpenGLImplementation -> get_ViewPort(viewPort);

   short partOfWorldDomain;
   get_PartOfWorldDomain(&partOfWorldDomain);

   double theRange[] = {RANGE,RANGE,RANGE};

   if ( partOfWorldDomain ) {
      theRange[0] = worldXMax - worldXMin;
      theRange[1] = worldYMax - worldYMin;
      theRange[2] = worldZMax - worldZMin;
      toTextDomainScaleX = 1.0;
      toTextDomainScaleY = 1.0;
      toTextDomainScaleZ = 1.0;
      textDomainMinX = worldXMin;
      textDomainMinY = worldYMin;
      textDomainMinZ = worldZMin;
      textDomainMaxX = worldXMax;
      textDomainMaxY = worldYMax;
      textDomainMaxZ = worldZMax;
   } else {
      toTextDomainScaleX = 2.0 * theRange[0] / (worldXMax - worldXMin);
      toTextDomainScaleY = 2.0 * theRange[1] / (worldYMax - worldYMin);
      toTextDomainScaleZ = 2.0 * theRange[2] / (worldZMax - worldZMin);
      textDomainMinX = -RANGE;
      textDomainMinY = -RANGE;
      textDomainMinZ = -RANGE;
      textDomainMaxX = RANGE;
      textDomainMaxY = RANGE;
      textDomainMaxZ = RANGE;
   }

   long fontSizeUnits;
   double dyScale,dxScale;

   propertySizeUnits -> get_longValue(&fontSizeUnits);

   if ( ! doOpenGLRendering ) {

      memset(&logicalFont,0,sizeof(LOGFONT));
      
      createFont(&logicalFont);
      hOriginalFont = (HFONT)SelectObject(hdc,hFont);

      TEXTMETRIC tm;
      GetTextMetrics(hdc,&tm);

      mHeight = tm.tmHeight;
      mWidth = tm.tmAveCharWidth;

      pIDataSet -> ReSet();
 #if 0
      DataPoint dp;

      propertyPositionX -> get_doubleValue(&dp.x);
      propertyPositionY -> get_doubleValue(&dp.y);
      propertyPositionZ -> get_doubleValue(&dp.z);

      pIDataSet -> pushDataPoint(&dp);
#endif
      RECT rectText;
      memset(&rectText,0,sizeof(RECT));
      if ( pszText )
         DrawText(hdc,pszText,-1L,&rectText,DT_CALCRECT);

      pIDataSet -> ReSet();

      pIDataSet -> pushDataPoint(&movePoint);
 
      DataPoint dp;

      dp.x = (double)rectText.left;
      dp.y = (double)rectText.top;
      dp.z = 0.0;

      pIDataSet -> pushDataPoint(&dp);

      dp.x = rectText.right;
      dp.y = rectText.bottom;
      dp.z = 0.0;

      pIDataSet -> pushDataPoint(&dp);

      pIDataSet -> get_minX(&textMinX);
      pIDataSet -> get_maxX(&textMaxX);
      pIDataSet -> get_minY(&textMinY);
      pIDataSet -> get_maxY(&textMaxY);
      pIDataSet -> get_minZ(&textMinZ);
      pIDataSet -> get_maxZ(&textMaxZ);

// The rectangle of the text is in pixels
   
   } else {

      renderText(NULL,"M");

      pIDataSet -> get_minX(&textMinX);
      pIDataSet -> get_maxX(&textMaxX);
      pIDataSet -> get_minY(&textMinY);
      pIDataSet -> get_maxY(&textMaxY);
      pIDataSet -> get_minZ(&textMinZ);
      pIDataSet -> get_maxZ(&textMaxZ);

      mWidth = textMaxX - textMinX;
      mHeight = textMaxY - textMinY;

      if ( ! renderText() ) {
         SelectObject(hdc,hOriginalFont);
         return S_OK;
      }

   }

   pIDataSet -> GenerateBoundingBox(pIDataSetBoundingBox);

   switch ( fontSizeUnits ) {

   case TEXT_SIZE_PIXELS:
   case TEXT_SIZE_POINTS: {

      double pixelsY = (double)GetDeviceCaps(hdc,LOGPIXELSY);
      double pixelsX = (double)GetDeviceCaps(hdc,LOGPIXELSX);

//      if ( doOpenGLRendering ) {

         double desiredHeightInInches;

         if ( TEXT_SIZE_PIXELS == fontSizeUnits )
            desiredHeightInInches = fontSize / pixelsY;
         else
            desiredHeightInInches = fontSize / 72.0;

         double desiredWidthInInches = desiredHeightInInches * mWidth / mHeight;

         double desiredWidthInPixels = pixelsX * desiredWidthInInches;
         double desiredHeightInPixels = pixelsY * desiredHeightInInches;

if ( doOpenGLRendering ) {
         dxScale = desiredWidthInPixels * ( textDomainMaxX - textDomainMinX ) / mWidth / (double)(viewPort[2] - viewPort[0]);
         dyScale = desiredHeightInPixels * ( textDomainMaxY - textDomainMinY ) / mHeight / (double)(viewPort[3] - viewPort[1]);
} else {
         dxScale = desiredWidthInPixels * ( worldXMax - worldXMin ) / mWidth / (double)(viewPort[2] - viewPort[0]);
         dyScale = desiredHeightInPixels * ( worldYMax - worldYMin ) / mHeight / (double)(viewPort[3] - viewPort[1]);
//dxScale = 1.0;
//dyScale = 1.0;
}

#if 0
      } else {

//         dyScale = ( textDomainMaxY - textDomainMinY ) / (double)(viewPort[3] - viewPort[1]);
//         dxScale = ( textDomainMaxX - textDomainMinX ) / (double)(viewPort[2] - viewPort[0]);
dyScale = 1.0;
dxScale = 1.0;

      }
#endif
      }
      break;

   case TEXT_SIZE_PERCENT:

      switch ( coordinatePlane ) {
      case CoordinatePlane_XY:
         dyScale = theRange[0] * fontSize / mHeight / 100.0;
         break;
      case CoordinatePlane_YX:
         dyScale = theRange[1] * fontSize / mHeight / 100.0;
         break;
      case CoordinatePlane_XZ:
         dyScale = theRange[0] * fontSize / mHeight / 100.0;
         break;
      case CoordinatePlane_YZ:
         dyScale = theRange[1] * fontSize / mHeight / 100.0;
         break;
      case CoordinatePlane_ZY:
         dyScale = theRange[2] * fontSize / mHeight / 100.0;
         break;
      case CoordinatePlane_ZX:
         dyScale = theRange[2] * fontSize / mHeight / 100.0;
         break;
      case CoordinatePlane_screen:
         dyScale = theRange[0] * fontSize / mHeight / 100.0;
         break;
      }

      dxScale = dyScale * mWidth / mHeight;

      break;

   }

   BSTR bstrEval = SysAllocStringLen(NULL,256);
   swprintf(bstrEval,L"mHeight = %lf",mHeight * dyScale);
   evalBSTR(pIEvaluator,bstrEval);
   swprintf(bstrEval,L"mWidth = %lf",mWidth * dxScale);
   evalBSTR(pIEvaluator,bstrEval);
   SysFreeString(bstrEval);

   char szPosition[256];
   propertyPositionString -> get_szValue(szPosition);

   if ( strlen(szPosition) ) {

      BSTR bstrPosition = SysAllocStringLen(NULL,256);

      char *p = strtok(szPosition,",");
      if ( p ) {
         MultiByteToWideChar(CP_ACP,0,p,-1,bstrPosition,strlen(p) + 1);
         pIDataSetWorld -> Evaluate(bstrPosition,&dpStart.x);
         p = strtok(NULL,",");
         if ( p ) {
            MultiByteToWideChar(CP_ACP,0,p,-1,bstrPosition,strlen(p) + 1);
            pIDataSetWorld -> Evaluate(bstrPosition,&dpStart.y);
            p = strtok(NULL,",");
            if ( p ) {
               MultiByteToWideChar(CP_ACP,0,p,-1,bstrPosition,strlen(p) + 1);
               pIDataSetWorld -> Evaluate(bstrPosition,&dpStart.z);
            } else {
               dpStart.z = 0.0;
            }
         } else {
            dpStart.y = 0.0;
            dpStart.z = 0.0;
         }
      } else {
         dpStart.x = 0.0;
         dpStart.y = 0.0;
         dpStart.z = 0.0;
      }

      SysFreeString(bstrPosition);

   } else {

      SAFEARRAY *pStart = NULL;
      BSTR *pData;
      get_Position(&pStart);
      SafeArrayAccessData(pStart,(void **)&pData);
      dpStart.x = evalBSTR(pIEvaluator,*pData);
      pData++;
      dpStart.y = evalBSTR(pIEvaluator,*pData);
      pData++;
      dpStart.z = evalBSTR(pIEvaluator,*pData);
      SafeArrayUnaccessData(pStart);
      SafeArrayDestroy(pStart);

   }

   DataPoint dpScale = {dxScale,dyScale,1.0};

#if 0
   dpStart.x = worldXMin + (dpStart.x - worldXMin) * toTextDomainScaleX;
   dpStart.y = worldYMin + (dpStart.y - worldYMin) * toTextDomainScaleY;
   dpStart.z = worldZMin + (dpStart.z - worldZMin) * toTextDomainScaleZ;
#endif

   DataPoint minPoint,maxPoint,dpTranslateFormat = {0.0,0.0,0.0};
   
   pIDataSet -> GetDomain(&minPoint,&maxPoint);

   switch ( coordinatePlane ) {

   case CoordinatePlane_XY:
      if ( format & TEXT_FORMAT_CENTER )
         dpTranslateFormat.x -= (maxPoint.x - minPoint.x) / 2.0;
      
      if ( format & TEXT_FORMAT_RIGHT ) 
         dpTranslateFormat.x -= maxPoint.x - minPoint.x;
      
      if ( format & TEXT_COORDINATES_FROM_TOP ) 
         dpTranslateFormat.y -= maxPoint.y - minPoint.y;
      
      if ( format & TEXT_COORDINATES_FROM_CENTER ) 
         dpTranslateFormat.y -= (maxPoint.y - minPoint.y)/2.0;
      break;

   case CoordinatePlane_YX: 
      pIDataSet -> Rotate('Z',90.0);
      pIDataSet -> GetDomain(&minPoint,&maxPoint);
      if ( format & TEXT_FORMAT_CENTER )
         dpTranslateFormat.y -= (maxPoint.y - minPoint.y) / 2.0;
      
      if ( format & TEXT_FORMAT_RIGHT ) 
         dpTranslateFormat.y -= maxPoint.y - minPoint.y;
      
      if ( format & TEXT_COORDINATES_FROM_TOP ) 
         dpTranslateFormat.x -= maxPoint.x - minPoint.x;
      
      if ( format & TEXT_COORDINATES_FROM_CENTER ) 
         dpTranslateFormat.x -= (maxPoint.x - minPoint.x)/2.0;
      logicalFont.lfOrientation = (long)900.0;
      logicalFont.lfEscapement = (long)900.0;
      break;

   case CoordinatePlane_XZ:
      pIDataSet -> Rotate('X',90.0);
      break;

   case CoordinatePlane_YZ:
      pIDataSet -> Rotate('Y',90.0);
      pIDataSet -> Rotate('X',90.0);
      break;

   case CoordinatePlane_ZY:
      pIDataSet -> Rotate('Y',-90.0);
      break;

   case CoordinatePlane_ZX:
      pIDataSet -> Rotate('Z',-90.0);
      pIDataSet -> Rotate('X',-90.0);
      break;

   case CoordinatePlane_screen: {
      double viewTheta,viewPhi;
      if ( flipVertical ) 
         pIDataSet -> Rotate('X',180.0);
      if ( flipHorizontal )
         pIDataSet -> Rotate('Y',180.0);
      pIOpenGLImplementation -> get_ViewTheta(&viewTheta);
      pIOpenGLImplementation -> get_ViewPhi(&viewPhi);
      pIDataSet -> Rotate('y',90.0);
      pIDataSet -> Rotate('X',90.0);
      pIDataSet -> Rotate('Y',-viewPhi);
      pIDataSet -> Rotate('Z',viewTheta);
      }
      break;
   }

   pIDataSet -> Translate(&dpTranslateFormat);
   
   pIDataSet -> Scale(&dpScale);

   pIDataSet -> Translate(&dpStart);

   pIDataSet -> GenerateGDICoordinates(pIOpenGLImplementation);

   pIDataSet -> GenerateBoundingBox(pIDataSetBoundingBox);

   SelectObject(hdc,hOriginalFont);

   if ( ! doOpenGLRendering )
      ReleaseDC(hwndOwner,hdc);

   return S_OK;
   }


   HRESULT Text::Draw() {

   if ( doOpenGLRendering ) {
      if ( partOfMainGraphic ) {
         pIOpenGLImplementation -> Push();
         pIOpenGLImplementation -> SetUp(pIDataSet);
         pIBasePlot -> Draw();
         pIOpenGLImplementation -> Pop();
      }
      if ( partOfWorldDomain ) {
         pIBasePlot -> Draw();
      }
      return S_OK;
   }

   if ( ! pszText )
      return S_OK;

   HDC hdc = GetDC(hwndOwner);

   createFont(&logicalFont);
   hOriginalFont = (HFONT)SelectObject(hdc,hFont);

   DataPoint minPoint,maxPoint;
   RECT rectText;

   pIDataSet -> GenerateGDICoordinates(pIOpenGLImplementation);

   pIDataSet -> GetDomainGDI(&minPoint,&maxPoint);

   rectText.left = (long)minPoint.x;
   rectText.top = (long)minPoint.y;
   rectText.right = (long)maxPoint.x;
   rectText.bottom = (long)maxPoint.y;

   if ( CoordinatePlane_YX == coordinatePlane ) {
      long cy = rectText.bottom - rectText.top;
      rectText.bottom += cy;
      rectText.top += cy;
   }

   int oldBackground = SetBkMode(hdc,TRANSPARENT);

   DrawText(hdc,pszText,-1L,&rectText,DT_NOCLIP | DT_LEFT | DT_BOTTOM);

   SetBkMode(hdc,oldBackground);

   SelectObject(hdc,hOriginalFont);

   ReleaseDC(hwndOwner,hdc);

   return S_OK;
   }
 
 
   HRESULT Text::Erase() {

   if ( partOfMainGraphic && doOpenGLRendering ) {
      pIOpenGLImplementation -> Push();
      pIOpenGLImplementation -> SetUp(pIDataSet);
      pIBasePlot -> Erase();
      pIOpenGLImplementation -> Pop();
   }
   return S_OK;
   }


   HRESULT Text::Redraw() {

   if ( ! doOpenGLRendering ) 
      return Draw();

   if ( partOfMainGraphic ) {
      pIOpenGLImplementation -> Push();
      pIOpenGLImplementation -> SetUp(pIDataSet);
      pIBasePlot -> Redraw();
      pIOpenGLImplementation -> Pop();
   }

   return S_OK;
   }
 
 
   long Text::renderText(HDC hdcp,char* theText) {

   HDC hdc = hdcp;

   if ( ! hdc ) 
      pIOpenGLImplementation -> get_HDC(&hdc);

   if ( ! hFont ) {
      if ( ! szFace[0] )
         strcpy(szFace,DEFAULT_FONT);
      memset(&logicalFont,0,sizeof(LOGFONT));
      logicalFont.lfHeight = -MulDiv((long)fontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
      logicalFont.lfQuality = PROOF_QUALITY;
      logicalFont.lfWeight = FW_NORMAL;
      strcpy(logicalFont.lfFaceName,szFace);
      logicalFont.lfCharSet = DEFAULT_CHARSET;
      hOriginalFont = (HFONT)SelectObject(hdc,hFont = CreateFontIndirect(&logicalFont));
   }

   TEXTMETRIC fontMetrics;
   memset(&fontMetrics,0,sizeof(TEXTMETRIC));
   GetTextMetrics(hdc,&fontMetrics);

   int charWidth;
   char *c;
   char *text;

   DataPoint startPoint = {0.0,0.0,0.0};

   pIDataSet -> ReSet();

   if ( ! theText ) {
      if ( ! pszText )
         return 0;
      long n = strlen(pszText) + 1;
      text = new char[n];
      memset(text,0,n * sizeof(char));
      strcpy(text,pszText);
   } else {
      long n = strlen(theText);
      text = new char[n + 1];
      strcpy(text,theText);
   }

   c = text;
   while ( *c && *c > 0 ) {
      if ( *c == (char)0x0A || *c == (char)0x0D || *c == '\n' ) {
         if ( *c == (char)0x0D ) {
            c++;
            if ( *c == (char)0x0A ) c++;
         } else {
            if ( *c == (char)0x0A ) {
               c++;
               if ( *c == (char)0X0D ) c++;
            } else {
               c++;
            }
         }
         startPoint.x = 0;
         startPoint.y -= fontMetrics.tmHeight;
      } else {
         renderGlyph(hdc,*c,startPoint);
         GetCharWidth32(hdc,*c,*c,&charWidth);
         startPoint.x += charWidth;
         c++;
      }
   }

   pIDataSet -> get_maxX(&dpEnd.x);
   pIDataSet -> get_maxY(&dpEnd.y);
   pIDataSet -> get_maxZ(&dpEnd.z);

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
   long k,n;
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
   if ( cbCharacter < 0 ) return 0;

   b = new BYTE[cbCharacter];
   cbCharacter = GetGlyphOutline(hdc,c,GGO_NATIVE,&glyphMetrics,cbCharacter,b,&matrix);
 
   pPolygonHeader = reinterpret_cast<TTPOLYGONHEADER *>(b);
 
   dp.z = zText;
   while ( cbCharacter > 0 ) {
 
      pPolyCurve = reinterpret_cast<TTPOLYCURVE *>(reinterpret_cast<BYTE *>(pPolygonHeader) + sizeof(TTPOLYGONHEADER));
 
      cbPolygon = (long)pPolygonHeader -> cb - sizeof(TTPOLYGONHEADER);
 
      Ax = mapX(pPolygonHeader -> pfxStart.x);
      Ay = mapY(pPolygonHeader -> pfxStart.y);
 
      pIDataSet -> pushDataPoint(&movePoint);
 
      dp.x = xText + Ax;
      dp.y = yText + Ay;
      pIDataSet -> pushDataPoint(&dp);
 
      while ( cbPolygon > 0 ) {
 
         switch ( pPolyCurve -> wType ) {
         case TT_PRIM_LINE:
            for ( k = 0; k < pPolyCurve -> cpfx; k++ ) {
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
 
            for ( k = 0; k < pPolyCurve -> cpfx - 1; k++ ) {
               Bx = mapX(pPolyCurve -> apfx[k].x);
               By = mapY(pPolyCurve -> apfx[k].y);
               if ( k < pPolyCurve -> cpfx - 2) {
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

         }
 
         bInner = reinterpret_cast<BYTE *>(pPolyCurve);
         n = sizeof(TTPOLYCURVE) + sizeof(POINTFX) * (pPolyCurve -> cpfx - 1);
         bInner += n;
         pPolyCurve = reinterpret_cast<TTPOLYCURVE *>(bInner);
         cbPolygon -= n;

      }
 
      dp.x = xText + mapX(pPolygonHeader -> pfxStart.x);
      dp.y = yText + mapY(pPolygonHeader -> pfxStart.y);

      pIDataSet -> pushDataPoint(&dp);
 
      cbCharacter -= pPolygonHeader -> cb;

      pPolygonHeader = reinterpret_cast<TTPOLYGONHEADER *>(pPolyCurve);

   }
 
   delete [] b;
 
   return 0;
   }


   int Text::drawSample() {

   pIOpenGLImplementation -> SetTargetWindow(hwndSample);

   bool temporaryText = false;

#if 0 //FIXME
   propertyText -> get_size(&n);
   char *text = new char[n + 1];
   propertyText -> get_szValue(text);
   if ( n < 1 || strcmp("",text) == 0 ) {
      propertyText -> put_szValue("<sample>");
      temporaryText = true;
   }
   delete [] text;
#endif
   pIOpenGLImplementation -> Erase();

   short oldPartOfWorldDomain;
   IDataSet* pIDataSetOld = pIDataSet;
   IDataSet* pIDataSetWorld_Old = pIDataSetWorld;
   TextFormat oldFormat = format;
   CoordinatePlane oldCoordinatePlane = coordinatePlane;
   SAFEARRAY *pOldStart = NULL;

   DataPoint oldDirectionForward = {directionForward.x,directionForward.y,directionForward.z};

   get_Position(&pOldStart);

   get_PartOfWorldDomain(&oldPartOfWorldDomain);

   CoCreateInstance(CLSID_DataSet,
                      NULL,
                      CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                      IID_IDataSet,
                      reinterpret_cast<void **>(&pIDataSet));

   pIBasePlot -> put_DataSet(pIDataSet);

   pIDataSetWorld = pIDataSet;

   put_PositionX(0.0);
   put_PositionY(0.0);
   put_PositionZ(0.0);

   directionForward.x = 1.0;
   directionForward.y = 0.0;
   directionForward.z = 0.0;

   format = (TextFormat)TEXT_FORMAT_CENTER;

   put_PartOfWorldDomain(FALSE);

   coordinatePlane = CoordinatePlane_XY;

   renderText();

   pIOpenGLImplementation -> Set2D(pIDataSetWorld,0,'X','Y');

   double textMinX,textMaxX,textMinY,textMaxY;

   pIDataSet -> get_minX(&textMinX);
   pIDataSet -> get_maxX(&textMaxX);
   pIDataSet -> get_minY(&textMinY);
   pIDataSet -> get_maxY(&textMaxY);

   DataPoint dpTranslate = {textMinX + (textMaxX - textMinX)/2.0,textMinY + (textMaxY - textMinY)/2.0,0.0};

   PrepData();

   pIDataSet -> Translate(&dpTranslate);

   pIBasePlot -> Draw();

   pIDataSet -> Release();

   pIOpenGLImplementation -> ResetTargetWindow();

   pIDataSetWorld = pIDataSetWorld_Old;

   pIDataSet = pIDataSetOld;

   pIBasePlot -> put_DataSet(pIDataSet);

   format = oldFormat;

   coordinatePlane = oldCoordinatePlane;

   directionForward.x = oldDirectionForward.x;
   directionForward.y = oldDirectionForward.y;
   directionForward.z = oldDirectionForward.z;

   put_PartOfWorldDomain(oldPartOfWorldDomain);

#if 0
   if ( temporaryText )
      propertyText -> put_szValue("");
#endif
   put_Position(pOldStart);

   SafeArrayDestroy(pOldStart);

   return 0;
   }


   int Text::eraseGDI() {
   HDC hdc;
Beep(2000,100);
#if 0
   pIOpenGLImplementation -> get_HDC(&hdc);
   SetROP2(hdc,R2_NOTXORPEN);
#endif
   pIBasePlot -> DrawGDI();
   return 0;
   }


   int Text::eraseBoundingBox() {
   pIBasePlotBoundingBox -> Erase();
   return 0;
   }


   int Text::eraseBoundingBoxGDI() {
   HDC hdc;
Beep(2000,100);
#if 0
   pIOpenGLImplementation -> get_HDC(&hdc);
   SetROP2(hdc,R2_NOTXORPEN);
#endif
   pIBasePlotBoundingBox -> DrawGDI();
   return 0;
   }