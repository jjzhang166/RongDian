#include "StdAfx.h"
#include "ContainerTest.h"

CContainerTest::CContainerTest()
{
	pSubButton = NULL;
	pSubLabel = NULL;
	pSubEdit = NULL;
}

CContainerTest::~CContainerTest()
{

}

LPCTSTR CContainerTest::GetClass() const
{
	return _T("TestContainerUI");
}
