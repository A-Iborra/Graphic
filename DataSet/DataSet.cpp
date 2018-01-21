// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataSet.h"

   DataPoint movePoint = {-DBL_MAX,-DBL_MAX,-DBL_MAX};
 
   long oleMiscStatus = OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST | OLEMISC_INSIDEOUT | 
                           OLEMISC_CANTLINKINSIDE | OLEMISC_RECOMPOSEONRESIZE | OLEMISC_ALWAYSRUN;

   WNDPROC DataSet::nativeEditHandler = NULL;
   WNDPROC DataSet::nativeStaticHandler = NULL;
 
   DataSet::DataSet(IUnknown *pUnk) : 

     pWhenChangedCallback(NULL),
     pWhenChangedCallbackArg(NULL),
     whenChangedCallbackCookie(0L),

     isFunction(false),

     pUnknownOuter(pUnk),
     otherDomains() {

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

   memset(szName,0,sizeof(szName));
   memset(szDataSource,0,sizeof(szDataSource));
   memset(szNamedRange,0,sizeof(szNamedRange));
   memset(szCellRange,0,sizeof(szCellRange));
   memset(szSpreadsheetName,0,sizeof(szSpreadsheetName));
   memset(szExportWorkbookName,0,sizeof(szExportWorkbookName));
   memset(szExportWorksheetName,0,sizeof(szExportWorksheetName));
   memset(szExportWorksheetTopLeftCell,0,sizeof(szExportWorksheetTopLeftCell));
   memset(szEquations,0,sizeof(szEquations));

   pIOleInPlaceActiveObject = new _IOleInPlaceActiveObject(this);

   pIConnectionPoint = new _IConnectionPoint(this,IID_IDataSetEvents);

   IUnknown* pIUnknownThis;

   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));

   CoCreateInstance(CLSID_InnoVisioNateProperties,pIUnknownThis,CLSCTX_INPROC_SERVER,IID_IUnknown,reinterpret_cast<void **>(&pIUnknownProperties));

   pIUnknownProperties -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&pIGProperties));

   pIUnknownThis -> Release();

   IGPropertiesClient *pIPropertiesClient = NULL;

   QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void **>(&pIPropertiesClient));

   pIGProperties -> Advise(pIPropertiesClient);

   pIPropertiesClient -> Release();

   pIGProperties -> Add(L"state",&pPropertyState);

   pPropertyState -> directAccess(TYPE_BINARY,&propertiesStart,offsetof(DataSet,propertiesEnd) - offsetof(DataSet,propertiesStart));

   HRESULT rc = CoCreateInstance(CLSID_Plot,NULL,CLSCTX_INPROC_SERVER,IID_IPlot,reinterpret_cast<void **>(&pIPlot));

   pIGProperties -> Add(L"plots",&pPropertyPlots);

   pIGProperties -> Add(L"data",&pPropertyEmbeddedData);

   pPropertyEmbeddedData -> put_type(PropertyType::TYPE_RAW_BINARY);

   InitNew();

   CLSIDFromString(L"Excel.Application",&CLSID_excel);

   return;
   }
 

   DataSet::~DataSet() {

   ReSet();

   for ( IDataSet *pOther : otherDomains ) 
      pOther -> Release();
   otherDomains.clear();

   for ( extents *pExtents : extentsStack ) 
      delete pExtents;
   extentsStack.clear();

   delete pBoundingBox;

   if ( pIEvaluator )
      pIEvaluator -> Release();

   if ( pIUnknownProperties )
      pIUnknownProperties -> Release();

   if ( pIOleInPlaceSite ) 
      pIOleInPlaceSite -> Release();

   if ( pIOleClientSite ) 
      pIOleClientSite -> Release();

   if ( pIPropertyNotifySink ) 
      pIPropertyNotifySink -> Release();

   if ( pAdviseSink ) 
      pAdviseSink -> Release();

   if ( pIGProperties ) 
      pIGProperties -> Release();

   if ( enumConnectionPoints ) 
      delete enumConnectionPoints;

   delete pIConnectionPoint;

   return;
   }
 

   int DataSet::initWindows() {

   HWND hwndOwner = NULL;
 
   HRESULT hr = pIOleInPlaceSite -> GetWindow(&hwndOwner);

   if ( ! hwndOwner )
      return 0;

   DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDDIALOG_DATASET_SPEC),RT_DIALOG));

   hwndSpecDialog = CreateDialogIndirectParam(hModule,dt,hwndOwner,(DLGPROC)dataSetDialogHandler,(LPARAM)this);

   RECT rc;
   GetWindowRect(hwndSpecDialog,&rc);

   containerSize.cx = rc.right - rc.left;
   containerSize.cy = rc.bottom - rc.top;

   return 0;
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

   return rv;
   }
 
 
   int DataSet::resizeBounds() {
   adjustRange(&xMin,&xMax);
   adjustRange(&yMin,&yMax);
   adjustRange(&zMin,&zMax);
   return TRUE;
   }



   LRESULT EXPENTRY DataSet::overrideEditHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   if ( WM_CHAR == msg ) {
      MessageBeep(MB_ICONEXCLAMATION);
      return (LRESULT)0;
   }

   return CallWindowProc(DataSet::nativeEditHandler,hwnd,msg,wParam,lParam);
   }



   LRESULT EXPENTRY DataSet::statusAndErrorTextStaticHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   if ( WM_SETTEXT == msg ) {
      KillTimer(hwnd,1);
      if ( 0 < strlen((char *)lParam) ) {
         SetTimer(hwnd,1,ERROR_MESSAGE_DURATION,NULL);
         InvalidateRect(hwnd,NULL,TRUE);
      }
   }

   if ( WM_TIMER == msg ) {
      SetWindowText(hwnd,"");
      SetWindowPos(hwnd,HWND_BOTTOM,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
      KillTimer(hwnd,1);
   }

   if ( WM_PAINT == msg ) {

      PAINTSTRUCT ps = {0};
      BeginPaint(hwnd,&ps);
      char szText[1024];
      GetWindowText(hwnd,szText,1024);

      RECT rcThis,rcParent;
      GetWindowRect(GetParent(hwnd),&rcParent);
      GetWindowRect(hwnd,&rcThis);
      HDC hdcParent = GetDC(GetParent(hwnd));
      COLORREF crBackground = GetPixel(hdcParent,rcThis.left - rcParent.left - 4,rcThis.top - rcParent.top + 4);
      ReleaseDC(GetParent(hwnd),hdcParent);

      HFONT hGUIFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SelectObject(ps.hdc,hGUIFont);

      COLORREF cr = (COLORREF)GetWindowLongPtr(hwnd,GWLP_USERDATA);

      SetTextColor(ps.hdc,cr);

      SetBkColor(ps.hdc,crBackground);
      //FillRect(ps.hdc,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 1));
      DrawText(ps.hdc,szText,(DWORD)strlen(szText),&ps.rcPaint,DT_TOP);
      EndPaint(hwnd,&ps);
   }

   return CallWindowProc(DataSet::nativeStaticHandler,hwnd,msg,wParam,lParam);
   }