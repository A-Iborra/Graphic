 
#include "Graphic.h"

   HRESULT G::ActivateLighting() {
   pIOpenGLImplementation -> SetLighting(ppPropertyLightOn,ppPropertyAmbientLight,ppPropertyDiffuseLight,ppPropertySpecularLight,ppPropertyLightPos,propertyCountLights,propertyShinyness);
   return S_OK;
   }
 