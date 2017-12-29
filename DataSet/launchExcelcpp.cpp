
#include "DataSet.h"

#include <time.h>

   long DataSet::launchExcel(char *pszWorkbookName) {

   Excel::_Application *pIApplication = NULL;
   Excel::Workbooks *pIWorkbooks = NULL;

   BSTR bstrFile = SysAllocStringLen(NULL,MAX_PATH);

   MultiByteToWideChar(CP_ACP,0,pszWorkbookName,-1,bstrFile,MAX_PATH);

   HRESULT hr = CoCreateInstance(CLSID_excel,NULL,CLSCTX_LOCAL_SERVER,__uuidof(Excel::_Application),reinterpret_cast<void**>(&pIApplication));

   pIApplication -> put_Visible(0,VARIANT_TRUE);

   hr = pIApplication -> get_Workbooks(&pIWorkbooks);

   pIWorkbooks -> Open(bstrFile);

   if ( pIWorkbooks )
      pIWorkbooks -> Release();

   if ( pIApplication )
      pIApplication -> Release();

   return 0;
   }
