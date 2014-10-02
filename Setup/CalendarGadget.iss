; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Calendar Gadget"
#define MyAppVersion "1.0.7"
#define MyAppPublisher "Home"
#define MyAppURL "http://www.codeproject.com/KB/gadgets/CalendarGadget.aspx"
#define MyAppExeName "CalendarGadget.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppID={{30C20186-E915-4BFE-B5D5-DF678D1A9BAE}
AppName={#MyAppName}
AppVersion = {#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=true
LicenseFile=..\Doc\LICENSE.rtf
;InfoBeforeFile=D:\Work\CPlusPlus\Projects\CalendarGadget\Doc\LICENSE.rtf
OutputDir=d:\Work\CPlusPlus\Projects\CalendarGadget\Setup\
OutputBaseFilename=CalendarGadgetSetup
Compression=lzma/Max
SolidCompression=true
ShowLanguageDialog=no
AppCopyright=Copyright (C) 2010-2011 B.D. Mihai.
SetupIconFile=Setup.ico
WizardImageFile=WizModernImage-IS.bmp
WizardSmallImageFile=SmallImage.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\Source\Release\CalendarGadget.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Source\CalendarGadget\Settings.ini"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt\4.7.1\bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt\4.7.1\bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt\4.7.1\bin\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\OpenSSL-Win32\bin\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\OpenSSL-Win32\bin\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\Microsoft.VC90.CRT.manifest"; DestDir: {app}; Flags: IgnoreVersion; 
Source: "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcm90.dll"; DestDir: {app}; Flags: IgnoreVersion; 
Source: "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcp90.dll"; DestDir: {app}; Flags: IgnoreVersion; 
Source: "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcr90.dll"; DestDir: {app}; Flags: IgnoreVersion; 

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: {app}\CalendarGadget.exe; Flags: nowait postinstall skipifsilent;