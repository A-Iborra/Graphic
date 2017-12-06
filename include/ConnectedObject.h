
#pragma once

#include "ocidl.h"

  template<class Parent> class ConnectedObject {

  public:

     ConnectedObject(Parent* pParent);
     ~ConnectedObject();

     virtual int connectEvents() = 0;
     virtual int unConnectEvents() = 0;

  private:

   protected:

     IUnknown* pIUnknownObject;

     unsigned int refCount;
     Parent* pParent;

     IConnectionPoint* pIConnectionPoint;
     DWORD dwConnectionCookie;

  };

