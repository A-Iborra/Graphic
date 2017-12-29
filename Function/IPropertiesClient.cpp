/*

                       Copyright (c) 1999,2000,2001,2002 Nathan T. Clark

*/

#include "Function.h"
#include "Graphic_resource.h"

#include "List.cpp"

   HRESULT Function::SavePrep() {

   if ( hwndSpecDialog )
      GetDlgItemText(hwndSpecDialog,IDDI_FUNCTION_EQUATION_ENTRY,expression,MAX_PROPERTY_SIZE);
 
   pIPropertyVariables -> clearStorageObjects();

   IVariable *v = (IVariable *)NULL;
   while ( v = pVariableList -> GetNext(v) ) 
      pIPropertyVariables -> addStorageObject(v);

   pIPropertyVariables -> writeStorageObjects();
   pIPropertyVariables -> clearStorageObjects();

   pIPropertyManuallyAddedVariables -> clearStorageObjects();

   while ( v = pManuallyAddedVariables -> GetNext(v) )
      pIPropertyManuallyAddedVariables -> addStorageObject(v);

   pIPropertyManuallyAddedVariables -> writeStorageObjects();
   pIPropertyManuallyAddedVariables -> clearStorageObjects();

   pIPropertyPlots -> clearStorageObjects();

   if ( pIPlot ) {
      pIPropertyPlots -> addStorageObject(pIPlot);
      pIPropertyPlots -> writeStorageObjects();
      pIPropertyPlots -> clearStorageObjects();
   }

   pIPropertyDataSets -> clearStorageObjects();

   if ( pIDataSet ) {
      pIPropertyDataSets -> addStorageObject(pIDataSet);
      pIPropertyDataSets -> writeStorageObjects();
      pIPropertyDataSets -> clearStorageObjects();
   }

   return S_OK;
   }


   HRESULT Function::InitNew() {
 
   memset(expression,0,sizeof(expression));
 
   switch ( functionCount ) {
   case 1:
      LoadString((HINSTANCE)hModule,IDSTRING_FUNCTION_DEFAULT1,expression,MAX_PROPERTY_SIZE);
      break;
   case 2:
      LoadString((HINSTANCE)hModule,IDSTRING_FUNCTION_DEFAULT2,expression,MAX_PROPERTY_SIZE);
      break;
   case 3:
      LoadString((HINSTANCE)hModule,IDSTRING_FUNCTION_DEFAULT3,expression,MAX_PROPERTY_SIZE);
      break;
   case 4:
      LoadString((HINSTANCE)hModule,IDSTRING_FUNCTION_DEFAULT4,expression,MAX_PROPERTY_SIZE);
      break;
   case 5:
      LoadString((HINSTANCE)hModule,IDSTRING_FUNCTION_DEFAULT5,expression,MAX_PROPERTY_SIZE);
      break;
   default:
      LoadString((HINSTANCE)hModule,IDSTRING_FUNCTION_DEFAULT1,expression,MAX_PROPERTY_SIZE);
      break;
   }

   allowUserProperties = true;
   allowUserPropertiesControls = true;

   expressionVisible = TRUE;
   resultsVisible = TRUE;
   variablesVisible = TRUE;
   controlsVisible = TRUE;
   startVisible = TRUE;
   pauseVisible = TRUE;
   resumeVisible = TRUE;
   stopVisible = TRUE;
   plotPropertiesVisible = TRUE;
   dataSetPropertiesVisible = TRUE;

#if 0
   strcpy(expressionLabel,"Specify some expression here");
   strcpy(resultsLabel,"results:");
#endif

   return Loaded();
   }
 
 
   HRESULT Function::Loaded() {

   long cntObjects;

   IVariable *v;
   while ( v = pVariableList -> GetFirst() )
      pVariableList -> Remove(v,&currentShowingVariable);
   
   while ( v = pManuallyAddedVariables -> GetFirst() ) 
      pManuallyAddedVariables -> Remove(v,&currentShowingVariable);

   pIPropertyManuallyAddedVariables -> get_storedObjectCount(&cntObjects);

   if ( cntObjects ) {

      for ( long k = 0; k < cntObjects; k++ )
         pManuallyAddedVariables -> Add(NULL);

      pIPropertyManuallyAddedVariables -> clearStorageObjects();

      while ( v = pManuallyAddedVariables -> GetNext(v) )
         pIPropertyManuallyAddedVariables -> addStorageObject(v);

      pIPropertyManuallyAddedVariables -> readStorageObjects();
      while ( v = pManuallyAddedVariables -> GetNext(v) ) {
         BSTR bstrName = SysAllocStringLen(NULL,64);
         v -> get_Name(&bstrName);
         long n = wcslen(bstrName) + 1;
         char *pszName = new char[n];
         memset(pszName,0,n);
         WideCharToMultiByte(CP_ACP,0,bstrName,-1,pszName,n,0,0);
         pManuallyAddedVariables -> Name(v,pszName);
         delete [] pszName;
         SysFreeString(bstrName);
      }

      pIPropertyManuallyAddedVariables -> clearStorageObjects();

   }

   pIPropertyVariables -> get_storedObjectCount(&cntObjects);

   if ( cntObjects ) {

      if ( expression[0] )
         parseExpression();

      pIPropertyVariables -> clearStorageObjects();

      v = NULL;
      while ( v = pVariableList -> GetNext(v) )
         pIPropertyVariables -> addStorageObject(v);

      pIPropertyVariables -> readStorageObjects();

      pIPropertyVariables -> clearStorageObjects();

   }

   pIPropertyPlots -> get_storedObjectCount(&cntObjects);

   if ( cntObjects ) {
      pIPropertyPlots -> clearStorageObjects();
      pIPropertyPlots -> addStorageObject(pIPlot);
      pIPropertyPlots-> readStorageObjects();
      pIPropertyPlots -> clearStorageObjects();
   }

   pIPropertyDataSets -> get_storedObjectCount(&cntObjects);

   if ( cntObjects ) {
      pIPropertyDataSets -> clearStorageObjects();
      pIPropertyDataSets -> addStorageObject(pIDataSet);
      pIPropertyDataSets-> readStorageObjects();
      pIPropertyDataSets -> clearStorageObjects();
   }

   return S_OK;
   }
 
 
   HRESULT Function::Saved() {
   return S_OK;
   }

 
   HRESULT Function::IsDirty() {
   return S_FALSE;
   }


   HRESULT Function::GetClassID(BYTE *pCLSID) {
   *(CLSID*)pCLSID = CLSID_GSystemFunctioNater;
   return S_OK;
   }


