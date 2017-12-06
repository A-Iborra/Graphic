/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include "Graphic.h"

  void __stdcall G::OnDataChange(FORMATETC *,STGMEDIUM *) {
  return;
  }

  void __stdcall G::OnViewChange(DWORD dwAspect,long index) {
  render(0);
  return;
  }

  void __stdcall G::OnRename(IMoniker *) {
  return;
  }

  void __stdcall G::OnSave() {
  return;
  }

  void __stdcall G::OnClose() {
  return ;
  }
