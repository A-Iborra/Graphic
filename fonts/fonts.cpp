
#include <Windows.h>

/***************************************************************************
 * Glyph viewing program by Steven Reichenthal, 1993. Although this code
 * is basically C, it uses certain C++ constructs such as in-place
 * declaration of variables that require use of a C++ compiler.
 **************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <commdlg.h>
#include <math.h>

#include "resource.h"

//#define IDM_NEW                 101     // File Menu ID's
//#define IDM_EXIT                108

#define MARKERSIZE              4

//#define IDM_SHOWCONTROLPOINTS   402
//#define IDM_FILL                403
//#define IDM_OUTLINE             404
//#define IDM_NORMAL              405
//#define IDM_PINCH               406
//#define IDM_PUNCH               407

//long FAR PASCAL WndProc (HWND hwnd, unsigned iMessage, WORD wParam,LONG lParam);
LRESULT APIENTRY WndProc (HWND hwnd, unsigned iMessage, WORD wParam, LONG lParam);

HANDLE hInst;
HWND hwnd;
HDC             hdc;
PAINTSTRUCT     ps;
HWND hwndFontsFrame;
WORD            maxClient;
WORD            cxClient,cyClient;
LOGFONT         lf;
CHOOSEFONT      cf;
int             character;
int             nEffect;
BOOL            bShowControlPoints = TRUE;
BOOL            bFill;
BOOL            bOutline = TRUE;
float           pi;

static char  szAppName [] = "TrueType Font Demo";

/*----------------------------------------------------------------------*/
// set the caption for the frame window
void set_frame_caption ()
{
    char sz [80];
    wsprintf (sz, "%s - %s", (LPSTR) szAppName, (LPSTR) lf.lfFaceName);
    SetWindowText (hwndFontsFrame, sz);
}
/*----------------------------------------------------------------------*/
#pragma argsused

   int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpszCmdLine, int nCmdShow) {

   HWND hwnd;
   MSG msg;
   hInst = hInstance;

   WNDCLASS wc = {0};
   wc.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNWINDOW;
   wc.lpfnWndProc = (WNDPROC) WndProc;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hInstance = hInstance;
   wc.hIcon = 0;
   wc.hCursor = LoadCursor (NULL, IDC_ARROW);
   wc.hbrBackground = NULL;//(HBRUSH)GetStockObject(WHITE_BRUSH);
   //wc.lpszMenuName = "MENU_1";
   wc.lpszClassName = "FONTS";

   RegisterClass(&wc);

   pi = (float)atan2 (0, -1);

   lf.lfHeight = -400;

   lf.lfWeight = 400;

   strcpy (lf.lfFaceName, "Tahoma");

   character = '8';//'a';

   HMENU hMenu = LoadMenu(NULL,MAKEINTRESOURCE(IDR_MAIN_MENU));

   hwnd = CreateWindowEx(0L,"FONTS",szAppName,
                           WS_OVERLAPPEDWINDOW,// | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                           100, 100, 1024,768,
                           NULL, hMenu, hInstance, NULL);

    hwndFontsFrame = hwnd;

    set_frame_caption ();

    ShowWindow (hwnd, nCmdShow);

    UpdateWindow (hwnd);

    while (GetMessage (&msg, NULL, NULL, NULL)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    return 0;
}


   int xOffset,yOffset;

   struct LONGPOINT {
    long x, y;
   };


   inline int fixed_to_int (long value) { // convert a fixed value to an integer
   return (value + 32767) >> 16;
   }


// draw the array of control points

   void draw_control_points(POINTFX pt [], int nPoints) {   

   LONGPOINT *p = (LONGPOINT *) pt;

   if ( ! bShowControlPoints )
      return;

   POINT ptPoint;

ptPoint.x = xOffset + fixed_to_int(p[0].x);
ptPoint.y = yOffset - fixed_to_int(p[0].y);

MoveToEx(hdc,ptPoint.x,ptPoint.y,NULL);

   for ( int i = 0; i < nPoints; i++) {

      ptPoint.x = xOffset + fixed_to_int (p [i].x);

      ptPoint.y = yOffset - fixed_to_int (p [i].y);

LineTo(hdc,ptPoint.x,ptPoint.y);

      PatBlt (hdc,ptPoint.x - MARKERSIZE / 2, ptPoint.y - MARKERSIZE / 2,MARKERSIZE, MARKERSIZE, BLACKNESS);
   }

   return;
   }

#define DEPTH                   8       // Bezier recursion depth
#define MAX_BEZIER_POINTS       ((1 << DEPTH) + 1)

static POINT  *     Pts;            // array of contour points
static POINT  *     pPts;           // current point
static POINT  *     pPtsStart;      // first contour point
static int *            polyCounts;     // array of coutour point counts
static int              nContours;      // number of contours
static GLYPHMETRICS     gm;

/*----------------------------------------------------------------------*/
// transform the array of control points

   void transform (POINTFX pt [], int nPoints) {

   LONGPOINT *p = (LONGPOINT *) pt;

   switch (nEffect) {
   case IDC_NORMAL:
      return;

    case IDC_PINCH:
        while (nPoints--)
        {
            float xx = 2.0f * pi * float ((p->x >> 16) - gm.gmptGlyphOrigin.x) / float (gm.gmBlackBoxX);
            float yy = 2.0f * pi * float ((p->y >> 16) - gm.gmptGlyphOrigin.y) / float (gm.gmBlackBoxY);
            p->x = p->x + float (gm.gmBlackBoxX) / 10.0f * 65536.0f * sin (xx);
            p->y = p->y + float (gm.gmBlackBoxY) / 10.0f * 65536.0f * sin (yy);
            p++;
        }
        break;

    case IDC_PUNCH:
        while (nPoints--)
        {
            float xx = 2 * pi * float ((p->x >> 16) - gm.gmptGlyphOrigin.x) / float (gm.gmBlackBoxX);
            float yy = 2 * pi * float ((p->y >> 16) - gm.gmptGlyphOrigin.y) / float (gm.gmBlackBoxY);
            p->x = p->x + float (gm.gmBlackBoxX) / 5.0 * 32768.0 * (1 + cos (yy) * sin (xx));
            p->y = p->y + float (gm.gmBlackBoxY) / 5.0 * 32768.0 * (1 + cos (xx) * sin (yy));
            p++;
        }
        break;
    }
}

// store a point in the array of contour points

   void store(LONGPOINT pt) {

   pPts -> x = xOffset + fixed_to_int(pt.x);

   pPts -> y = yOffset - fixed_to_int(pt.y);

   pPts++;

   }

// sub-divide the quadratic Bezier curve

   void near pascal subDivide(LONGPOINT p[]) {

   static int depth = DEPTH;

   LONGPOINT q[8];

   int x = xOffset + fixed_to_int(p[2].x);

   int y = yOffset - fixed_to_int(p[2].y);

   if ( x == pPts[-1].x && y == pPts[-1].y )
      return;

   if ( ! depth ) {
      store(p[2]);
      return;
   }

   q[0] = p[0];
   q[4] = p[2];

   q[1].x = (p[0].x + p[1].x) >> 1;
   q[3].x = (p[1].x + p[2].x) >> 1;
   q[2].x = (q[1].x + q[3].x) >> 1;

   q[1].y = (p[0].y + p[1].y) >> 1;
   q[3].y = (p[1].y + p[2].y) >> 1;
   q[2].y = (q[1].y + q[3].y) >> 1;

   depth--;
   subDivide(q);

   subDivide(q + 2);
   depth++;
}

// draw the quadratic Bezier curve

   void drawBezierCurve(LONGPOINT p[]) {
   store(p[0]);
   subDivide(p);
   }

// draw the quadratic B-spline from the array of points

   void drawQuadraticBSpline(POINTFX pt[], int nPoints) {

   LONGPOINT b[3];
   LONGPOINT *p = (LONGPOINT *)pt;

   if ( 3 == nPoints ) {
      drawBezierCurve(p);
      return;
   }

   b[0] = p[0];
   b[1] = p[1];
   b[2].x = (p[1].x + p[2].x) >> 1;
   b[2].y = (p[1].y + p[2].y) >> 1;

   drawBezierCurve(b);

   for ( int i = 1; i < nPoints - 3; i++ ) {

      b[0].x = (p[i].x + p[i + 1].x) >> 1;
      b[0].y = (p[i].y + p[i + 1].y) >> 1;
      b[1] = p[i + 1];
      b[2].x = (p[i + 1].x + p[i + 2].x) >> 1;
      b[2].y = (p[i + 1].y + p[i + 2].y) >> 1;

      drawBezierCurve(b);
   }

   int i = nPoints - 3;

   b[0].x = (p[i].x + p[i + 1].x) >> 1;
   b[0].y = (p[i].y + p[i + 1].y) >> 1;
   b[1] = p[i + 1];
   b[2] = p[i + 2];

   drawBezierCurve(b);

   return;
   }

// draw the polyline from the array of points

   void draw_polyline(POINTFX points [], int nPoints) {

   LONGPOINT *p = (LONGPOINT *) points;

   for (int i = 0; i < nPoints; i++)
      store (p [i]);

   return;
   }

// calculate the number of bytes needed for the array of contour points

DWORD compute_memory_requirement (TTPOLYGONHEADER *header, DWORD cbBuffer)
{
    DWORD count = 1;
    do
    {
        TTPOLYGONHEADER *nextHeader =
        (TTPOLYGONHEADER *) (header->cb + (char *) header);
        TTPOLYCURVE *curve = (TTPOLYCURVE *) (header + 1);
        POINTFX pfxStart = header->pfxStart;

        while (1)
        {
            UINT cpfx = curve->cpfx + 1;
            POINTFX *ppfx = curve->apfx - 1;
            POINTFX pfxEnd = ppfx [cpfx - 1];

            if (curve->wType == TT_PRIM_LINE)
                count += cpfx;
            else
                count += (cpfx - 2) * MAX_BEZIER_POINTS;

            curve = (TTPOLYCURVE *) (ppfx + cpfx);
            if (nextHeader <= (TTPOLYGONHEADER *) curve)
            {
                if (memcmp (&pfxEnd, &pfxStart, sizeof (pfxEnd)))
                    count += 2;
                break;
            }
        }
        count++;
        cbBuffer -= header->cb;
        header = nextHeader;
    }
    while (cbBuffer);

    return count * (DWORD) sizeof (POINT);
}


   int drawGlyphOutline(HDC hdc, int x, int y, int ch) {

    TEXTMETRIC tm;
    MAT2 mat2;

    GetTextMetrics (hdc, &tm);
    xOffset = x;
    yOffset = (y + tm.tmAscent);

    memset (&mat2, 0, sizeof (mat2));

    mat2.eM11.value = 1;
    mat2.eM22.value = 1;

    DWORD cbBuffer = GetGlyphOutline (hdc,ch, GGO_NATIVE, &gm, 0, NULL, &mat2);

    if ( long (cbBuffer) <= 0 || cbBuffer > 32767 )
        return 0;

    void *buffer = malloc (int (cbBuffer));

    GetGlyphOutline (hdc, ch, GGO_NATIVE, &gm, cbBuffer, buffer, &mat2);

    TTPOLYGONHEADER *header = (TTPOLYGONHEADER *) buffer;

    DWORD cbPolygons = compute_memory_requirement (header, cbBuffer);
    HANDLE hPolygons = GlobalAlloc (GMEM_FIXED, cbPolygons);

    if ( ! hPolygons )  {
        free (buffer);
        return 0;
    }

    Pts = (POINT *) GlobalLock (hPolygons);

    pPts = Pts;

    nContours = 0;

    polyCounts = ((int *) header) + 2;  //use the area beyond cb for the counts

    do {

        TTPOLYGONHEADER *nextHeader = (TTPOLYGONHEADER *) (header->cb + (char *) header);

        TTPOLYCURVE *curve = (TTPOLYCURVE *) (header + 1);

        POINTFX pfxEnd = header->pfxStart;

        POINTFX pfxStart = pfxEnd;

        pPtsStart = pPts;

        while ( 1 ) {

            UINT wType = curve->wType;
            UINT cpfx = curve->cpfx + 1;
            POINTFX *ppfx = curve->apfx - 1;

            ppfx [0] = pfxEnd; // this overwrites 8 bytes before apfx,
            // but we are done with them at this point.

            pfxEnd = ppfx [cpfx - 1];

            transform (ppfx, cpfx);

            draw_control_points(ppfx, cpfx);

            if ( wType == TT_PRIM_LINE )
                draw_polyline (ppfx, cpfx);
            else
                drawQuadraticBSpline(ppfx, cpfx);

            curve = (TTPOLYCURVE *) (ppfx + cpfx);
            if (nextHeader <= (TTPOLYGONHEADER *) curve)
            {
                if (memcmp (&pfxEnd, &pfxStart, sizeof (pfxEnd)))
                {
                    ppfx [0] = pfxEnd;
                    ppfx [1] = pfxStart;
                    transform (ppfx, 2);
                    draw_polyline (ppfx, 2);
                }
                break;
            }
        }

        *pPts++ = *pPtsStart;
        polyCounts [nContours++] = pPts - pPtsStart;
        cbBuffer -= header->cb;
        header = nextHeader;

    } while ( cbBuffer );

    //PolyPolygon (hdc, (LPPOINT) Pts, polyCounts, nContours);

    GlobalUnlock (hPolygons);
    GlobalFree (hPolygons);

    free (buffer);

   return 1;

   }


   LRESULT APIENTRY WndProc (HWND hwnd, unsigned msg, WORD wParam, LONG lParam) {

   const long MF [2] = { MF_UNCHECKED, MF_CHECKED };

   switch(msg) {

   case WM_CREATE:
      return (LRESULT)TRUE;

   case WM_NCCREATE:
      return (LRESULT)TRUE;

   case WM_COMMAND: 

      switch (wParam) {

      case IDC_NEW:
            cf.lStructSize = sizeof (cf);
            cf.hwndOwner = hwnd;
            cf.lpLogFont = &lf;
            cf.Flags = CF_SCREENFONTS | CF_TTONLY | CF_FORCEFONTEXIST | CF_INITTOLOGFONTSTRUCT;
            cf.nFontType = SCREEN_FONTTYPE;

            if (ChooseFont (&cf))
            {
                //set_frame_caption ();
                InvalidateRect (hwnd, NULL, TRUE);
            }
            break;

      case IDC_EXIT:
            SendMessage (hwnd, WM_CLOSE, 0, 0L);
            break;

      case IDC_SHOWCONTROLPOINTS:
            bShowControlPoints ^= TRUE;
            CheckMenuItem (GetMenu (hwnd), IDC_SHOWCONTROLPOINTS,MF [bShowControlPoints]);
            InvalidateRect (hwnd, NULL, TRUE);
            break;

      case IDC_FILL:

         bFill ^= TRUE;
         CheckMenuItem (GetMenu (hwnd), IDC_FILL, MF [bFill]);
         InvalidateRect (hwnd, NULL, TRUE);
         break;

        case IDC_OUTLINE:
            bOutline ^= TRUE;
            CheckMenuItem (GetMenu (hwnd), IDC_OUTLINE, MF [bOutline]);
            InvalidateRect (hwnd, NULL, TRUE);
            break;
        case IDC_NORMAL:
        case IDC_PINCH:
        case IDC_PUNCH:
            nEffect = wParam;
            CheckMenuItem (GetMenu (hwnd),IDC_NORMAL, MF [wParam == IDC_NORMAL]);
            CheckMenuItem (GetMenu (hwnd), IDC_PINCH, MF [wParam == IDC_PINCH]);
            CheckMenuItem (GetMenu (hwnd),IDC_PUNCH, MF [wParam == IDC_PUNCH]);
            InvalidateRect (hwnd, NULL, TRUE);
            break;
        }
        break;

    case WM_SIZE:
        cxClient = LOWORD (lParam);
        cyClient = HIWORD (lParam);
        break;

    case WM_CHAR:
        character = wParam;
        InvalidateRect (hwnd, NULL, TRUE);
        break;

    case WM_PAINT: {

        hdc = BeginPaint (hwnd, &ps);

        HFONT hFont = (HFONT)SelectObject (hdc, CreateFontIndirect (&lf));

        //int PolyFillMode = SetPolyFillMode (hdc, ALTERNATE);

        HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));//(HBRUSH)SelectObject (hdc,bFill ? CreatePatternBrush(hBitmap) : GetStockObject (NULL_BRUSH));

        SelectObject(hdc,hBrush);

        HPEN hPen = (HPEN)SelectObject (hdc, bOutline ? GetStockObject (BLACK_PEN) : GetStockObject (NULL_PEN));

        drawGlyphOutline(hdc, 100, 0, character);

        SelectObject (hdc, hPen);

        hBrush = (HBRUSH)SelectObject (hdc, hBrush);

        if (bFill)
        {
            DeleteObject (hBrush);
        }

        //SetPolyFillMode (hdc, PolyFillMode);

        DeleteObject (SelectObject (hdc, hFont));

        EndPaint (hwnd, &ps);

        }
        break;

    case WM_QUERYENDSESSION:
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage (0);
        return 1;

   }

   return DefWindowProc(hwnd, msg, wParam, lParam);
   }