
#include "DataSet.h"

   long DataSet::populateData(HWND hwndListView,SAFEARRAY *pArray) {

   LONG lowerBound[2],upperBound[2];

   SafeArrayGetUBound(pArray,1,&upperBound[0]);
   SafeArrayGetUBound(pArray,2,&upperBound[1]);

   SafeArrayGetLBound(pArray,1,&lowerBound[0]);
   SafeArrayGetLBound(pArray,2,&lowerBound[1]);

   if ( hwndListView ) {

      long columnCount = upperBound[1] - lowerBound[1] + 1;

      RECT rc;

      GetWindowRect(hwndListView,&rc);

      long cxColumns = rc.right - rc.left;

      long cxOneColumn = max(32,(cxColumns - 4)/ columnCount);

      char szHeader[32];

      LVCOLUMN lvColumn;
      memset(&lvColumn,0,sizeof(LVCOLUMN));

      lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
      lvColumn.fmt = LVCFMT_LEFT;
      lvColumn.cx = cxOneColumn;
      lvColumn.pszText = szHeader;

      for ( long k = 0; k < columnCount; k++ ) {

         sprintf(szHeader,"Col %ld",k + 1);

         SendMessage(hwndListView,LVM_INSERTCOLUMN,k,(LPARAM)&lvColumn);

      }

   }

   VARIANT *pValues = NULL;

   SafeArrayAccessData(pArray,(void **)&pValues);

   VARIANT *pv = pValues;

   WCHAR szwValue[EXCEL_VALUE_SIZE];
   char szValue[EXCEL_VALUE_SIZE];

   LVITEM lvItem = {0};
   lvItem.mask = LVIF_TEXT;
   lvItem.pszText = szValue;

   ReSet();

   DataPoint dp{0.0,0.0,0.0};

   long countRows = upperBound[0] - lowerBound[0] + 1;
   long countColumns = upperBound[1] - lowerBound[1] + 1;

   switch ( countColumns ) {
   case 1:
      put_DataArity(DATA_ARITY_1D);
      break;
   case 2:
      put_DataArity(DATA_ARITY_2D);
      break;
   default:
      put_DataArity(DATA_ARITY_3D);
      break;
   }

   for ( long rowIndex = 0; rowIndex < countRows; rowIndex++ ) {

      pv = pValues + rowIndex;

      for ( long columnIndex = 0; columnIndex < countColumns; columnIndex++ ) {

         VARIANT *pvt = pv;

         pv += countRows;

         try {

#if 1
         if ( ! ( S_OK == VariantChangeTypeEx(pvt,pvt,NULL,0,VT_BSTR) ) ) 
            swprintf(szwValue,L"#error");
         else
            swprintf(szwValue,L"%ls",pvt -> bstrVal);

#else

         switch ( pvt -> vt ) {
         case VT_INT:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%d",pvt -> intVal); break;
         case VT_I1:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%c",pvt -> cVal); break;
         case VT_I2:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%d",pvt -> iVal); break;
         case VT_I4:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%d",pvt -> lVal); break;
         case VT_UI1:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%c",pvt -> cVal); break;
         case VT_UI2:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%d",pvt -> iVal); break;
         case VT_UI4:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%ld",pvt -> uintVal); break;
         case VT_UINT:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%ld",pvt -> uintVal); break;
         case VT_R4:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%f",pvt -> fltVal); break;
         case VT_R8:  
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%lf",pvt -> dblVal); break;
         case VT_BSTR:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%ls",pvt -> bstrVal); break;
         case VT_DATE: {
            if ( ! ( S_OK == VariantChangeTypeEx(pvt,pvt,NULL,0,VT_BSTR) ) ) 
               swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"#error");
            else
               swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"%ls",pvt -> bstrVal);
            }
            break;
         default:
            swprintf_s(szwValue,EXCEL_VALUE_SIZE,L"nan");
            break;
         }
#endif

         } catch ( ... ) {
OutputDebugString("\nhello world");

         }

         bool ignoreData = false;
         switch ( columnIndex ) {
         case 0:
            dp.x = _wtof(szwValue);
            if ( 1 == countColumns )
               pushDataPoint(&dp);
            break;
         case 1:
            dp.y = _wtof(szwValue);
            if ( 2 == countColumns )
               pushDataPoint(&dp);
            break;
         case 2:
            dp.z = _wtof(szwValue);
            pushDataPoint(&dp);
            break;
         default:
            ignoreData = true;
            break;
         }

         if ( hwndListView ) {

            lvItem.iItem = rowIndex;

            lvItem.iSubItem = columnIndex;

            WideCharToMultiByte(CP_ACP,0,szwValue,-1,szValue,EXCEL_VALUE_SIZE,0,0);

            if ( 0 == columnIndex )
               SendMessageW(hwndListView,LVM_INSERTITEM,0L,(LPARAM)&lvItem);
            else
               SendMessageW(hwndListView,LVM_SETITEM,0L,(LPARAM)&lvItem);

         }
      }

   }

   SafeArrayUnaccessData(pArray);

   return 0;
   }