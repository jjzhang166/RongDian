#pragma once

/*************************************************************************
 * Method:    		SET_CONTROL_BEGIN
 * Description:		设置控件文本宏 -- Header(必须配合SET_CONTROL_END()宏一起使用)
 * ParameterList:	man, lang, ls
 * Parameter:       man: CPaintManagerUI指针，确保此参数为CPaintManagerUI指针
 *						CPaintManagerUI m_PaintManager; ==> SET_CONTROL_BEGIN((&m_PaintManager), xxx, xxx)
 *						CPaintManagerUI *m_pPaintManager; ==> SET_CONTROL_BEGIN(m_pPaintManager, xxx, xxx)
 * Parameter:       lang: 语言文件全路径("E:\Source\XXXX\Root\lang\lang_en.ini")
 * Parameter:       ls: 语言文件(ini文件)的Section名称
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_BEGIN(man, lang, ls) \
	{ \
		CPaintManagerUI *pManager = man; \
		LPCWSTR pLang = lang; \
		LPCWSTR pSection = ls; \
		assert(pManager!=NULL);

/*************************************************************************
 * Method:    		SET_TITLE_TEXT
 * Description:		设置控件文本宏 -- Item
 * ParameterList:	hwnd, id, str
 * Parameter:       hwnd: 控件句柄(在DuiLib中，一般是所在窗体的窗口句柄)，用于设置标题
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
 * Parameter:       str: 控件文本
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_TITLE_TEXT(hwnd, id, str) \
		CControlUI *p##id = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id) \
		{ \
			p##id->SetText(str); \
			SetWindowText(hwnd, str); \
		}

/*************************************************************************
 * Method:    		SET_TITLE_TEXT2
 * Description:		设置控件文本宏 -- Item
 * ParameterList:	hwnd, id
 * Parameter:       hwnd: 控件句柄(在DuiLib中，一般是所在窗体的窗口句柄)，用于设置标题
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_TITLE_TEXT2(hwnd, id) \
		wchar_t sz##id[1024]; \
		CControlUI *p##id = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
		{ \
			p##id->SetText(sz##id); \
			SetWindowText(hwnd, sz##id); \
		}

/*************************************************************************
 * Method:    		SET_TITLE_TEXT3
 * Description:		设置控件文本宏 -- Item
 *						wchar_t szAdmin[1024] = L"(Admin)";
 *						SET_TITLE_TEXT3(m_hWnd, kFrameTitle, szAdmin)
 *							查找kFrameTitle控件，从语言文件中取出kFrameTitle控件对应的文本信息L"RongDian Tools"
 *							然后将L"RongDian Tools"与L"(Admin)"进行拼接
 *							拼接结果为L"RongDian Tools(Admin)"，最后设置给对应的控件
 * ParameterList:	hwnd, id, tail
 * Parameter:       hwnd: 控件句柄(在DuiLib中，一般是所在窗体的窗口句柄)，用于设置标题
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
 * Parameter:       tail: 根据id获取语言文件对应的文本后，将此字符串拼接到尾部
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_TITLE_TEXT3(hwnd, id, tail) \
		wchar_t sz##id##Text[1024], sz##id[1024]; \
		CControlUI *p##id = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
		{ \
			swprintf(sz##id##Text, L"%s%s", sz##id, tail); \
			p##id->SetText(sz##id##Text); \
			SetWindowText(hwnd, sz##id##Text); \
		}

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT
 * Description:		设置控件文本宏 -- Item
 * ParameterList:	id, str
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
 * Parameter:       str: 控件文本
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT(id, str) \
		CControlUI *p##id##Text = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##Text) \
			p##id##Text->SetText(str);

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT2
 * Description:		设置控件文本宏 -- Item
 * ParameterList:	id
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT2(id) \
		wchar_t sz##id[1024]; \
		CControlUI *p##id##Text2 = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##Text2 && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
			p##id##Text2->SetText(sz##id);

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT3
 * Description:		设置控件文本宏 -- Item
 *						wchar_t szVer[1024] = L"1.0.0.1";
 *						SET_CONTROL_TEXT3(kAboutVer, szAppVer)
 *							查找kAboutVer控件，从语言文件中取出kAboutVer控件对应的文本信息L"Version:"
 *							然后将L"Version:"与L"1.0.0.1"进行拼接
 *							拼接结果为L"Version:1.0.0.1"，最后设置给对应的控件
 * ParameterList:	id, tail
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中文本对应的id，要求控件id与ini文件中控件对应的文本id一致
 * Parameter:       tail: 根据id获取语言文件对应的文本后，将此字符串拼接到尾部
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT3(id, tail) \
		wchar_t sz##id##Text[1024], sz##id[1024]; \
		CControlUI *p##id##Text3 = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##Text3 && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
		{ \
			swprintf(sz##id##Text, L"%s%s", sz##id, tail); \
			p##id##Text3->SetText(sz##id##Text); \
		}

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT4
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
#define SET_CONTROL_TEXT4(id, iid, nid) \
		wchar_t sz##id[1024] = {0}; \
		CControlUI *p##id##Text4 = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##Text4) \
		{ \
			if(pLang) \
				Utility::GetINIStr(lpszLang, pSection, iid, sz##id); \
			else \
				LoadString(NULL, nid, sz##id, sizeof(sz##id)); \
			p##id##Text4->SetText(sz##id); \
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
			p##id##Tip->SetToolTip(str); \

/*************************************************************************
 * Method:    		SET_CONTROL_TIP2
 * Description:		设置控件提示信息宏 -- Item
 * ParameterList:	id
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中提示信息的id，要求控件id与ini文件中控件对应的提示信息id一致
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TIP2(id) \
		wchar_t sz##id[1024]; \
		CControlUI *p##id##Tip2 = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##Tip2 && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
			p##id##Tip2->SetToolTip(sz##id);

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT_AND_TIP
 * Description:		设置控件文本及提示信息宏 -- Item
 * ParameterList:	id, tip_id
 * Parameter:       id: 控件id(在xml布局文件中指定)及语言文件中提示信息的id，要求控件id与ini文件中控件对应的提示信息id一致
 * Parameter:       tip_id: 语言文件中提示信息的id
 * Return Value:	无
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT_AND_TIP(id, tip_id) \
		wchar_t sz##id[1024], sz##tip_id[1024]; \
		CControlUI *p##id##TextTip = static_cast<CControlUI*>(pManager->FindControl(id)); \
		if(p##id##TextTip && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
			p##id##TextTip->SetText(sz##id); \
		if(p##id##TextTip && Utility::GetINIStr(pLang, pSection, tip_id, sz##tip_id)) \
			p##id##TextTip->SetToolTip(sz##tip_id);

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