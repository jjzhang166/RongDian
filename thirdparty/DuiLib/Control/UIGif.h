#ifndef __UIGIF_H_
#define __UIGIF_H_
//////////////////////////////////////////////////////////////////////////
//
// FileName : UIGif.h
//
//////////////////////////////////////////////////////////////////////////
#pragma once
namespace DuiLib 
{
	class UILIB_API CGifUI : public CControlUI
	{
		enum
		{
			GIF_TIMER_ID = 5,
		};
	public:
		CGifUI();
		~CGifUI();
		
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void DoEvent(TEventUI& event);
		void SetVisible(bool bVisible = true);

		//�Ի�ͼ��
		void PaintStatusImage(HDC hDC);
		//��̬����ICO 
		void SetGifImage(LPCTSTR pstrIcoPath);
		//���ico��ʾĬ�ϵ�ico
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void DoInit();
	protected:
		CGifHandler* m_pGifHandler;
	};
} //namespace DuiLib

#endif //__UIGIF_H_