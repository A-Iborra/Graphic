// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Text.h"
#include <math.h>
#include <stdio.h>

#include "utils.h"

#define EPSILON 1.0E-6

   static DataPoint movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};

   static double piover2 = 0.0;
 
   HRESULT Text::PrepData() {

   if ( ! propertyContent -> pointer() )
      return E_FAIL;

   if ( 0.0 == piover2 )
      piover2 = 2.0 * atan(1.0);

   HDC hdc = pIOpenGLImplementation -> TargetDC();

   DataPoint dpMinWorld,dpMaxWorld;

   pIDataSetWorld -> GetDomain(&dpMinWorld,&dpMaxWorld);

   if ( ( -DBL_MAX == dpMinWorld.x && DBL_MAX == dpMaxWorld.x ) || ( DBL_MAX == dpMinWorld.x && -DBL_MAX == dpMaxWorld.x ) )
      return 0;

   int viewPort[4];

   pIOpenGLImplementation -> get_ViewPort(viewPort);

   long plotView = PlotViews::gcPlotView2D;

   pIOpenGLImplementation -> get_PlotView(&plotView);

   long fontSizeUnits;

   propertySizeUnits -> get_longValue(&fontSizeUnits);

   DataPoint dpMinGDI,dpMaxGDI;

   memset(&logicalFont,0,sizeof(LOGFONT));
      
   createFont(&logicalFont);

   hOriginalFont = (HFONT)SelectObject(hdc,hFont);

   if ( ! ( CoordinatePlane_screen == coordinatePlane ) ) 
      put_PartOfWorldDomain(VARIANT_TRUE);
   else
      put_PartOfWorldDomain(VARIANT_FALSE);

   if ( ! renderText() ) 
      return S_OK;

   dpTranslateFormatGDI = {0.0,0.0,0.0};
   
   DataPoint dpScaleGDIToWorld{1.0,1.0,1.0};

   pIDataSet -> GetDomain(&dpMinGDI,&dpMaxGDI);

   double textWidth = dpMaxGDI.x - dpMinGDI.x;
   double textHeight = dpMaxGDI.y - dpMinGDI.y;

   char verticalAxis = ' ';
   char horizontalAxis = ' ';

   switch ( coordinatePlane ) {

   case CoordinatePlane_XY:

      if ( format & TEXT_FORMAT_CENTER )
         dpTranslateFormatGDI.x -= textWidth / 2.0;
      
      if ( format & TEXT_FORMAT_RIGHT ) 
         dpTranslateFormatGDI.x -= textWidth;

      if ( format & TEXT_COORDINATES_FROM_TOP ) 
         dpTranslateFormatGDI.y -= textHeight;

      if ( format & TEXT_COORDINATES_FROM_CENTER ) 
         dpTranslateFormatGDI.y -= textHeight/2.0;

      pIDataSet -> Translate(&dpTranslateFormatGDI);

      dpScaleGDIToWorld.x = (dpMaxWorld.x - dpMinWorld.x) / (double)viewPort[2];
      dpScaleGDIToWorld.y = (dpMaxWorld.y - dpMinWorld.y) / (double)viewPort[3];

      horizontalAxis = 'X';
      verticalAxis = 'Y';

      break;

   case CoordinatePlane_YX: {

      DataPoint dpOldMin = dpMinGDI;

      pIDataSet -> Rotate('Z',90.0);
      pIDataSet -> Rotate('Y',180.0);

      pIDataSet -> GetDomain(&dpMinGDI,&dpMaxGDI);

      dpTranslateFormatGDI.x = dpOldMin.x - dpMinGDI.x;
      dpTranslateFormatGDI.y = dpOldMin.y - dpMinGDI.y;
      dpTranslateFormatGDI.z = dpOldMin.z - dpMinGDI.z;

      if ( format & TEXT_FORMAT_CENTER )
         dpTranslateFormatGDI.y -= textHeight / 2.0;

      if ( format & TEXT_FORMAT_RIGHT ) 
{
Beep(2000,100);
         dpTranslateFormatGDI.y -= textWidth; // ? This isn't correct yet
}

      if ( format & TEXT_COORDINATES_FROM_TOP ) 
         dpTranslateFormatGDI.x -= textHeight;

      pIDataSet -> Translate(&dpTranslateFormatGDI);

      if ( format & TEXT_COORDINATES_FROM_CENTER ) 
{
Beep(2000,100);
         dpTranslateFormatGDI.x -= textWidth / 2.0;
}

      dpScaleGDIToWorld.x = (dpMaxWorld.x - dpMinWorld.x) / (double)viewPort[2];
      dpScaleGDIToWorld.y = (dpMaxWorld.y - dpMinWorld.y) / (double)viewPort[3];

      horizontalAxis = 'Y';
      verticalAxis = 'X';

      }
      break;

   case CoordinatePlane_XZ:

      dpScaleGDIToWorld.x = (dpMaxWorld.x - dpMinWorld.x) / (double)viewPort[2];
      dpScaleGDIToWorld.y = (dpMaxWorld.y - dpMinWorld.y) / (double)viewPort[3];
      dpScaleGDIToWorld.z = (dpMaxWorld.z - dpMinWorld.z) / (double)viewPort[3];

      pIDataSet -> Rotate('X',90.0);

      dpTranslateFormatGDI.z += textHeight / 2.0;
      dpTranslateFormatGDI.y -= textHeight / 2.0;

      if ( format & TEXT_COORDINATES_FROM_CENTER ) 
         dpTranslateFormatGDI.z -= textHeight / 2.0;

      pIDataSet -> Translate(&dpTranslateFormatGDI);

      horizontalAxis = 'X';
      verticalAxis = 'Z';

      break;

   case CoordinatePlane_YZ: 

      dpScaleGDIToWorld.x = (dpMaxWorld.x - dpMinWorld.x) / (double)viewPort[2];
      dpScaleGDIToWorld.y = (dpMaxWorld.y - dpMinWorld.y) / (double)viewPort[3];
      dpScaleGDIToWorld.z = (dpMaxWorld.z - dpMinWorld.z) / (double)viewPort[3];

      pIDataSet -> Rotate('Z',90.0);
      pIDataSet -> Rotate('Y',90.0);

      dpTranslateFormatGDI.y += textWidth / 2.0;
      dpTranslateFormatGDI.y -= textHeight / 2.0;
      dpTranslateFormatGDI.x -= textWidth / 2.0;
      dpTranslateFormatGDI.z += textHeight / 2.0;

      pIDataSet -> Translate(&dpTranslateFormatGDI);

      horizontalAxis = 'Y';
      verticalAxis = 'Z';

      break;

   case CoordinatePlane_ZY:

      dpScaleGDIToWorld.x = (dpMaxWorld.x - dpMinWorld.x) / (double)viewPort[2];
      dpScaleGDIToWorld.y = (dpMaxWorld.y - dpMinWorld.y) / (double)viewPort[3];
      dpScaleGDIToWorld.z = (dpMaxWorld.z - dpMinWorld.z) / (double)viewPort[3];

      pIDataSet -> Rotate('Z',90.0);
      pIDataSet -> Rotate('Y',90.0);
      pIDataSet -> Rotate('X',90.0);
      pIDataSet -> Rotate('Z',180.0);

      dpTranslateFormatGDI.x -= textWidth / 2.0;
      dpTranslateFormatGDI.z += textWidth / 2.0;

      pIDataSet -> Translate(&dpTranslateFormatGDI);

      horizontalAxis = 'Z';
      verticalAxis = 'Y';

      break;

   case CoordinatePlane_ZX:

      dpScaleGDIToWorld.x = (dpMaxWorld.x - dpMinWorld.x) / (double)viewPort[2];
      dpScaleGDIToWorld.y = (dpMaxWorld.y - dpMinWorld.y) / (double)viewPort[3];
      dpScaleGDIToWorld.z = (dpMaxWorld.z - dpMinWorld.z) / (double)viewPort[3];

      pIDataSet -> Rotate('Z',-90.0);
      pIDataSet -> Rotate('X',-90.0);

      dpTranslateFormatGDI.x -= textWidth / 2.0;
      dpTranslateFormatGDI.x += textHeight / 2.0;
      dpTranslateFormatGDI.y -= textHeight / 2.0;
      dpTranslateFormatGDI.z += textWidth / 2.0;

      pIDataSet -> Translate(&dpTranslateFormatGDI);

      horizontalAxis = 'Z';
      verticalAxis = 'X';

      break;

   case CoordinatePlane_screen: {

      dpScaleGDIToWorld.x = 1.0;
      dpScaleGDIToWorld.y = -1.0;
      dpScaleGDIToWorld.z = 1.0;

      if ( format & TEXT_FORMAT_CENTER )
         dpTranslateFormatGDI.x -= textWidth / 2.0;
      
      if ( format & TEXT_FORMAT_RIGHT ) 
         dpTranslateFormatGDI.x -= textWidth;

      if ( format & TEXT_COORDINATES_FROM_CENTER ) 
         dpTranslateFormatGDI.y -= textHeight / 2.0;

      if ( format & TEXT_COORDINATES_FROM_TOP ) 
         dpTranslateFormatGDI.y -= textHeight;

      pIDataSet -> Translate(&dpTranslateFormatGDI);

      verticalAxis = 'X';
      horizontalAxis = 'Y';

      }
      break;
   }

   if ( flipVertical )
      pIDataSet -> Rotate(verticalAxis,180.0);

   if ( flipHorizontal )
      pIDataSet -> Rotate(horizontalAxis,180.0);

   pIDataSet -> SaveAsGDICoordinates();

   propertyPositionX -> get_doubleValue(&dpStart.x);
   propertyPositionY -> get_doubleValue(&dpStart.y);
   propertyPositionZ -> get_doubleValue(&dpStart.z);

   pIDataSet -> Scale(&dpScaleGDIToWorld);

   if ( ! ( CoordinatePlane_screen == coordinatePlane ) )

      pIDataSet -> Translate(&dpStart);

   else {

      DataPoint dpStartGDI;

      pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpStartGDI);

      pIDataSet -> Translate(&dpStartGDI);

   }

   pIDataSet -> GenerateBoundingBox(pIDataSetBoundingBox);

   SelectObject(hdc,hOriginalFont);

pIDataSet -> GetDomain(&dpMinGDI,&dpMaxGDI);

   return S_OK;
   }


   HRESULT Text::Draw() {
   renderOpenGLPoints();
   return S_OK;
   }
 
   HRESULT Text::Erase() {

   if ( partOfMainGraphic ) {
      pIOpenGLImplementation -> Push();
      pIOpenGLImplementation -> SetUp(pIDataSet);
      pIBasePlot -> Erase();
      pIOpenGLImplementation -> Pop();
   }

   HDC hdc = pIOpenGLImplementation -> TargetDC();
   RECT rcBox;
   get_GDIBoundingBox(&rcBox);
   rcBox.bottom += 2;
   COLORREF thePixel = GetPixel(hdc,2,2);
   HBRUSH hb = CreateSolidBrush(thePixel);
   FillRect(hdc,&rcBox,hb);
   DeleteObject(hb);

   return S_OK;
   }


   HRESULT Text::Redraw() {
   return Draw();
   return S_OK;
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

   get_Position(&pOldStart);

   get_PartOfWorldDomain(&oldPartOfWorldDomain);

   CoCreateInstance(CLSID_DataSet,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IDataSet,reinterpret_cast<void **>(&pIDataSet));

   pIBasePlot -> put_IDataSet(pIDataSet);

   pIDataSetWorld = pIDataSet;

   put_PositionX(0.0);
   put_PositionY(0.0);
   put_PositionZ(0.0);

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

   pIDataSetWorld = pIDataSetWorld_Old;

   pIDataSet = pIDataSetOld;

   pIBasePlot -> put_IDataSet(pIDataSet);

   format = oldFormat;

   coordinatePlane = oldCoordinatePlane;

   put_PartOfWorldDomain(oldPartOfWorldDomain);

#if 0
   if ( temporaryText )
      propertyText -> put_szValue("");
#endif
   put_Position(pOldStart);

   SafeArrayDestroy(pOldStart);

   return 0;
   }


   int Text::eraseBoundingBoxGDI() {

   if ( NULL == hbmBoundingBoxBackground ) 
      return 0;

   HDC hdc = pIOpenGLImplementation -> TargetDC();

   HDC hdcSource = CreateCompatibleDC(hdc);

   HGDIOBJ oldBitmap = SelectObject(hdcSource,hbmBoundingBoxBackground);
   
   long cx = rcSavedBoundingBox.right - rcSavedBoundingBox.left + 4;
   long cy = rcSavedBoundingBox.bottom - rcSavedBoundingBox.top + 4;

   BitBlt(hdc,rcSavedBoundingBox.left - 2,rcSavedBoundingBox.top - 2,cx,cy,hdcSource,0,0,SRCCOPY);

   SelectObject(hdcSource,oldBitmap);

   DeleteObject(hbmBoundingBoxBackground);

   DeleteDC(hdcSource);

   hbmBoundingBoxBackground = NULL;

   return 0;
   }


   void Text::saveBoundingBoxBackground() {

   HDC hdc = pIOpenGLImplementation -> TargetDC();

   HDC hdcTarget = CreateCompatibleDC(hdc);

   get_GDIBoundingBox(&rcSavedBoundingBox);

   long x = rcSavedBoundingBox.left - 2;
   long y = rcSavedBoundingBox.top - 2;
   long cx = rcSavedBoundingBox.right - rcSavedBoundingBox.left + 4;
   long cy = rcSavedBoundingBox.bottom - rcSavedBoundingBox.top + 4;

   hbmBoundingBoxBackground = CreateCompatibleBitmap(hdc,cx,cy);

   HGDIOBJ oldBitmap = SelectObject(hdcTarget,hbmBoundingBoxBackground);

   BitBlt(hdcTarget,0,0,cx,cy,hdc,x,y,SRCCOPY);

   long countPixels = cx * cy;

   float *pPixels = new float[4 * countPixels];

   memset(pPixels,0,4 * countPixels * sizeof(float));

   pIOpenGLImplementation -> GetPixels(x,y,x + cx,y + cy,(BYTE *)pPixels);

   float *pf = pPixels;
   BYTE b[3];

   for ( int k = 0; k < cy; k++ ) {
      for ( int j = 0; j < cx; j++ ) {
         b[0] = (BYTE)(255.0 * pf[0]);
         b[1] = (BYTE)(255.0 * pf[1]);
         b[2] = (BYTE)(255.0 * pf[2]);
         COLORREF cr = RGB(b[0],b[1],b[2]) & GetPixel(hdcTarget,j,k);
         SetPixel(hdcTarget,j,k,cr);
         pf += 4;
      }
   }

   delete [] pPixels;

   SelectObject(hdcTarget,oldBitmap);

   DeleteDC(hdcTarget);

   return;
   }
