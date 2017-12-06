#ifndef DO_DEFINED
#define DO_DEFINED

#include <commctrl.h>

#include "list.h"
#include "Properties.h"
#include "dataset.h"
#include "analyzer.h"
#include "statusline.h"

  class fileItem;

  class DataObject : public Properties {
  public:

     __declspec(dllexport) DataObject(HWND hwndOwner,char *ownersName,int ownersCount = 0);
     __declspec(dllexport) ~DataObject();

     __declspec(dllexport) int ShowSpecWindow();
     __declspec(dllexport) HWND SpecWindow();

     __declspec(dllexport) char *Name(char *newName = (char *)NULL);

     __declspec(dllexport) LONG Start();
     __declspec(dllexport) LONG Stop();
     __declspec(dllexport) LONG Pause();
     __declspec(dllexport) LONG Resume();

     __declspec(dllexport) int Finished();

     __declspec(dllexport) int SetPropertyDefaults();
     __declspec(dllexport) int RestoreProperties();
     __declspec(dllexport) int SaveProperties();

  private:
     static long dataObjectCount;
     int idForOwner,activeFileItem;
     HWND hwndSpecDialog,hwndOwner,hwndFilelist,hwndFilelistMenu;
     char name[NAME_SIZE];
     ULONG threadID;
     long fileTypeCount,countInFileList;
     List<DataSet> dataSetList;
     List<fileItem> fileItemList;
     HDC deviceContext;
     USHORT working,makingConnection;
     ULONG segmentNo;
     StatusLine *statusLine;
     TanalysisTable *analysisTable;

     HIMAGELIST imageListSmall,imageListBig;

     Property *propertyFrameLeft;
     Property *propertyFrameTop;
     Property *propertyDefaultFileSpec;
     Property *propertyFileList;

     LRESULT (CALLBACK *defaultContainerHandler)(HWND,UINT,WPARAM,LPARAM);

     int processFiles();

     static LRESULT CALLBACK doSpecHandler(HWND,UINT,WPARAM,LPARAM);
     friend unsigned int __stdcall doEvaluate(void *);
     friend LRESULT CALLBACK doLeastSquaresHandler(HWND,UINT,WPARAM,LPARAM);
     friend LRESULT CALLBACK doEvaluatorObjectWindow(HWND,UINT,WPARAM,LPARAM);
     static LRESULT CALLBACK fileListContainerHandler(HWND,UINT,WPARAM,LPARAM);
     friend LRESULT CALLBACK analysisListContainerHandler(HWND,UINT,WPARAM,LPARAM);

     class fileItem {
     public:
        HWND hwndFilelistItemMenu;
        char name[BIG_STRING_SIZE];
        Property *propertyDoAnalysis;
        Property *propertyDoRaw;
        Property *propertySelected;

        fileItem(char *fileName);
        ~fileItem();
     };

  };

#define FILELIST_X      6
#define FILELIST_Y     65
#define FILELIST_CX   120
#define FILELIST_CY    64

#endif
