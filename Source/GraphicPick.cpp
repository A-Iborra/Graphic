/*

                       Copyright (c) 1996,1997,1998,1999,2000,2001,2002 Nathan T. Clark

*/

#include <windows.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>
#include <math.h>

#include "Graphic_resource.h"

#include "Graphic.h"
#include "utils.h"

#include "list.cpp"


   int G::doPickBox(POINTL *ptl) {
 
   if ( hitTable ) 
      return FALSE;
 
   if ( ! pIOpenGLImplementation ) return FALSE;
 
   long *pCallLists = NULL;

   getSegments(&pCallLists);

   hitTable = new unsigned int[HIT_TABLE_SIZE];
 
   hitTableHits = 0;

   if ( pCallLists ) {

      pIOpenGLImplementation -> GetPickBoxHits(ptl,pickBoxSize.cx,hitTable,HIT_TABLE_SIZE,pCallLists,&hitTableHits);

      delete [] pCallLists;

   }

   delete [] hitTable;

   hitTable = NULL;
   
   if ( hitTableHits ) 
      return 1;
 
   return 0;
   }
 
 
   int G::pick(POINTL *ptl,unsigned int (__stdcall *actionFunction)(void *),int forceToThread) {
 
   if ( hitTable ) 
      return FALSE;

   long *pCallLists = NULL;

   getSegments(&pCallLists);

   if ( ! pCallLists ) 
      return FALSE;

   hitTable = new unsigned int[HIT_TABLE_SIZE];

   if ( S_FALSE == pIOpenGLImplementation -> GetPickBoxHits(ptl,pickBoxSize.cx,hitTable,HIT_TABLE_SIZE,pCallLists,&hitTableHits) ) {
      delete [] pCallLists;
      delete [] hitTable;
      hitTable = NULL;
      return FALSE;
   }

   delete [] pCallLists;

   if ( actionFunction ) {
      if ( forceToThread ) {
///        unsigned int threadAddr;
///        _beginthreadex(NULL,4096,actionFunction,(void *)this,0,&threadAddr);
actionFunction((void*)this);
         return TRUE;
      }
      else 
         return (actionFunction)((void *)this);
   }
 
   return TRUE;
   }
 
 
   unsigned int __stdcall G::processSelections(void *arg) {

   G *p = reinterpret_cast<G *>(arg);

   unsigned int *pTable,hitID,nNamesThisHit;
   List<unsigned int> hitList;
   long segID;
 
   pTable = p -> hitTable;
 
   for ( unsigned int k = 0; k < p -> hitTableHits; k++ ) {
 
      nNamesThisHit = *pTable;
 
      pTable++;
      pTable++;
      pTable++;
 
      for ( unsigned int j = 0; j < nNamesThisHit; j++ ) {
 
         hitID = *pTable;
 
         pTable++;
 
         if ( hitList.Get((int)hitID) ) continue;
 
         IAxis *pAxis = reinterpret_cast<IAxis *>(NULL);
         while ( pAxis = p -> axisList.GetNext(pAxis) ) {
            pAxis -> get_SegmentID(&segID);
            if ( (unsigned int)(segID) == hitID ) {
               hitList.Add(pTable - 1,(char *)NULL,(int)hitID);
               break;
            }
         }
 
         if ( hitList.Get((int)hitID) ) continue;
 
         IPlot* pPlot = reinterpret_cast<IPlot *>(NULL);
         while ( pPlot = p -> plotList.GetNext(pPlot) ) {
            pPlot -> get_SegmentID(&segID);
            if ( (unsigned int)(segID) == hitID ) {
               hitList.Add(pTable - 1,(char *)NULL,hitID);
               break;
            }
         }
 
         if ( hitList.Get((int)hitID) ) continue;
 
         pAxis = reinterpret_cast<IAxis *>(NULL);
         while ( pAxis = p -> axisList.GetNext(pAxis) ) {
            List<IText> *pTextList;
            pAxis -> GetTextList(reinterpret_cast<void **>(&pTextList));
            IText *pIText = reinterpret_cast<IText *>(NULL);
            while ( pIText = pTextList -> GetNext(pIText) ) {
               pIText -> get_SegmentID(&segID);
               if ( (unsigned int)(segID) == hitID ) {
                  hitList.Add(pTable - 1,(char *)NULL,(int)hitID);
                  break;
               }
            }
            if ( hitList.Get((int)hitID) ) break;
         }
 
         if ( hitList.Get((int)hitID) ) continue;
 
         IText *pText = reinterpret_cast<IText*>(NULL);
         while ( pText = p -> textList.GetNext(pText) ) {
            pText -> get_SegmentID(&segID);
            if ( (unsigned int)(segID) == hitID ) {
               hitList.Add(pTable - 1,(char *)NULL,hitID);
               break;
            }
         }
 
      }
   }
 
   IGraphicSegmentAction *pCallTable;
 
   unsigned int *pID = (unsigned int *)NULL;
 
   while ( pID = hitList.GetNext(pID) ) {
 
      long objectID = *pID;
 
      IPlot* pPlot = reinterpret_cast<IPlot *>(NULL);
      while ( pPlot = p -> plotList.GetNext(pPlot) ) {
         pPlot -> get_SegmentID(&segID);
         if ( segID == objectID ) break;
      }
 
      if ( pPlot ) {
         pPlot -> get_ActionTable(&pCallTable);
         if ( pCallTable ) 
            pCallTable -> Selector();
         continue;
      }
 
      IAxis *pAxis = reinterpret_cast<IAxis *>(NULL);
      while ( pAxis = p -> axisList.GetNext(pAxis) ) {
         pAxis -> get_SegmentID(&segID);
         if ( segID == objectID ) break;
      }
 
      if ( pAxis ) {
         pAxis -> get_ActionTable(&pCallTable);
         if ( pCallTable ) 
            pCallTable -> Selector();
         continue;
      }
 
      IText *pIText = reinterpret_cast<IText *>(NULL);
      pAxis = reinterpret_cast<IAxis *>(NULL);
      while ( pAxis = p -> axisList.GetNext(pAxis) ) {
         int found = 0;
         List<IText> *pTextList;
         pAxis -> GetTextList(reinterpret_cast<void **>(&pTextList));
         pIText = reinterpret_cast<IText *>(NULL);
         while ( pIText = pTextList -> GetNext(pIText) ) {
            pIText -> get_SegmentID(&segID);
            if ( segID == objectID ) {
               pIText -> get_ActionTable(&pCallTable);
               if ( pCallTable ) {
                  pCallTable -> Selector();
                  found = 1;
                  break;
               }
            }
         }
         if ( found ) break;
      }
 
      if ( pIText )
         continue;
 
      while ( pIText = p -> textList.GetNext(pIText) ) {
         pIText -> get_SegmentID(&segID);
         if ( segID == objectID ) break;
      }
    
      if ( pIText ) {
         pIText -> get_ActionTable(&pCallTable);
         if ( pCallTable ) {
            if ( S_OK == pCallTable -> Selector() )
                p -> pSelectedGraphicSegmentAction = pCallTable;
            continue;
         }
      }
 
   }
 
   delete [] p -> hitTable;
   p -> hitTable = (unsigned int *)NULL;
   p -> hitTableHits = 0;
 
   return TRUE;
   }
 
 
   unsigned int __stdcall G::processMenus(void *arg) {

   G *p = (G *)arg;
   IPlot *pPlot;
   unsigned int *pTable,hitID,nNamesThisHit;
   List<unsigned int> hitList;
   long segID;
   bool found;
 
   pTable = p -> hitTable;
 
   for ( unsigned int k = 0; k < p -> hitTableHits; k++ ) {
 
      nNamesThisHit = *pTable;

      pTable++;
      pTable++;
      pTable++;
 
      for ( unsigned int j = 0; j < nNamesThisHit; j++ ) {
 
         found = false;
 
         hitID = *pTable;

         pTable++;
 
         if ( hitList.Get((int)hitID) ) continue;
 
         IAxis *pAxis = NULL;
         while ( pAxis = p -> axisList.GetNext(pAxis) ) {
            pAxis -> get_SegmentID(&segID);
            if ( (unsigned int)(segID) == hitID ) {
               hitList.Add(pTable - 1,(char *)NULL,(int)hitID);
               found = true;
               break;
            }
         }
 
         if ( found ) continue;
 
         pPlot = NULL;
         while ( pPlot = p -> plotList.GetNext(pPlot) ) {
            pPlot -> get_SegmentID(&segID);
            if ( (unsigned int)(segID) == hitID ) {
               hitList.Add(pTable - 1,(char *)NULL,hitID);
               found = true;
               break;
            }
         }
 
         if ( found ) continue;
 
         pAxis = NULL;
         List<IText> *pTextList;
         while ( pAxis = p -> axisList.GetNext(pAxis) ) {
            pAxis -> GetTextList(reinterpret_cast<void **>(&pTextList));
            IText *pIText = reinterpret_cast<IText *>(NULL);
            while ( pIText = pTextList -> GetNext(pIText) ) {
               pIText -> get_SegmentID(&segID);
               if ( (unsigned int)(segID) == hitID ) {
                  hitList.Add(pTable - 1,(char *)NULL,(int)hitID);
                  found = true;
                  break;
               }
            }
 
            if ( found ) break;
         }
 
         if ( found ) continue;
 
         IText *pText = (IText *)NULL;
         while ( pText = p -> textList.GetNext(pText) ) {
            pText -> get_SegmentID(&segID);
            if ( (unsigned int)(segID) == hitID ) {
               hitList.Add(pTable - 1,(char *)NULL,(int)hitID);
               break;
            }
         }
 
      }
   }
 
   IGraphicSegmentAction *pCallTable;
 
   unsigned int *pID = (unsigned int *)NULL;
 
   while ( pID = hitList.GetNext(pID) ) {
 
      long objectID = *pID;
 
      pPlot = (IPlot *)NULL;
      while ( pPlot = p -> plotList.GetNext(pPlot) ) {
         pPlot -> get_SegmentID(&segID);
         if ( segID == objectID ) 
            break;
      }
 
      if ( pPlot ) {
         pPlot -> get_ActionTable(&pCallTable);
         if ( pCallTable ) {
            if ( pCallTable -> MenuRequest() ) {
               delete [] p -> hitTable;
               p -> hitTable = (unsigned int *)NULL;
               p -> hitTableHits = 0;
               return TRUE;
            }
         }
         continue;
      } 
 
      IAxis *pAxis = NULL;
      while ( pAxis = p -> axisList.GetNext(pAxis) ) {
         pAxis -> get_SegmentID(&segID);
         if ( segID == objectID ) break;
      }
 
      if ( pAxis ) {
         pAxis -> get_ActionTable(&pCallTable);
         if ( pCallTable ) {
            if ( pCallTable -> MenuRequest() ) {
               delete [] p -> hitTable;
               p -> hitTable = (unsigned int *)NULL;
               p -> hitTableHits = 0;
               return TRUE;
            }
         }
         continue;
      }
  
      pAxis = NULL;
      while ( pAxis = p -> axisList.GetNext(pAxis) ) {
         List<IText> *pTextList;
         pAxis -> GetTextList(reinterpret_cast<void **>(&pTextList));
         IText *pIText = reinterpret_cast<IText *>(NULL);
         while ( pIText = pTextList -> GetNext(pIText) ) {
            pIText -> get_SegmentID(&segID);
            if ( segID == objectID ) break;
         }
         if ( pIText ) {
//            pIText -> get_ActionTable(&pCallTable);
            pAxis -> get_ActionTable(&pCallTable);
            if ( pCallTable ) {
               pCallTable -> MenuRequest();
               delete [] p -> hitTable;
               p -> hitTable = (unsigned int *)NULL;
               p -> hitTableHits = 0;
               return TRUE;
            }
         }
      }
 
      IText* pIText = NULL;
      while ( pIText = p -> textList.GetNext(pIText) ) {
         pIText -> get_SegmentID(&segID);
         if ( segID == objectID ) break;
      }

      if ( pIText ) {

         pIText -> get_ActionTable(&pCallTable);

         if ( pCallTable ) {
            if ( pCallTable -> MenuRequest() ) {
               delete [] p -> hitTable;
               p -> hitTable = (unsigned int *)NULL;
               p -> hitTableHits = 0;
               return TRUE;
            }
         }
      }
 
   }
 
   delete [] p -> hitTable;
   p -> hitTable = (unsigned int *)NULL;
   p -> hitTableHits = 0;
 
   return FALSE;
   }
