#ifndef __UIICONIMAGE_H__
#define __UIICONIMAGE_H__
//////////////////////////////////////////////////////////////////////////
//
// FileName : UIIconImage.h
// Creator : kk
// Date : 2012-10-23 10:00:00
// Comment : ico图标控件
//
//////////////////////////////////////////////////////////////////////////
#pragma once
namespace DuiLib 
{
	class UILIB_API CIconUI : public CControlUI
	{
	public:
		CIconUI();
		~CIconUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void DoEvent(TEventUI& event);
		//自绘图标
		void PaintStatusImage(HDC hDC);
		//动态设置ICO 
		void SetIcoImage(LPCTSTR pstrIcoPath);
		//设置默认显示ICO
		void SetDefaultIcon(LPCTSTR pstrIcoPath);
		//清除ico显示默认的ico
		void ClearIcon();
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	protected:
		HICON m_hdefaultico;
		HICON m_hico;
	};
} //namespace DuiLib

#endif //__UIICONIMAGE_H__