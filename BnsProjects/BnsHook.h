#ifndef __GITBNSPROJECTS_BNSPROJECTS_HOOK_BNSHOOK_H__
#define __GITBNSPROJECTS_BNSPROJECTS_HOOK_BNSHOOK_H__

#include "GameVariable.h"

class CBnsHook : public MyTools::CClassInstance<CBnsHook>
{
public:
	explicit CBnsHook() = default;
	~CBnsHook() = default;

public:
	// �Զ���F(����)!
	BOOL SetHookTakeItemMode(_In_ BOOL bHook);

	// ��������ͼ
	BOOL SetHookDungoenMode(_In_ BOOL bHook); 

	// ���ø߶�
	BOOL HookHeight(_In_ BOOL bHook, _In_ float fHeight) CONST;
private:
	DSIABLE_COPY_AND_ASSIGN(CBnsHook)
};



#endif