// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include "utils.h"
#include "Graphic_resource.h"

#include "List.cpp"

   long __stdcall G::_IGPropertiesClient::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
 
   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown*>(this); 
   else

   if ( riid == IID_IDispatch )
      *ppv = this;

   if ( riid == IID_IGPropertiesClient )
      *ppv = static_cast<IGPropertiesClient*>(this);
   else
 
      return pParent -> QueryInterface(riid,ppv);
 
   static_cast<IUnknown*>(*ppv) -> AddRef();
  
   return S_OK; 
   }
 
   unsigned long __stdcall G::_IGPropertiesClient::AddRef() {
   return 1;
   }
   unsigned long __stdcall G::_IGPropertiesClient::Release() {
   return 1;
   }


   HRESULT G::_IGPropertiesClient::SavePrep() {

   pParent -> dataSetCount = pParent -> dataSetList.Count();
   pParent -> functionCount = pParent -> functionList.Count();
   pParent -> textCount = pParent -> textList.Count();
 
   DataPoint dp[2];
   pParent -> pIDataSetMaster -> GetDomain(&dp[0],&dp[1]);

   pParent -> propertyDataExtents -> put_binaryValue(sizeof(dp),reinterpret_cast<byte *>(dp));

   if ( pParent -> hwndFrame ) {
      RECT rectFrame;
      GetWindowRect(pParent -> hwndFrame,&rectFrame);
      pParent -> containerSize.cx = rectFrame.right - rectFrame.left;
      pParent -> containerSize.cy = rectFrame.bottom - rectFrame.top;
   }

   pParent -> propertyFunctions -> clearStorageObjects();

   IGSFunctioNater *f = (IGSFunctioNater *)NULL;
   while ( f = pParent -> functionList.GetNext(f) ) 
      pParent -> propertyFunctions -> addStorageObject(f);

   pParent -> propertyFunctions -> writeStorageObjects();

   pParent -> propertyFunctions -> clearStorageObjects();

   pParent -> propertyDataSets -> clearStorageObjects();

   IDataSet *pds = (IDataSet *)NULL;
   while ( pds = pParent -> dataSetList.GetNext(pds) ) 
      pParent -> propertyDataSets -> addStorageObject(pds);

   pParent -> propertyDataSets -> writeStorageObjects();

   pParent -> propertyDataSets -> clearStorageObjects();


   pParent -> propertyTexts -> clearStorageObjects();

   IText *pIText = (IText *)NULL;
   while ( pIText = pParent -> textList.GetNext(pIText) ) 
      pParent -> propertyTexts -> addStorageObject(pIText);

   pParent -> propertyTexts -> writeStorageObjects();

   pParent -> propertyTexts -> clearStorageObjects();

   pParent -> propertyAxiis -> clearStorageObjects();

   IAxis *pIAxis = (IAxis *)NULL;
   while ( pIAxis = pParent -> axisList.GetNext(pIAxis) ) 
      pParent -> propertyAxiis -> addStorageObject(pIAxis);

   pParent -> propertyAxiis -> writeStorageObjects();

   pParent -> propertyAxiis -> clearStorageObjects();

   pParent -> propertyPlotServicesObject -> clearStorageObjects();
   pParent -> propertyPlotServicesObject -> addStorageObject(pParent -> pIPlotServicesObject);
   pParent -> propertyPlotServicesObject -> writeStorageObjects();
   pParent -> propertyPlotServicesObject -> clearStorageObjects();

   return S_OK;
   }


   HRESULT G::_IGPropertiesClient::InitNew() {

   IGSFunctioNater *pIFunction = (IGSFunctioNater *)NULL;
   while ( pIFunction = pParent -> functionList.GetFirst() ) {
      pParent -> deleteFunction(pIFunction);
      pParent -> functionList.Remove(pIFunction);
   }

   IDataSet *pIDataSet = (IDataSet *)NULL;
   while ( pIDataSet = pParent -> dataSetList.GetFirst() ) {
      pParent -> deleteDataSet(pIDataSet);
      pParent -> dataSetList.Remove(pIDataSet);
   }

   IText *pIText = (IText *)NULL;
   while ( pIText = pParent -> textList.GetFirst() ) {
      pIText -> Release();
      pParent -> textList.Remove(pIText);
   }

   IAxis *pIAxis = NULL;
   while ( pIAxis = pParent -> axisList.GetFirst() ) {
      pIAxis -> Release();
      pParent -> axisList.Remove(pIAxis);
   }

   float fvAmbient[4],fvDiffuse[4],fvSpecular[4];
   float clrBlack[] = {CLR_BLACK};
   float clrGreen[] = {CLR_GREEN};
   float clrWhite[] = {CLR_WHITE};
   float clrGray75[]= {CLR_GRAY75};
   float fv[4];

   pParent -> autoClear = true;

   pParent -> plotView = gcPlotView3D;
   pParent -> default2DPlotType = gcPlotType2DExternal1;
   pParent -> default3DPlotType = gcPlotType3DNone;
 
   pParent -> dataSetCount = 0;
   pParent -> textCount = 0;
   pParent -> functionCount = 0;
   pParent -> showFunctions = true;

   pParent -> propertyBackgroundColor -> put_binaryValue(4 * sizeof(float),(BYTE*)clrGray75);

   pParent -> propertyTextColor -> put_binaryValue(4 * sizeof(float),(BYTE*)clrBlack);
   pParent -> propertyTextBackgroundColor -> put_binaryValue(4 * sizeof(float),(BYTE*)clrWhite);
  
   pParent -> propertyViewTheta -> put_doubleValue(45.0);
   pParent -> propertyViewPhi -> put_doubleValue(30.0);
   pParent -> propertyViewSpin -> put_doubleValue(0.0);

   pParent -> denyUserPropertySettings = FALSE;

   pParent -> floorX = 0.0;
   pParent -> ceilingX = 0.0;

   pParent -> floorY = 0.0;
   pParent -> ceilingY = 0.0;

   pParent -> floorZ = 0.0;
   pParent -> ceilingZ = 1.0;
 
   pParent -> containerSize.cx = PLOT_DEFAULT_WIDTH;
   pParent -> containerSize.cy = PLOT_DEFAULT_HEIGHT;

   pParent -> showStatusBar = true;
   pParent -> autoPlotViewDetection = false;
   pParent -> useGraphicsCursor = true;

   // Margins

   pParent -> margins.left = PLOT_MARGIN_LEFT;
   pParent -> margins.right = PLOT_MARGIN_RIGHT;
   pParent -> margins.top = PLOT_MARGIN_TOP;
   pParent -> margins.bottom = PLOT_MARGIN_BOTTOM;

   strcpy(pParent -> plotMarginUnits,PLOT_MARGIN_UNITS);

   pParent -> showMargins = false;
   pParent -> stretchToMargins = false;
 
   // Colors and Lights

   fvAmbient[0] = 0.1f; fvAmbient[1] = 0.1f; fvAmbient[2] = 0.1f; fvAmbient[3] = 1.0f;

   fvDiffuse[0] = 0.1f; fvDiffuse[1] = 0.1f; fvDiffuse[2] = 0.1f; fvDiffuse[3] = 1.0f;

   fvSpecular[0] = 1.0f; fvSpecular[1] = 1.0f; fvSpecular[2] = 1.0f; fvSpecular[3] = 1.0f;

   for ( int k = 0; k < pParent -> supportedLightCount; k++ ) {

      pParent -> ppPropertyLightOn[k] -> put_boolValue(false);
      pParent -> ppPropertyAmbientLight[k] -> put_binaryValue(4 * sizeof(float),(BYTE*)fvAmbient);
      pParent -> ppPropertyDiffuseLight[k] -> put_binaryValue(4 * sizeof(float),(BYTE*)fvDiffuse);
      pParent -> ppPropertySpecularLight[k] -> put_binaryValue(4 * sizeof(float),(BYTE*)fvSpecular);

   }

   pParent -> ppPropertyLightOn[0] -> put_boolValue(true);

   fvAmbient[0] = 0.2f; fvAmbient[1] = 0.2f; fvAmbient[2] = 0.2f; fvAmbient[3] = 1.0f;
   fvDiffuse[0] = 0.35f; fvDiffuse[1] = 0.35f; fvDiffuse[2] = 0.35f; fvDiffuse[3] = 1.0f;
   fvSpecular[0] = 0.5f; fvSpecular[1] = 0.5f; fvSpecular[2] = 0.5f; fvSpecular[3] = 1.0f;
   pParent -> ppPropertyAmbientLight[1] -> put_binaryValue(4 * sizeof(float),(BYTE*)fvAmbient);
   pParent -> ppPropertyDiffuseLight[1] -> put_binaryValue(4 * sizeof(float),(BYTE*)fvDiffuse);
   pParent -> ppPropertySpecularLight[1] -> put_binaryValue(4 * sizeof(float),(BYTE*)fvSpecular);

   char szLightPos[256];

   sprintf(szLightPos,"MaxX + 1.1 * (MaxX - MinX),MinY + (MaxY - MinY)/2,MinZ + (MaxZ - MinZ)/2");       // Front
   pParent -> ppPropertyLightPos[0] -> put_szValue(szLightPos);

   sprintf(szLightPos,"MinX + (MaxX - MinX)/2,MaxY + 1.1 * (MaxY - MinY),MinZ + (MaxZ - MinZ)/2");       // Right
   pParent -> ppPropertyLightPos[1] -> put_szValue(szLightPos);

   sprintf(szLightPos,"MinX - 1.1 * (MaxX - MinX),MinY + (MaxY - MinY)/2,MaxZ + (MaxZ - MinZ)/2");       // Back
   pParent -> ppPropertyLightPos[2] -> put_szValue(szLightPos);

   sprintf(szLightPos,"MinX + (MaxX - MinX)/2,MinY - 1.1 * (MaxY - MinY),MinZ + (MaxZ - MinZ)/2");       // Left
   pParent -> ppPropertyLightPos[3] -> put_szValue(szLightPos);

   sprintf(szLightPos,"MinX + (MaxX - MinX)/2,MinY + (MaxY - MinY)/2,MaxZ + 1.1 * (MaxZ - MinZ)");       // Top
   pParent -> ppPropertyLightPos[4] -> put_szValue(szLightPos);

   sprintf(szLightPos,"MinX + (MaxX - MinX)/2,MinY + (MaxY - MinY)/2,MinZ - 1.1 * (MaxZ - MinZ)");       // Bottom
   pParent -> ppPropertyLightPos[5] -> put_szValue(szLightPos);

   sprintf(szLightPos,"MaxX + 1.1 * (MaxX - MinX),MaxY + 1.1 * (MaxY - MinY),MaxZ + 1.1 * (MaxZ - MinZ)");  // Front Right Upper Corner
   pParent -> ppPropertyLightPos[6] -> put_szValue(szLightPos);

   sprintf(szLightPos,"MinX - 1.1 * (MaxX - MinX),MaxY - 1.1 * (MaxY - MinY),MaxZ + 1.1 * (MaxZ - MinZ)");  // Back Left Upper Corner
   pParent -> ppPropertyLightPos[7] -> put_szValue(szLightPos);
 
   pParent -> propertyCountLights -> put_longValue(pParent -> supportedLightCount);
 
   fv[0] = 1.0f; fv[1] = 1.0f; fv[2] = 1.0f;
   pParent -> propertySpecularReference -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);
   pParent -> propertyAmbientReference -> put_binaryValue(4 * sizeof(float),(BYTE*)fv);
   pParent -> propertyShinyness -> put_longValue(50);
 
   PRINTDLG printDialog;
   memset(&printDialog,0,sizeof(PRINTDLG));
 
   printDialog.lStructSize = sizeof(PRINTDLG);
   printDialog.hwndOwner = (HWND)NULL;
   printDialog.hDevMode = NULL;
   printDialog.hDevNames = NULL;
   printDialog.Flags = PD_RETURNDC | PD_ALLPAGES;
 
   pParent -> propertyPrintProperties -> put_binaryValue(sizeof(PRINTDLG),reinterpret_cast<BYTE *>(&printDialog));
 
   pParent -> propertyCustomColors -> put_binaryValue(128,NULL);
 
   DataPoint dp[2];
   dp[0].x = 0.0;
   dp[0].y = 0.0;
   dp[0].z = 0.0;
   dp[1].x = 1.0;
   dp[1].y = 1.0;
   dp[1].z = 1.0;
   pParent -> propertyDataExtents -> put_binaryValue(sizeof(dp),reinterpret_cast<BYTE *>(dp));

   CoCreateInstance(CLSID_GSystemAxis,pParent -> pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IAxis,reinterpret_cast<void **>(&pParent -> xaxis));
   CoCreateInstance(CLSID_GSystemAxis,pParent -> pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IAxis,reinterpret_cast<void **>(&pParent -> yaxis));
   CoCreateInstance(CLSID_GSystemAxis,pParent -> pIUnknownOuter,CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,IID_IAxis,reinterpret_cast<void **>(&pParent -> zaxis));

   pParent -> xaxis -> AdviseGSystemStatusBar(pParent -> pIGSystemStatusBar);
   pParent -> yaxis -> AdviseGSystemStatusBar(pParent -> pIGSystemStatusBar);
   pParent -> zaxis -> AdviseGSystemStatusBar(pParent -> pIGSystemStatusBar);

   pParent -> axisList.Add(pParent -> xaxis);

   pParent -> xaxis -> put_Type('X');
   pParent -> yaxis -> put_Type('Y');
   pParent -> zaxis -> put_Type('Z');

   pParent -> axisList.Add(pParent -> yaxis);
   pParent -> axisList.Add(pParent -> zaxis);

   pIAxis = NULL;
   while ( pIAxis = pParent -> axisList.GetNext(pIAxis) ) {
      IGPropertiesClient *pIGPropertiesClient = NULL;
      pIAxis -> QueryInterface(IID_IGPropertiesClient,reinterpret_cast<void **>(&pIGPropertiesClient));
      pIGPropertiesClient -> InitNew();
      pIGPropertiesClient -> Release();
   }

   pParent -> reInitializeAxiis();

   return S_OK;//Loaded();
   }
 
 
   HRESULT G::_IGPropertiesClient::Loaded() {

   DataPoint dp[2];
   BYTE *pdp = (BYTE *)&dp[0];

   pParent -> propertyDataExtents -> get_binaryValue(sizeof(dp),reinterpret_cast<BYTE **>(&pdp));

   pParent -> pIDataSetMaster -> SetDomain(&dp[0],&dp[1]);

   long cntObjects = 0;

   pParent -> propertyFunctions -> get_storedObjectCount(&cntObjects);

   VARIANT_BOOL anyFunctionControlVisible = VARIANT_FALSE;

   if ( cntObjects ) {

      IGSFunctioNater *p = (IGSFunctioNater *)NULL;
      while ( p = pParent -> functionList.GetFirst() ) {
         pParent -> deleteFunction(p);
         pParent -> functionList.Remove(p);
      }

      for ( long k = 0; k < cntObjects; k++ )
         pParent -> newFunction(false);

      pParent -> propertyFunctions -> clearStorageObjects();

      p = (IGSFunctioNater *)NULL;
      while ( p = pParent -> functionList.GetNext(p) ) 
         pParent -> propertyFunctions -> addStorageObject(p);

      pParent -> propertyFunctions -> readStorageObjects();

      pParent -> propertyFunctions -> clearStorageObjects();

   }

   pParent -> propertyDataSets -> get_storedObjectCount(&cntObjects);

   if ( cntObjects ) {

      IDataSet *p = (IDataSet *)NULL;
      while ( p = pParent -> dataSetList.GetFirst() ) {
         pParent -> deleteDataSet(p);
         pParent -> dataSetList.Remove(p);
      }

      for ( long k = 0; k < cntObjects; k++ )
         pParent -> newDataSet(false);

      pParent -> propertyDataSets -> clearStorageObjects();

      IDataSet *pds = (IDataSet *)NULL;
      while ( pds = pParent -> dataSetList.GetNext(pds) ) 
         pParent -> propertyDataSets -> addStorageObject(pds);

      pParent -> propertyDataSets -> readStorageObjects();

      pParent -> propertyDataSets -> clearStorageObjects();

   }

   pParent -> propertyTexts -> get_storedObjectCount(&cntObjects);

   if ( cntObjects ) {

      IText *t = (IText *)NULL;
      while ( t = pParent -> textList.GetFirst() ) {
         t -> Release();
         pParent -> textList.Remove(t);
      }

      for ( long k = 0; k < cntObjects; k++ )
         pParent -> newText();

      pParent -> propertyTexts -> clearStorageObjects();

      t = (IText *)NULL;
      while ( t = pParent -> textList.GetNext(t) ) 
         pParent -> propertyTexts -> addStorageObject(t);

      pParent -> propertyTexts -> readStorageObjects();

      pParent -> propertyTexts -> clearStorageObjects();

   }

   pParent -> propertyAxiis -> get_storedObjectCount(&cntObjects);

   if ( cntObjects ) {

      pParent -> propertyAxiis -> clearStorageObjects();

      IAxis *a = (IAxis *)NULL;
      while ( a = pParent -> axisList.GetNext(a) ) 
         pParent -> propertyAxiis -> addStorageObject(a);

      pParent -> propertyAxiis -> readStorageObjects();

      pParent -> propertyAxiis -> clearStorageObjects();

   }

   if ( pParent -> hwndStatusBar ) {
      long n;
      pParent -> propertyStatusText -> get_size(&n);
      if ( n ) {
         char *pText = new char[n + 1];
         pParent -> propertyStatusText -> get_szValue(pText);
         pParent -> put_StatusText(0,pText);
         delete [] pText;
      }
   }

   pParent -> propertyPlotServicesObject -> get_storedObjectCount(&cntObjects);

   if ( cntObjects ) {

      pParent -> propertyPlotServicesObject -> clearStorageObjects();
      pParent -> propertyPlotServicesObject -> addStorageObject(pParent -> pIPlotServicesObject);
      pParent -> propertyPlotServicesObject -> readStorageObjects();
      pParent -> propertyPlotServicesObject -> clearStorageObjects();

   }

   pParent -> isInitializing = FALSE;

   return S_OK;
   }
 
 
   HRESULT G::_IGPropertiesClient::Saved() {
   return S_OK;
   }


   HRESULT G::_IGPropertiesClient::IsDirty() {
   return S_FALSE;
   }

   HRESULT G::_IGPropertiesClient::GetClassID(BYTE *pCLSID) {
   *(CLSID*)pCLSID = CLSID_GSystemGraphic;
   return S_OK;
   }
