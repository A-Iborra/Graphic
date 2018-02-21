;
; If you are not Nate Clark - you should define the macro below - define DONT_SIGN
; because you don't have the signing certificate or nsis include scripts that use the
; signing tool.
;
; If the build fails, note it is probably when trying to include these files, if you make your
; own such include files, note that row entries in these files are in the form:
;
;     !system 'signtool sign /tr "http://timestamp.digicert.com" /f "<path to your certificate>\certificate.pfx" /p <password> "..\..\Common\Artifacts\${CONFIGURATION}\${ARCH}\GraphicHost.exe"'
;
;!define DONT_SIGN

!define INFILE GraphicRelease32.nsi

!define OUTFILE GraphicSetup_x86.exe

!define ARCH win32

!define CONFIGURATION Release

!include "GraphicBase.nsi"
