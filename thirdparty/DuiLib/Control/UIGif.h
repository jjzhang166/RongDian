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

	// 自绘图标
	void PaintStatusImage(HDC hDC);
	// 动态设置Gif路径
	void SetGifImage(LPCTSTR pstrIcoPath);
	// 设置控件属性
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void DoInit();

	static DWORD WINAPI GifThread(LPVOID lpParam);

protected:
	CGifHandler* m_pGifHandler;
	HANDLE m_hGifEvent;
};
} //namespace DuiLib

#endif //__UIGIF_H_