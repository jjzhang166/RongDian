#pragma once

/*************************************************************************
 * Method:    		SET_CONTROL_BEGIN
 * Description:		���ÿؼ��ı��� -- Header(�������SET_CONTROL_END()��һ��ʹ��)
 * ParameterList:	man, lang, ls
 * Parameter:       man: CPaintManagerUIָ�룬ȷ���˲���ΪCPaintManagerUIָ��
 *						CPaintManagerUI m_PaintManager; ==> SET_CONTROL_BEGIN((&m_PaintManager), xxx, xxx)
 *						CPaintManagerUI *m_pPaintManager; ==> SET_CONTROL_BEGIN(m_pPaintManager, xxx, xxx)
 * Parameter:       lang: �����ļ�ȫ·��("E:\Source\XXXX\Root\lang\lang_en.ini")
 * Parameter:       ls: �����ļ�(ini�ļ�)��Section����
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_BEGIN(man, lang, ls) \
	{ \
		CPaintManagerUI *pPaintManager = man; \
		LPCWSTR pLang = lang; \
		LPCWSTR pSection = ls; \
		assert(pPaintManager!=NULL);

/*************************************************************************
 * Method:    		SET_TITLE_TEXT
 * Description:		���ÿؼ��ı��� -- Item
 * ParameterList:	hwnd, id, str
 * Parameter:       hwnd: �ؼ����(��DuiLib�У�һ�������ڴ���Ĵ��ھ��)���������ñ���
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)�������ļ����ı���Ӧ��id��Ҫ��ؼ�id��ini�ļ��пؼ���Ӧ���ı�idһ��
 * Parameter:       str: �ؼ��ı�
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_TITLE_TEXT(hwnd, id, str) \
		CControlUI *p##id = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
		if(p##id) \
		{ \
			p##id->SetText(str); \
			SetWindowText(hwnd, str); \
		}

/*************************************************************************
 * Method:    		SET_TITLE_TEXT2
 * Description:		���ÿؼ��ı��� -- Item
 * ParameterList:	hwnd, id
 * Parameter:       hwnd: �ؼ����(��DuiLib�У�һ�������ڴ���Ĵ��ھ��)���������ñ���
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)�������ļ����ı���Ӧ��id��Ҫ��ؼ�id��ini�ļ��пؼ���Ӧ���ı�idһ��
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_TITLE_TEXT2(hwnd, id) \
		wchar_t sz##id[1024]; \
		CControlUI *p##id = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
		if(p##id && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
		{ \
			p##id->SetText(sz##id); \
			SetWindowText(hwnd, sz##id); \
		}

/*************************************************************************
 * Method:    		SET_TITLE_TEXT3
 * Description:		���ÿؼ��ı��� -- Item
 *						wchar_t szAdmin[1024] = L"(Admin)";
 *						SET_TITLE_TEXT3(m_hWnd, kFrameTitle, szAdmin)
 *							����kFrameTitle�ؼ����������ļ���ȡ��kFrameTitle�ؼ���Ӧ���ı���ϢL"RongDian Tools"
 *							Ȼ��L"RongDian Tools"��L"(Admin)"����ƴ��
 *							ƴ�ӽ��ΪL"RongDian Tools(Admin)"��������ø���Ӧ�Ŀؼ�
 * ParameterList:	hwnd, id, tail
 * Parameter:       hwnd: �ؼ����(��DuiLib�У�һ�������ڴ���Ĵ��ھ��)���������ñ���
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)�������ļ����ı���Ӧ��id��Ҫ��ؼ�id��ini�ļ��пؼ���Ӧ���ı�idһ��
 * Parameter:       tail: ����id��ȡ�����ļ���Ӧ���ı��󣬽����ַ���ƴ�ӵ�β��
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_TITLE_TEXT3(hwnd, id, tail) \
		wchar_t sz##id##Text[1024], sz##id[1024]; \
		CControlUI *p##id = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
		if(p##id && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
		{ \
			swprintf(sz##id##Text, L"%s%s", sz##id, tail); \
			p##id->SetText(sz##id##Text); \
			SetWindowText(hwnd, sz##id##Text); \
		}

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT
 * Description:		���ÿؼ��ı��� -- Item
 * ParameterList:	id, str
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)�������ļ����ı���Ӧ��id��Ҫ��ؼ�id��ini�ļ��пؼ���Ӧ���ı�idһ��
 * Parameter:       str: �ؼ��ı�
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT(id, str) \
		CControlUI *p##id##Text = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
		if(p##id##Text) \
			p##id##Text->SetText(str);

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT2
 * Description:		���ÿؼ��ı��� -- Item
 * ParameterList:	id
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)�������ļ����ı���Ӧ��id��Ҫ��ؼ�id��ini�ļ��пؼ���Ӧ���ı�idһ��
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT2(id) \
		wchar_t sz##id[1024]; \
		CControlUI *p##id##Text2 = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
		if(p##id##Text2 && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
			p##id##Text2->SetText(sz##id);

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT3
 * Description:		���ÿؼ��ı��� -- Item
 *						wchar_t szVer[1024] = L"1.0.0.1";
 *						SET_CONTROL_TEXT3(kAboutVer, szAppVer)
 *							����kAboutVer�ؼ����������ļ���ȡ��kAboutVer�ؼ���Ӧ���ı���ϢL"Version:"
 *							Ȼ��L"Version:"��L"1.0.0.1"����ƴ��
 *							ƴ�ӽ��ΪL"Version:1.0.0.1"��������ø���Ӧ�Ŀؼ�
 * ParameterList:	id, tail
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)�������ļ����ı���Ӧ��id��Ҫ��ؼ�id��ini�ļ��пؼ���Ӧ���ı�idһ��
 * Parameter:       tail: ����id��ȡ�����ļ���Ӧ���ı��󣬽����ַ���ƴ�ӵ�β��
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT3(id, tail) \
		wchar_t sz##id##Text[1024], sz##id[1024]; \
		CControlUI *p##id##Text3 = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
		if(p##id##Text3 && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
		{ \
			swprintf(sz##id##Text, L"%s%s", sz##id, tail); \
			p##id##Text3->SetText(sz##id##Text); \
		}

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT4
 * Description:		���ÿؼ��ı��� -- Item
 *						��������û��ָ��ini�����ļ������Դ�rc��Դ�ļ��л�ȡ��Ӧ���ı���Ϣ
 * ParameterList:	id, iid, nid
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)
 * Parameter:		iid: �����ļ�(ini�ļ�)���ı���Ӧ��id
 * Parameter:		nid: ��Դ�ļ�(rc�ļ�)���ı���Ӧ��id
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT4(id, iid, nid) \
		wchar_t sz##id[1024] = {0}; \
		CControlUI *p##id##Text4 = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
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
 * Description:		���ÿؼ���ʾ��Ϣ�� -- Item
 * ParameterList:	id, str
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)�������ļ�����ʾ��Ϣ��id��Ҫ��ؼ�id��ini�ļ��пؼ���Ӧ����ʾ��Ϣidһ��
 * Parameter:       str: ��ʾ��Ϣ
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TIP(id, str) \
		CControlUI *p##id##Tip = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
		if(p##id##Tip) \
			p##id##Tip->SetToolTip(str); \

/*************************************************************************
 * Method:    		SET_CONTROL_TIP2
 * Description:		���ÿؼ���ʾ��Ϣ�� -- Item
 * ParameterList:	id
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)�������ļ�����ʾ��Ϣ��id��Ҫ��ؼ�id��ini�ļ��пؼ���Ӧ����ʾ��Ϣidһ��
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TIP2(id) \
		wchar_t sz##id[1024]; \
		CControlUI *p##id##Tip2 = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
		if(p##id##Tip2 && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
			p##id##Tip2->SetToolTip(sz##id);

/*************************************************************************
 * Method:    		SET_CONTROL_TEXT_AND_TIP
 * Description:		���ÿؼ��ı�����ʾ��Ϣ�� -- Item
 * ParameterList:	id, tip_id
 * Parameter:       id: �ؼ�id(��xml�����ļ���ָ��)�������ļ�����ʾ��Ϣ��id��Ҫ��ؼ�id��ini�ļ��пؼ���Ӧ����ʾ��Ϣidһ��
 * Parameter:       tip_id: �����ļ�����ʾ��Ϣ��id
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TEXT_AND_TIP(id, tip_id) \
		wchar_t sz##id[1024], sz##tip_id[1024]; \
		CControlUI *p##id##TextTip = static_cast<CControlUI*>(pPaintManager->FindControl(id)); \
		if(p##id##TextTip && Utility::GetINIStr(pLang, pSection, id, sz##id)) \
			p##id##TextTip->SetText(sz##id); \
		if(p##id##TextTip && Utility::GetINIStr(pLang, pSection, tip_id, sz##tip_id)) \
			p##id##TextTip->SetToolTip(sz##tip_id);

/*************************************************************************
 * Method:    		SET_CONTROL_END
 * Description:		���ÿؼ��ı�����ʾ��Ϣ�� -- Tail(�������SET_CONTROL_BEGIN()��һ��ʹ��)
 * ParameterList:	��
 * Return Value:	��
 * Date:        	13:09:01 16:42:56
 * Author:			LIng
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_END() \
	}

/*************************************************************************
 * Method:    		SET_CONTROL_TIP3
 * Description:		���ÿؼ���ʾ��Ϣ��
 * ParameterList:	control section id str
 * Return Value:	��
 * CopyRight:		
 ************************************************************************/
#define SET_CONTROL_TIP3(section,control,id) \
{\
	wchar_t tip[50] = {0}; \
	Utility::GetINIStr(g_pLangManager->GetLangName(),section,id,tip); \
	control->SetToolTip(tip);\
}

/*************************************************************************
* Method:    		SET_CONTROL_TIP4
* Description:		���ÿؼ���ʾ��Ϣ�꣬��ȡ�����ļ��а���%s����Ϣ
* ParameterList:	control section id lpszDynamicStr
* Return Value:	��
* CopyRight:		
************************************************************************/
#define SET_CONTROL_TIP4(section,control,id,lpszDynamicStr) \
{\
	wchar_t szContent[1024]={0};\
	wchar_t tip[50] = {0}; \
	Utility::GetINIStr(g_pLangManager->GetLangName(),section,id,tip); \
	swprintf(szContent, tip, lpszDynamicStr);\
	control->SetToolTip(szContent);\
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