/*

                       Copyright (c) 1999,2000,2001,2002 Nathan T. Clark

*/

#include "DataSet.h"

   BYTE nothing = 0xF;

   HRESULT DataSet::SavePrep() {

   if ( szCellRange[0] ) {
      memset(szNamedRange,0,sizeof(szNamedRange));
      memset(szSpreadsheetName,0,sizeof(szSpreadsheetName));
   }

   long countPoints;

   get_countPoints(&countPoints);

   if ( szDataSource[0] && ( szNamedRange[0] || ( szSpreadsheetName[0] && szCellRange[0] ) ) && 0 < countPoints )
      hasHeaderRowDetermined = true;
   else
      hasHeaderRowDetermined = false;

   if ( pPropertyPlots ) {

      pPropertyPlots -> clearStorageObjects();

      if ( pIPlot ) {
         pPropertyPlots -> addStorageObject(pIPlot);
         pPropertyPlots -> writeStorageObjects();
         pPropertyPlots -> clearStorageObjects();
      }

   }

   if ( isEmbedded ) {


      long storageSize = countPoints * sizeof(DataPoint);

      BYTE *pStorage = new BYTE[storageSize];

      DataPoint *pTarget = (DataPoint *)pStorage;

      DataList *pDataList = NULL;
      DataPoint dp;

      for ( long k = 0; k < countPoints; k++ ) {

         get(pDataList,&dp,&pDataList);

         memcpy(pTarget++,&dp,sizeof(DataPoint));

      }

      pPropertyEmbeddedData -> put_binaryValue(storageSize,pStorage);

      delete [] pStorage;

   } else {

      pPropertyEmbeddedData -> put_binaryValue(1,&nothing);

   }

   return S_OK;
   }


   HRESULT DataSet::InitNew() {
   return Loaded();
   }
 
 
   HRESULT DataSet::Loaded() {

   if ( pPropertyPlots ) {

      long cntObjects = 0;

      pPropertyPlots -> get_storedObjectCount(&cntObjects);

      if ( cntObjects ) {
         pPropertyPlots -> clearStorageObjects();
         pPropertyPlots -> addStorageObject(pIPlot);
         pPropertyPlots-> readStorageObjects();
         pPropertyPlots -> clearStorageObjects();
      }

   }

   long countBytes = 0L;

   pPropertyEmbeddedData -> get_size(&countBytes);

   if ( 1 < countBytes ) {

      isEmbedded = true;

      BYTE *pSource = NULL;

      pPropertyEmbeddedData -> get_binaryData(&pSource);

      BYTE *pEnd = pSource + countBytes;

      DataPoint *pDPSource = (DataPoint *)pSource;

      DataPoint dp;

      ReSet();

      long pointCount = countBytes / sizeof(DataPoint);

      for ( long k = 0; k < pointCount; k++ ) {

         memcpy(&dp,(BYTE *)pDPSource,sizeof(DataPoint));

         pushDataPoint(&dp);

         pDPSource++;

      }

   } else

      isEmbedded = false;

   if ( szDataSource[0] && ( szNamedRange[0] || ( szSpreadsheetName[0] && szCellRange[0] ) ) )
      hasHeaderRowDetermined = true;
   else
      hasHeaderRowDetermined = false;

   return S_OK;
   }
 
 
   HRESULT DataSet::Saved() {
   return S_OK;
   }

 
   HRESULT DataSet::IsDirty() {
   return S_FALSE;
   }


   HRESULT DataSet::GetClassID(BYTE *pCLSID) {
   *(CLSID*)pCLSID = CLSID_DataSet;
   return S_OK;
   }


