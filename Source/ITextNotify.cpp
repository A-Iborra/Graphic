// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

#include "utils.h"

  long __stdcall G::DeleteText(IText *toDelete) {
  IText *pIText = NULL;
  while ( pIText = textList.GetNext(pIText) )
     if ( pIText == toDelete ) {
        textList.Remove(pIText);
        pIText -> Release();
        render(0);
        break;
     }
  return S_OK; 
  } 