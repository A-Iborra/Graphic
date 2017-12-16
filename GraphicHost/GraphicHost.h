#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <gl\GL.h>
#include <stdio.h>
#include <float.h>

#include "Graphic_resource.h"

#include "GSystem_i.h"
#include "Properties_i.h"
#include "DataSet_i.h"
#include "Variable_i.h"
#include "Evaluator_i.h"
#include "OpenGLImplementation_i.h"
#include "Plot_i.h"

#define WM_STARTOPENGL     (WM_USER) + 1
#define WM_SETSIZES        (WM_USER) + 2

   LRESULT __stdcall handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

extern IOpenGLImplementation *pIOpenGLImplementation;
extern IGProperties *pIGProperties;
extern IGProperty *pIPropertyBackground;
extern IGProperty *pIPropertyPlotView;
extern IDataSet *pIDataSetMaster;
extern IPlot *pIPlot;

extern HWND hwndFrame;
extern HWND hwndClient1;
extern HWND hwndClient2;

extern HGLRC renderingContext;

extern GLint viewPort[];
extern GLdouble projectionMatrix[];
extern GLdouble modelMatrix[];