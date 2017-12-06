
#pragma once

#include <list>

#include "GSystem_i.h"
#include "Properties_i.h"
#include "DataSet_i.h"

#include "Variable_i.h"
#include "Evaluator_i.h"

#include "OpenGLImplementation_i.h"

#include "List.h"

   struct boundingBox {
      boundingBox(double *pxmin,double *pymin,double *pzmin,double *pxmax,double *pymax,double *pzmax) :
                pxMin(pxmin),pyMin(pymin),pzMin(pzmin),pxMax(pxmax),pyMax(pymax),pzMax(pzmax) {};
      double *pxMin,*pyMin,*pzMin,*pxMax,*pyMax,*pzMax;
   };

   struct extents {
      double xMin,yMin,zMin,xMax,yMax,zMax;
   };

   class DataSet : public IDataSet  {
   public:
 
      DataSet(IUnknown *pUnknownOuter);
      ~DataSet();
 
//   IDataSet
 
      STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_(ULONG, AddRef)();
      STDMETHOD_(ULONG, Release)();
 
      STDMETHOD(put_maxX)(double);                     
      STDMETHOD(get_maxX)(double *getVal);                     
      STDMETHOD(put_maxY)(double);                     
      STDMETHOD(get_maxY)(double *getVal);                     
      STDMETHOD(put_maxZ)(double);
      STDMETHOD(get_maxZ)(double *getVal);                     
      STDMETHOD(put_minX)(double);
      STDMETHOD(get_minX)(double *getVal);                     
      STDMETHOD(put_minY)(double);
      STDMETHOD(get_minY)(double *getVal);                     
      STDMETHOD(put_minZ)(double);
      STDMETHOD(get_minZ)(double *getVal);                     
                                                                 
      STDMETHOD(put_floor)(IGProperty *);
      STDMETHOD(put_ceiling)(IGProperty *);

      STDMETHOD(get_top)(DataList **getTop);                    
      STDMETHOD(get_bottom)(DataList **getBottom);              
 
      STDMETHOD(get_countPoints)(long*);
 
      STDMETHOD(put_DataArity)(DataArity);
      STDMETHOD(get_DataArity)(DataArity *);

      STDMETHOD(peek)(DataList *afterItem,DataList **getItem);                     
      STDMETHOD(peekInt)(int item,DataList **getItem);
 
      STDMETHOD(peekGDI)(DataList *afterItem,DataList **getItem);                     
 
      STDMETHOD(get)(DataList *AfterItem,DataPoint *data,DataList **nextItem);                   
      STDMETHOD(set)(DataList *item,DataPoint *data);

      STDMETHOD(pushString)(char *valueString);
 
      STDMETHOD(pushDataPoint)(DataPoint *point);
 
      STDMETHOD(popDataPoint)(DataPoint *point);
 
      STDMETHOD(ReSet)();
 
      STDMETHOD(insert)(DataList *insertBefore,DataPoint *data);
     
      STDMETHOD(addXYZ)(double *,double *, double *);

      STDMETHOD(addMovePoint)();

      STDMETHOD(makeUnitSize)();
 
      STDMETHOD(ResetLimits)(DataPoint*);
 
      STDMETHOD(SetDomain)(DataPoint*,DataPoint*);

      STDMETHOD(SetDomainXYZ)(double *xmin,double *ymin,double *zmin,double *xmax,double *ymax,double *zmax);
 
      STDMETHOD(GetDomain)(DataPoint*,DataPoint*);
 
      STDMETHOD(IncludeDomain)(IDataSet*);
 
      STDMETHOD(get_IsIncludedDomain)(IDataSet*,boolean *);

      STDMETHOD(RemoveIncludedDomain)(IDataSet*);
 
      STDMETHOD(SetColorRGB)(double *red,double *green,double *blue);

      STDMETHOD(Scale)(DataPoint*);
 
      STDMETHOD(Translate)(DataPoint*);
 
      STDMETHOD(TranslateGDI)(POINT*);
 
      STDMETHOD(Rotate)(char,double);
 
      STDMETHOD(RotateVector)(double,DataPoint*);

      STDMETHOD(Shear)(char,double);
 
      STDMETHOD(Move)(DataPoint*);
 
      STDMETHOD(GenerateBoundingBox)(IDataSet*);
 
      STDMETHOD(GenerateGDICoordinates)(IUnknown*);

      STDMETHOD(PushExtents)();

      STDMETHOD(PopExtents)();

      STDMETHOD(Evaluate)(BSTR,double *);

      STDMETHOD(GetDomainGDI)(struct DataPoint* minPoint,struct DataPoint* maxPoint);
 
   private:
 
      boundingBox *pBoundingBox;
 
      IUnknown *pUnknownOuter;
 
      unsigned int refCount;
 
      double xMax,xMin,yMax,yMin,zMax,zMin;
      double currentColor[3];
      char szEquations[1024];

      IGProperty *pPropertyFloor,*pPropertyCeiling;
      IEvaluator *pIEvaluator;

      DataList *firstData,*topData;
      DataList *gdiData;
      DataArity dataArity;

      std::list<IDataSet *> otherDomains;
      std::list<extents *> extentsStack;
 
      int resetLimits(const DataPoint&);
      int resizeBounds();
 
   };

   extern DataPoint movePoint;

