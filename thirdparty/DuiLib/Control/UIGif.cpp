//////////////////////////////////////////////////////////////////////////
//
// FileName : UIGif.cpp
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "UIGif.h"

namespace DuiLib 
{
	CGifUI::CGifUI()
		:m_pGifHandler(NULL)
	{
	}

	CGifUI::~CGifUI()
	{
		if(m_pGifHandler)
			delete m_pGifHandler;
	}

	LPCTSTR CGifUI::GetClass() const
	{
		return _T("GifUI");
	}

	LPVOID CGifUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, _T("Gif")) == 0 ) return static_cast<CGifUI*>(this);
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
		if(event.Type == UIEVENT_TIMER)
		{
		/*	if(m_pGifHandler)
			{
				m_pManager->KillTimer(this);
				TImageInfo* pNextImage = m_pGifHandler->GetNextFrameInfo();
				if(pNextImage)
				{
					m_pManager->SetTimer(this, GIF_TIMER_ID, pNextImage->delay);
				}
			}*/
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
		{
			if(bVisible)
			{
				if(m_pGifHandler)
				{
					TImageInfo* pImageInfo = m_pGifHandler->GetCurrentFrameInfo();
					if(pImageInfo)
					{
						m_pManager->SetTimer(this, GIF_TIMER_ID, pImageInfo->delay);
					}
				}
			}
			else
			{
				m_pManager->KillTimer(this);
			}
		}
		CControlUI::SetVisible(bVisible);
	}

	void CGifUI::SetGifImage(LPCTSTR pstrIcoPath)
	{
		if (pstrIcoPath)
		{
			if (m_pGifHandler)
			{
				delete m_pGifHandler;
				m_pGifHandler = NULL;
			}
			m_pGifHandler = CRenderEngine::LoadGif(pstrIcoPath);
			if(m_pManager)
			{
				if(m_pGifHandler)
				{
					TImageInfo* pImageInfo = m_pGifHandler->GetCurrentFrameInfo();
					if(pImageInfo)
					{
						m_pManager->SetTimer(this, GIF_TIMER_ID, pImageInfo->delay);
					}
				}
			}
		}

		Invalidate();
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
				CRenderEngine::DrawImage(hDC, pImageInfo->hBitmap, m_rcItem, m_rcItem, rcBmpPart, rcCorner, \
					pImageInfo->alphaChannel, 255);
				m_pGifHandler->AddCurrentIndex();
			}
		}
	/*	if (m_pGifHandler)
		{
			:
			::DrawIconEx(hDC,m_rcItem.left,m_rcItem.top,m_hico,16,16,0,NULL,DI_NORMAL);
		}
		else
		{
		
				::DrawIconEx(hDC,m_rcItem.left,m_rcItem.top,m_hdefaultico,16,16,0,NULL,DI_NORMAL);
			}
		}*/
	}

	void CGifUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if(_tcscmp(pstrName, _T("gif")) == 0)
		{
			SetGifImage(pstrValue);
		}
		else
			CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CGifUI::DoInit()
	{
		if(m_pGifHandler)
		{
			TImageInfo* pImageInfo = m_pGifHandler->GetCurrentFrameInfo();
			if(pImageInfo)
			{
				m_pManager->SetTimer(this, GIF_TIMER_ID, pImageInfo->delay);
			}
		}
	}

} //namespace DuiLib