
#include "DataSet.h"

   long DataSet::loadDataSet(HWND hwndListView) {

   SendMessage(hwndListView,LVM_DELETEALLITEMS,0L,0L);

   LVCOLUMN lvColumn = {0};

   lvColumn.mask = LVCF_WIDTH;

   while ( SendMessage(hwndListView,LVM_GETCOLUMN,0,(LPARAM)&lvColumn) )
      SendMessage(hwndListView,LVM_DELETECOLUMN,0L,0L);

   RECT rc;

   GetWindowRect(hwndListView,&rc);

   long cxColumns = rc.right - rc.left;

   long cxOneColumn = (cxColumns - 4) / 3;

   char szHeader[32];

   memset(&lvColumn,0,sizeof(LVCOLUMN));

   lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
   lvColumn.fmt = LVCFMT_LEFT;
   lvColumn.cx = cxOneColumn;
   lvColumn.pszText = szHeader;

   sprintf(szHeader,"X");

   SendMessage(hwndListView,LVM_INSERTCOLUMN,0,(LPARAM)&lvColumn);

   sprintf(szHeader,"Y");

   SendMessage(hwndListView,LVM_INSERTCOLUMN,1,(LPARAM)&lvColumn);

   sprintf(szHeader,"Z");

   SendMessage(hwndListView,LVM_INSERTCOLUMN,2,(LPARAM)&lvColumn);

   DataList *pDataList = NULL;
   DataPoint dp;

   char szValue[64];

   LVITEM lvItem = {0};
   lvItem.mask = LVIF_TEXT;
   lvItem.pszText = szValue;

   long k = 0;

   get(pDataList,&dp,&pDataList);

   if ( pDataList ) while ( 1 ) {

      sprintf(szValue,"%lf",dp.x);

      lvItem.iItem = k++;

      lvItem.iSubItem = 0;

      SendMessageW(hwndListView,LVM_INSERTITEM,0L,(LPARAM)&lvItem);

      lvItem.iSubItem = 1;

      sprintf(szValue,"%lf",dp.y);

      SendMessageW(hwndListView,LVM_SETITEM,0L,(LPARAM)&lvItem);

      sprintf(szValue,"%lf",dp.z);

      lvItem.iSubItem = 2;

      SendMessageW(hwndListView,LVM_SETITEM,0L,(LPARAM)&lvItem);

      if ( ! pDataList )
         break;

      get(pDataList,&dp,&pDataList);

   } 

   return 0;
   }