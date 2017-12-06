
#pragma once

#define DOUBLE_AS_DOUBLE

#ifdef DOUBLE_AS_LONG_DOUBLE
#define DOUBLE long double
#else
#ifdef DOUBLE_AS_DOUBLE
#define DOUBLE double
#endif
#endif

#ifndef __FLOAT_H
#include <float.h>
#endif
#include <limits.h>
#include <memory.h>
#include <stddef.h>

#define EXPRESSION_SIZE             1024
#define NAME_SIZE                    128
#define IND_VAR_SIZE                  32

#define MAX_QUEUE_BUFFER_SIZE      63488

#define G_UTILITY_STRING_SIZE        256
#define GENERAL_STRING_SIZE          256
#define BIG_STRING_SIZE             1024
#define GENERAL_FILE_NAME_SIZE       128

#define EVALstringSIZE               128
#define EVALnameSIZE                  32
#define EVALresultSIZE                32

#define DATAENGINE_OWNER_TYPE   0x1

#define INDVAR_OFFSET_X  24
#define INDVAR_OFFSET_Y  30

#define PLOT_VALUE_DELIMITERS " \t,"

#define EVALOPERATORS "+-/*^"
#define VARIABLE_DELIMITERS ", =*+-/^)"
#define FUNCTION_DELIMITERS "([])"

#define DB_RAISED       0x0400
#define DB_DEPRESSED    0x0800
#define DB_TROUGH       0x1000
#define DB_FENCE        0x2000
#define DB_FIELD        0x4000
#define DB_CORNERBORDER 0x8000

#define CLR_RED                (float)1.0f,(float)0.0f,(float)0.0f,(float)1.0f
#define CLR_GREEN              (float)0.0f,(float)1.0f,(float)0.0f,(float)1.0f
#define CLR_BLUE               (float)0.0f,(float)0.0f,(float)0.5f,(float)1.0f
#define CLR_WHITE              (float)1.0f,(float)1.0f,(float)1.0f,(float)1.0f
#define CLR_BLACK              (float)0.0f,(float)0.0f,(float)0.0f,(float)1.0f
#define CLR_GRAY75             (float)0.75f,(float)0.75f,(float)0.75f,(float)1.0f
#define CLR_GRAY50             (float)0.50f,(float)0.50f,(float)0.50f,(float)1.0f
#define CLR_GRAY25             (float)0.25f,(float)0.25f,(float)0.25f,(float)1.0f

#define UNITS_ARRAY {"pixel","percent",""}//,"Percent",""}//"inch","cm","mm"}
#define TEXT_UNITS_ARRAY {"pixel","percent","points",""}

  typedef struct Z_INDEX {
     DOUBLE z;
     ULONG indexValue;
  } Z_INDEX;


  typedef union tValue {
     DOUBLE doubleValue;
     ULONG  ulongValue;
     LONG   longValue;
     USHORT ushortValue;
     SHORT  shortValue;
     BYTE   byteValue;
  } tValue;

  typedef DOUBLE DATATYPE;

  typedef struct LDATAPOINT {
     LONG x,y;
  } LDATAPOINT;

  typedef struct CallTable {
     int (*selector)(void *);
     int (*unselector)(void *);
     int (*menuRequest)(void *);
     int (*leftMouse)(void *);
     int (*rightMouse)(void *);
  } CallTable;

  typedef struct MessageTableEntry {
     UINT messageNumber;
     LRESULT (*messageInterface)(void *,HWND,UINT,WPARAM,LPARAM);
     HWND hwndTarget;
  } MessageTableEntry;

  typedef struct MessageTable {
     int size;
     MessageTableEntry *start;
  } MessageTable ;

#define _atold(x) atol(x)

#define PARENT_PTR(toClass,member) (toClass*)((BYTE*)(this) - offsetof(toClass,member))

