// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Graphic.h"
#include <list>

#include "Graphic_resource.h"
#include "utils.h"
#include "list.cpp"

   int G::doPickBox(POINTL *ptl) {
 
   if ( hitTable ) 
      return FALSE;
 
   if ( ! pIOpenGLImplementation ) 
      return FALSE;
 
   long *pCallLists = NULL;

   getSegments(&pCallLists);

   hitTable = new unsigned int[HIT_TABLE_SIZE];
 
   hitTableHits = 0;

   if ( pCallLists ) {

      pIOpenGLImplementation -> GetPickBoxHits(ptl,pickBoxSize.cx,hitTable,HIT_TABLE_SIZE,pCallLists,&hitTableHits);

      delete [] pCallLists;

      std::list<List<IText> *> textLists;

      textLists.push_back(&textList);

      IPlot *pIPlot = NULL;
      while ( pIPlot = plotList.GetNext(pIPlot) ) {
         List<IText> *pList = NULL;
         pIPlot -> GetTextList((void **)&pList);
         textLists.push_back(pList);
      }

      IAxis *pIAxis = NULL;
      while ( pIAxis = axisList.GetNext(pIAxis) ) {
         List<IText> *pList = NULL;
         pIAxis -> GetTextList((void **)&pList);
         textLists.push_back(pList);
      }

      for ( List<IText> *pTextList : textLists ) {

         IText *pText = NULL;

         while ( pText = pTextList -> GetNext(pText) ) {

            boolean isOpenGLRendered = false;

            pText -> get_TextRenderOpenGL(&isOpenGLRendered);

            if ( isOpenGLRendered )
               continue;

            RECT rcText;

            pText -> get_GDIBoundingBox(&rcText);

            if ( ptl -> x < rcText.left || ptl -> x > rcText.right || ptl -> y < rcText.top || ptl -> y > rcText.bottom )
               continue;

            hitTableHits++;

         }

      }

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

   long hitTableIndex = hitTableHits * 4;

   std::list<List<IText> *> textLists;

   textLists.push_back(&textList);

   IPlot *pIPlot = NULL;
   while ( pIPlot = plotList.GetNext(pIPlot) ) {
      List<IText> *pList = NULL;
      pIPlot -> GetTextList((void **)&pList);
      textLists.push_back(pList);
   }

   IAxis *pIAxis = NULL;
   while ( pIAxis = axisList.GetNext(pIAxis) ) {
      List<IText> *pList = NULL;
      pIAxis -> GetTextList((void **)&pList);
      textLists.push_back(pList);
   }

   for ( List<IText> *pTextList : textLists ) {

      IText *pText = NULL;

      while ( pText = pTextList -> GetNext(pText) ) {
         boolean isOpenGLRendered = false;
         pText -> get_TextRenderOpenGL(&isOpenGLRendered);
         if ( isOpenGLRendered )
            continue;
         RECT rcText;
         pText -> get_GDIBoundingBox(&rcText);
         if ( ptl -> x < rcText.left || ptl -> x > rcText.right || ptl -> y < rcText.top || ptl -> y > rcText.bottom )
            continue;
         long segmentID;
         pText -> get_SegmentID(&segmentID);
         hitTable[hitTableIndex] = 1;
         hitTable[hitTableIndex + 3] = (unsigned int)segmentID;
         hitTableHits++;
         hitTableIndex += 4;
      } 

   }

   delete [] pCallLists;

   if ( actionFunction ) {
      if ( forceToThread ) {
#if 0
         unsigned int threadAddr;
        _beginthreadex(NULL,4096,actionFunction,(void *)this,0,&threadAddr);
#else
         actionFunction((void*)this);
#endif
         return TRUE;
      }
      else 
         return (actionFunction)((void *)this);
   }
 
   return TRUE;
   }


   unsigned int __stdcall G::processSelections(void *pvArg) {
   G *p = (G *)pvArg;
   std::function<HRESULT(IGraphicSegmentAction *)> *pTask = new std::function<HRESULT(IGraphicSegmentAction *)>( [=](IGraphicSegmentAction *pCallTable) { return pCallTable -> Selector(); });
   HRESULT rc = processAction(pvArg,&p -> pSelectedGraphicSegmentAction,false,pTask);
   delete pTask;
   return rc;
   }


   unsigned int __stdcall G::processMenus(void *pvArg) {
   std::function<HRESULT(IGraphicSegmentAction *)> *pTask = new std::function<HRESULT(IGraphicSegmentAction *)>( [=](IGraphicSegmentAction *pCallTable) { return pCallTable -> MenuRequest(); });
   HRESULT rc = processAction(pvArg,NULL,false,pTask);
   delete pTask;
   return rc;
   }


   unsigned int __stdcall G::processDefaultAction(void *pvArg) {
   std::function<HRESULT(IGraphicSegmentAction *)> *pTask = new std::function<HRESULT(IGraphicSegmentAction *)>( [=](IGraphicSegmentAction *pCallTable) { return pCallTable -> DefaultAction(); });
   HRESULT rc = processAction(pvArg,NULL,true,pTask);
   delete pTask;
   return rc;
   }


   unsigned int __stdcall G::processAction(void *pvArg,IGraphicSegmentAction **ppRetainCallTable,boolean allowMultiple,std::function<HRESULT(IGraphicSegmentAction *)> *pTask) {

   G *p = (G *)pvArg;
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
 
         if ( found ) 
            continue;
 
         pPlot = NULL;
         while ( pPlot = p -> visiblePlotList.GetNext(pPlot) ) {
            pPlot -> get_SegmentID(&segID);
            if ( (unsigned int)(segID) == hitID ) {
               hitList.Add(pTable - 1,(char *)NULL,hitID);
               found = true;
               break;
            }
         }
 
         if ( found ) 
            continue;
 
         pAxis = NULL;
         List<IText> *pTextList;
         while ( pAxis = p -> axisList.GetNext(pAxis) ) {
            pAxis -> GetTextList((void **)&pTextList);
            IText *pIText = NULL;
            while ( pIText = pTextList -> GetNext(pIText) ) {
               pIText -> get_SegmentID(&segID);
               if ( (unsigned int)(segID) == hitID ) {
                  hitList.Add(pTable - 1,(char *)NULL,(int)hitID);
                  found = true;
                  break;
               }
            }
 
            if ( found ) 
               break;
         }
 
         if ( found ) 
            continue;
 
         pPlot = NULL;
         while ( pPlot = p -> plotList.GetNext(pPlot) ) {
            pPlot -> GetTextList((void **)&pTextList);
            IText *pIText = reinterpret_cast<IText *>(NULL);
            while ( pIText = pTextList -> GetNext(pIText) ) {
               pIText -> get_SegmentID(&segID);
               if ( (unsigned int)(segID) == hitID ) {
                  hitList.Add(pTable - 1,(char *)NULL,(int)hitID);
                  found = true;
                  break;
               }
            }
 
            if ( found ) 
               break;
         }
 
         if ( found ) 
            continue;
 
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
      while ( pPlot = p -> visiblePlotList.GetNext(pPlot) ) {
         pPlot -> get_SegmentID(&segID);
         if ( segID == objectID ) 
            break;
      }
 
      if ( pPlot ) {
         pPlot -> get_ActionTable(&pCallTable);
         if ( pCallTable ) {
            HRESULT rc = (*pTask)(pCallTable);
            if ( S_OK == rc && ppRetainCallTable )
               *ppRetainCallTable = pCallTable;
            if ( ! allowMultiple ) {
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
         if ( segID == objectID )
            break;
      }
 
      if ( pAxis ) {
         pAxis -> get_ActionTable(&pCallTable);
         if ( pCallTable ) {
            HRESULT rc = (*pTask)(pCallTable);
            if ( S_OK == rc && ppRetainCallTable )
               *ppRetainCallTable = pCallTable;
            if ( ! allowMultiple ) {
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
         pAxis -> GetTextList((void **)&pTextList);
         IText *pIText = NULL;
         while ( pIText = pTextList -> GetNext(pIText) ) {
            pIText -> get_SegmentID(&segID);
            if ( segID == objectID ) 
               break;
         }
         if ( pIText ) {
            pAxis -> get_ActionTable(&pCallTable);
            if ( pCallTable ) {
               HRESULT rc = (*pTask)(pCallTable);
               if ( S_OK == rc && ppRetainCallTable )
                  *ppRetainCallTable = pCallTable;
               if ( ! allowMultiple ) {
                  delete [] p -> hitTable;
                  p -> hitTable = (unsigned int *)NULL;
                  p -> hitTableHits = 0;
                  return TRUE;
               }
            }
         }
      }
 
      pPlot = NULL;
      while ( pPlot = p -> plotList.GetNext(pPlot) ) {
         List<IText> *pTextList;
         pPlot -> GetTextList((void **)&pTextList);
         IText *pIText = NULL;
         while ( pIText = pTextList -> GetNext(pIText) ) {
            pIText -> get_SegmentID(&segID);
            if ( segID == objectID ) 
               break;
         }
         if ( pIText ) {
            pIText -> get_ActionTable(&pCallTable);
            if ( pCallTable ) {
               HRESULT rc = (*pTask)(pCallTable);
               if ( S_OK == rc && ppRetainCallTable )
                  *ppRetainCallTable = pCallTable;
               if ( ! allowMultiple ) {
                  delete [] p -> hitTable;
                  p -> hitTable = (unsigned int *)NULL;
                  p -> hitTableHits = 0;
                  return TRUE;
               }
            }
         }
      }
 
      IText* pIText = NULL;
      while ( pIText = p -> textList.GetNext(pIText) ) {
         pIText -> get_SegmentID(&segID);
         if ( segID == objectID )
            break;
      }

      if ( pIText ) {
         pIText -> get_ActionTable(&pCallTable);
         if ( pCallTable ) {
            HRESULT rc = (*pTask)(pCallTable);
            if ( S_OK == rc && ppRetainCallTable )
               *ppRetainCallTable = pCallTable;
            if ( ! allowMultiple ) {
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