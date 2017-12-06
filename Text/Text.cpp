/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002,2008 Nathan T. Clark

*/

#include <windows.h>
#include <commctrl.h>
                                
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <process.h>

#include <gl\gl.h>
#include <gl\glu.h>

#include "general.h"
#include "Graphic_resource.h"
#include "utils.h"

#include "Text.h"

#include "list.cpp"

   static bool didRegister = false;
   extern HMODULE hModule;
 
   Text::Text(IUnknown *pUnk) : 

       pIUnknownOuter(pUnk), 
       iPropertiesClient(), 

       pIProperties(NULL),

       pIOleClientSite(NULL),
       pIOleInPlaceSite(NULL),
       pOleAdviseHolder(NULL),
       pAmbientDispatch(NULL),
 
       oldSampleHandler(NULL),

       pIOpenGLImplementation(0),
       pIEvaluator(0),

       pIDataSet(0),
       pIDataSetBoundingBox(0),
       pIDataSetWorld(0),
       pIBasePlot(0),
       pIBasePlotBoundingBox(0),
       pIGSystemStatusBar(0),
       pITextNotify(0),

       hwndOwner(0),
       hwndFrame(0),
       hwndSample(0),
       hwndStyle(0),
       hwndContent(0),
       hwndOrientation(0),

       hMainMenu(0),
       hMenu(0),

       hFont(0),
       hOriginalFont(0),
       moveCursor(0),

       coordinatePlane(CoordinatePlane_XY),

#ifdef USE_OPENGL_RENDERING
       doOpenGLRendering(TRUE),
#else
       doOpenGLRendering(FALSE),
#endif

       flipHorizontal(FALSE),
       flipVertical(FALSE),
       showContentPropertyPage(TRUE),
       enablePositionSettings(TRUE),
       partOfMainGraphic(TRUE),

       partOfWorldDomain(FALSE),

       fontSize(12.0),
       fontWeight(FW_NORMAL),
       mWidth(0.0),
       mHeight(0.0),

       pszText(NULL)
   {
  
   refCount = 100;
 
   memset(&dpStart,0,sizeof(DataPoint));
   memset(&dpEnd,0,sizeof(DataPoint));
   memset(&dpSelectOffsetRestore,0,sizeof(DataPoint));
   memset(&ptSelectPoint,0,sizeof(POINT));
   memset(&ptSelectOffset,0,sizeof(POINT));

   memset(&logicalFont,0,sizeof(LOGFONT));

   defaultPatchPainter = (WNDPROC)NULL;

   CoCreateInstance(CLSID_InnoVisioNateProperties,NULL,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IGProperties,reinterpret_cast<void **>(&pIProperties));
 
   IGPropertiesClient *pIPropertiesClient = NULL;
   IGPropertyPageClient* pIPropertyPageClient = NULL;

   QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void **>(&pIPropertiesClient));
   QueryInterface(IID_IGPropertyPageClient,reinterpret_cast<void**>(&pIPropertyPageClient));

   pIProperties -> Advise(pIPropertiesClient);
   pIProperties -> AdvisePropertyPageClient(pIPropertyPageClient,true);

   pIPropertiesClient -> Release();
   pIPropertyPageClient -> Release();
 
   pIProperties -> Add(L"text color",&propertyTextColor);
   pIProperties -> Add(L"text font weight",NULL);
   pIProperties -> Add(L"text font",NULL);
   pIProperties -> Add(L"text font style",&propertyFaceStyle);
   pIProperties -> Add(L"text size",NULL);
   pIProperties -> Add(L"text size units",&propertySizeUnits);
   pIProperties -> Add(L"properties position",&propertyPropertiesPosition);
   propertyPropertiesPosition -> put_type(TYPE_BINARY);
   propertyPropertiesPosition -> put_size(4 * sizeof(long));

   pIProperties -> Add(L"line weight",&propertyLineWeight);
 
   pIProperties -> Add(L"text format",&propertyFormat);
   pIProperties -> Add(L"position string",&propertyPositionString);
   pIProperties -> Add(L"positionX",&propertyPositionX);
   pIProperties -> Add(L"positionY",&propertyPositionY);
   pIProperties -> Add(L"positionZ",&propertyPositionZ);
   propertyPositionString -> put_type(TYPE_STRING);
   propertyPositionX -> put_type(TYPE_STRING);
   propertyPositionY -> put_type(TYPE_STRING);
   propertyPositionZ -> put_type(TYPE_STRING);

   pIProperties -> Add(L"direction forward",&propertyForwardDirection);
   pIProperties -> Add(L"direction up",&propertyUpDirection);

   pIProperties -> Add(L"coordinate plane",&propertyCoordinatePlane);
   pIProperties -> Add(L"plane height",&propertyPlaneHeight);
   pIProperties -> Add(L"flip l-r",&propertyFlipHorizontal);
   pIProperties -> Add(L"flip t-b",&propertyFlipVertical);

   pIProperties -> Add(L"part of world domain",&propertyPartOfWorldDomain);
   pIProperties -> Add(L"show content property page",&propertyShowContentPropertyPage);
   pIProperties -> Add(L"enable position settings",&propertyEnablePositionSettings);

   pIProperties -> Add(L"description",&propertyDescription);
   propertyDescription -> put_type(TYPE_STRING);

   pIProperties -> Add(L"part of main graphic",&propertyPartOfMainGraphic);

   propertyFormat -> directAccess(TYPE_LONG,reinterpret_cast<void*>(&format),sizeof(format));

   propertyForwardDirection -> directAccess(TYPE_BINARY,reinterpret_cast<void*>(&directionForward),sizeof(directionForward));
   propertyUpDirection -> directAccess(TYPE_BINARY,reinterpret_cast<void*>(&directionUp),sizeof(directionUp));

   pIProperties -> DirectAccess(L"text font",TYPE_SZSTRING,reinterpret_cast<void*>(szFace),MAX_PATH);
   pIProperties -> DirectAccess(L"text size",TYPE_DOUBLE,reinterpret_cast<void*>(&fontSize),sizeof(double));
   pIProperties -> DirectAccess(L"text font weight",TYPE_DOUBLE,reinterpret_cast<void*>(&fontWeight),sizeof(double));

   propertyCoordinatePlane -> directAccess(TYPE_LONG,reinterpret_cast<void*>(&coordinatePlane),sizeof(coordinatePlane));
   propertyFlipHorizontal -> directAccess(TYPE_BOOL,reinterpret_cast<void*>(&flipHorizontal),sizeof(flipHorizontal));
   propertyFlipVertical -> directAccess(TYPE_BOOL,reinterpret_cast<void*>(&flipVertical),sizeof(flipVertical));
   propertyPlaneHeight -> directAccess(TYPE_DOUBLE,reinterpret_cast<void*>(&planeHeight),sizeof(planeHeight));
   propertyPartOfWorldDomain -> directAccess(TYPE_BOOL,reinterpret_cast<void *>(&partOfWorldDomain),sizeof(partOfWorldDomain));
   propertyShowContentPropertyPage -> directAccess(TYPE_BOOL,reinterpret_cast<void *>(&showContentPropertyPage),sizeof(showContentPropertyPage));
   propertyEnablePositionSettings -> directAccess(TYPE_BOOL,reinterpret_cast<void *>(&enablePositionSettings),sizeof(enablePositionSettings));
   propertyPartOfMainGraphic -> directAccess(TYPE_BOOL,reinterpret_cast<void *>(&partOfMainGraphic),sizeof(partOfMainGraphic));

   CoCreateInstance(CLSID_BasePlot,pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IBasePlot,reinterpret_cast<void **>(&pIBasePlot));

   CoCreateInstance(CLSID_BasePlot,pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IBasePlot,reinterpret_cast<void **>(&pIBasePlotBoundingBox));
 
   iPropertiesClient.InitNew();
 
   memset(&containerSize,0,sizeof(SIZEL));
 
   if ( ! didRegister ) {

      didRegister = true;

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
      aClass.lpszClassName = "t-handler";

      RegisterClass(&aClass);
   }
 
   hwndObjectWindow = CreateWindowEx(WS_EX_TRANSPARENT,"t-handler","",0,0,0,0,0,HWND_DESKTOP,NULL,hModule,(void *)this);

   SetWindowLongPtr(hwndObjectWindow,GWLP_USERDATA,(ULONG_PTR)this);

   hMainMenu = LoadMenu((HINSTANCE)hModule,MAKEINTRESOURCE(IDMENU_TEXT));

   hMenu = GetSubMenu(hMainMenu,0);
 
   refCount = 0;

   pIPropertiesClient -> InitNew();
 
   return;
   }
 
 
   Text::~Text() {

   DestroyWindow(hwndObjectWindow);
   DestroyMenu(hMainMenu);

   pIProperties -> Release();

   if ( pIBasePlot )
      pIBasePlot -> Release();

   pIBasePlot = NULL;

   if ( pIBasePlotBoundingBox )
      pIBasePlotBoundingBox -> Release();

   pIBasePlotBoundingBox = NULL;

   if ( pIDataSet ) 
      pIDataSet -> Release();

   pIDataSet = NULL;

   if ( pIDataSetBoundingBox ) 
      pIDataSetBoundingBox -> Release();

   pIDataSetBoundingBox = NULL;

   if ( pIDataSetWorld ) 
      pIDataSetWorld -> Release();

   pIDataSetWorld = NULL;

   if ( propertySizeUnits )
      propertySizeUnits -> Release();

   propertySizeUnits = NULL;

   if ( pITextNotify )
      pITextNotify -> Release();

   pITextNotify = NULL;

   return;
   }


   int __stdcall Text::fontListEnumerator(const LOGFONTA *lfa,const TEXTMETRICA *,DWORD type,LPARAM arg) {
   if ( type != TRUETYPE_FONTTYPE ) return 1;
   long itemIndex;
   HWND hwndList = (HWND)arg;
   if ( (itemIndex = SendMessage(hwndList,CB_FINDSTRINGEXACT,-1L,reinterpret_cast<LPARAM>(lfa -> lfFaceName))) == CB_ERR )
      itemIndex = SendMessage(hwndList,CB_ADDSTRING,0L,reinterpret_cast<LPARAM>(lfa -> lfFaceName));
   if ( lfa -> lfCharSet != ANSI_CHARSET && lfa -> lfCharSet != DEFAULT_CHARSET ) return 1;
   fontListData *flData = reinterpret_cast<fontListData *>(SendMessage(hwndList,CB_GETITEMDATA,itemIndex,0L));
   if ( ! flData ) {
      flData = new fontListData();
      flData -> countVariations = 1;
      memcpy(flData -> lf,lfa,sizeof(LOGFONTA));
   } else {
      BYTE *b = new BYTE[sizeof(long) + (flData -> countVariations + 1) * sizeof(LOGFONTA)];
      memcpy(b,flData,sizeof(long) + flData -> countVariations * sizeof(LOGFONTA));
      delete [] flData;
      flData = reinterpret_cast<fontListData *>(b);
      flData -> countVariations++;
      memcpy(&flData -> lf[flData -> countVariations - 1],lfa,sizeof(LOGFONTA));
   }
   SendMessage(hwndList,CB_SETITEMDATA,itemIndex,reinterpret_cast<LPARAM>(flData));
   return 1;
   }
 
 
   int __stdcall Text::fontSelectEnumerator(const LOGFONTA *lfa,const TEXTMETRICA *ptm,DWORD,LPARAM arg) {
   Text *p = reinterpret_cast<Text *>(arg);
   return 0;
   }


   int Text::statusPosition() {
   char szText[MAX_PROPERTY_SIZE];
   sprintf(szText,"Text(%lf,%lf,%lf)",dpStart.x,dpStart.y,dpStart.z);
   if ( pIGSystemStatusBar )
      pIGSystemStatusBar -> put_StatusText(0,szText);
   return 0;
   }

   int Text::createFont(LOGFONT *pLogFont) {
   if ( ! pIOpenGLImplementation )
      return 0;
   if ( hFont )
      DeleteObject(hFont);
   HDC hdc;
   pIOpenGLImplementation -> get_HDC(&hdc);
   if ( ! szFace[0] ) 
      strcpy(szFace,DEFAULT_FONT);
   pLogFont -> lfHeight = -MulDiv((long)fontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
   pLogFont -> lfQuality = PROOF_QUALITY;
   pLogFont -> lfWeight = (long)fontWeight;
   strcpy(pLogFont -> lfFaceName,szFace);
   pLogFont -> lfCharSet = DEFAULT_CHARSET;
   hFont = CreateFontIndirect(pLogFont);
   return 1;
   }
