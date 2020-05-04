#ifndef __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEAM_CROSSAREATEAM_H__
#define __GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEAM_CROSSAREATEAM_H__

#include <atomic>
#include <map>
#include <MyTools/ClassInstance.h>
#include "ServerCommon.h"

class CCrossAreaTeam : public MyTools::CClassInstance<CCrossAreaTeam>
{
public:
	//				  wsAccountName
	typedef std::map<std::wstring, CrossAreaTeamGroup> ListCrossAreaTeamGroup;

	//                 wsAccountName
	typedef std::map<std::wstring, CrossAreaTeamMember> VecCrossAreaTeamMember;

	//
	//typedef typename std::vector<CrossAreaTeamMember>::iterator CrossAreaTeamMemberItr;

	struct DungeonClassesText
	{
		std::wstring wsDungeonName;
		DWORD dwMapIndex;
		UINT uMaxTeamMember;
		std::vector<std::wstring> VecClasses;
	};

	struct DungeonClasses
	{
		std::wstring wsDungeonName;
		UINT uMaxTeamMember;
		std::vector<em_TeamMeber_Classes> VecClasses;
	};

public:
	CCrossAreaTeam();
	~CCrossAreaTeam() = default;

	// ��������Ŷ�
	BOOL JoinTeam(_In_ CrossAreaTeamMember& Member, _Out_ DWORD& dwTeamId);

	// ���ݶ���ID��ȡ������������
	BOOL GetTeamMemberContent(_In_ DWORD dwTeamId, _Out_ CrossAreaTeamGroup& Result);

	// ɾ������(һ���ǿͻ����Ǳ߲�����)
	VOID DeleteTeamGroup(_In_ CONST std::wstring& wsAccountName);

	// ���(ɾ���ȴ��б�)
	VOID DeleteTeamMember(_In_ CONST std::wstring& wsAccountName);

	// �����ǰ�Ŷ���ϸ����
	VOID Print();

	// 
	VOID Initialize();

	//
	VOID Test();
private:
	// ��ȡ�Ѿ���õĶ���ID
	CONST CrossAreaTeamGroup* ExistCrossAreaTeamGroup(_In_ CONST CrossAreaTeamMember& Member) CONST;

	//
	BOOL ExistCrossAreaTeamMember(_In_ CONST CrossAreaTeamMember& Member) CONST;

	//
	VOID AddToCrossAreaTeamMember(_In_ CONST CrossAreaTeamMember& Member);

	//
	VOID UpdateTickInTeamMember(_In_ CONST CrossAreaTeamMember& Member);

	//////////////////////////////////////////////////////////////////////////

	//
	UINT FindSameDungoen(_In_ CONST std::wstring& wsDungeonName, _In_ VecCrossAreaTeamMember& MapMember, _Out_ std::vector<CrossAreaTeamMember*>& VecItr);

	//
	VOID Scheduling();

	//
	BOOL SchedulingSameDungeon(_In_ CONST std::wstring& wsDungeonName, _In_ VecCrossAreaTeamMember& MapMember, _In_ std::vector<CrossAreaTeamMember*>& VecItr, _In_ CONST std::vector<em_TeamMeber_Classes>& VecClasses);

	//
	VOID AddtoCrossAreaTeamGroup(_In_ CONST std::wstring& wsDungeonName,_In_ std::vector<CrossAreaTeamMember*>& VecItr);

	//
	VOID PrintCrossAreaTeamMember(_In_ CONST CrossAreaTeamMember& Member) CONST;

	//
	VOID PrintCrossAreaTeamGroup(_In_ CONST CrossAreaTeamGroup& Teamgroup) CONST;

	//
	BOOL ExistAccountNameInTeamGroup(_In_ CONST std::wstring& wsAccountName, _In_ CONST CrossAreaTeamGroup& Teamgroup);

	//
	BOOL IsKeepALive(_In_ CONST CrossAreaTeamMember& Member) CONST;

	//
	VOID RemoveTeamMember(_In_ CONST std::wstring& wsAccountName);

	VOID RemoveTeamMemberName(_In_ CONST std::wstring& wsAccountName);
private:
	std::atomic_bool									_bPrint;

	//       dwTeamId
	std::map<DWORD, CrossAreaTeamGroup>					_MapCrossAreaTeamGroup;

	std::mutex											_MtxCrossAreaTeam;

	std::set<std::wstring>								_SetTeamMemberName;
	std::set<std::wstring>								_SetDungeonName;
	//        wsGroupTitle
	std::map<std::wstring, VecCrossAreaTeamMember>		_MapTeamMember;
	//
	std::map<std::wstring, std::vector<em_TeamMeber_Classes>> _MapDungeonClasses;
};


#endif // !__GITBNSPROJECTS_BNSKORSERVER_IOCP_SERVER_TEAM_CROSSAREATEAM_H__
