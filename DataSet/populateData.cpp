
#include "DataSet.h"
#include <vector>

   long DataSet::populateData(HWND hwndListView,HWND hwndErrorReport,bool createColumns,SAFEARRAY *pArray) {

   LONG lowerBound[2],upperBound[2];

   SafeArrayGetUBound(pArray,1,&upperBound[0]);
   SafeArrayGetUBound(pArray,2,&upperBound[1]);

   SafeArrayGetLBound(pArray,1,&lowerBound[0]);
   SafeArrayGetLBound(pArray,2,&lowerBound[1]);

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

   //switch ( countColumns ) {
   //case 1:
   //   put_DataArity(DATA_ARITY_1D);
   //   break;
   //case 2:
   //   put_DataArity(DATA_ARITY_2D);
   //   break;
   //default:
   //   put_DataArity(DATA_ARITY_3D);
   //   break;
   //}

   long rowStart = 0;

   if ( hasHeaderRow ) 
      rowStart = 1;

   if ( hwndListView && createColumns ) {

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

         if ( hasHeaderRow ) {
            pv = pValues + k * countRows;
            VariantChangeTypeEx(pv,pv,NULL,0,VT_BSTR);
            WideCharToMultiByte(CP_ACP,0,pv -> bstrVal,-1,szHeader,32,0,0);
         } else
            sprintf(szHeader,"Col %ld",k + 1);

         SendMessage(hwndListView,LVM_INSERTCOLUMN,k,(LPARAM)&lvColumn);

      }

   }

   //bool anyYValue = false;
   //bool anyZValue = false;

   for ( long rowIndex = rowStart; rowIndex < countRows; rowIndex++ ) {

      pv = pValues + rowIndex;

      for ( long columnIndex = 0; columnIndex < countColumns; columnIndex++ ) {

         VARIANT *pvt = pv;

         pv += countRows;

         try {

         if ( ! ( S_OK == VariantChangeTypeEx(pvt,pvt,NULL,0,VT_BSTR) ) ) 
            swprintf(szwValue,L"#error");
         else
            swprintf(szwValue,L"%ls",pvt -> bstrVal);

         } catch ( ... ) {

            if ( hwndErrorReport ) {
               char szMessage[256];
               sprintf_s(szMessage,256,"There was an error in the datatype populating row %ld",rowIndex);
               SetWindowText(hwndErrorReport,szMessage);
            }

         }

         switch ( columnIndex ) {
         case 0:
            dp.x = _wtof(szwValue);
            if ( 1 == countColumns )
               pushDataPoint(&dp);
            break;
         case 1:
            dp.y = _wtof(szwValue);
            //if ( ! ( 0.0 == dp.y ) )
            //   anyYValue = true;
            if ( 2 == countColumns )
               pushDataPoint(&dp);
            break;
         case 2:
            dp.z = _wtof(szwValue);
            //if ( ! ( 0.0 == dp.z ) )
            //   anyZValue = true;
            pushDataPoint(&dp);
            break;
         default:
            break;
         }

         if ( hwndListView ) {

            lvItem.iItem = rowIndex - rowStart;

            lvItem.iSubItem = columnIndex;

            WideCharToMultiByte(CP_ACP,0,szwValue,-1,szValue,EXCEL_VALUE_SIZE,0,0);

            if ( 0 == columnIndex )
               SendMessageW(hwndListView,LVM_INSERTITEM,0L,(LPARAM)&lvItem);
            else
               SendMessageW(hwndListView,LVM_SETITEM,0L,(LPARAM)&lvItem);

         }

      }

   }

   //if ( DATA_ARITY_3D == dataArity && ! anyZValue )
   //   dataArity = DATA_ARITY_2D;

   //if ( DATA_ARITY_2D == dataArity && ! anyYValue )
   //   dataArity = DATA_ARITY_1D;

   SafeArrayUnaccessData(pArray);

   return 0;
   }