// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
