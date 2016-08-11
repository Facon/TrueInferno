# Application (Name and description)
!define APPNAME "True Inferno"
!define COMPANYNAME "True Inferno"
!define DESCRIPTION "Hell is only the beginning!"

# Version (These three must be integers)
!define VERSIONMAJOR 0
!define VERSIONMINOR 1
!define VERSIONBUILD 0

# Icons
!include "MUI2.nsh"
!define MUI_ICON "TrueInferno.ico"

# These will be display
# name the installer
OutFile "TrueInferno_v${VERSIONMAJOR}_${VERSIONMINOR}.exe"
 
# set desktop as install directory
InstallDir "$PROGRAMFILES\${APPNAME}"

# For removing Start Menu shortcut in Windows 7
RequestExecutionLevel admin

Page directory
Page instfiles
 
# default section start; every NSIS script has at least one section.
Section
	# define the output path for this file
	SetOutPath $INSTDIR
	
	# Logs needed :( /x *.log
	File /a /r /x *.pdb /x *.ilk Exes\*.*
	
	# Make the directory "$INSTDIR\database" read write accessible by all users
	AccessControl::GrantOnFile \
	"$INSTDIR" "(BU)" "GenericRead + GenericWrite"
	Pop $0
	
	#File /oname=TrueInferno.exe
	# define uninstaller name
	WriteUninstaller $INSTDIR\Uninstaller.exe
	
	createDirectory "$SMPROGRAMS\${APPNAME}"
	createShortCut "$SMPROGRAMS\True Inferno\True Inferno.lnk" "$INSTDIR\TrueInferno.exe"
	createShortCut "$SMPROGRAMS\True Inferno\Uninstaller.lnk" "$INSTDIR\Uninstaller.exe"
	
	# create a shortcut named "new shortcut" in the start menu programs directory
	# presently, the new shortcut doesn't call anything (the second field is blank)
 
# default section end
SectionEnd

# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
Section "Uninstall"
	# Always delete uninstaller first
	Delete $INSTDIR\uninstaller.exe
	
	RMDir /r $INSTDIR
	
	RMDir /r "$SMPROGRAMS\${APPNAME}"
 
SectionEnd
