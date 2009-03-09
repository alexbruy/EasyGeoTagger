; easygt.nsi

; NSIS script to create an installer for easy geo tagger
; NSIS home page: http://nsis.sourceforge.net/


; The name of the installer
Name "EasyGeoTagger"

; The file to write
OutFile "EasyGeoTagger_0_3_0_OSGeo4W.exe"

; The default installation directory
InstallDir C:\OSGeo4W

; The text to prompt the user to enter a directory
DirText "This will install EasyGeoTagger on your computer. Choose the location where OSGeo4W is installed"

;--------------------------------

; The stuff to install
Section "Install" 

; Set output path to the installation directory.
SetOutPath $INSTDIR

; Put file there
File /r apps
File /r bin

; Write bat file
ClearErrors
FileOpen $0 $INSTDIR\bin\easygt.bat" w
IfErrors done
FileWrite $0 "@echo off$\n"
FileWrite $0 "SET OSGEO4W_ROOT=$INSTDIR$\n"
FileWrite $0 "@echo off$\n"
FileWrite $0 "start $\"EasyGeoTagger$\" /B $\"%OSGEO4W_ROOT%$\"\bin\easygeotagger.exe %*$\n"
FileClose $0
done:

; Create shortcuts
CreateDirectory "$SMPROGRAMS\EasyGeoTagger"

CreateShortCut "$SMPROGRAMS\EasyGeoTagger\easygt.lnk" "$INSTDIR\bin\easygt.bat" \
  "" "$INSTDIR\apps\easygt\icons\easygt.ico" "" SW_SHOWNORMAL \
  "" "EasyGeoTagger"
  
CreateShortCut "$SMPROGRAMS\EasyGeoTagger\uninstall.lnk" "$INSTDIR\UninstallEasyGt.exe" \
  "" "$INSTDIR\apps\easygt\Uninst.exe" "" SW_SHOWNORMAL \
  "" "Uninstall easygeotagger"
  
CreateShortCut "$DESKTOP\easygt.lnk" "$INSTDIR\bin\easygt.bat" \
  "" "$INSTDIR\apps\easygt\icons\easygt.ico" "" SW_SHOWNORMAL \
  "" "EasyGeoTagger"

WriteUninstaller $INSTDIR\UninstallEasyGt.exe 
 
SectionEnd 

Section "Uninstall"
  
  Delete $INSTDIR\UninstallEasyGt.exe
  Delete $DESKTOP\easygt.lnk
  Delete $SMPROGRAMS\EasyGeoTagger\easygt.lnk
  Delete $SMPROGRAMS\EasyGeoTagger\uninstall.lnk
  RMDir $SMPROGRAMS\EasyGeoTagger
  Delete $INSTDIR\bin\easygeotagger.exe
  Delete $INSTDIR\bin\easygt.bat
  Delete $INSTDIR\bin\easygt.dll
  Delete $INSTDIR\bin\libraw.dll
  Delete $INSTDIR\apps\easygt\icons\*.*
  RMDir $INSTDIR\apps\easygt\icons
  Delete $INSTDIR\apps\easygt\plugins\*.*
  RMDir $INSTDIR\apps\easygt\plugins
  RMDir $INSTDIR\apps\easygt
  Delete $INSTDIR\apps\Python25\Lib\site-packages\PyEasyGT.pyd
  Delete $INSTDIR\apps\qgis\plugins\easygeotaggerplugin.dll
  Delete $INSTDIR\apps\qgis\plugins\exifprovider.dll
  Delete $INSTDIR\apps\qgis\plugins\loadexifdataset.dll
  
SectionEnd
