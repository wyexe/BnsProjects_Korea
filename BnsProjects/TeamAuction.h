#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_AUCTION_TEAMAUCTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_AUCTION_TEAMAUCTION_H__

#include "GameVariable.h"

class CTeamAuction : public MyTools::CClassInstance<CTeamAuction>
{
public:
	CTeamAuction() = default;
	~CTeamAuction() = default;

	// ��������Ӿ���
	VOID SetAuction_By_Gungoen_HeiLongJiao() CONST;

	// ����Ϊ�յľ���
	VOID SetAuction_Null() CONST;
private:

};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_AUCTION_TEAMAUCTION_H__

