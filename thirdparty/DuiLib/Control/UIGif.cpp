//////////////////////////////////////////////////////////////////////////
//
// FileName : UIGif.cpp
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "UIGif.h"

namespace DuiLib 
{
CGifUI::CGifUI()
	: m_pGifHandler(NULL)
	, m_hGifEvent(NULL)
	, m_hGifThread(NULL)
{
}

CGifUI::~CGifUI()
{
	if(m_pGifHandler)
		delete m_pGifHandler;
	if(m_hGifEvent)
	{
		CloseHandle(m_hGifEvent);
		m_hGifEvent = NULL;
	}
	WaitForSingleObject(m_hGifThread, INFINITE);
	CloseHandle(m_hGifThread);
}

LPCTSTR CGifUI::GetClass() const
{
	return _T("GifUI");
}

LPVOID CGifUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("Gif")) == 0 ) 
		return static_cast<CGifUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CGifUI::DoEvent(TEventUI& event)
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

void CGifUI::SetVisible(bool bVisible)
{
	if(m_pManager)
		goto default_pro;
	if(!bVisible)
		goto default_pro;
	if(m_pGifHandler && m_hGifEvent)
	{
		SetEvent(m_hGifEvent);
	}
default_pro:
	CControlUI::SetVisible(bVisible);
}

void CGifUI::SetGifImage(LPCTSTR pstrIcoPath)
{
	if(!m_pManager)
		return;
	if (!pstrIcoPath)
		return;
	if (m_pGifHandler)
	{
		delete m_pGifHandler;
		m_pGifHandler = NULL;
	}
	m_pGifHandler = CRenderEngine::LoadGifEx(pstrIcoPath);
	if(!m_pGifHandler)
		return;
	TImageInfo* pImageInfo = m_pGifHandler->GetCurrentFrameInfo();
	if(pImageInfo)
	{
		m_hGifEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if(!m_hGifEvent)
			return;
		DWORD dwThread = 0;
		m_hGifThread = CreateThread(NULL, 0, GifThread, (LPVOID)this, 0, &dwThread);
		if(!m_hGifThread)
			return;
		SetEvent(m_hGifEvent);
	}
}

void CGifUI::PaintStatusImage(HDC hDC)
{
	if(m_pGifHandler)
	{
		const TImageInfo* pImageInfo = m_pGifHandler->GetCurrentFrameInfo();
		if(pImageInfo)
		{
			int iWidth = pImageInfo->nX;
			int iHeight = pImageInfo->nY;
			CDuiRect rcBmpPart(0,0,iWidth,iHeight);
			CDuiRect rcCorner(0, 0, 0, 0);
			
			/*
			HDC hScrDC = GetDC(NULL);
			HDC hMemDC = CreateCompatibleDC(hScrDC);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, pImageInfo->hBitmap);
			BLENDFUNCTION bf = { AC_SRC_OVER, 0, pImageInfo->dwMask, AC_SRC_ALPHA };
			typedef BOOL (WINAPI *LPALPHABLEND)(HDC, int, int, int, int,HDC, int, int, int, int, BLENDFUNCTION);
			static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "AlphaBlend");
			lpAlphaBlend(hDC, 500 + (m_pGifHandler->GetCurrentFrame() * iWidth), 40, iWidth, iHeight, hMemDC, \
				0, 0, iWidth, iHeight, bf);

			//BitBlt(hScrDC, 500 + (m_pGifHandler->GetCurrentFrame() * iWidth), 40, m_rcItem.right-m_rcItem.left, m_rcItem.bottom-m_rcItem.top,
			//	hMemDC, 0, 0, SRCCOPY);
			SelectObject(hMemDC, hOldBitmap);
			DeleteObject(hMemDC);
			ReleaseDC(NULL, hScrDC);
			*/
			
			CRenderEngine::DrawImage(hDC, pImageInfo->hBitmap, m_rcItem, m_rcItem, rcBmpPart, rcCorner, \
				pImageInfo->alphaChannel, pImageInfo->dwMask);
			m_pGifHandler->AddCurrentIndex();
		}
	}
}

void CGifUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if(_tcscmp(pstrName, _T("gif")) == 0)
	{
		SetGifImage(pstrValue);
	}
	else
	{
		CControlUI::SetAttribute(pstrName, pstrValue);
	}
}

void CGifUI::DoInit()
{
	if(m_pGifHandler && m_pManager && m_hGifEvent)
		SetEvent(m_hGifEvent);
}

DWORD WINAPI CGifUI::GifThread(LPVOID lpParam)
{
	CGifUI *pThis = (CGifUI *)lpParam;
	if(!pThis)
		return 0;
	TImageInfo* pImageInfo = NULL;
	while(pThis->m_pGifHandler)
	{
		pImageInfo = pThis->m_pGifHandler->GetCurrentFrameInfo();
		if(pImageInfo)
		{
			pThis->Invalidate();
			WaitForSingleObject(pThis->m_hGifEvent, pImageInfo->delay);
			pThis->m_pGifHandler->GetNextFrameInfo();
			if(!pThis->m_hGifEvent)
				return 0;
			ResetEvent(pThis->m_hGifEvent);
		}
	}
	return 0;
}
} //namespace DuiLib