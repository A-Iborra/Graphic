
!define PRODUCT_NAME "Graphic"
!define PRODUCT_VERSION "5"
!define PRODUCT_PUBLISHER "InnoVisioNate"
!define PRODUCT_WEB_SITE "http://www.innovisionate.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\Graphic"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_MINOR_VERSION "0"
!define PRODUCT_MAJOR_VERSION "5"

!include "MUI2.nsh"
!include "Library.nsh"

; MUI Settings

!define MUI_ABORTWARNING
;!define MUI_ICON "..\..\writer.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

;!define MUI_WELCOMEFINISHPAGE_BITMAP "..\..\Resources\CursiVision-Rotated.bmp"

!define MUI_TEXT_WELCOME_INFO_TEXT "$\r$\n$\r$\nThis wizard will guide you through the installation of $(^NameDA).$\r$\n$\r$\n  $\r$\n$\r$\n$\r$\n$\r$\n$_CLICK"

!define MUI_TEXT_DIRECTORY_TITLE "Please choose the installation location"

!define MUI_TEXT_DIRECTORY_SUBTITLE

!insertmacro MUI_PAGE_WELCOME

;!insertmacro MUI_PAGE_LICENSE "..\EULA.rtf"

!insertmacro MUI_PAGE_DIRECTORY

!insertmacro MUI_PAGE_INSTFILES

;!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

!define MULTIUSER_EXECUTIONLEVEL Admin

!include MultiUser.nsh

!ifdef INNER

  !echo "Inner invocation"

  OutFile "D:\temp\tempinstaller.exe"

!else

  !echo "Outer invocation"

  ; Call makensis again, defining INNER.  This writes an installer for us which, when
  ; it is invoked, will just write the uninstaller to some location, and then exit.
  ; Be sure to substitute the name of this script here.

  !system "$\"${NSISDIR}\makensis$\" /DINNER ${INFILE}" = 0

  ; So now run that installer we just created as %TEMP%\tempinstaller.exe.  Since it
  ; calls quit the return value isn't zero.

  !system "D:\temp\tempinstaller.exe" = 2

  ; That will have written an uninstaller binary for us.  Now we sign it with your
  ; favourite code signing tool.

  !ifndef DONT_SIGN
  !system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "D:\temp\GraphicUninst.exe"'
  !endif
  
  ; Good.  Now we can carry on writing the real installer.

  OutFile "${OUTFILE}"

!endif

Name "${PRODUCT_NAME}"

InstallDir "$PROGRAMFILES\InnoVisioNate\Graphic"

InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""

ShowInstDetails show

ShowUnInstDetails show

SetPluginUnload  alwaysoff

Function .onInit

!ifdef INNER
  WriteUninstaller "D:\temp\GraphicUninst.exe"
  Quit
!endif

!insertmacro MULTIUSER_INIT

!echo '${ARCH}'

;${If} 'x64' == '${ARCH}';\
;
;   StrCpy $INSTDIR "$PROGRAMFILES64\InnoVisioNate\Graphic"
;
;   StrCpy $PostInstallDLL "PostInstall32"
;
;   SetRegView 64
;
;${Else}
;
;   strCpy $PostInstallDLL "PostInstall"
;
;   SetRegView 32
;
;${EndIf}

   SetOutPath "$INSTDIR"
  
   InitPluginsDir

   Return
   
FunctionEnd

Section "Graphic" SEC01

  SetOutPath "$INSTDIR"

  SetDetailsView show

  SetOverwrite on

!ifdef INNER
!ifndef DONT_SIGN
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\GraphicHost.exe"'
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Function.dll"'
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\DataSet.dll"'
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Evaluator.dll"'
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\ViewSet.dll"'
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Graphic.dll"'
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Text.dll"'
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Axis.dll"'
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\OpenGLImplementation.dll"'
!system 'signtool sign /tr "http://timestamp.digicert.com" /f "D:\InnoVisioNate\Code Signing Certificate\Nathan Clark.pfx" /p pubfly "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Properties.ocx"'
!endif
!endif

  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\GraphicHost.exe"
  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Function.dll"
  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\DataSet.dll"
  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Evaluator.dll"
  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\ViewSet.dll"
  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Graphic.dll"
  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Text.dll"
  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Axis.dll"
  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\OpenGLImplementation.dll"
  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\Properties.ocx"

  File "\Common\Artifacts\${CONFIGURATION}\${ARCH}\PostInstall*.dll"

  ${If} 'x64' == '${ARCH}'
;  ExecWait 'cursivision /regcomponents'
  ${EndIf}
  
  System::Call '$PostInstallDLL::registerAll(t "$INSTDIR") i .r1 ? u'

  SetShellVarContext all

  SetOverwrite ifnewer
  
  CreateDirectory "$APPDATA\InnoVisioNate\Graphic"
  
  SetOutPath "$INSTDIR"

  System::Call '$PostInstallDLL::GrantFullAccess(t "$APPDATA\InnoVisioNate\Graphic") i .r1'

  SetOutPath "$APPDATA\InnoVisioNate\Graphic"

  SetOverwrite ifnewer

  SetOutPath "$DESKTOP"
  
  SetOverwrite ifnewer
  
  SetOutPath "$INSTDIR"
  
  CreateShortCut "$DESKTOP\Graphic.lnk" "$INSTDIR\GraphicHost.exe"

  SetOutPath "$INSTDIR"

  System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'

SectionEnd


Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\Graphic\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\Graphic\Uninstall.lnk" "$INSTDIR\CVuninst.exe"
SectionEnd

Section -Post

!ifndef INNER
  SetOutPath $INSTDIR
  File "D:\temp\GraphicUninst.exe"
  !system 'del "D:\temp\GraphicUninst.exe"'
  !system 'del "D:\temp\tempinstaller.exe"'
!endif

  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\GraphicHost.exe"

  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\GraphicUninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\GraphicHost.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "VersionMajor" "${PRODUCT_MAJOR_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "VersionMinor" "${PRODUCT_MINOR_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "MajorVersion" "${PRODUCT_MAJOR_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "MinorVersion" "${PRODUCT_MINOR_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "InstallLocation" "$INSTDIR"
  
  WriteRegStr HKLM "Software\InnoVisioNate\Graphic" "Installation Directory" "$INSTDIR"
  
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} ""
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) has been removed."
FunctionEnd


Function un.onInit
!insertmacro MULTIUSER_UNINIT
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Do you want to remove $(^Name) ?" IDYES +2
  Abort
FunctionEnd

!ifdef INNER
Section Uninstall

  SetOutPath "$INSTDIR"

${If} 'x64' == '${ARCH}'
  ;ExecWait 'cursivision /unregcomponents'
${EndIf}

  System::Call '$PostInstallDLL::unRegisterAll(t "$INSTDIR") i .r1 ? u'

  Delete "$INSTDIR\*.*"
  
  RMDir "$INSTDIR"

  Delete "$DESKTOP\Graphic.lnk"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"

  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"

  DeleteRegKey HKLM "Software\InnoVisioNate\Graphic"

  SetAutoClose true

  System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'

SectionEnd
!endif