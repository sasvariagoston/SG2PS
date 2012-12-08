; sg2ps.nsi

; The name of the installer
Name "Installer of SG2PS software"

; The name of the future install file
OutFile "setup_sg2ps.exe"

; The default installation directory
InstallDir C:\sg2ps

; The text to prompt the user to enter a directory
DirText "This will install SG2PS version 0.2.0 on your computer. Please choose a directory."

; The stuff to install---------------------------------------------------------------------------
Section "Install"

; Set output path to the installation directory.
SetOutPath $INSTDIR

; Put file there
File libgcc_s_dw2-1.dll
File mingwm10.dll
File QtCore4.dll
File QtGui4.dll
File settings.txt
File sg2ps.exe
File sg2ps_gui.exe
File manual.pdf

SectionEnd


; uninstall---------------------------------------------------------------------------
Section "Uninstall"  

; First try to remove files that might be locked (if synergy is running)

Delete $INSTDIR\libgcc_s_dw2-1.dll
Delete $INSTDIR\mingwm10.dll
Delete $INSTDIR\QtCore4.dll
Delete $INSTDIR\QtGui4.dll
Delete $INSTDIR\settings.txt
Delete $INSTDIR\sg2ps.exe
Delete $INSTDIR\sg2ps_gui.exe    

; Remove files and directory
Delete $INSTDIR\*.*
RMDir $INSTDIR



SectionEnd