
#pragma once

#include "list.h"
#include "properties_i.h"
#include "variable_i.h"
#include "evaluator_i.h"

   class VList : public List<IVariable> {
   public:

      VList::VList(IEvaluator* piev);
      VList::~VList();

      void SetHwnds(HWND hwndP,HWND hwndTC);

      IVariable* Add(char *varName);
      IVariable* Copy(IVariable* vToCopy);
      int Remove(IVariable* removeItem,IVariable** currentActiveVariable);

      static IClassFactory* pIClassFactory_variables;

   private:

      HWND hwndParent,hwndTabControl;
      IEvaluator* pIEvaluator;

      friend class FunctionFactory;
      friend class Function;

   };