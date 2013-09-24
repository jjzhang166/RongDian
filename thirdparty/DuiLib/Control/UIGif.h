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

	// �Ի�ͼ��
	void PaintStatusImage(HDC hDC);
	// ��̬����Gif·��
	void SetGifImage(LPCTSTR pstrIcoPath);
	// ���ÿؼ�����
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void DoInit();

	static DWORD WINAPI GifThread(LPVOID lpParam);

protected:
	CGifHandler* m_pGifHandler;
	HANDLE m_hGifEvent;
};
} //namespace DuiLib

#endif //__UIGIF_H_