#ifndef __UICHILDLAYOUT_H__
#define __UICHILDLAYOUT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CChildLayoutUI : public CContainerUI
	{
	public:
		CChildLayoutUI();

		void Init();
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetChildLayoutXML(CDuiString pXML, IDialogBuilderCallback* m_pCallback = NULL);
		DuiLib::CDuiString GetChildLayoutXML();
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual LPCTSTR GetClass() const;

	private:
		DuiLib::CDuiString m_pstrXMLFile;
		IDialogBuilderCallback* m_pCallback;
	};
} // namespace DuiLib
#endif // __UICHILDLAYOUT_H__
