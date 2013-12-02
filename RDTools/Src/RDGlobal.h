#pragma once

#include "constant.h"

HANDLE				g_hUserSignature = NULL;
HANDLE				g_hAdminSignature = NULL;
BOOL				g_bActiveRunAsAdminThread = FALSE;
HINSTANCE			g_hInstance = NULL;
wchar_t				g_szModule[1024] = {0};
wchar_t				g_szAppName[1024] = {0};
wchar_t				g_szAppVer[1024] = {0};
//wchar_t				g_szAppConfig[1024] = {0};
wchar_t				g_szAppConfigDB[1024] = {0};
wchar_t				g_szPanelsXml[1024] = {0};
wchar_t				g_szResPath[1024] = {0};
wchar_t				g_szLogPath[1024] = {0};
wchar_t				g_szLangPath[1024] = {0};
wchar_t				g_szSnapShot[1024] = {0};
wchar_t				g_szCoderPath[1024] = {0};
wchar_t				g_szCoderBackupPath[1024] = {0};
wchar_t				g_szBackground[1024] = L"FF32506E";
wchar_t				g_szLangFile[1024] = L"lang_ZH-CN.ini";	//指定当前使用的语言版本lang_ZH-CN.ini lang_en.ini
wchar_t				g_szAddrSolution[1024] = {0};
Logger				g_Logger;
Options				g_OptOptions;
Parser				g_OptParser;
CLangManager		*g_pLangManager = NULL;
CSkinManager		*g_pSkinManager = NULL;
CSystemTray			*g_pSystemTray = NULL;
CMainFrame			*g_pMainFrame = NULL;
list<LPPANEL_INFO>	g_lstPanelInfo;
CSQLite				g_SQLite;
IMG_INFO			g_ImgInfo[MAX_IMG_TYPE] = {
	{ L"UnValid",				IMG_UNDEFINE,		FALSE },
	{ L".jpeg",					IMG_JPEG,			TRUE },
	{ L".jpg",					IMG_JPG,			TRUE },
	{ L".png",					IMG_PNG,			TRUE },
	{ L".bmp",					IMG_BMP,			TRUE },
	{ L".gif",					IMG_GIF,			FALSE },
	{ L".psd",					IMG_PSD,			FALSE }
};

TIDY_INFO			g_TidyInfo[MAX_TIDY_LANG] = {
	{ L"ActionScript",			L"*.as;*.mx;",																			TIDY_ACTIONSCRIPT},
	{ L"Ada",					L"*.ada;*.ads;*.adb;",																	TIDY_ADA},
	{ L"ASM",					L"*.asm;",																				TIDY_ASM},
	{ L"ASP",					L"*.asp;",																				TIDY_ASP},
	{ L"AutoHotkey",			L"*.ahk;*.ia;*.scriptlet;*.hkml;",														TIDY_AUTOHOTKEY},
	{ L"AutoIt",				L"*.au3;",																				TIDY_AUTOIT},
	{ L"Batch",					L"*.bat;*.cmd;*.nt;",																	TIDY_BATCH},
	{ L"COBOL",					L"*.cbl;*.cbd;*.cdb;*.cdc;*.cob;",														TIDY_COBOL},
	{ L"C++",					L"-A1pNYk3",																			TIDY_CPP},
	{ L"C#",					L"-A1pNYk3",																			TIDY_CS},
	{ L"CSS",					L"001100210100000CSS2.1default",														TIDY_CSS},
	{ L"D",						L"*.d;",																				TIDY_D},
	{ L"Fortran",				L"*.f;*.for;*.f90;*.f95;*.f2k;",														TIDY_FORTRAN},
	{ L"Haskell",				L"*.hs;*.lhs;*.las;",																	TIDY_HASKELL},
	{ L"HTML",					L"-aan-dep-fb-fbc-fu-js-ll-n-ox-pe-qa-qn-m-wa-wj-wp-ws-sw-fo-i0-d1-ce0-ie0-oe0-w0",		TIDY_HTML},
	{ L"INI",					L"*.ini;*.inf;*.reg;*.url;",															TIDY_INI},
	{ L"Java",					L"-A1pNYk3",																			TIDY_JAVA},
	{ L"JavaScript",			L"-nb-cn4",																				TIDY_JAVASCRIPT},
	{ L"JSON",					L"-cn3",																				TIDY_JSON},
	{ L"JSP",					L"*.jsp;",																				TIDY_JSP},
	{ L"LISP",					L"*.lsp;*.lisp;",																		TIDY_LISP},
	{ L"LUA",					L"*.lua;",																				TIDY_LUA},
	{ L"NormalText",			L"*.txt;",																				TIDY_NORMALTEXT},
	{ L"Objective-C",			L"*.h;*.m;*.mm;",																		TIDY_OBJECTIVEC},
	{ L"Pascal",				L"*.dpr;*.dpk;*.pas;*.dfm;*.inc;*.pp;",													TIDY_PASCAL},
	{ L"Perl",					L"*.pl;*.pm;*.plx;",																	TIDY_PERL},
	{ L"PHP",					L"-sas-icd-samt-salo-saeo-saro-sabo-saao-samp-aas-rsl-iel-rpc-rst-st",					TIDY_PHP},
	{ L"Python",				L"*.py;*.pyw;",																			TIDY_PYTHON},
	{ L"Ruby",					L"*.rb;*.rbw;",																			TIDY_RUBY},
	{ L"SQL",					L"-cn2-el-ml0",																			TIDY_SQL},
	{ L"VB",					L"*.vb;*.bas;*.frm;*.cls;*.ctl;*.pag;*.dsr;*.dob;*.vbs;*.dsm;*.vbp;*.vbg;*.mak;*.vbw;",	TIDY_VB},
	{ L"Verilog",				L"*.v;*.vl;*.vmd;",																		TIDY_VERILOG},
	{ L"VHDL",					L"*.vhd;*.vhdl;*.vho;",																	TIDY_VHDL},
	{ L"XML",					L"-aan-dep-fb-fbc-js-ll-n-ix-qa-qn-m-wa-wj-wp-ws-sw-fo-i1-ce0-ie0-oe0-w0",				TIDY_XML}
};

MSG_INFO			g_LangIDs[MAX_MSG_ID];

