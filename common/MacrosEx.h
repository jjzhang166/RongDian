#pragma once

// 文本宏优化调整 - 2013-10-31
/*
a、SET_TITLE_TEXT(id, str)
b、SET_TITLE_TEXT_EX(id, tail)
e、SET_TITLE_TEXT_RID(id, iid, nid)
c、SET_CONTROL_TEXT(id, str)
d、SET_CONTROL_TEXT_EX(id, tail)
e、SET_CONTROL_TEXT_RID(id, iid, nid)
f、SET_CONTROL_TIP(id, str)
g、SET_CONTROL_TIP_EX(id, str)
h、SET_CONTROL_TT(id, tid)
*/

/*************************************************************************
* Method: SET_CONTROL_BEGIN
* Description: 设置控件文本宏 -- Header(必须配合SET_CONTROL_END()宏一起使用)
* ParameterList: man, lang, ls
* Parameter: man: CPaintManagerUI指针，确保此参数为CPaintManagerUI指针
* CPaintManagerUI m_PaintManager; ==> SET_CONTROL_BEGIN((&m_PaintManager), xxx, xxx)
* CPaintManagerUI *m_pPaintManager; ==> SET_CONTROL_BEGIN(m_pPaintManager, xxx, xxx)
* Parameter: lang: 语言文件全路径("E:\Source\XXXX\Root\lang\lang_en.ini")
* Parameter: ls: 语言文件(ini文件)的Section名称
* Return Value: 无
* Date: 13:09:01 16:42:56
* Author: LIng
* CopyRight:
************************************************************************/
#define SET_CONTROL_BEGIN(man, lang, ls) \
	{ \
		CPaintManagerUI *pManager = man; \
		LPCWSTR pLang = lang; \
		LPCWSTR pSection = ls; \
		assert(pManager!=NULL);
		
/*************************************************************************
* Method: SET_TITLE_TEXT
* Description: 设置控件文本宏 -- Item
* ParameterList: id, str
* Parameter: id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
* Parameter: str: 控件文本，若为空，从ini文件中通过控件id获取对应文本
* Return Value: 无
* Date: 13:09:01 16:42:56
* Author: LIng
* CopyRight:
************************************************************************/
#define SET_TITLE_TEXT(id, str) \
		CControlUI *p##id = static_cast<CControlUI*>(pManager->FindControl(id)); \
		HWND h##id##Wnd = pManager->GetPaintWindow(); \
		wchar_t sz##id[2048] = {0}; \
		if(p##id) \
		{ \
			if(str) \
			{ \
				p##id->SetText(str); \
				SetWindowText(h##id##Wnd, str); \
			} \
			else if(p##id && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
			{ \
				p##id->SetText(sz##id); \
				SetWindowText(h##id##Wnd, sz##id); \
			} \
		}
	
/*************************************************************************
 * Method:    		SET_TITLE_TEXT_EX
 * Description:		设置控件文本宏 -- Item
 *						wchar_t szAdmin[1024] = L"(Admin)";
 *						SET_TITLE_TEXT_EX(kFrameTitle, szAdmin)
 *						查找kFrameTitle控件，从语言文件中取出kFrameTitle控件对应的文本信息L"RongDian Tools"
 *							然后将L"RongDian Tools"与L"(Admin)"进行拼接
 *							拼接结果为L"RongDian Tools(Admin)"，最后设置给对应的控件
 * ParameterList:	id, tail
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
 * Parameter:       tail: 根据id获取语言文件对应的文本后，将此字符串拼接到尾部
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_TITLE_TEXT_EX(id, tail) \
		wchar_t sz##id##Text[2048], sz##id[2048]; \
		CControlUI *p##id = static_cast<CControlUI*>(pManager->FindControl(id)); \
		HWND h##id##Wnd = pManager->GetPaintWindow(); \
		if(p##id && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
		{ \
			swprintf(sz##id##Text, L"%s%s", sz##id, tail); \
			p##id->SetText(sz##id##Text); \
			SetWindowText(h##id##Wnd, sz##id##Text); \
		}	
		
/*************************************************************************
 * Method:    		SET_TITLE_TEXT_RID
 * Description:		设置控件文本宏 -- Item
 *						若工程中没有指定ini语言文件，则尝试从rc资源文件中获取对应的文本信息
 * ParameterList:	id, iid, nid
 * Parameter:       id: 控件id(在xml布局文件中指定)
 * Parameter:		iid: 语言文件(ini文件)中文本对应的id
 * Parameter:		nid: 资源文件(rc文件)中文本对应的id
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_TITLE_TEXT_RID(id, iid, nid) \
		wchar_t sz##id[1024] = {0}; \
		CControlUI *p##id##TextRid = static_cast<CControlUI*>(pManager->FindControl(id)); \
		HWND h##id##Wnd = pManager->GetPaintWindow(); \
		if(p##id##TextRid) \
		{ \
			if(pLang) \
				Utility::GetINIStr(lpszLang, pSection, iid, sz##id); \
			else \
				LoadString(NULL, nid, sz##id, sizeof(sz##id)); \
			p##id##TextRid->SetText(sz##id); \
			SetWindowText(h##id##Wnd, sz##id); \
		}
		
/*************************************************************************
* Method: SET_CONTROL_TEXT
* Description: 设置控件文本宏 -- Item
* ParameterList: id, str
* Parameter: id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
* Parameter: str: 控件文本，若为空，从ini文件中通过控件id获取对应文本
* Return Value: 无
* Date: 13:09:01 16:42:56
* Author: LIng
* CopyRight:
************************************************************************/
#define SET_CONTROL_TEXT(id, str) \
		CControlUI *p##id##Text = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##Text) \
		{ \
			wchar_t sz##id[2048] = {0}; \
			if(str && p##id##Text) \
				p##id##Text->SetText(str); \
			else if(p##id##Text && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
				p##id##Text->SetText(sz##id); \
		}

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT_EX
 * Description:		设置控件文本宏 -- Item
 *						// const wchar_t* const kAcceptBtn = L"AcceptBtn"; // 控件id
 *						// AcceptBtn控件对应文本信息"Acc"
 *						wchar_t szTextTail[1024] = L"ept";
 *						SET_CONTROL_TEXT_EX(kAcceptBtn, szTextTail)
 *						查找kAcceptBtn控件，从语言文件中取出kAcceptBtn控件对应的文本信息L"Acc"
 *							然后将L"Acc"与L"ept"进行拼接
 *							拼接结果为L"Accept"，最后设置给对应控件的文本
 * ParameterList:	id, tail
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
 * Parameter:       tail: 根据id获取语言文件对应的文本后，将此字符串拼接到尾部
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT_EX(id, tail) \
		wchar_t sz##id##Text[2048], sz##id[2048]; \
		CControlUI *p##id##TextEx = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##TextEx && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
		{ \
			swprintf(sz##id##Text, L"%s%s", sz##id, tail); \
			p##id##TextEx->SetText(sz##id##Text); \
		}		
	
/*************************************************************************
 * Method:    		SET_CONTROL_TEXT_RID
 * Description:		设置控件文本宏 -- Item
 *						若工程中没有指定ini语言文件，则尝试从rc资源文件中获取对应的文本信息
 * ParameterList:	id, iid, nid
 * Parameter:       id: 控件id(在xml布局文件中指定)
 * Parameter:		iid: 语言文件(ini文件)中文本对应的id
 * Parameter:		nid: 资源文件(rc文件)中文本对应的id
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT_RID(id, iid, nid) \
		CControlUI *p##id##TextRid = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##TextRid) \
		{ \
			wchar_t sz##id[2048] = {0}; \
			if(pLang) \
				Utility::GetINIStr(lpszLang, pSection, iid, sz##id); \
			else \
				LoadString(NULL, nid, sz##id, sizeof(sz##id)); \
			p##id##TextRid->SetText(sz##id); \
		}	
		
/*************************************************************************
 * Method:    		SET_CONTROL_TIP
 * Description:		设置控件提示信息宏 -- Item
 * ParameterList:	id, str
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中提示信息的id，要求控件id与ini文件中控件对应的提示信息id一致
 * Parameter:       str: 提示信息
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TIP(id, str) \
		CControlUI *p##id##Tip = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##Tip) \
		{ \
			wchar_t sz##id[2048] = {0}; \
			if(str) \
				p##id##Tip->SetToolTip(str); \
			else if(p##id##Tip && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
				p##id##Tip->SetToolTip(sz##id); \
		}
		
/*************************************************************************
 * Method:    		SET_CONTROL_TIP_EX
 * Description:		设置控件文本提示宏 -- Item
 *						// const wchar_t* const kAcceptBtn = L"AcceptBtn"; // 控件id
 *						// AcceptBtn控件对应文本信息"Accept"
 *						wchar_t szTipTail[1024] = L" Button.";
 *						SET_CONTROL_TIP_EX(kAcceptBtn, szTipTail)
 *						查找kAcceptBtn控件，从语言文件中取出kAcceptBtn控件对应的文本信息L"Accept"
 *							然后将L"Accept"与L" Button."进行拼接
 *							拼接结果为L"Accept Button."，最后设置给对应控件的文本提示
 * ParameterList:	id, tail
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
 * Parameter:       tail: 根据id获取语言文件对应的文本后，将此字符串拼接到尾部
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TIP_EX(id, tail) \
		wchar_t sz##id##Tip[2048] = {0}, sz##id[2048] = {0}; \
		CControlUI *p##id##TipEx = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##TipEx && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
		{ \
			swprintf(sz##id##Tip, L"%s%s", sz##id, tail); \
			p##id##TipEx->SetToolTip(sz##id##Tip); \
		}
		
/*************************************************************************
 * Method:    		SET_CONTROL_TT
 * Description:		设置控件文本及提示信息宏 -- Item
 * ParameterList:	id, tid
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中提示信息的id，要求控件id与ini文件中控件对应的提示信息id一致
 * Parameter:       tid: 语言文件中提示信息的id
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TT(id, tid) \
		CControlUI *p##id##TextTip = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##TextTip) \
		{ \
			wchar_t sz##id[2048] = {0}, sz##tid[2048] = {0}; \
			if(Utility::GetINIStr(pLang, pSection, id, sz##id)) \
				p##id##TextTip->SetText(sz##id); \
			if(Utility::GetINIStr(pLang, pSection, tid, sz##tid)) \
				p##id##TextTip->SetToolTip(sz##tid); \
		}
		
/*************************************************************************
 * Method:    		SET_CONTROL_END
 * Description:		设置控件文本及提示信息宏 -- Tail(必须配合SET_CONTROL_BEGIN()宏一起使用)
 * ParameterList:	无
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_END() \
	}

//////////////////////////////////////////////////////////////////////////
#define FIND_CONTROL_BY_ID(ctl, cls, man, id) \
	assert(man!=NULL);\
	ctl = static_cast<cls *>(man->FindControl(id));

#define FIND_CONTROL_BY_PT(ctl, cls, man, pt) \
	assert(man!=NULL);\
	ctl = static_cast<cls *>(man->FindControl(pt));

//////////////////////////////////////////////////////////////////////////
#define ASSOC_BEGIN() \
	{ \
		LPMSG_INFO lpLangIDs = g_LangIDs; \
		assert(lpLangIDs!=NULL); \
		memset(lpLangIDs, 0, sizeof(MSG_INFO)*MAX_MSG_ID);

#define ASSOC_LANG(rid, sid, section) \
		if(rid<MAX_MSG_ID) \
		{ \
			lpLangIDs[rid].nId = rid; \
			lpLangIDs[rid].pId = sid; \
			lpLangIDs[rid].pSection = section; \
		} 

#define ASSOC_END() \
	}

#define GET_ASSOC_ID(ids, rid) \
	(rid)<(MAX_MSG_ID) ? ids[rid].pId : L""

#define GET_ASSOC_SECTION(ids, rid) \
	(rid)<(MAX_MSG_ID) ? ids[rid].pSection : L""