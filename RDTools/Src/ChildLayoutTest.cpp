#include "StdAfx.h"
#include "ChildLayoutTest.h"
#include "ContainerTest.h"

const wchar_t* const kChildLayoutTest = L"childlayouttest_test";
const wchar_t* const kChildLayoutContainer = L"childlayouttest_container";

const wchar_t* const kTestContainer = L"TestContainer";
const wchar_t* const kSubButton = L"SubButton";
const wchar_t* const kSubLabel = L"SubLabel";
const wchar_t* const kSubEdit = L"SubEdit";

CChildLayoutTest::CChildLayoutTest()
{

}

BOOL CChildLayoutTest::IsCanQuit(HWND hWnd)
{
	return TRUE;
}

void CChildLayoutTest::OnQuit()
{

}

BOOL CChildLayoutTest::OnInit(WPARAM wParam, LPARAM lParam)
{
	CPaintManagerUI *pManager = (CPaintManagerUI *)wParam;
	if(!pManager)
		return FALSE;
	return TRUE;
}

BOOL CChildLayoutTest::SetLang(CPaintManagerUI* pManager, LPCWSTR lpszLang)
{
	if(!pManager)
		return FALSE;

	return TRUE;
}

CControlUI* CChildLayoutTest::OnCreateControl(LPCTSTR pstrClass, CControlUI *pParent)
{
	BOOL bSubControl = FALSE;
	int nSubType = 0; // 自定义标识，0为非子控件，1为button，2为label，3为edit
	CControlUI* pControl = NULL;
	if (wcsicmp(pstrClass, kTestContainer)==0)
	{
		pControl = new CContainerTest();
	}
	else if (wcsicmp(pstrClass, kSubButton)==0)
	{
		pControl = new CButtonUI();
		nSubType = 1;
	}
	else if (wcsicmp(pstrClass, kSubLabel)==0)
	{
		pControl = new CLabelUI();
		nSubType = 2;
	}
	else if (wcsicmp(pstrClass, kSubEdit)==0)
	{
		pControl = new CEditUI();
		nSubType = 3;
	}
	if(pControl && nSubType!=0)
	{
		CContainerTest *pTestConainer = NULL;
		CControlUI *pContainer = pParent;
		while(pContainer && pContainer->GetParent())
			pContainer = pContainer->GetParent();
		if(!pContainer)
		{
			delete pControl;
			pControl = NULL;
			return pControl;
		}
		if(wcsicmp(pContainer->GetClass(), L"TestContainerUI")==0)
		{
			pTestConainer = static_cast<CContainerTest *>(pContainer);
			if(nSubType==1)
				pTestConainer->SaveButton(pControl);
			else if(nSubType==2)
				pTestConainer->SaveLabel(pControl);
			else if(nSubType==3)
				pTestConainer->SaveEdit(pControl);
		}
	}
	return pControl;
}

void CChildLayoutTest::OnClick(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	BOOL bHandle = FALSE;
	CDuiString sCtrlName = msg.pSender->GetName();
	if(sCtrlName == kChildLayoutTest)
	{
		CreateTestContainer(pManager);
	}
	else if(wcsstr(sCtrlName.GetData(), L"SubButton"))
	{
		wchar_t szErr[1024] = {0}, szFormat[1024] = {0}, szTitle[1024];
		Utility::GetINIStr(g_pLangManager->GetLangName(), GET_ASSOC_SECTION(g_LangIDs, MSG_ERR), GET_ASSOC_ID(g_LangIDs, MSG_ERR), szTitle);
		Utility::GetINIStr(g_pLangManager->GetLangName(), GET_ASSOC_SECTION(g_LangIDs, MSG_DEBUG_TEXT), GET_ASSOC_ID(g_LangIDs, MSG_DEBUG_TEXT), szFormat);
		swprintf(szErr, szFormat, sCtrlName.GetData());
		DuiMsgBox(hWnd, szErr, szTitle, MB_OK);
	}
}

void CChildLayoutTest::OnItemActive(HWND hWnd, CPaintManagerUI* pManager, TNotifyUI& msg, BOOL& bHandled)
{
	bHandled = FALSE;
	CListBodyUI *pParent = (CListBodyUI *)msg.pSender->GetParent();
	if(!pParent)
		return;
	CListUI *pList = (CListUI *)pParent->GetParent();
	if(!pList)
		return;
}

BOOL CChildLayoutTest::CreateTestContainer(CPaintManagerUI* pManager)
{
	CVerticalLayoutUI *pContainer = NULL;
	if(!pManager)
		return FALSE;
	FIND_CONTROL_BY_ID(pContainer, CVerticalLayoutUI, pManager, kChildLayoutContainer);
	if(!pContainer)
		return FALSE;
	CChildLayoutUI *pChild = new CChildLayoutUI();
	if(!pChild)
		return FALSE;
	wchar_t szChild[1024] = {0}, szContainer[1024] = {0}, szPath[1024] = {0};
	wchar_t szButton[1024] = {0}, szLabel[1024] = {0}, szEdit[1024] = {0};
	swprintf(szChild, L"Child_%d", pContainer->GetCount());
	swprintf(szContainer, L"TestContainer_%d", pContainer->GetCount());
	swprintf(szButton, L"SubButton_%d", pContainer->GetCount());
	swprintf(szLabel, L"SubLabel_%d", pContainer->GetCount());
	swprintf(szEdit, L"SubEdit_%d", pContainer->GetCount());
	swprintf(szPath, L"GroupTest.xml");
	pChild->SetName(szChild);
	pChild->SetChildLayoutXML(szPath, (DuiLib::IDialogBuilderCallback *)g_pMainFrame);
	pChild->SetVisible(true);
	int nIndex = pContainer->GetCount();
	if(!pContainer->AddAt(pChild, nIndex))
		return FALSE;
	if(pChild->GetCount()<=0)
	{
		return FALSE;
	}
	pChild->SetFixedHeight(pChild->GetItemAt(0)->GetFixedHeight());

	CContainerTest *pTestContainer = (CContainerTest *)pChild->GetItemAt(0);
	pTestContainer->SetName(szContainer);
	CControlUI *pSubButton = pTestContainer->GetButton();
	if(pSubButton)
	{
		pSubButton->SetName(szButton);
		pSubButton->SetText(szButton);
	}
	CControlUI *pSubLabel = pTestContainer->GetLabel();
	if(pSubLabel)
	{
		pSubLabel->SetName(szLabel);
		pSubLabel->SetText(szLabel);
	}
	CControlUI *pSubEdit = pTestContainer->GetEdit();
	if(pSubEdit)
	{
		pSubEdit->SetName(szEdit);
		pSubEdit->SetText(szEdit);
	}
	CContainerTest *pTestContainer2 = NULL, *pTestContainer3 = NULL;
	FIND_CONTROL_BY_ID(pTestContainer2, CContainerTest, pManager, szContainer); // 哈希表没有更新，无法通过id查找控件
	pManager->UpdateControls(pTestContainer); // 更新哈希表
	FIND_CONTROL_BY_ID(pTestContainer3, CContainerTest, pManager, szContainer);
	return TRUE;
}