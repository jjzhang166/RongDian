//////////////////////////////////////////////////////////////////////////
//
// FileName : UIIconImage.cpp
// Creator : kk
// Date : 2012-10-23 10:00:00
// Comment : icoÍ¼±ê¿Ø¼þ
//
//////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "UIIcon.h"

#define ICON_DEFAULT_SIZE 32

namespace DuiLib 
{
	CIconUI::CIconUI()
	{
		m_hdefaultico=NULL;
		m_hico=NULL;
	}

	CIconUI::~CIconUI()
	{
		if (m_hico)
		{
			::DestroyIcon(m_hico);
		}

		if (m_hdefaultico)
		{
			::DestroyIcon(m_hdefaultico);
		}
	}

	LPCTSTR CIconUI::GetClass() const
	{
		return _T("IconImageUI");
	}

	LPVOID CIconUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("Icon")) == 0 ) 
			return static_cast<CIconUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CIconUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			m_bFocused = true;
			Invalidate();
			return;
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			m_bFocused = false;
			Invalidate();
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			return;
		}
		CControlUI::DoEvent(event);
	}
	void CIconUI::SetDefaultIcon(LPCTSTR pstrIcoPath)
	{
		if (m_hdefaultico == NULL)
		{ 
			m_hdefaultico = (HICON)LoadImage(NULL, pstrIcoPath, IMAGE_ICON, 
				ICON_DEFAULT_SIZE,
				ICON_DEFAULT_SIZE,
				LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		}
	}
	
	void CIconUI::ClearIcon()
	{
		if (m_hico)
		{
			::DestroyIcon(m_hico);
		}
	}

	void CIconUI::SetIcoImage(LPCTSTR pstrIcoPath)
	{
		if (pstrIcoPath)
		{
			if (m_hico)
			{
				::DestroyIcon(m_hico);
			}
			m_hico = (HICON)LoadImage(NULL, pstrIcoPath, IMAGE_ICON, 
									ICON_DEFAULT_SIZE,
									ICON_DEFAULT_SIZE,
									LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		}

		Invalidate();
	}

	void CIconUI::PaintStatusImage(HDC hDC)
	{
		if (m_hico)
		{
			::DrawIconEx(hDC,m_rcItem.left,m_rcItem.top,m_hico,16,16,0,NULL,DI_NORMAL);
		}
		else
		{
			if (m_hdefaultico)
			{
				::DrawIconEx(hDC,m_rcItem.left,m_rcItem.top,m_hdefaultico,16,16,0,NULL,DI_NORMAL);
			}
		}
	}

	void CIconUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if(_tcscmp(pstrName, _T("icon")) == 0)
		{
			CDuiString tmpPath(m_pManager->GetResourcePath());
	//		tmpPath += _T("\\");
			tmpPath += pstrValue;
			m_hico = (HICON)LoadImage(NULL, tmpPath, IMAGE_ICON, 
				ICON_DEFAULT_SIZE,
				ICON_DEFAULT_SIZE,
				LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		}
		else
		{
			CControlUI::SetAttribute(pstrName, pstrValue);
		}
	}

} //namespace DuiLib