/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/
#include <windows.h>

#include <math.h>

#include "Graphic_resource.h"
#include "gmessage.h"
#include "utils.h"

#include "Vlist.h"
#include "Evaluator.h"

#include "list.cpp"

   STDAPI GSystemsVariablesDllGetClassObject(HMODULE hModule,REFCLSID clsid,REFIID riid,void** ppObject );

   static double DegToRad;
 
   _exception Evaluator::exception;
 
   static Evaluator theSystemEvaluator;

   IClassFactory* Evaluator::pIClassFactory_variables = NULL;;

   Evaluator *systemEvaluator = NULL;
   
   Evaluator::Evaluator(IUnknown* pUnkOuter) :
      variableList(),

      enumConnectionPoints(0),
      enumConnections(0),
      connectionPoint(this),

      terminateNow(FALSE),

      threadControl(NULL),
      functionIsBusy(NULL),

      pszFunctionExpression(NULL),
      pszResultName(NULL),
      functionIterationCount(0),

      refCount(0) {

   if ( ! systemEvaluator ) {

 	   double pi = 4.0 * atanl(1.0);
      double e = expf(1.0);

      DegToRad = pi / 180.0;
      IVariable* iV;

#ifdef EMBEDDED_VARIABLES
      if ( ! Evaluator::pIClassFactory_variables )
         GSystemsVariablesDllGetClassObject(gsEvaluator_hModule,CLSID_Variable,IID_IClassFactory,reinterpret_cast<void **>(&Evaluator::pIClassFactory_variables));
      if ( ! VList::pIClassFactory_variables )
         GSystemsVariablesDllGetClassObject(gsEvaluator_hModule,CLSID_Variable,IID_IClassFactory,reinterpret_cast<void **>(&VList::pIClassFactory_variables));
#else
      if ( ! Evaluator::pIClassFactory_variables )
         CoGetClassObject(CLSID_Variable,CLSCTX_INPROC_SERVER,NULL,IID_IClassFactory,reinterpret_cast<void **>(&Evaluator::pIClassFactory_variables));
      if ( ! VList::pIClassFactory_variables )
         CoGetClassObject(CLSID_Variable,CLSCTX_INPROC_SERVER,NULL,IID_IClassFactory,reinterpret_cast<void **>(&VList::pIClassFactory_variables));
#endif
      systemEvaluator = &theSystemEvaluator;

      Evaluator::pIClassFactory_variables -> CreateInstance(NULL,IID_IVariable,reinterpret_cast<void **>(&iV));
      systemEvaluator -> Add(iV,"PI");
      iV -> put_Value(pi);

      Evaluator::pIClassFactory_variables -> CreateInstance(NULL,IID_IVariable,reinterpret_cast<void **>(&iV));
      systemEvaluator -> Add(iV,"pi");
      iV -> put_Value(pi);

      Evaluator::pIClassFactory_variables -> CreateInstance(NULL,IID_IVariable,reinterpret_cast<void **>(&iV));
      systemEvaluator -> Add(iV,"Pi");
      iV -> put_Value(pi);

      Evaluator::pIClassFactory_variables -> CreateInstance(NULL,IID_IVariable,reinterpret_cast<void **>(&iV));
      systemEvaluator -> Add(iV,"pI");
      iV -> put_Value(pi);

      Evaluator::pIClassFactory_variables -> CreateInstance(NULL,IID_IVariable,reinterpret_cast<void **>(&iV));
      systemEvaluator -> Add(iV,"e");
      iV -> put_Value(e);


   }

   pszResultName = new char[7];
   strcpy(pszResultName,"result");

   threadControl = CreateMutex(NULL,TRUE,NULL);
   ReleaseMutex(threadControl);

   functionIsBusy = CreateMutex(NULL,TRUE,NULL);
   ReleaseMutex(functionIsBusy);

   return;
   }
 

   Evaluator::Evaluator() : 

    enumConnectionPoints(0),
    enumConnections(0),
    connectionPoint(this),

    pszFunctionExpression(NULL),
    functionIterationCount(0),

    refCount(0),
    variableList() {}


   Evaluator::~Evaluator() {

   IVariable* v;
   while ( v = variableList.GetFirst() ) {
      v -> Release();
      variableList.Remove(v);
   }

   if ( pszFunctionExpression ) 
      delete [] pszFunctionExpression;

   if ( pszResultName ) 
      delete [] pszResultName;

   return;
   }
 

   double Evaluator::eval(char* input,char **externalTrack) {

   char *c,*expressionLocation,*dummy = NULL;
   double rValue;
 
   if ( externalTrack )
      expressionLocation = *externalTrack;
   else {
      dummy = new char[strlen(input) + 1];
      memset(dummy,0,strlen(input) + 1);
      strcpy(dummy,input);
      expressionLocation = dummy;
   }
 
   if ( (c = strchr(expressionLocation,'=')) != NULL ) {
      char *varName;
      IVariable *v;

      *c = '\0';
      varName = expressionLocation;
      trim(varName);
      lowercase(varName);
      expressionLocation = c + 1;
      trim(expressionLocation);

      if ( (v = Get(varName)) == (IVariable *)NULL ) {
         if ( (v = systemEvaluator -> Get(varName)) == (IVariable *)NULL ) {
            if ( (v = variableList.Get(varName)) == (IVariable *)NULL ) {
               pIClassFactory_variables -> CreateInstance(NULL,IID_IVariable,reinterpret_cast<void**>(&v));
               v -> put_IsIndependent(FALSE);
               variableList.Add(v,varName);
            }
         }
      }

      rValue = plusSequence(&expressionLocation);

      v -> put_Value(rValue);

      if ( dummy ) 
         delete [] dummy;
      else
         if ( externalTrack )
            *externalTrack = expressionLocation;

      return rValue;
   }
 
   rValue = plusSequence(&expressionLocation);

   if ( dummy ) 
      delete [] dummy;
   else
      if ( externalTrack )
         *externalTrack = expressionLocation;

   return rValue;

   }
 
 
   double Evaluator::plusSequence(char **expression) {
   double value;
   static int done;
   char c;
 
   done = 0;
 
   value = starSequence(expression);

   if ( !**expression ) return value;
 
   c = nextChar(expression);
   while ( c && (c == (char)0x96 || strchr(EVALOPERATORS,(int)c) ) ) {

      switch (c) {
      case '+' : 
         value += plusSequence(expression);
         break;
      case (char)0x96 :
      case '-' : 
         c = nextChar(expression);
         if ( ! c )
            return value;
         replaceChar(c,expression);
         value -= plusSequence(expression);
         break;
      case '/' :
         c = nextChar(expression);
         if ( ! c )
            return 0.0;
         if ( c != '/' ) {
            replaceChar(c,expression);
            double divisor = starSequence(expression);
            if ( 0.0 == divisor ) 
               Fire_DivideByZero();
            value /= divisor;
         } else
            done = 1;
         break;
      case '*' :
         c = nextChar(expression);
         if ( ! c )
            return 0.0;
         if ( c == '*' ) 
            value = pow(value,starSequence(expression)); 
         else {
            replaceChar(c,expression);
            value *= starSequence(expression);
         }
         break;
      case '^' :
         value = pow(value,starSequence(expression));
         break;
      }
 
      if ( done ) break;
 
      c = nextChar(expression);
   }
 
   replaceChar(c,expression);

   return value;
 
   }
  
 
   double Evaluator::starSequence(char **expression) {
   char c;
 
   c = nextChar(expression);
 
   if (c == ',') return starSequence(expression);
   if (c == ';') return starSequence(expression);
   if (c == '-') return -starSequence(expression);
   if (c == '+') return starSequence(expression);
   if (c == '(') {
      double value = plusSequence(expression);
      nextChar(expression);
      return value;
   }
 
   replaceChar(c,expression);
 
   if ( isdigit(c) || c == '.' ) {
      char *endptr;
      double value = strtod(*expression,&endptr);
      *expression = endptr;
      return value;
   }
 
   if ( isalpha(c) ) {
      double value;
      IVariable *v;
      char *varName,*keepChar,*keepChar2;
      boolean endLineVariable = false;
      varName = *expression;
      keepChar = strpbrk(*expression,VARIABLE_DELIMITERS);

      if ( ! keepChar )
         if ( 0 == strcmp(varName,*expression) && (strlen(varName) == strlen(*expression)) )
            endLineVariable = true;

      if ( endLineVariable || keepChar ) {
         if ( ! endLineVariable ) {
            c = *keepChar;
            *keepChar = '\0';
         }
         keepChar2 = strpbrk(*expression,FUNCTION_DELIMITERS);
         if ( keepChar2 != NULL ) {
            if ( keepChar ) *keepChar = c;
            return handleFunction(expression);
         }
         lowercase(varName);
         if ( ! (v = Get(varName)) ) {
            if ( ! (v = systemEvaluator -> Get(varName)) ) {
               if ( ! (v = variableList.Get(varName)) ) {
                  Fire_UnknownVariable(varName);
                  if ( v = Get(varName) )
                     v -> get_Value(&value);
                  else
                     value = 0.0;
               } else
                  v -> get_Value(&value);
            }
            else
               v -> get_Value(&value);
         }
         else
            v -> get_Value(&value);
 
         if ( ! endLineVariable ) {
            *keepChar = c;
            *expression = keepChar;
         }
         else
            **expression = '\0';
      }
      else {
         keepChar = strpbrk(*expression,FUNCTION_DELIMITERS);
         if ( keepChar != NULL )
            return handleFunction(expression);
         lowercase(varName);
         if ( ! (v = Get(varName)) ) {
            if ( ! (v = systemEvaluator -> Get(varName)) ) {
               if ( ! (v = variableList.Get(varName)) ) {
                  Fire_UnknownFunction(varName);
                  value = 0.0;
               } else
                  v -> get_Value(&value);
            }
            else
               v -> get_Value(&value);
         }
         else
            v -> get_Value(&value);
 
         *expression += strlen(*expression);
      }
 
      return value;
   }
 
   return 0.0;
   }
 
 
 
   double Evaluator::handleFunction(char **expression) {
   char *funName,*holdLoc,holdChar;
   double argument,returnVal;
 
   holdLoc = strpbrk(*expression,FUNCTION_DELIMITERS);
   holdChar = *holdLoc;
   *holdLoc = '\0';
   funName = new char[strlen(*expression) + 1];
   strcpy(funName,*expression);
   *holdLoc = holdChar;
   *expression = holdLoc;
 
   argument = starSequence(expression);
 
   returnVal = -DBL_MAX;
   uppercase(funName);
 
   memset(&exception,0,sizeof(_exception));
 
   try {
 
   if ( !strcmp(funName,"COS")  ) {
      returnVal = cosl(argument);
   }
   if ( !strcmp(funName,"COSD") ) {
      returnVal = cosl(argument * DegToRad);
   }
   if ( !strcmp(funName,"SIN")  ) {
      returnVal = sinl(argument);
   }
   if ( !strcmp(funName,"SIND") ) {
      returnVal = sinl(argument * DegToRad);
   }
   if ( !strcmp(funName,"TAN")  ) {
      returnVal = tanl(argument);
   }
   if ( !strcmp(funName,"TAND") ) {
      returnVal = tanl(argument * DegToRad);
   }
   if ( !strcmp(funName,"ACOS") ) {
      returnVal = acosl(argument);
   }
   if ( !strcmp(funName,"ACOSD")) {
      returnVal = acosl(argument) / DegToRad;
   }
   if ( !strcmp(funName,"ASIN") ) {
      returnVal = asinl(argument);
   }
   if ( !strcmp(funName,"ASIND")) {
      returnVal = asinl(argument) / DegToRad;
   }
   if ( !strcmp(funName,"ATAN") ) {
      returnVal = atanl(argument);
   }
   if ( !strcmp(funName,"ATAND")) {
      returnVal = atanl(argument) / DegToRad;
   }
   if ( !strcmp(funName,"ABS")  ) {
      returnVal = fabsl(argument);
   }
   if ( !strcmp(funName,"CEIL") ) {
      returnVal = ceill(argument);
   }
   if ( !strcmp(funName,"COSH") ) {
      returnVal = coshl(argument);
   }
   if ( !strcmp(funName,"EXP")  ) {
      returnVal = expl(argument);
   }
   if ( !strcmp(funName,"FABS") ) {
      returnVal = fabsl(argument);
   }
   if ( !strcmp(funName,"FLOOR")) {
      returnVal = floorl(argument);
   }
   if ( !strcmp(funName,"LOG")  ) {
      returnVal = logl(argument);
   }
   if ( !strcmp(funName,"LOG10")) {
      returnVal = log10l(argument);
   }
   if ( !strcmp(funName,"POW")  ) {
      returnVal = powl(argument,plusSequence(expression));
   }
   if ( !strcmp(funName,"RAND") ) {
      returnVal = rand();
   }
   if ( !strcmp(funName,"SINH") ) {
      returnVal = sinhl(argument);
   }
   if ( !strcmp(funName,"SQRT") ) {
      returnVal = sqrtl(argument);
   }
   if ( !strcmp(funName,"TANH") ) {
      returnVal = tanhl(argument);
   }
   if ( !strcmp(funName,"J0") ) {
      returnVal = _j0(argument);
   }
   if ( !strcmp(funName,"J1") ) {
      returnVal = _j1(argument);
   }
   if ( !strcmp(funName,"JN") ) {
      returnVal = _jn((int)argument,plusSequence(expression));
   }
   if ( !strcmp(funName,"Y0") ) {
      returnVal = _j0(argument);
   }
   if ( !strcmp(funName,"Y1") ) {
      returnVal = _j1(argument);
   }
   if ( !strcmp(funName,"YN") ) {
      returnVal = _jn((int)argument,plusSequence(expression));
   }
   if ( !strcmp(funName,"MAX")  ) {
      double y = plusSequence(expression);
      returnVal = argument > y ? argument : y;
   }
   if ( !strcmp(funName,"MIN")  ) {
      double y = plusSequence(expression);
      returnVal = argument < y ? argument : y;
   }
 
   } catch ( ... ) {
 
      Fire_MathError(funName,argument);
      delete [] funName;
      return 0.0;
 
   }
 
   if ( exception.type == _DOMAIN )
      Fire_InvalidArgument(funName,argument);
 
   if ( returnVal == -DBL_MAX ) {
      Fire_UnknownFunction(funName);
      returnVal = 0.0;
   }
 
   delete[] funName;
   return returnVal;
   }
 
 
   int _matherr(_exception *exception) {
   memcpy(&Evaluator::exception,exception,sizeof(_exception));
   return 0;
   }
