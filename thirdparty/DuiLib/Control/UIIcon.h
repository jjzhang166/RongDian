#ifndef __UIICONIMAGE_H__
#define __UIICONIMAGE_H__
//////////////////////////////////////////////////////////////////////////
//
// FileName : UIIconImage.h
// Creator : kk
// Date : 2012-10-23 10:00:00
// Comment : icoͼ��ؼ�
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
		//�Ի�ͼ��
		void PaintStatusImage(HDC hDC);
		//��̬����ICO 
		void SetIcoImage(LPCTSTR pstrIcoPath);
		//����Ĭ����ʾICO
		void SetDefaultIcon(LPCTSTR pstrIcoPath);
		//���ico��ʾĬ�ϵ�ico
		void ClearIcon();
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	protected:
		HICON m_hdefaultico;
		HICON m_hico;
	};
} //namespace DuiLib

#endif //__UIICONIMAGE_H__