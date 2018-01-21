// Copyright 2018 InnoVisioNate Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLImplementor.h"


   HRESULT OpenGLImplementor::OpenSegment(long segmentID,IGProperty* pPropColor,IGProperty* pPropLineWeight) {
   strCall_OpenSegment *ps = new strCall_OpenSegment();
   ps -> segmentID = segmentID;
   ps -> pPropColor = pPropColor;
   ps -> pPropLineWeight = pPropLineWeight;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_OPENSEGMENT,ps)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::RedrawSegment(long segmentID) {
   long *pSegmentID = new long(segmentID);
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_REDRAWSEGMENT,pSegmentID)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::CloseSegment(long segmentID,unsigned short drawOnClose) {
   strCall_CloseSegment *ps = new strCall_CloseSegment();
   ps -> segmentID = segmentID;
   ps -> drawOnClose = drawOnClose;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_CLOSESEGMENT,ps)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::NewSegments(long numberToMake,long *firstInRange) {
   strCall_NewSegments *ps = new strCall_NewSegments();
   ps ->  numberToMake = numberToMake;
   ps -> firstInRange = firstInRange;
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_NEWSEGMENTS,ps)
   return S_OK;
   }

 
   HRESULT OpenGLImplementor::StartSegment(long segmentID) {
   long *pSegmentID = new long(segmentID);
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_STARTSEGMENT,pSegmentID)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::EndSegment() {
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_ENDSEGMENT,0L)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::PlaySegment(long segmentID) {
   long *pSegmentID = new long(segmentID);
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_PLAYSEGMENT,pSegmentID)
   return S_OK;
   }
 
 
   HRESULT OpenGLImplementor::DeleteSegment(long segmentID) {
   long *pSegmentID = new long(segmentID);
   SYNCHRONOUS_CALL(WM_OPENGLIMPLEMENTATION_DELETESEGMENT,pSegmentID)
   return S_OK;
   }