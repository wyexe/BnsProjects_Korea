#include "stdafx.h"
#include "FunctionTest.h"
#include <MailAction.h>

BOOL CFunctionTest::MailTest() CONST
{
	std::vector<CMailAction::MailItemContent> VecItem;
	return CMailAction::GetInstance().MailItemSupport(L"길현", L"NowNick", VecItem, 10);
}
