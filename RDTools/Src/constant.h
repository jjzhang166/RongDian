#pragma once

// Mail
const wchar_t* const kAuthorMail					= L"XXX@gmail.com";
// Url
const wchar_t* const kAuthorUrl						= L"http://www.rongdian.net";
// CopyRight
const wchar_t* const kCopyRight						= L"CopyRight 2013 RongDian.net";

// SnapShot.exe
#ifdef _DEBUG
const wchar_t* const kSnapShotExec					= L"SnapShotd.exe";
#else
const wchar_t* const kSnapShotExec					= L"SnapShot.exe";
#endif

// RDZoomIn.exe
#ifdef _DEBUG
const wchar_t* const kZoomInExec					= L"RDZoomInd.exe";
#else
const wchar_t* const kZoomInExec					= L"RDZoomIn.exe";
#endif

// Config.ini
const wchar_t* const kConfigIni						= L"config.ini";
const wchar_t* const kDefaultLangIni				= L"lang_ZH-CN.ini"; // 语言文件

// XML文件
const wchar_t* const kMainFrameXml					= L"MainFrame.xml"; // 主窗体
const wchar_t* const kPanelsXml						= L"Panels.xml";
const wchar_t* const kSysMenuXml					= L"SysMenu.xml"; // 

// Lang Section
const wchar_t* const LS_SETTINGS					= L"Settings";
const wchar_t* const LS_LANG						= L"Lang";
const wchar_t* const LS_SKIN						= L"Skin";
const wchar_t* const LS_MAINFRAME					= L"MainFrame";
const wchar_t* const LS_SYSTRAY						= L"SysTray";
const wchar_t* const LS_ABOUTPANEL					= L"AboutPanel";
const wchar_t* const LS_PICKERPANEL					= L"PickerPanel";
const wchar_t* const LS_CODERPANEL					= L"CoderPanel";
const wchar_t* const LS_FINDERPANEL					= L"FinderPanel";
const wchar_t* const LS_FORMATTERPANEL				= L"FormatterPanel";
const wchar_t* const LS_IPCHANGERPANEL				= L"IPChangerPanel";
const wchar_t* const LS_MSG							= L"Msg";
const wchar_t* const LS_DUIMSG						= L"DuiMsg";

// Color Picker Panel
const wchar_t* const kLayoutPicker					= L"layout_picker";
const wchar_t* const kXmlPicker						= L"Picker.xml";
const wchar_t* const kTabPicker						= L"tab_picker";
const wchar_t* const kPanelPicker					= L"panel_picker";
const wchar_t* const kNamePicker					= L"name_picker";
const wchar_t* const kDescPicker					= L"desc_picker";
// 
const wchar_t* const kImageArrow					= L"ZoomIn.png";
const wchar_t* const kImageCross					= L"Cross.png";

// About Panel
const wchar_t* const kLayoutAbout					= L"layout_about";
const wchar_t* const kXmlAbout						= L"About.xml";
const wchar_t* const kTabAbout						= L"tab_about";
const wchar_t* const kPanelAbout					= L"panel_about";
const wchar_t* const kNameAbout						= L"name_about";
const wchar_t* const kDescAbout						= L"desc_about";

// MSG
const wchar_t* const kMsgIconName					= L"RongDian.png";
const wchar_t* const kMsgErr						= L"msg_err";
const wchar_t* const kPluginErr						= L"plugin_err";
const wchar_t* const kNoFoundErr					= L"nofound_err";
const wchar_t* const kFolderTitle					= L"folder_title";
const wchar_t* const kStillWorkingErr				= L"still_working";
const wchar_t* const kHandleText					= L"handle_text";
const wchar_t* const kNoHandleText					= L"nohandle_text";
const wchar_t* const kInvalidPathErr				= L"invalid_path_err";
const wchar_t* const kInvalidFinderErr				= L"invalid_finder_err";
const wchar_t* const kInvalidFormatterErr			= L"invalid_foramtter_err";

// 
const wchar_t* const kIsColor						= L"iscolor";
const wchar_t* const kColor							= L"color";

// Image's Extension
const wchar_t* const kJPEGExt						= L".jpeg";
const wchar_t* const kJPGExt						= L".jpg";
const wchar_t* const kPNGExt						= L".png";
const wchar_t* const kBMPExt						= L".bmp";

typedef struct _tagPANEL_INFO
{
	wchar_t szLayout[1024];
	wchar_t szXml[1024];
	wchar_t szTab[1024];
	wchar_t szPanel[1024];
	wchar_t szName[1024];
	wchar_t szDesc[1024];
} PANEL_INFO, *PPANEL_INFO, *LPPANEL_INFO;

typedef struct _tagSKIN_INFO
{
	wchar_t szImage[1024];
	DWORD dwBKColor;
	BOOL bImage;
} SKIN_INFO, *LSKIN_INFO, *LPSKIN_INFO;

typedef struct _tagCODER_INFO
{
	wchar_t szPath[1024];
	char szFrom[64];
	char szTo[64];
	BOOL bHandle;
	BOOL bValid;
} CODER_INFO, *LCODER_INFO, *LPCODER_INFO;
