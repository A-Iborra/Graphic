
#include "graphic.h"

#include <stdio.h>

#include "List.cpp"

   int G::drawGraphicCursor(POINTL *pPtlPickBox,int doPickBox) {

   if ( ! useGraphicsCursor ) 
      return 0;

   //
   //NTC: 12-05-2017: I am going to rethink how to have a 3-D cursor.
   //
   if ( gcPlotView3D == plotView ) {
      HDC hdc = GetDC(hwndGraphic);
      erasePickBox(hdc);
      savePickBox(hdc,pPtlPickBox);
      if ( doPickBox )
         drawPickBox(hdc,pPtlPickBox);
      ReleaseDC(hwndGraphic,hdc);
      ptlPickBox.x = pPtlPickBox -> x;
      ptlPickBox.y = pPtlPickBox -> y; 
      return 0;
   }

   POINT ptMouse;
   RECT rectWindow;

   GetCursorPos(&ptMouse);

   GetWindowRect(hwndGraphic,&rectWindow);

   if ( ptMouse.x > rectWindow.right || ptMouse.y > rectWindow.bottom || ptMouse.x < rectWindow.left || ptMouse.y < rectWindow.top ) 
      return 0;

   if ( 0 < graphicCursorList.Count() )
      eraseGraphicCursor();

   ptMouse.x -= rectWindow.left;
   ptMouse.y -= rectWindow.top;

   double minX,minY,minZ,maxX,maxY,maxZ;

   pIDataSetMaster -> get_minX(&minX);
   pIDataSetMaster -> get_minY(&minY);
   pIDataSetMaster -> get_minZ(&minZ);
   pIDataSetMaster -> get_maxX(&maxX);
   pIDataSetMaster -> get_maxY(&maxY);
   pIDataSetMaster -> get_maxZ(&maxZ);

   DataPoint dpWindow,dpStart,dpCurrent;

   dpWindow.x = (double)ptMouse.x;
   dpWindow.y = (double)ptMouse.y;
   dpWindow.z = 0.0;

   if ( S_FALSE == pIOpenGLImplementation -> WindowToData(&dpWindow,&dpStart) ) 
      return 0;

   char szPosition[64];
   sprintf(szPosition,"(%f,%f,%f)",dpStart.x,dpStart.y,dpStart.z);
   put_StatusText(1,szPosition);

   memcpy(&dpCurrent,&dpStart,sizeof(DataPoint));

   dpStart.x = minX;
   dpStart.z = minZ;

   if ( S_FALSE == pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpWindow) ) 
      return 0;

   struct graphicCursor* pCursor = new struct graphicCursor();

   graphicCursorList.Add(pCursor);

   pCursor -> ptlGraphicCursor[0].x = (long)dpWindow.x;
   pCursor -> ptlGraphicCursor[0].y = (long)dpWindow.y;

   dpStart.x = maxX;

   if ( S_FALSE == pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpWindow) ) {
      eraseGraphicCursor();
      return 0;
   }

   pCursor -> ptlGraphicCursor[1].x = (long)dpWindow.x;
   pCursor -> ptlGraphicCursor[1].y = (long)dpWindow.y;

   dpStart.x = dpCurrent.x;

   if ( dpCurrent.x > maxX || dpCurrent.x < minX ) {
      if ( S_FALSE == pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpWindow) ) {
         eraseGraphicCursor();
         return 0;
      }
      pCursor -> ptlGraphicCursor[6].x = pCursor -> ptlGraphicCursor[1].x;
      pCursor -> ptlGraphicCursor[6].y = pCursor -> ptlGraphicCursor[1].y;
      if ( dpCurrent.x < minX ) {
         pCursor -> ptlGraphicCursor[6].x = pCursor -> ptlGraphicCursor[0].x;
         pCursor -> ptlGraphicCursor[6].y = pCursor -> ptlGraphicCursor[0].y;
      }
      pCursor -> ptlGraphicCursor[7].x = (long)dpWindow.x;
      pCursor -> ptlGraphicCursor[7].y = (long)dpWindow.y;
   } else {
      pCursor -> ptlGraphicCursor[6].x = -1L;
      pCursor -> ptlGraphicCursor[6].y = -1L;
      pCursor -> ptlGraphicCursor[7].x = -1L;
      pCursor -> ptlGraphicCursor[7].y = -1L;
   }

   dpStart.y = minY;

   if ( S_FALSE == pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpWindow) ) {
      eraseGraphicCursor();
      return 0;
   }

   pCursor -> ptlGraphicCursor[2].x = (long)dpWindow.x;
   pCursor -> ptlGraphicCursor[2].y = (long)dpWindow.y;

   dpStart.y = maxY;

   if ( S_FALSE == pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpWindow) ) {
      eraseGraphicCursor();
      return 0;
   }

   pCursor -> ptlGraphicCursor[3].x = (long)dpWindow.x;
   pCursor -> ptlGraphicCursor[3].y = (long)dpWindow.y;

   dpStart.y = dpCurrent.y;
   
   if ( dpCurrent.y > maxY || dpCurrent.y < minY ) {
      if ( S_FALSE == pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpWindow) ) {
         eraseGraphicCursor();
         return 0;
      }
      pCursor -> ptlGraphicCursor[8].x = pCursor -> ptlGraphicCursor[3].x;
      pCursor -> ptlGraphicCursor[8].y = pCursor -> ptlGraphicCursor[3].y;
      if ( dpCurrent.y < minY ) {
         pCursor -> ptlGraphicCursor[8].x = pCursor -> ptlGraphicCursor[2].x;
         pCursor -> ptlGraphicCursor[8].y = pCursor -> ptlGraphicCursor[2].y;
      }
      pCursor -> ptlGraphicCursor[9].x = (long)dpWindow.x;
      pCursor -> ptlGraphicCursor[9].y = (long)dpWindow.y;
   } else {
      pCursor -> ptlGraphicCursor[8].x = -1L;
      pCursor -> ptlGraphicCursor[8].y = -1L;
      pCursor -> ptlGraphicCursor[9].x = -1L;
      pCursor -> ptlGraphicCursor[9].y = -1L;
   }

   dpStart.z = minZ;

   if ( S_FALSE == pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpWindow) ) {
      eraseGraphicCursor();
      return 0;
   }

   pCursor -> ptlGraphicCursor[4].x = (long)dpWindow.x;
   pCursor -> ptlGraphicCursor[4].y = (long)dpWindow.y;

   dpStart.z = minZ + zLevel * (maxZ - minZ);

   if ( dpStart.z > dpCurrent.z )
      dpStart.z = dpCurrent.z;

   if ( S_FALSE == pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpWindow) ) {
      eraseGraphicCursor();
      return 0;
   }

   pCursor -> ptlGraphicCursor[5].x = (long)dpWindow.x;
   pCursor -> ptlGraphicCursor[5].y = (long)dpWindow.y;

   if ( dpCurrent.z > maxZ || dpCurrent.z < minZ ) {
      dpStart.z = dpCurrent.z;
      if ( S_FALSE == pIOpenGLImplementation -> DataToWindow(&dpStart,UNIT_PIXEL,&dpWindow) ) {
         eraseGraphicCursor();
         return 0;
      }
      pCursor -> ptlGraphicCursor[10].x = pCursor -> ptlGraphicCursor[5].x;
      pCursor -> ptlGraphicCursor[10].y = pCursor -> ptlGraphicCursor[5].y;
      if ( dpCurrent.z < minZ ) {
         pCursor -> ptlGraphicCursor[10].x = pCursor -> ptlGraphicCursor[4].x;
         pCursor -> ptlGraphicCursor[10].y = pCursor -> ptlGraphicCursor[4].y;
         pCursor -> ptlGraphicCursor[4].x = -1L;
         pCursor -> ptlGraphicCursor[4].y = -1L;
         pCursor -> ptlGraphicCursor[5].x = -1L;
         pCursor -> ptlGraphicCursor[5].y = -1L;
      }
      pCursor -> ptlGraphicCursor[11].x = (long)dpWindow.x;
      pCursor -> ptlGraphicCursor[11].y = (long)dpWindow.y;
   } else {
      pCursor -> ptlGraphicCursor[10].x = -1L;
      pCursor -> ptlGraphicCursor[10].y = -1L;
      pCursor -> ptlGraphicCursor[11].x = -1L;
      pCursor -> ptlGraphicCursor[11].y = -1L;
   }

   long bitmapIndex = 0;

   long itemCount = 6;
   if ( gcPlotView3D == plotView ) 
      itemCount = 12;

   HDC hdc = GetDC(hwndGraphic);

   HDC hdcTarget = CreateCompatibleDC(NULL);

   for ( long k = 0; k < itemCount; k += 2 ) {

      long x = pCursor -> ptlGraphicCursor[k].x - 4;
      long y = pCursor -> ptlGraphicCursor[k].y - 4;
      long cx = pCursor -> ptlGraphicCursor[k + 1].x + 4 - x;
      long cy = pCursor -> ptlGraphicCursor[k + 1].y + 4 - y;

      if ( 0 > cx ) {
         x = pCursor -> ptlGraphicCursor[k + 1].x - 4;
         cx = pCursor -> ptlGraphicCursor[k].x + 4 - x;
      }
         
      if ( 0 > cy ) {
         y = pCursor -> ptlGraphicCursor[k + 1].y - 4;
         cy = pCursor -> ptlGraphicCursor[k].y + 4 - y;
      }

      pCursor -> ptlCrossBitmaps[bitmapIndex].x = x;
      pCursor -> ptlCrossBitmaps[bitmapIndex].y = y;
      pCursor -> sizelCrossBitmaps[bitmapIndex].cx = cx;
      pCursor -> sizelCrossBitmaps[bitmapIndex].cy = cy;

      pCursor -> crossBitmaps[bitmapIndex] = CreateCompatibleBitmap(hdc,cx,cy);
      HGDIOBJ oldBitmap = SelectObject(hdcTarget,pCursor -> crossBitmaps[bitmapIndex]);
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

      bitmapIndex++;

   }

   DeleteDC(hdcTarget);

   if ( doPickBox ) 
      savePickBox(hdc,pPtlPickBox);

   for ( long k = 0; k < 6; k += 2 ) {
      MoveToEx(hdc,pCursor -> ptlGraphicCursor[k].x,pCursor -> ptlGraphicCursor[k].y,(POINT *)NULL); 
      LineTo(hdc,pCursor -> ptlGraphicCursor[k + 1].x,pCursor -> ptlGraphicCursor[k + 1].y);
   }

   if ( gcPlotView3D == plotView ) {

      HPEN hPen,hPenOld;
      hPen = CreatePen(PS_SOLID,4,RGB(255,0,0));
      hPenOld = (HPEN)SelectObject(hdc,hPen);

      for ( long k = 6; k < 12; k += 2 ) {
         MoveToEx(hdc,pCursor -> ptlGraphicCursor[k].x,pCursor -> ptlGraphicCursor[k].y,(POINT *)NULL); 
         LineTo(hdc,pCursor -> ptlGraphicCursor[k + 1].x,pCursor -> ptlGraphicCursor[k + 1].y);
      }

      SelectObject(hdc,hPenOld);

   }

   if ( doPickBox )
      drawPickBox(hdc,pPtlPickBox);

   ReleaseDC(hwndGraphic,hdc);

   return 0;
   }


   void G::drawPickBox(HDC hdc,POINTL *pPtlPickBox) {

   MoveToEx(hdc,pPtlPickBox -> x - pickBoxSize.cx,pPtlPickBox -> y - pickBoxSize.cy,(POINT *)NULL); 

   LineTo(hdc,pPtlPickBox -> x + pickBoxSize.cx,pPtlPickBox -> y - pickBoxSize.cy);
   LineTo(hdc,pPtlPickBox -> x + pickBoxSize.cx,pPtlPickBox -> y + pickBoxSize.cy);
   LineTo(hdc,pPtlPickBox -> x - pickBoxSize.cx,pPtlPickBox -> y + pickBoxSize.cy);
   LineTo(hdc,pPtlPickBox -> x - pickBoxSize.cx,pPtlPickBox -> y - pickBoxSize.cy);
   
   ptlPickBox.x = pPtlPickBox -> x;
   ptlPickBox.y = pPtlPickBox -> y;

   return;
   }


   void G::savePickBox(HDC hdc,POINTL *pPtlPickBox) {

   HDC hdcTarget = CreateCompatibleDC(hdc);

   long x = pPtlPickBox -> x - pickBoxSize.cx - 2;
   long y = pPtlPickBox -> y - pickBoxSize.cy - 2;
   long cx = 2 * pickBoxSize.cx + 4;
   long cy = 2 * pickBoxSize.cy + 4;

   pickBoxBitmap = CreateCompatibleBitmap(hdc,cx,cy);

   HGDIOBJ oldBitmap = SelectObject(hdcTarget,pickBoxBitmap);

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



   int G::eraseGraphicCursor() {

   if ( ! useGraphicsCursor ) 
      return 0;

   HDC hdc = GetDC(hwndGraphic);

   //
   //NTC: 12-05-2017: I am going to rethink how to have a 3-D cursor.
   //
   if ( gcPlotView3D == plotView ) {
      erasePickBox(hdc);
      ReleaseDC(hwndGraphic,hdc);
      return 0;
   }

   HDC hdcSource = CreateCompatibleDC(NULL);

   graphicCursor *p = NULL;

   while ( p = graphicCursorList.GetFirst() ) {

      long itemCount = 3;
      if ( gcPlotView3D == plotView ) 
         itemCount = 6;

      for ( long k = 0; k < itemCount; k++ ) {

         HGDIOBJ oldBitmap = SelectObject(hdcSource,p -> crossBitmaps[k]);
         BitBlt(hdc,p -> ptlCrossBitmaps[k].x,p -> ptlCrossBitmaps[k].y,p -> sizelCrossBitmaps[k].cx,p -> sizelCrossBitmaps[k].cy,hdcSource,0,0,SRCCOPY);
         DeleteObject(p -> crossBitmaps[k]);
         p -> crossBitmaps[k] = NULL;

      }

      graphicCursorList.Remove(p);

      delete p;

   }

   DeleteDC(hdcSource);

   erasePickBox(hdc);

   ReleaseDC(hwndGraphic,hdc);

   return 0;
   }


   void G::erasePickBox(HDC hdcTarget) {

   if ( ( NULL == pickBoxBitmap ) )
      return;

   HDC hdcSource = CreateCompatibleDC(NULL);

   HGDIOBJ oldBitmap = SelectObject(hdcSource,pickBoxBitmap);

   BitBlt(hdcTarget,ptlPickBox.x - pickBoxSize.cx - 2,ptlPickBox.y - pickBoxSize.cy - 2,2 * pickBoxSize.cx + 4,2 * pickBoxSize.cy + 4,hdcSource,0,0,SRCCOPY);

   DeleteObject(pickBoxBitmap);

   DeleteDC(hdcSource);

   pickBoxBitmap = NULL;

   ptlPickBox.x = -1L;
   ptlPickBox.y = -1L;

   return;
   }