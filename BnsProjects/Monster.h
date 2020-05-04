#ifndef __BNS_MONSTER_H__
#define __BNS_MONSTER_H__

#include "Player.h"

class CNpc;
class CMonster : public CPlayer
{
public:
	enum _em_Monster_Move_Type{ em_Monster_Move_Type_Front, em_Monster_Move_Type_Behind };
public:
	explicit CMonster(DWORD dwBase_);			/*不拷贝CPlayer的变量,重新实例化*/
	explicit CMonster(CONST CPlayer& Player);	/*拷贝了CPlaye的变量,如Namer*/
	explicit CMonster();
	~CMonster();

	// MonsterType
	DWORD	GetMonsterType() CONST;

	// Name
	virtual VOID SetName() CONST;
	virtual CONST std::wstring& GetName() CONST;

	// 获取怪物前面||后面的坐标
	virtual Point	GetBehindPoint(float fDis = 2.0f) CONST;
	virtual Point	GetFrontPoint(float fDis = 2.0f) CONST;
	virtual Point	GetFacePoint(float fDis = 2.0f) CONST;

	// Exist
	virtual BOOL	IsExist() CONST;

	// 是否火罩子
	enum em_Maze_Shield_Color { em_Maze_Shield_Color_None, em_Maze_Shield_Color_Fire, em_Maze_Shield_Color_Ice };
	em_Maze_Shield_Color Maze_GetShieldColor() CONST;

	// Npc捡物
	BOOL	TakeNpcItem();

	// sort
	bool operator < (const CMonster& GameObj) CONST;
private:

};


#endif