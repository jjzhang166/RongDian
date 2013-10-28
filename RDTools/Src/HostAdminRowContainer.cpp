#include "stdAfx.h"
#include "HostAdminRowContainer.h"

CHostAdminRowContainer::CHostAdminRowContainer()
{
	pCheckBox = NULL;
	pIPEdit = NULL;
	pDomainEdit = NULL;
	pDescEdit = NULL;
	pDeleteBtn = NULL;
}

CHostAdminRowContainer::~CHostAdminRowContainer()
{
}

LPCTSTR CHostAdminRowContainer::GetClass() const
{
	return _T("HostAdminRowContainerUI");
}