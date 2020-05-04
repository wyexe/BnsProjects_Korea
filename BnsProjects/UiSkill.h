#ifndef __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_SKILL_UISKILL_H__
#define __GITBNSPROJECTS_BNSPROJECTS_OBJECT_OTHEROBJECT_SKILL_UISKILL_H__

#include "GameVariable.h"

class CPlayer;
class CUiSkill
{
public:
	explicit CUiSkill();
	explicit CUiSkill(_In_ DWORD dwBase_, _In_ DWORD dwIndex_);
	~CUiSkill();

	virtual DWORD GetBase() CONST throw();

	virtual DWORD GetId() CONST throw();

	virtual BOOL  IsCool() CONST throw();

	virtual BOOL  IsActive() CONST throw();

	virtual BOOL IsExist() CONST throw();

	virtual CONST std::wstring& GetTextKeyByIndex() CONST throw();

	virtual DWORD GetIndex() CONST throw();

	virtual std::wstring GetName() CONST;

	DWORD GetSendPacketParmater() CONST;

	bool operator < (CONST CUiSkill& CUiSkill_) CONST;
private:
	DWORD dwBase;
	DWORD dwIndex;
};



#endif