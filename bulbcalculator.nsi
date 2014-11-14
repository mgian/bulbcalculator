;BulbCalculator Installer

!define PRODUCT_NAME "BulbCalculator" 
!define PRODUCT_VERSION "2.2.0"
!define PRODUCT_DIR_REGKEY "Software\${PRODUCT_NAME}"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\bulbcalculator-${PRODUCT_VERSION}-win32.exe"

SetCompressor lzma

InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
InstallDirRegKey HKCU "${PRODUCT_DIR_REGKEY}" ""

!include "MUI.nsh"

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_LICENSE "COPYING"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\bulbcalculator.exe"
!define MUI_FINISHPAGE_LINK "Visit the BulbCalculator website"
!define MUI_FINISHPAGE_LINK_LOCATION "http://bulbcalculator.wordpress.com/"
!insertmacro MUI_PAGE_FINISH
  
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
  
!insertmacro MUI_LANGUAGE "English"

Section "BulbCalculator"  SecBulbCalculator
  SectionIn RO

  SetOutPath "$INSTDIR"
  WriteRegStr HKCU "${PRODUCT_DIR_REGKEY}" "" $INSTDIR

  File release\bin\bulbcalculator.exe
  File release\bin\*.dll
  ;File ChangeLog
  File COPYING
  ;File README
  ;File /r data
  File /r release\bin\share

  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Create Desktop Shortcut" DescShortcut
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\bulbcalculator.exe" "" "$INSTDIR\share\images\bulbcalculator.ico"
SectionEnd

Section "Start Menu Shortcuts" SecShortcut
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\BulbCalculator.lnk" "$INSTDIR\bulbcalculator.exe" "" "$INSTDIR\share\images\bulbcalculator.ico"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

;Section "Register File Types" SecFileType
;  WriteRegStr HKCR "BulbCalculator.blb" "" "BulbCalculator Bulb definition"
;  WriteRegStr HKCR "BulbCalculator.blb\shell\open\command" "" '"$INSTDIR\bulbcalculator.exe" "%1"'
;  WriteRegStr HKCR "BulbCalculator.blb\DefaultIcon" "" "$INSTDIR\share\images\bulbcalculator.ico"
;  WriteRegStr HKCR ".blb" "" "BulbCalculator.blb"
;SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecBulbCalculator} "Installs BulbCalculator."
  !insertmacro MUI_DESCRIPTION_TEXT ${DescShortcut} "Installs BulbCalculator."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecShortcut} "Adds share to your start menu for easy access."
;  !insertmacro MUI_DESCRIPTION_TEXT ${SecFileType} "Register BulbCalculator file types."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"
  DeleteRegKey HKCR "BulbCalculator.blb"
  DeleteRegKey HKCR ".blb"
  DeleteRegKey HKCU "${PRODUCT_DIR_REGKEY}"

  RMDir /r "$SMPROGRAMS\${PRODUCT_NAME}"

  Delete "$INSTDIR\bulbcalculator.exe"
  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\ChangeLog"
  Delete "$INSTDIR\COPYING"
  Delete "$INSTDIR\README"
  RMDir /r "$INSTDIR\data"
  RMDir /r "$INSTDIR\share"
  RMDir /r "$INSTDIR\doc"
  Delete "$INSTDIR\uninstall.exe"
  RMDir "$INSTDIR"
SectionEnd
