#ifndef __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_AUCTION_TEAMAUCTION_H__
#define __GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_AUCTION_TEAMAUCTION_H__

#include "GameVariable.h"

class CTeamAuction : public MyTools::CClassInstance<CTeamAuction>
{
public:
	CTeamAuction() = default;
	~CTeamAuction() = default;

	// 黑龙教组队竞拍
	VOID SetAuction_By_Gungoen_HeiLongJiao() CONST;

	// 设置为空的竞拍
	VOID SetAuction_Null() CONST;
private:

};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_GAME_SOMEFUN_AUCTION_TEAMAUCTION_H__

