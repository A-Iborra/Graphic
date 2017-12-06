/*

                       Copyright (c) 1996,1997,1998,1999,2000,2008 Nathan T. Clark

*/

#include <windows.h>

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