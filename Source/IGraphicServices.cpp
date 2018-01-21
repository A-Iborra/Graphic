// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"

   HRESULT G::ActivateLighting() {
   pIOpenGLImplementation -> SetLighting(ppPropertyLightOn,ppPropertyAmbientLight,ppPropertyDiffuseLight,ppPropertySpecularLight,ppPropertyLightPos,propertyCountLights,propertyShinyness);
   return S_OK;
   }
 