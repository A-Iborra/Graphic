
#include <PlotTypes.h>

   void PlotTypes::blocks(long segmentID) {
   if ( DATA_ARITY_2D == pIDataSet -> DataArity() ) 
      blocks2D(segmentID);
   else
      blocks3D(segmentID);
   return;
   }


   void PlotTypes::blocks2D(long segmentID) {

   DECLARE_PLANE

   double minx,maxx,miny,maxy,minz,maxz;
   BOOL endOfData;
   DataList *dl;

   pIOpenGLImplementation -> BeginSolids(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);

   pIDataSet -> get_minX(&minx);
   pIDataSet -> get_maxX(&maxx);
   pIDataSet -> get_minY(&miny);
   pIDataSet -> get_maxY(&maxy);
   pIDataSet -> get_minZ(&minz);
   pIDataSet -> get_maxZ(&maxz);

   double xWidth = (maxx - minx) / 10.0;
   double zWidth = (maxz - minz) / 10.0;
   double floor = miny;
   
   endOfData = FALSE;
   dl = (DataList *)NULL;
   pIDataSet -> get(dl,&homePoint,&dl);
   while ( dl ) {
   
/* Back facing on the Y-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = floor;
      firstPoint.z = homePoint.z - zWidth;
   
      secondPoint.x = homePoint.x - xWidth;
      secondPoint.y = homePoint.y;
      secondPoint.z = homePoint.z - zWidth;
   
      thirdPoint.x = homePoint.x - xWidth;
      thirdPoint.y = homePoint.y;
      thirdPoint.z = homePoint.z + zWidth;
   
      fourthPoint.x = homePoint.x - xWidth;
      fourthPoint.y = floor;
      fourthPoint.z = homePoint.z + zWidth;
    
      DRAW_PLANE
    
/* Up facing on the X-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = floor;
      firstPoint.z = homePoint.z + zWidth;
    
      secondPoint.x = homePoint.x + xWidth;
      secondPoint.y = floor;
      secondPoint.z = homePoint.z + zWidth;
    
      thirdPoint.x = homePoint.x + xWidth;
      thirdPoint.y = homePoint.y;
      thirdPoint.z = homePoint.z + zWidth;

      fourthPoint.x = homePoint.x - xWidth;
      fourthPoint.y = homePoint.y;
      fourthPoint.z = homePoint.z + zWidth;
    
      DRAW_PLANE
    
/* Front facing on the X-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y + zWidth;
      firstPoint.z = floor;
    
      secondPoint.x = homePoint.x - xWidth;
      secondPoint.y = homePoint.y + zWidth;
      secondPoint.z = homePoint.z;

      thirdPoint = secondPoint;

      thirdPoint.x = homePoint.x + xWidth;
    
      fourthPoint = thirdPoint;
      fourthPoint.z = floor;
    
//      DRAW_PLANE
    
/* Back facing on the Y-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y + zWidth;
      firstPoint.z = floor;
    
      secondPoint.x = firstPoint.x;
      secondPoint.y = homePoint.y - zWidth;
      secondPoint.z = firstPoint.z;
    
      thirdPoint.x = secondPoint.x;
      thirdPoint.y = secondPoint.y;
      thirdPoint.z = homePoint.z;
    
      fourthPoint.x = firstPoint.x;
      fourthPoint.y = firstPoint.y;
      fourthPoint.z = homePoint.z;
    
//      DRAW_PLANE
    
/* Up facing on the X-Y plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y - zWidth;
      firstPoint.z = homePoint.z;
    
      secondPoint.x = homePoint.x + xWidth;
      secondPoint.y = homePoint.y - zWidth;
      secondPoint.z = homePoint.z;
    
      thirdPoint.x = homePoint.x + xWidth;
      thirdPoint.y = homePoint.y + zWidth;
      thirdPoint.z = homePoint.z;
    
      fourthPoint.x = homePoint.x - xWidth;
      fourthPoint.y = homePoint.y + zWidth;
      fourthPoint.z = homePoint.z;
    
//      DRAW_PLANE
    
      pIDataSet -> get(dl,&homePoint,&dl);
    
   }
    
   pIOpenGLImplementation -> EndSolids(segmentID);

   return;
   }


   void PlotTypes::blocks3D(long segmentID) {
   
   DECLARE_PLANE

   double minx,maxx,miny,maxy,minz,maxz;
   BOOL endOfData;
   DataList *dl;

   pIOpenGLImplementation -> BeginSolids(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);

   pIDataSet -> get_minX(&minx);
   pIDataSet -> get_maxX(&maxx);
   pIDataSet -> get_minY(&miny);
   pIDataSet -> get_maxY(&maxy);
   pIDataSet -> get_minZ(&minz);
   pIDataSet -> get_maxZ(&maxz);

   double xWidth = (maxx - minx) * defaultSolidSizeInPercentDomain / 100.0;
   double zScaleFactor = (maxx - minx) / (maxz - minz);
   double yScaleFactor = (maxx - minx) / (maxy - miny);
   double yWidth = xWidth / yScaleFactor;
   double zHeight = xWidth / zScaleFactor;
   
   endOfData = FALSE;
   dl = (DataList *)NULL;
   pIDataSet -> get(dl,&homePoint,&dl);
   while ( dl ) {
   
/* Back facing on the X-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y - yWidth;
      firstPoint.z = homePoint.z - zHeight;
   
      secondPoint.x = homePoint.x + xWidth;
      secondPoint.y = firstPoint.y;
      secondPoint.z = firstPoint.z;
   
      thirdPoint.x = secondPoint.x;
      thirdPoint.y = secondPoint.y;
      thirdPoint.z = homePoint.z + zHeight;
   
      fourthPoint.x = firstPoint.x;
      fourthPoint.y = firstPoint.y;
      fourthPoint.z = homePoint.z + zHeight;
    
      DRAW_PLANE
    
/* Front facing on the Y-Z plane */

      firstPoint = secondPoint;
    
      secondPoint.y = homePoint.y + yWidth;
    
      thirdPoint = secondPoint;
      thirdPoint.z = homePoint.z + zHeight;
    
      fourthPoint = firstPoint;
      fourthPoint.z = homePoint.z + zHeight;
    
      DRAW_PLANE
    
/* Front facing on the X-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y + yWidth;
      firstPoint.z = homePoint.z - zHeight;
    
      secondPoint.x = homePoint.x - xWidth;
      secondPoint.y = homePoint.y + yWidth;
      secondPoint.z = homePoint.z + zHeight;

      thirdPoint = secondPoint;

      thirdPoint.x = homePoint.x + xWidth;
    
      fourthPoint = thirdPoint;
      fourthPoint.z = homePoint.z - zHeight;
    
      DRAW_PLANE
    
/* Back facing on the Y-Z plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y + yWidth;
      firstPoint.z = homePoint.z - zHeight;
    
      secondPoint.x = firstPoint.x;
      secondPoint.y = homePoint.y - yWidth;
      secondPoint.z = firstPoint.z;
    
      thirdPoint.x = secondPoint.x;
      thirdPoint.y = secondPoint.y;
      thirdPoint.z = homePoint.z + zHeight;
    
      fourthPoint.x = firstPoint.x;
      fourthPoint.y = firstPoint.y;
      fourthPoint.z = homePoint.z + zHeight;
    
      DRAW_PLANE
    
/* Up facing on the X-Y plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y - yWidth;
      firstPoint.z = homePoint.z + zHeight;
    
      secondPoint.x = homePoint.x + xWidth;
      secondPoint.y = homePoint.y - yWidth;
      secondPoint.z = homePoint.z + zHeight;
    
      thirdPoint.x = homePoint.x + xWidth;
      thirdPoint.y = homePoint.y + yWidth;
      thirdPoint.z = homePoint.z + zHeight;
    
      fourthPoint.x = homePoint.x - xWidth;
      fourthPoint.y = homePoint.y + yWidth;
      fourthPoint.z = homePoint.z + zHeight;
    
      DRAW_PLANE
    
      pIDataSet -> get(dl,&homePoint,&dl);
    
   }
    
   pIOpenGLImplementation -> EndSolids(segmentID);

   return;
   }