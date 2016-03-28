; sg2ps.nsi

!define MUI_PRODUCT "SG2PS"
!define MUI_FILE "sg2ps_gui"

; The name of the installer
Name "Installer of SG2PS software"

; The name of the future install file
OutFile "setup_sg2ps.exe"

; The default installation directory
InstallDir C:\sg2ps

; The text to prompt the user to enter a directory
DirText "This will install SG2PS on your computer. Please choose a directory."


LicenseData "LICENSES.txt"

page license
page directory
Page instfiles


; The stuff to install---------------------------------------------------------------------------
Section "Install"

!define MUI_PAGE_LICENSE "LICENSES.txt"

; Set output path to the installation directory.
SetOutPath $INSTDIR


File libgcc_s_dw2-1.dll
File mingwm10.dll
File QtCore4.dll
File QtGui4.dll
File gsdll32.dll
File gsdll32.lib
File python27.dll
File library.zip
File pdf_concat.exe
File ps2pdf.exe
File sg2ps.exe
File sg2ps_gui.exe
File demo.rgf
File manual.pdf
File LICENSES.txt

CreateShortCut "$DESKTOP\${MUI_PRODUCT}.lnk" "$INSTDIR\${MUI_FILE}.exe" ""


SectionEnd

Function .onInstSuccess
  MessageBox MB_OK "You have successfully installed ${MUI_PRODUCT}. Use the desktop icon to start the program."
FunctionEnd


