/*

                       Copyright (c) 1996,1997,1998,1999,2000 Nathan T. Clark

*/

#include <windows.h>

#include "Graphic.h"

#include "utils.h"

  long __stdcall G::DeletePlot(IPlot *toDelete) {
  IPlot *pIPlot = NULL;
  while ( pIPlot = plotList.GetNext(pIPlot) )
     if ( pIPlot == toDelete ) {
        plotList.Remove(pIPlot);
        pIPlot -> Release();
        render(0);
        break;
     }
  return S_OK; 
  } 