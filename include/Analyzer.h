#ifndef ANALYZER_DEFINED
#define ANALYZER_DEFINED

#include "DataSet.h"

  typedef struct _TanalysisTable {
     char name[32];
     char tabName[32];
     int selected;
     int settingsTag;
     HWND hwndSettings;
     ULONG pageNo;
     LONG linkColor;
     int (*analyzer)(DataSet *,DataSet *,HWND);
  } TanalysisTable;

  int LeastSquares(DataSet *inData,DataSet *outData,HWND specWindow);
  int LeastSquares3D(DataSet *inData,DataSet *outData,HWND specWindow);
  int Quait(DataSet *inData,DataSet *outData,HWND specWindow);
  DOUBLE interp(DOUBLE a,DOUBLE b,DOUBLE c,DOUBLE d,DOUBLE x);
  DOUBLE porob(DOUBLE x1,DOUBLE y1,DOUBLE x2,DOUBLE y2,DOUBLE x3,DOUBLE y3,DOUBLE x,int set,int time);

  int Derivate(DataSet *inData,DataSet *outData,HWND specWindow);
  int Integrate(DataSet *inData,DataSet *outData,HWND specWindow);

  __declspec(dllexport) int AnalysisTableSize();
  __declspec(dllexport) HWND AnalyzerMenu();
  __declspec(dllexport) int AnalyzerMenuID(int index);
  __declspec(dllexport) int CountAnalyzers();
  __declspec(dllexport) int LoadAnalyzers(TanalysisTable *t,HWND hwndParent,HWND hwndNotebook,
                                           WNDPROC specHandler,
                                           void *param);

#endif
