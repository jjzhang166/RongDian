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
wchar_t				g_szHostsPath[1024] = {0};
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
list<LPMAIN_PANEL>	g_lstPanelInfo;
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
	{ L"ActionScript",			"*.as;*.mx;",																			TIDY_ACTIONSCRIPT},
	{ L"Ada",					"*.ada;*.ads;*.adb;",																	TIDY_ADA},
	{ L"ASM",					"*.asm;",																				TIDY_ASM},
	{ L"ASP",					"*.asp;",																				TIDY_ASP},
	{ L"AutoHotkey",			"*.ahk;*.ia;*.scriptlet;*.hkml;",														TIDY_AUTOHOTKEY},
	{ L"AutoIt",				"*.au3;",																				TIDY_AUTOIT},
	{ L"Batch",					"*.bat;*.cmd;*.nt;",																	TIDY_BATCH},
	{ L"COBOL",					"*.cbl;*.cbd;*.cdb;*.cdc;*.cob;",														TIDY_COBOL},
	{ L"C++",					"-A1pNYk3",																				TIDY_CPP},
	{ L"C#",					"-A1pNYk3",																				TIDY_CS},
	{ L"CSS",					"001100210100000CSS2.1default",															TIDY_CSS},
	{ L"D",						"*.d;",																					TIDY_D},
	{ L"Fortran",				"*.f;*.for;*.f90;*.f95;*.f2k;",															TIDY_FORTRAN},
	{ L"Haskell",				"*.hs;*.lhs;*.las;",																	TIDY_HASKELL},
	{ L"HTML",					"-aan-dep-fb-fbc-fu-js-ll-n-ox-pe-qa-qn-m-wa-wj-wp-ws-sw-fo-i0-d1-ce0-ie0-oe0-w0",		TIDY_HTML},
	{ L"HTMLat",					"",																					TIDY_HTML_AT},
	{ L"HTMLcp",					"",																					TIDY_HTML_CP},
	{ L"HTMLnbt",					"",																					TIDY_HTML_NBT},
	{ L"HTMLnet",					"",																					TIDY_HTML_NET},
	{ L"HTMLnit",					"",																					TIDY_HTML_NIT},
	{ L"HTMLnpt",					"",																					TIDY_HTML_NPT},
	{ L"INI",					"*.ini;*.inf;*.reg;*.url;",																TIDY_INI},
	{ L"Java",					"-A1pNYk3",																				TIDY_JAVA},
	{ L"JavaScript",			"-nb-cn4",																				TIDY_JAVASCRIPT},
	{ L"JSON",					"-cn3",																					TIDY_JSON},
	{ L"JSP",					"*.jsp;",																				TIDY_JSP},
	{ L"LISP",					"*.lsp;*.lisp;",																		TIDY_LISP},
	{ L"LUA",					"*.lua;",																				TIDY_LUA},
	{ L"NormalText",			"*.txt;",																				TIDY_NORMALTEXT},
	{ L"Objective-C",			"*.h;*.m;*.mm;",																		TIDY_OBJECTIVEC},
	{ L"Pascal",				"*.dpr;*.dpk;*.pas;*.dfm;*.inc;*.pp;",													TIDY_PASCAL},
	{ L"Perl",					"*.pl;*.pm;*.plx;",																		TIDY_PERL},
	{ L"PHP",					"-sas-icd-samt-salo-saeo-saro-sabo-saao-samp-aas-rsl-iel-rpc-rst-st",					TIDY_PHP},
	{ L"Python",				"*.py;*.pyw;",																			TIDY_PYTHON},
	{ L"Ruby",					"*.rb;*.rbw;",																			TIDY_RUBY},
	{ L"SQL",					"-cn2-el-ml0",																			TIDY_SQL},
	{ L"VB",					"*.vb;*.bas;*.frm;*.cls;*.ctl;*.pag;*.dsr;*.dob;*.vbs;*.dsm;*.vbp;*.vbg;*.mak;*.vbw;",	TIDY_VB},
	{ L"Verilog",				"*.v;*.vl;*.vmd;",																		TIDY_VERILOG},
	{ L"VHDL",					"*.vhd;*.vhdl;*.vho;",																	TIDY_VHDL},
	{ L"XML",					"-aan-dep-fb-fbc-js-ll-n-ix-qa-qn-m-wa-wj-wp-ws-sw-fo-i1-ce0-ie0-oe0-w0",				TIDY_XML}
};

MSG_INFO			g_LangIDs[MAX_MSG_ID];

