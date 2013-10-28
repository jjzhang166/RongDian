#include "stdAfx.h"
#include "HostAdminGroupContainer.h"

CHostAdminGroupContainer::CHostAdminGroupContainer()
{
	pVisibleBtn = NULL;
	pNewRowBtn = NULL;
	pDeleteBtn = NULL;
	pTitleEdit = NULL;
	pRowsVLayout = NULL;
}

CHostAdminGroupContainer::~CHostAdminGroupContainer()
{
}
LPCTSTR CHostAdminGroupContainer::GetClass() const
{
	return _T("HostAdminGroupContainerUI");
}