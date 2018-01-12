
#pragma once

#include <Windows.h>
#include <stddef.h>
#include <process.h>

#include "general.h"

#include "GSystem_i.h"
#include "GraphicControl_i.h"
#include "Properties_i.h"
#include "DataSet_i.h"

#include "Variable_i.h"
#include "Evaluator_i.h"

#include "OpenGLImplementation_i.h"
#include "Plot_i.h"
#include "Text_i.h"

#include "list.h"

  class BasePlot : public IBasePlot {

  public:

     BasePlot();
     virtual ~BasePlot();

     STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
     STDMETHOD_ (ULONG, AddRef)();
     STDMETHOD_ (ULONG, Release)();

  protected:

     STDMETHOD(Initialize)(IDataSet *,IOpenGLImplementation *,IEvaluator *,IGProperty*,IGProperty*,IGProperty*,IGProperty*);

     STDMETHOD(Erase)();
     STDMETHOD(AddText)(void *text,char *name,int id);
     STDMETHOD(RemoveText)(void *t);
     STDMETHOD(PlotData)(IDataSet *ds); // <<---- not in the interface yet

     STDMETHOD(StubPlot)();
     STDMETHOD(Draw)();
     STDMETHOD(Redraw)();

     STDMETHOD(put_IDataSet)(IDataSet *newDataSet);
     STDMETHOD(get_IDataSet)(IDataSet **getDataSet);

     enum DataArity __stdcall DataArity();

     STDMETHOD(get_SegmentID)(long*);

     STDMETHOD(put_ActionTable)(IGraphicSegmentAction *);
     STDMETHOD(get_ActionTable)(IGraphicSegmentAction**);

     STDMETHOD(put_PlotView)(PlotViews plotView);
     STDMETHOD(get_PlotView)(PlotViews *plotView);

     STDMETHOD(put_PlotType2D)(gc2DPlotTypes plotType);
     STDMETHOD(get_PlotType2D)(gc2DPlotTypes *plotType);

     STDMETHOD(put_PlotType3D)(gc3DPlotTypes plotType);
     STDMETHOD(get_PlotType3D)(gc3DPlotTypes *plotType);

     STDMETHOD(put_ColorProperty)(IGProperty*);
     STDMETHOD(get_ColorProperty)(IGProperty**);

     STDMETHOD(put_LineWeight)(IGProperty*);
     STDMETHOD(get_LineWeight)(IGProperty**);

     STDMETHOD(get_SegmentCount)(long *);
     STDMETHOD(GetSegments)(long *);

     STDMETHOD(Create)(long *);
     STDMETHOD(Destroy)();

     STDMETHOD(DrawPolygon)();

     STDMETHOD(DrawGDI)();

     STDMETHOD(RemoveFromWorldDomain)();

     STDMETHOD(GetDomainGDI)(struct DataPoint* minPoint,struct DataPoint* maxPoint);

     virtual HANDLE PlotData(DataPoint v[],IGraphicSegment *segmentOveride = (IGraphicSegment *)NULL);

     int ReplaceDataSet(IDataSet *newDataSet);

     int newline(DataPoint);
     int vertex(DataPoint);

     IDataSet* pIDataSet;
     IDataSet* pIDataSetDomain;

     void (__stdcall *pWhenChangedCallback)(void *,ULONG_PTR);
     void *pWhenChangedCallbackArg;
     ULONG_PTR whenChangedCallbackCookie;

     PlotViews plotView;
     long plotType2D;
     long plotType3D;
     USHORT externalDataSet,eraseMode;

     List<IText> textList;

     IGraphicSegment *pIGraphicSegment;
     IOpenGLImplementation *pIOpenGLImplementation;
     IEvaluator *pIEvaluator;
     IGProperty* pPropertyLastDrawLineWeight;
     IGProperty* pPropertyFloor;
     IGProperty* pPropertyCeiling;

     unsigned int refCount;

     friend class BasePlotFactory;

  };