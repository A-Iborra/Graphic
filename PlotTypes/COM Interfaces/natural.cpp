
#include "PlotTypes.h"

   void PlotTypes::natural(long segmentID) {

   pIOpenGLImplementation -> OpenSegment(segmentID,propertyLineColor,propertyLineWeight);

   DataList *pItem = NULL;

   pIDataSet -> peek(pItem,&pItem);

   if ( pItem ) {

      if ( ! ( -DBL_MAX == pItem -> data.x ) ) {
         pIOpenGLImplementation -> NewLine(&pItem -> data);
         pIDataSet -> peek(pItem,&pItem);
      }

      while ( pItem ) {
         if ( -DBL_MAX == pItem -> data.x ) {
            while ( pItem && -DBL_MAX == pItem -> data.x ) 
               pIDataSet -> peek(pItem,&pItem);
            if ( ! pItem ) 
               break;
            pIOpenGLImplementation -> NewLine(&pItem -> data);
         }
         pIOpenGLImplementation -> Vertex(&pItem -> data);
         pIDataSet -> peek(pItem,&pItem);
      }

   }

   pIOpenGLImplementation -> CloseSegment(segmentID,TRUE);

#if 0
   IText *t = NULL;

   while ( t = textList.GetNext(t) ) 
      t -> Draw();
#endif

   return;
   }