#include "stdafx.h"
#include "IFormatter.h"
#include "XMLFormatter.h"
#include "JSONFormatter.h"

const wchar_t* const kFormatterXml = L"formatter_xml";
const wchar_t* const kFormatterXmlTip = L"formatter_xml_tip";
const wchar_t* const kFormatterJson = L"formatter_json";
const wchar_t* const kFormatterJsonTip = L"formatter_json_tip";
const wchar_t* const kFormatterContent = L"formatter_content";
const wchar_t* const kFormatterFormat = L"formatter_format";
const wchar_t* const kFormatterFormatTip = L"formatter_format_tip";

IFormatter::IFormatter()
{
	m_hFormatterOwner = NULL;
	m_pFormatterManager = NULL;
	m_pFormatterContent = NULL;
	m_nFormatterType = FORMAT_XML;
	m_pFormatter = NULL;
	memset(m_szHandleText, 0, sizeof(m_szHandleText));
	memset(m_szNoHandleText, 0, sizeof(m_szNoHandleText));
}

IFormatter::~IFormatter()
{
	if(m_pFormatter)
		delete m_pFormatter;
}

BOOL IFormatter::InitFormatter()
{
	if(!m_pFormatterManager)
		return FALSE;
	FIND_CONTROL_BY_ID(m_pFormatterContent, CEditUI, m_pFormatterManager, kFormatterContent)
	return TRUE;
}

BOOL IFormatter::SetFormatterLang(LPCWSTR lpszLang)
{
	if(!m_pFormatterManager)
		return FALSE;
	memset(m_szHandleText, 0, sizeof(m_szHandleText));
	memset(m_szNoHandleText, 0, sizeof(m_szNoHandleText));
	Utility::GetINIStr(lpszLang, LS_MSG, kHandleText, m_szHandleText);
	Utility::GetINIStr(lpszLang, LS_MSG, kNoHandleText, m_szNoHandleText);

SET_CONTROL_BEGIN(m_pFormatterManager, lpszLang, LS_FORMATTERPANEL)
	SET_CONTROL_TEXT_AND_TIP(kFormatterXml, kFormatterXmlTip)
	SET_CONTROL_TEXT_AND_TIP(kFormatterJson, kFormatterJsonTip)
	SET_CONTROL_TEXT_AND_TIP(kFormatterFormat, kFormatterFormatTip)
SET_CONTROL_END()

	return TRUE;
}

void IFormatter::OnFormatterClick(TNotifyUI& msg, BOOL& bHandled)
{
	if(!m_pFormatterManager)
		return;
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kFormatterFormat) // 开始格式化
	{
		if(!m_pFormatterContent)
			return;
		// Format's String
		CDuiString strContent = m_pFormatterContent->GetText();
		
		// Init Formatter
		if(m_pFormatter)
			delete m_pFormatter;
		m_pFormatter = NULL;
		switch(m_nFormatterType)
		{
		case FORMAT_XML:
			m_pFormatter = new CXMLFormatter();
			break;
		case FORMAT_JSON:
			m_pFormatter = new CJSONFormatter();
			break;
		}
		if(!m_pFormatter)
		{
			wchar_t szErr[1024], szTitle[1024];
			Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kMsgErr, szTitle);
			Utility::GetINIStr(g_pLangManager->GetLangName(), LS_MSG, kInvalidFormatterErr, szErr);
			DuiMsgBox(m_hFormatterOwner, szErr, szTitle, MB_OK);
			bHandled = TRUE;
			return;
		}
		m_pFormatter->Format(strContent.GetData());
		vector<wchar_t*> rows = m_pFormatter->GetRows();

		// 格式化后的文本
		size_t newDataLen = strContent.GetLength() + rows.size(); // rows.size():换行符数量
		for (unsigned int i=0; i<rows.size(); i++)
			newDataLen = newDataLen + wcslen(rows.at(i));

		newDataLen++;
		wchar_t *szNewText = new wchar_t[newDataLen];
		memset(szNewText, 0, newDataLen*sizeof(wchar_t));
		for (unsigned int i=0;i<rows.size();i++)
		{
			wchar_t *row = rows.at(i);
			wcscat_s(szNewText, newDataLen, row);
			wcscat_s(szNewText, newDataLen, L"\n");
		}
		m_pFormatterContent->SetText(szNewText);
		delete[] szNewText;
		bHandled = TRUE;
	}
	else if(sCtrlName == kFormatterXml)
	{
		m_nFormatterType = FORMAT_XML;
		bHandled = TRUE;
	}
	else if (sCtrlName == kFormatterJson)
	{
		m_nFormatterType = FORMAT_JSON;
		bHandled = TRUE;
	}
}