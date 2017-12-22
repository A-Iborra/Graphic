
#include "DataSet.h"

#include <time.h>

   long DataSet::launchExcel() {

   //long count = 0L;
   //long cutoff = -1L;
   //bool wasOpen = false;

   //VARIANT vtFalse;
   //vtFalse.vt = VT_BOOL;
   //vtFalse.boolVal = false;
   //
   //VARIANT vtIndex;
   //vtIndex.vt = VT_I4;
   //vtIndex.lVal = 0L;

   Excel::_Application *pIApplication = NULL;
   Excel::Workbooks *pIWorkbooks = NULL;
   //Excel::_Workbook *pIWorkbook = NULL;
   //Excel::Sheets *pIWorksheets = NULL;
   //Excel::_Worksheet *pIWorksheet = NULL;
   //Excel::_Worksheet *pIWorksheet_Selected = NULL;

   BSTR bstrFile = SysAllocStringLen(NULL,MAX_PATH);

   MultiByteToWideChar(CP_ACP,0,szDataSource,-1,bstrFile,MAX_PATH);

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
