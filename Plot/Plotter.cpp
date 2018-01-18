
#include "plot.h"

#include "utils.h"


   unsigned int __stdcall Plot::plotter(void *pObject) {
 
   Plot* pThis = (Plot *)pObject;
 
   long okToPlot,segmentID;

   long plotTypes;

   bool sentDiagnosticMessage = false;

   for (  int plotIndex = pThis -> currentPlotCount - 1; plotIndex > -1; plotIndex-- ) {
 
      Plot *p = static_cast<Plot *>(pThis -> pIPlots[plotIndex]);
 
      p -> get_OkToPlot(&okToPlot);

      if ( ! okToPlot )
         continue;

      p -> Create(&segmentID);

      if ( p -> overrideOwnerView ) {
         //p -> propertyPlotView -> get_longValue((long *)&view);
         p -> pIOpenGLImplementation -> Push();
         p -> pIOpenGLImplementation -> SetUp(p -> pIDataSet,p -> propertyPlotView,NULL,NULL,NULL);//p -> pOwnerPropertyTheta,p -> pOwnerPropertyPhi,p -> pOwnerPropertySpin);
      } //else      
         //p -> pOwnerPropertyPlotView -> get_longValue((long *)&view);

      if ( p -> overrideOwnerType || ( NULL == p -> pOwnerProperty2DPlotType ) ) 
         p -> property2DPlotType -> get_longValue(&plotTypes);
      else
         p -> pOwnerProperty2DPlotType -> get_longValue(&plotTypes);

      bool no2DPlotType = false;

      if ( ! ( gcPlotTypeNone == (gc2DPlotTypes)plotTypes ) ) {

         for ( std::pair<gc2DPlotTypes,IGSystemPlotType *> pPair : plotType2DProviderInstances ) {

            if ( ! ( plotTypes & pPair.first ) )
               continue;

            pPair.second -> Execute(plotType2DInstanceNumber[pPair.first],
                                          p -> overrideOwnerType ? p -> plotType2DInstanceGUIDS[(long)pPair.first] : GUID_NULL,
                                          segmentID,(void *)pThis -> pIPlots[plotIndex],(void *)p -> pIOpenGLImplementation,(void *)p -> pIDataSet);

         }

      } else

         no2DPlotType = true;

      if ( p -> overrideOwnerType || ( NULL == p -> pOwnerProperty3DPlotType ) ) 
         p -> property3DPlotType -> get_longValue(&plotTypes);
      else
         p -> pOwnerProperty3DPlotType -> get_longValue(&plotTypes);

      if ( ( gcPlotType3DNone == (gc3DPlotTypes)plotTypes ) ) {
         if ( no2DPlotType && pThis -> pIGSystemStatusBar ) {
            char szStatus[128];
            char szName[32];
            BSTR bstr;
            p -> get_Name(&bstr);
            WideCharToMultiByte(CP_ACP,0,bstr,-1,szName,32,0,0);
            if ( 0 == wcslen(bstr) )
               sprintf_s(szStatus,128,"There is no type associated with this plot. Please set the type to see the graphic.");
            else
               sprintf_s(szStatus,128,"There is no type associated with plot: %s. Please set the type to see the graphic.",szName);
            pThis -> pIGSystemStatusBar -> put_StatusText(0,szStatus);
            sentDiagnosticMessage = true;
            SysFreeString(bstr);
         }
         continue;
      }

      for ( std::pair<gc3DPlotTypes,IGSystemPlotType *> pPair : plotType3DProviderInstances ) {

         if ( ! ( plotTypes & pPair.first ) )
            continue;

         pPair.second -> Execute(plotType3DInstanceNumber[pPair.first],
                                    p -> overrideOwnerType ? p -> plotType3DInstanceGUIDS[(long)pPair.first] : GUID_NULL,
                                    segmentID,(void *)pThis -> pIPlots[plotIndex],(void *)p -> pIOpenGLImplementation,(void *)p -> pIDataSet);

      }

   }

   for ( int k = 0; k < pThis -> currentPlotCount; k++ ) 
      (pThis -> pIPlots[k]) -> Release();
 
   CoTaskMemFree(pThis -> pIPlots);

   pThis -> currentPlotCount = 0;

   if ( ! sentDiagnosticMessage && pThis -> pIGSystemStatusBar )
      pThis -> pIGSystemStatusBar -> put_StatusText(0,"");
 
   return TRUE;
  }
