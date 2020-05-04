#ifndef __BNS_MONSTER_H__
#define __BNS_MONSTER_H__

#include "Player.h"

class CNpc;
class CMonster : public CPlayer
{
public:
	enum _em_Monster_Move_Type{ em_Monster_Move_Type_Front, em_Monster_Move_Type_Behind };
public:
	explicit CMonster(DWORD dwBase_);			/*������CPlayer�ı���,����ʵ����*/
	explicit CMonster(CONST CPlayer& Player);	/*������CPlaye�ı���,��Namer*/
	explicit CMonster();
	~CMonster();

	// MonsterType
	DWORD	GetMonsterType() CONST;

	// Name
	virtual VOID SetName() CONST;
	virtual CONST std::wstring& GetName() CONST;

	// ��ȡ����ǰ��||���������
	virtual Point	GetBehindPoint(float fDis = 2.0f) CONST;
	virtual Point	GetFrontPoint(float fDis = 2.0f) CONST;
	virtual Point	GetFacePoint(float fDis = 2.0f) CONST;

	// Exist
	virtual BOOL	IsExist() CONST;

	// �Ƿ������
	enum em_Maze_Shield_Color { em_Maze_Shield_Color_None, em_Maze_Shield_Color_Fire, em_Maze_Shield_Color_Ice };
	em_Maze_Shield_Color Maze_GetShieldColor() CONST;

	// Npc����
	BOOL	TakeNpcItem();

	// sort
	bool operator < (const CMonster& GameObj) CONST;
private:

};


#endif