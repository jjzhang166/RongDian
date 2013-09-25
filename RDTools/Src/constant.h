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

// Config.db
const wchar_t* const kConfigDB						= L"Config.db";

// Config.ini
const wchar_t* const kConfigIni						= L"config.ini";
const wchar_t* const kDefaultLangIni				= L"lang_en.ini"; // 语言文件

// XML文件
const wchar_t* const kMainFrameXml					= L"MainFrame.xml"; // 主窗体
const wchar_t* const kPanelsXml						= L"Panels.xml";
const wchar_t* const kSysMenuXml					= L"SysMenu.xml"; // 

// Lang Section
const wchar_t* const LS_SETTINGS					= L"Settings";
const wchar_t* const LS_LANG						= L"Lang";
const wchar_t* const LS_SKIN						= L"Skin";
const wchar_t* const LS_IPSETS						= L"IPSets";
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
const wchar_t* const kMsgWarning					= L"msg_warning";
const wchar_t* const kPluginErr						= L"plugin_err";
const wchar_t* const kNoFoundErr					= L"nofound_err";
const wchar_t* const kFolderTitle					= L"folder_title";
const wchar_t* const kStillWorkingErr				= L"still_working";
const wchar_t* const kHandleText					= L"handle_text";
const wchar_t* const kNoHandleText					= L"nohandle_text";
const wchar_t* const kInvalidPathErr				= L"invalid_path_err";
const wchar_t* const kInvalidFinderErr				= L"invalid_finder_err";
const wchar_t* const kInvalidFormatterErr			= L"invalid_foramtter_err";
const wchar_t* const kInvalidIPErr					= L"invalid_ipv4_err";
const wchar_t* const kInvalidMaskErr				= L"invalid_subnet_mask_err";
const wchar_t* const kInvalidGatewayErr				= L"invalid_gateway_err";
const wchar_t* const kInvalidDnsErr					= L"invalid_dns_err";
const wchar_t* const kDeleteSolution				= L"delete_solution";

// 
const wchar_t* const kBackGround					= L"Background";
const wchar_t* const kLang							= L"Lang";
const wchar_t* const kSolution						= L"Solution";
const wchar_t* const kSelected						= L"Selected";
const wchar_t* const kValid							= L"Valid";
const wchar_t* const kName							= L"Name";
const wchar_t* const kAddrType						= L"AddrType";
const wchar_t* const kAddr							= L"Addr";
const wchar_t* const kMask							= L"Mask";
const wchar_t* const kGateway						= L"Gateway";
const wchar_t* const kDnsType						= L"DnsType";
const wchar_t* const kDns1							= L"Dns1";
const wchar_t* const kDns2							= L"Dns2";

#ifndef STRING_LENGTH
#define 			 STRING_LENGTH					260
#endif

typedef struct _tagPANEL_INFO
{
	wchar_t szLayout[STRING_LENGTH];
	wchar_t szXml[STRING_LENGTH];
	wchar_t szTab[STRING_LENGTH];
	wchar_t szPanel[STRING_LENGTH];
	wchar_t szName[STRING_LENGTH];
	wchar_t szDesc[STRING_LENGTH];
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

typedef enum _tagADDR_MODE
{
	ADDR_AUTO_SET = 0,
	ADDR_MANUAL_SET = 1
} ADDR_MODE;

typedef enum _tagDNS_MODE
{
	DNS_AUTO_SET = 0,
	DNS_MANUAL_SET = 1
} DNS_MODE;

typedef struct _tagIPCONFIG_INFO
{
	int nValid;
	int nAddrType;
	int nDnsType;
	wchar_t szSolution[STRING_LENGTH];
	wchar_t szAddr[STRING_LENGTH];
	wchar_t szMask[STRING_LENGTH];
	wchar_t szGateway[STRING_LENGTH];
	wchar_t szDns1[STRING_LENGTH];
	wchar_t szDns2[STRING_LENGTH];
} IPCONFIG_INFO, *PIPCONFIG_INFO, *LPIPCONFIG_INFO;

typedef struct _tagADAPTER_INFO
{
	int nIndex;
	BOOL bEnableDHCP;
	wchar_t szName[STRING_LENGTH];
	wchar_t szType[STRING_LENGTH];
	wchar_t szDesc[STRING_LENGTH];
	wchar_t szAdapter[STRING_LENGTH];
	wchar_t szAddr[4][STRING_LENGTH];
	wchar_t szMask[STRING_LENGTH];
	wchar_t szGateway[STRING_LENGTH];
	wchar_t szDns[2][STRING_LENGTH];
	wchar_t szMac[STRING_LENGTH];
} ADAPTER_INFO, *PADAPTER_INFO, *LPADAPTER_INFO;

typedef enum _tagIMG_TYPE
{
	IMG_JPEG = 0,
	IMG_JPG,
	IMG_PNG,
	IMG_BMP,
	IMG_GIF,
	IMG_PSD,
	IMG_UNDEFINE
} IMG_TYPE;

const UINT MAX_IMG_TYPE = IMG_UNDEFINE + 1;

typedef struct _tagIMG_INFO
{
	wchar_t szName[STRING_LENGTH];
	int nType;
	int bSupport;
} IMG_INFO, *PIMG_INFO, *LPIMG_INFO;

typedef enum _tagTIDY_TYPE {
	TIDY_ACTIONSCRIPT = 0,
	TIDY_ADA,	
	TIDY_ASM,	
	TIDY_ASP,
	TIDY_AUTOHOTKEY,
	TIDY_AUTOIT,
	TIDY_BATCH,
	TIDY_COBOL,
	TIDY_CPP,
	TIDY_CS,
	TIDY_CSS,
	TIDY_D,
	TIDY_FORTRAN,
	TIDY_HASKELL,
	TIDY_HTML,
	TIDY_INI,
	TIDY_JAVA,
	TIDY_JAVASCRIPT,
	TIDY_JSON,
	TIDY_JSP,
	TIDY_LISP,
	TIDY_LUA,
	TIDY_NORMALTEXT,
	TIDY_OBJECTIVEC,
	TIDY_PASCAL,
	TIDY_PERL,
	TIDY_PHP,
	TIDY_PYTHON,
	TIDY_RUBY,
	TIDY_SQL,
	TIDY_VB,
	TIDY_VERILOG,
	TIDY_VHDL,
	TIDY_XML,
	TIDY_END
} TIDY_TYPE;

const UINT MAX_TIDY_LANG = TIDY_END + 1;

typedef struct _tagTIDY_INFO
{
	wchar_t szLang[STRING_LENGTH];
	wchar_t szDesc[STRING_LENGTH];
	UINT uID;
} TIDY_INFO, *PTIDY_INFO, *LPTIDY_INFO;

typedef struct _tagCMD_EXE_INFO
{
	HWND	hWnd;
	LPWSTR	lpszCommand;
	LPVOID	lpLoader;
} CMD_EXE_INFO, *PCMD_EXE_INFO, *LPCMD_EXE_INFO;