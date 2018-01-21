// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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