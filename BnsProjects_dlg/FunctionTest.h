#ifndef __GITBNSPROJECTS_BNSPROJECTS_TEST_FUNCTIONTEST_H__
#define __GITBNSPROJECTS_BNSPROJECTS_TEST_FUNCTIONTEST_H__

#include "GameVariable.h"

class CFunctionTest : public MyTools::CClassInstance<CFunctionTest>
{
public:
	CFunctionTest() = default;
	~CFunctionTest() = default;

	BOOL MailTest() CONST;
private:

};




#endif // __GITBNSPROJECTS_BNSPROJECTS_TEST_FUNCTIONTEST_H__
