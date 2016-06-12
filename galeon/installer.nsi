!define APPNAME "True Inferno"
!define COMPANYNAME "True Inferno team"
!define DESCRIPTION "Hell is only the beginning!"
# These three must be integers
!define VERSIONMAJOR 1
!define VERSIONMINOR 0
!define VERSIONBUILD 0

# These will be display
# name the installer
OutFile "TrueInfernoInstaller1_0.exe"
 
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
	
	#File /oname=TrueInferno.exe Galeon_d.exe
	# define uninstaller name
	WriteUninstaller $INSTDIR\uninstaller.exe
	
	createDirectory "$SMPROGRAMS\${APPNAME}"
	createShortCut "$SMPROGRAMS\True Inferno\True Inferno.lnk" "$INSTDIR\Galeon_d.exe"
	createShortCut "$SMPROGRAMS\True Inferno\uninstaller.lnk" "$INSTDIR\uninstaller.exe"
	
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