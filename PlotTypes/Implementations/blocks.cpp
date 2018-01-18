
#include <PlotTypes.h>


   void PlotTypes::blocks(commonProperties *pProperties,long segmentID) {
   
   DECLARE_PLANE

   double minx,maxx,miny,maxy,minz,maxz;
   BOOL endOfData;

   pIOpenGLImplementation -> BeginSurface(segmentID,propertyTopSurfaceColor,propertyBottomSurfaceColor);

   pIDataSet -> get_minX(&minx);
   pIDataSet -> get_maxX(&maxx);
   pIDataSet -> get_minY(&miny);
   pIDataSet -> get_maxY(&maxy);
   pIDataSet -> get_minZ(&minz);
   pIDataSet -> get_maxZ(&maxz);

   blocksProperties *pBlocksProperties = (blocksProperties *)pProperties -> pvAdditionalProperties;

   double xWidth = (maxx - minx) * pBlocksProperties -> sizeInPercentDomain[0] / 100.0;

   double zScaleFactor = (maxx - minx) / (maxz - minz);
   double yScaleFactor = (maxx - minx) / (maxy - miny);
   double yWidth = xWidth / yScaleFactor;
   double zHeight = xWidth / zScaleFactor;

   if ( ! pBlocksProperties -> keepCubic ) {
      yWidth = (maxy - miny) * pBlocksProperties -> sizeInPercentDomain[1] / 100.0;
      zHeight = (maxz - minz) * pBlocksProperties -> sizeInPercentDomain[2] / 100.0;
      zScaleFactor = 1.0;
      yScaleFactor = 1.0;
   }

   endOfData = FALSE;

   pIOpenGLImplementation -> BeginOpenGLMode(GL_QUADS);

   DataList *pItem = NULL;

   pIDataSet -> peek(pItem,&pItem);

   while ( pItem ) {

      homePoint = pItem -> data;
   
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
    
      v[0] = firstPoint;
      v[1] = secondPoint;
      v[2] = thirdPoint;
      v[3] = fourthPoint;
            
      avgNormal[0] = 0.0;
      avgNormal[1] = -1.0;
      avgNormal[2] = 0.0;

      pIOpenGLImplementation -> Normal3dv(avgNormal);

      for ( vk = 0; vk < 4; vk++ ) 
         pIOpenGLImplementation -> Vertex(&v[vk]);

/* Front facing on the Y-Z plane */

//pIOpenGLImplementation -> EndOpenGLMode();
//pIOpenGLImplementation -> BeginOpenGLMode(GL_LINE_STRIP);
   
      firstPoint = secondPoint;
    
      secondPoint.y = homePoint.y + yWidth;
    
      thirdPoint = secondPoint;
      thirdPoint.z = homePoint.z + zHeight;
    
      fourthPoint = firstPoint;
      fourthPoint.z = homePoint.z + zHeight;

      v[0] = firstPoint;               
      v[1] = secondPoint;              
      v[2] = thirdPoint;               
      v[3] = fourthPoint;              
                                    
      avgNormal[0] = 1.0;
      avgNormal[1] = 0.0;
      avgNormal[2] = 0.0;

      pIOpenGLImplementation -> Normal3dv(avgNormal);

      for ( vk = 0; vk < 4; vk++ ) 
         pIOpenGLImplementation -> Vertex(&v[vk]);

//pIOpenGLImplementation -> EndOpenGLMode();
//pIOpenGLImplementation -> BeginOpenGLMode(GL_LINE_STRIP);

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

      v[0] = firstPoint;               
      v[1] = secondPoint;              
      v[2] = thirdPoint;               
      v[3] = fourthPoint;              
                                    
      avgNormal[0] = 0.0;
      avgNormal[1] = 1.0;
      avgNormal[2] = 0.0;

      pIOpenGLImplementation -> Normal3dv(avgNormal);

      for ( vk = 0; vk < 4; vk++ ) 
         pIOpenGLImplementation -> Vertex(&v[vk]);

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

      v[0] = firstPoint;               
      v[1] = secondPoint;              
      v[2] = thirdPoint;               
      v[3] = fourthPoint;              
                            
      avgNormal[0] = -1.0;
      avgNormal[1] = 0.0;
      avgNormal[2] = 0.0;

      pIOpenGLImplementation -> Normal3dv(avgNormal);

      for ( vk = 0; vk < 4; vk++ ) 
         pIOpenGLImplementation -> Vertex(&v[vk]);

    
      //DRAW_PLANE
    
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
    
      v[0] = firstPoint;               
      v[1] = secondPoint;              
      v[2] = thirdPoint;               
      v[3] = fourthPoint;              

      avgNormal[0] = 0.0;
      avgNormal[1] = 0.0;
      avgNormal[2] = 1.0;

      pIOpenGLImplementation -> Normal3dv(avgNormal);

      for ( vk = 0; vk < 4; vk++ ) 
         pIOpenGLImplementation -> Vertex(&v[vk]);

/* Down facing on the X-Y plane */

      firstPoint.x = homePoint.x - xWidth;
      firstPoint.y = homePoint.y - yWidth;
      firstPoint.z = homePoint.z - zHeight;
    
      secondPoint.x = homePoint.x + xWidth;
      secondPoint.y = homePoint.y - yWidth;
      secondPoint.z = homePoint.z - zHeight;
    
      thirdPoint.x = homePoint.x + xWidth;
      thirdPoint.y = homePoint.y + yWidth;
      thirdPoint.z = homePoint.z - zHeight;
    
      fourthPoint.x = homePoint.x - xWidth;
      fourthPoint.y = homePoint.y + yWidth;
      fourthPoint.z = homePoint.z - zHeight;
    
      v[0] = firstPoint;               
      v[1] = secondPoint;              
      v[2] = thirdPoint;               
      v[3] = fourthPoint;              

      avgNormal[0] = 0.0;
      avgNormal[1] = 0.0;
      avgNormal[2] = -1.0;

      pIOpenGLImplementation -> Normal3dv(avgNormal);

      for ( vk = 0; vk < 4; vk++ ) 
         pIOpenGLImplementation -> Vertex(&v[vk]);

      pIDataSet -> peek(pItem,&pItem);
    
   }

   pIOpenGLImplementation -> BeginOpenGLMode(GL_QUADS);

   pIOpenGLImplementation -> CloseSegment(segmentID,TRUE);

   return;
   }