; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Calendar Gadget"
#define MyAppVersion "1.2.1"
#define MyAppPublisher "Home"
#define MyAppURL "https://github.com/bdmihai/qt-calendar-gadget"
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
OutputDir=.\
OutputBaseFilename=CalendarGadgetSetup
Compression=lzma/Max
SolidCompression=true
ShowLanguageDialog=no
AppCopyright=Copyright (C) 2010-2016 B.D. Mihai.
SetupIconFile=Setup.ico
WizardImageFile=WizModernImage-IS.bmp
WizardSmallImageFile=SmallImage.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\Source\Release\CalendarGadget.exe"; DestDir: "{app}"; Flags: ignoreversion
; Qt files
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Multimedia.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5MultimediaWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5OpenGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Positioning.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5PrintSupport.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Qml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Quick.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Sensors.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Sql.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5WebChannel.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5WebKit.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5WebKitWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\icudt54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\icuin54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\bin\icuuc54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\Libraries\Qt5\5.5\msvc2013\plugins\platforms\qwindows.dll"; DestDir: "{app}\plugins\platforms"; Flags: ignoreversion
; OpenSSL
Source: "C:\Program Files (x86)\OpenSSL\bin\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Program Files (x86)\OpenSSL\bin\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\redist\x86\Microsoft.VC120.CRT\msvcp120.dll"; DestDir: {app}; Flags: IgnoreVersion; 
Source: "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\redist\x86\Microsoft.VC120.CRT\msvcr120.dll"; DestDir: {app}; Flags: IgnoreVersion; 

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: {app}\CalendarGadget.exe; Flags: nowait postinstall skipifsilent;
