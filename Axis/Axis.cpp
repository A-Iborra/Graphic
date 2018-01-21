// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "axis.h"

#include "general.h"
#include "Graphic_resource.h"

#include "plot_i.c"
#include "DataSet_i.c"

#include "list.cpp"

   Axis::Axis(IUnknown *pUnkOuter) 
   :
     selected(FALSE),
     pIUnknownOuter(pUnkOuter),
     pIOpenGLImplementation(NULL),
     pIEvaluator(NULL),
     pIGSystemStatusBar(NULL),

     pParentPropertyPlotView(NULL),
     pParentPropertyXFloor(NULL),
     pParentPropertyXCeiling(NULL),
     pParentPropertyYFloor(NULL),
     pParentPropertyYCeiling(NULL),
     pParentPropertyZFloor(NULL),
     pParentPropertyZCeiling(NULL),
     pParentPropertyOpenGLText(NULL),

     pIPlot(NULL),
     pLabel(NULL),
     refCount(0),
     defaultPatchPainter(NULL),
     defaultPatchPainter2(NULL),
     hwndOwner(0),
     hwndStyleSettings(0),
     hwndPositionSettings(0),
     hwndTextSettings(0),
     hwndColorSettings(0),
     currentPropertyTickLabelColor(NULL),
     currentPropertyAxisLabelColor(NULL),
     pIDataSetDomain(NULL),
     drawTickLabels(TRUE),
     doAutoPrecision(TRUE),

     tickCount(0),
     tickAbove(0.0),
     tickBelow(0.0),
     tickLength(0.0),
     gridLinesPerTick(0),
     tickPctAbove(0.0),
     drawAxisLabel(false),

     pWhenChangedCallback(NULL),
     pWhenChangedCallbackArg(NULL),

     hwndObjectWindow(0),
     hMainMenu(0),
     hMenu(0),

     phi(0.0),
     theta(0.0),

     type(0),
 
     textList() {
 
   memset(&rectPropertiesPosition,0,sizeof(RECT));
   memset(&origin,0,sizeof(DataPoint));
   memset(&endPoint,0,sizeof(DataPoint));
   memset(&minPoint,0,sizeof(DataPoint));
   memset(&maxPoint,0,sizeof(DataPoint));
   memset(&uvDirection,0,sizeof(DataPoint));

   dpLabelPosition.x = -DBL_MAX;
   dpLabelPosition.y = -DBL_MAX;
   dpLabelPosition.z = -DBL_MAX;

   IUnknown *pIUnknownThis;
   QueryInterface(IID_IUnknown,reinterpret_cast<void**>(&pIUnknownThis));

   CoCreateInstance(CLSID_InnoVisioNateProperties,
                      pIUnknownThis,
                      CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                      IID_IUnknown,
                      reinterpret_cast<void **>(&pIUnknownProperties));
 
   pIUnknownProperties -> QueryInterface(IID_IGProperties,reinterpret_cast<void**>(&pIGProperties));

   IGPropertyPageClient* pIPropertyPageClient;
   QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void**>(&pIPropertyPageClient));
   pIGProperties -> AdvisePropertyPageClient(pIPropertyPageClient,true);
   pIPropertyPageClient -> Release();
 
   IGPropertiesClient *pIPropertiesClient;
   QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void **>(&pIPropertiesClient));
   pIGProperties -> Advise(pIPropertiesClient);
   pIPropertiesClient -> Release();
 
   pIGProperties -> Add(L"custom colors"         ,&propertyCustomColors );
   propertyCustomColors -> put_type(TYPE_BINARY);
   propertyCustomColors -> put_size(4 * sizeof(double));

   pIGProperties -> Add(L"draw text",&propertyDrawText);
   pIGProperties -> Add(L"axis precision",&propertyAxisPrecision);
   pIGProperties -> Add(L"auto precision",&propertyAutoPrecision);
   pIGProperties -> Add(L"tick spaces per grid",&propertyGridLinesPerTick);
   pIGProperties -> Add(L"tick count",&propertyTickCount);
   pIGProperties -> Add(L"tick straddle style",&propertyTickStraddleStyle);
   pIGProperties -> Add(L"straddle pcnt above",&propertyTickStraddleAbove);
   pIGProperties -> Add(L"tick length",&propertyTickLength);
   pIGProperties -> Add(L"tick unit type",&propertyTickLengthUnits);

   pIGProperties -> Add(L"origin at minpoint",&propertyOriginAtMinpoint);
   pIGProperties -> Add(L"origin x value",&propertyOriginXValue);
   pIGProperties -> Add(L"origin y value",&propertyOriginYValue);
   pIGProperties -> Add(L"origin z value",&propertyOriginZValue);

   pIGProperties -> Add(L"endpoint at maxpoint",&propertyEndpointAtMaxpoint);
   pIGProperties -> Add(L"endpoint x value",&propertyEndpointXValue);
   pIGProperties -> Add(L"endpoint y value",&propertyEndpointYValue);
   pIGProperties -> Add(L"endpoint z value",&propertyEndpointZValue);

   pIGProperties -> Add(L"determines domain",&propertyDeterminesDomain);

   pIGProperties -> Add(L"properties position",&propertyPropertiesPosition );

   pIGProperties -> Add(L"label",&propertyLabel);
   pIGProperties -> Add(L"draw label",&propertyDrawLabel);
   pIGProperties -> Add(L"label size",&propertyAxisLabelSize);
   pIGProperties -> Add(L"label size units",&propertyAxisLabelSizeUnits);
   pIGProperties -> Add(L"label color",&propertyLabelColor);
   pIGProperties -> Add(L"label position natural",&propertyLabelPositionNatural);
   pIGProperties -> Add(L"label position x",&propertyLabelPositionXValue);
   pIGProperties -> Add(L"label position y",&propertyLabelPositionYValue);
   pIGProperties -> Add(L"label position z",&propertyLabelPositionZValue);
   pIGProperties -> Add(L"label angle perpendicular",&propertyLabelAnglePerpendicular);
   pIGProperties -> Add(L"label angle parallel",&propertyLabelAngleParallel);
   pIGProperties -> Add(L"label angle other",&propertyLabelAngleOther);
   pIGProperties -> Add(L"label angle other value",&propertyLabelAngleOtherValue);
   pIGProperties -> Add(L"label color track",&propertyAxisLabelColorTrackLineColor);

   pIGProperties -> Add(L"tick label size",&propertyTickLabelSize);
   pIGProperties -> Add(L"tick label size units",&propertyTickLabelSizeUnits);
   pIGProperties -> Add(L"tick label color",&propertyTickLabelColor);
   pIGProperties -> Add(L"tick label color track",&propertyTickLabelColorTrackLineColor);

   pIGProperties -> Add(L"text count",&propertyTextCount);

   pIGProperties -> Add(L"default font",&propertyDefaultFont);
   propertyDefaultFont -> put_type(TYPE_SZSTRING);
   propertyDefaultFont -> put_size(64);
 
   pIGProperties -> Add(L"line weight",&propertyLineWeight);
   propertyLineWeight -> put_type(TYPE_LONG);

   pIGProperties -> Add(L"line color",&propertyLineColor);

   pIGProperties -> Add(L"plot type",&propertyPlotType);

   pIGProperties -> DirectAccess(L"draw label",TYPE_BOOL,&drawAxisLabel,sizeof(drawAxisLabel));
   pIGProperties -> DirectAccess(L"draw text",TYPE_BOOL,&drawTickLabels,sizeof(drawTickLabels));

   pIGProperties -> DirectAccess(L"auto precision", TYPE_BOOL,&doAutoPrecision,sizeof(doAutoPrecision));
   pIGProperties -> DirectAccess(L"properties position", TYPE_BINARY,&rectPropertiesPosition,sizeof(RECT));
   pIGProperties -> DirectAccess(L"determines domain",TYPE_BOOL,&determinesDomain,sizeof(determinesDomain));

   pIGProperties -> DirectAccess(L"label position natural",TYPE_BOOL,&labelPositionNatural,sizeof(labelPositionNatural));
   pIGProperties -> DirectAccess(L"label angle perpendicular",TYPE_BOOL,&labelAnglePerpendicular,sizeof(labelAnglePerpendicular));
   pIGProperties -> DirectAccess(L"label angle parallel",TYPE_BOOL,&labelAngleParallel,sizeof(labelAngleParallel));
   pIGProperties -> DirectAccess(L"label angle other",TYPE_BOOL,&labelAngleOther,sizeof(labelAngleOther));

   pIGProperties -> DirectAccess(L"label color track",TYPE_BOOL,&axisLabelColorTrackLineColor,sizeof(axisLabelColorTrackLineColor));
   pIGProperties -> DirectAccess(L"tick label color track",TYPE_BOOL,&axisTickColorTrackLineColor,sizeof(axisTickColorTrackLineColor));

   propertyTickCount -> directAccess(TYPE_LONG,&tickCount,sizeof(tickCount));
   propertyTickLength -> directAccess(TYPE_DOUBLE,&tickLength,sizeof(tickLength));
   propertyGridLinesPerTick -> directAccess(TYPE_LONG,&gridLinesPerTick,sizeof(gridLinesPerTick));
   propertyTickStraddleAbove -> directAccess(TYPE_DOUBLE,&tickPctAbove,sizeof(tickPctAbove));

   CoCreateInstance(CLSID_Text,
                    NULL,
                    CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                    IID_IText,
                    reinterpret_cast<void **>(&pLabel));

   CoCreateInstance(CLSID_Text,
                    NULL,
                    CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                    IID_IText,
                    reinterpret_cast<void **>(&pRepresentativeText));

   pRepresentativeText -> put_Description(L"Values to appear along the axis, separated by commas.\nTo include a comma, specify \\, (backslash - comma).");

   pRepresentativeText -> put_EnablePositionSettings(FALSE);

   pRepresentativeText -> put_PartOfWorldDomain(FALSE);

   pRepresentativeText -> put_PartOfMainGraphic(FALSE);

   pIGProperties -> AddPropertyPage(pRepresentativeText);

   pIGProperties -> AddObject(pRepresentativeText);

   refCount = 0;

   return;
   }
 
 
   Axis::~Axis() {

   if ( pIPlot )
      pIPlot -> Release();

   IText *pIText = NULL;
   while ( pIText = textList.GetFirst() ) {
      pIText -> Release();
      textList.Remove(pIText);
   }

   if ( pIDataSetDomain )
      pIDataSetDomain -> Release();

   pLabel -> Release();

   pRepresentativeText -> Release();
   pRepresentativeText = NULL;

   if ( hMainMenu )
      DestroyMenu(hMainMenu);

   if ( hwndObjectWindow )
      DestroyWindow(hwndObjectWindow);

   return;
   }
 
 
   int Axis::initWindows() {

   WNDCLASS aClass;
 
   aClass.style = 0;
   aClass.lpfnWndProc = handler;
   aClass.cbClsExtra = 32;
   aClass.cbWndExtra = 32;
   aClass.hInstance = hModule;
   aClass.hIcon = NULL;
   aClass.hCursor = NULL;
   aClass.hbrBackground = 0;
   aClass.lpszMenuName = NULL;
   aClass.lpszClassName = "a-handler";
 
   RegisterClass(&aClass);
 
   hwndObjectWindow = CreateWindowEx(0,"a-handler","",0,0,0,0,0,HWND_DESKTOP,NULL,hModule,(void *)this);
 
   hMainMenu = LoadMenu((HINSTANCE)hModule,MAKEINTRESOURCE(IDMENU_AXIS));
   hMenu = GetSubMenu(hMainMenu,0);
 
   return TRUE;
   }
 
 
   IPlot *Axis::Plot() {
   return pIPlot;
   }
 
 
   BSTR Axis::axisValue(double numberIn,double spread,int *precision,double maxError,double *errorBack,
                         double *answer,int doPower,int *powerBack) {
   double number = numberIn;
   double rsign,test,error,spreadTest,lastError;
   char sign;
   long j,improving;
   char *c;
   char szNumber[256];
 
   if ( fabs(number / spread) <= maxError ) {
      *answer = 0.0;
      return SysAllocString(L"0");
   }
  
   memset(szNumber,0,64);
 
   if ( number < 0.0 ) {
      sign = '-';
      rsign = -1.0;
   } else {
      sign = ' ';
      rsign = 1.0;
   }
  
   number = fabs(number);
   j = (long)log10(number);
   if ( j < -5 || doPower ) {
      number = number * pow(10.0,-j);
      spreadTest = spread * pow(10.0,-j);
      *powerBack = j;
      test = myRound(number,*precision);
      error = fabs((1.0 - test / number) / spreadTest);
      lastError = error;
      improving = TRUE;
      while ( error  > maxError && improving) {
         *precision++;
         test = myRound(number,*precision);
         error = fabs( (test - number) / spreadTest);
         improving = error < lastError;
         lastError = error;
         if ( *precision > 16 ) break;
      }
      *answer = rsign * test;
      if ( sign != ' ' ) {
         szNumber[0] = sign;
         sprintf(szNumber + 1,"%4.*f",*precision,test);
         c = szNumber + 1;
         while ( (c < (szNumber + strlen(szNumber))) && *c == ' ' ) c++;
         strcpy(szNumber + 1,c);
      } else {
         sprintf(szNumber,"%4.*f",*precision,test);
         c = szNumber;
         while ( (c < (szNumber + strlen(szNumber))) && *c == ' ' ) c++;
         strcpy(szNumber,c);
      }
      strcpy(szNumber,c);
      *errorBack = lastError;
      if ( !strchr(szNumber,'.') ) {
         BSTR bstrNumber = SysAllocStringLen(NULL,strlen(szNumber));
         MultiByteToWideChar(CP_ACP,0,szNumber,-1,bstrNumber,strlen(szNumber));
         return bstrNumber;
      }
      c = szNumber + strlen(szNumber) - 1;
      while ( c > szNumber && *c == '0' ) 
         if ( *(c - 1) != '.' )
            *c-- = '\0';
         else
            break;

      BSTR bstrNumber = SysAllocStringLen(NULL,strlen(szNumber));
      MultiByteToWideChar(CP_ACP,0,szNumber,-1,bstrNumber,strlen(szNumber));
      return bstrNumber;
   }
  
   test = myRound(number,*precision);
   error = fabs( (test - number) / spread);
   lastError = error;
   improving = TRUE;
   while ( error > maxError && improving ) {
      *precision = *precision + 1;
      test = myRound(number,*precision);
      error = fabs( (test - number) / spread );
      improving = error <= lastError;
      lastError = error;
      if ( *precision > 16 ) break;
   }
   *answer = rsign * test; 
   if ( sign != ' ' ) {
      szNumber[0] = sign;
      sprintf(szNumber + 1,"%4.*f",*precision,test);
      c = szNumber + 1;
      while ( (c < (szNumber + strlen(szNumber))) && *c == ' ' ) c++;
      strcpy(szNumber + 1,c);
   } else {
      sprintf(szNumber,"%4.*f",*precision,test);
      c = szNumber;
      while ( (c < (szNumber + strlen(szNumber))) && *c == ' ' ) c++;
      strcpy(szNumber,c);
   }
   *errorBack = lastError;
   if ( !strchr(szNumber,'.') ) {
      BSTR bstrNumber = SysAllocStringLen(NULL,strlen(szNumber));
      MultiByteToWideChar(CP_ACP,0,szNumber,-1,bstrNumber,strlen(szNumber));
      return bstrNumber;
   }
   c = szNumber + strlen(szNumber) - 1;
   while ( c > szNumber && *c == '0' )
      if ( *(c - 1) != '.' ) 
         *c-- = '\0';
      else
         break;
   BSTR bstrNumber = SysAllocStringLen(NULL,strlen(szNumber));
   MultiByteToWideChar(CP_ACP,0,szNumber,-1,bstrNumber,strlen(szNumber));
   return bstrNumber;
   }
 
 
   double Axis::myRound(double x,int k) {
 
   if ( k > 31 ) return myRound(x,k - 1);
   if ( k < -31 ) return myRound(x,k + 1);
   
   double y,whole,fraction,power = pow(10.0,k);
   y = x * power;
   whole = (long)y;
   fraction = y - whole;
   if ( fraction >= 0.5 ) whole = whole + 1.0;
   return whole / power;
   }