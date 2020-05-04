#ifndef __GITBNSPROJECTS_BNSPROJECTS_TEXT_RESTEXT_H__
#define __GITBNSPROJECTS_BNSPROJECTS_TEXT_RESTEXT_H__

#include "GameStruct.h"
#include <MyTools/ClassInstance.h>
#include <MyTools/RelfexStruct.h>

struct ItemFilterVarInfo;
class CResText : public CClassInstance<CResText>, public CRelfexStruct
{
public:
	CResText() = default;
	virtual ~CResText() = default;

public:
	
	

public:
	// ��ʼ������
	VOID Initialize();

	
private:
	// ע����Ʒ����
	VOID RegisterItemFilterText();

	// ע��������
	VOID RegisterMonsterFilterText();

	// ע����Ʒ���۷���
	VOID RegisterItemSellText();

	// ע����Ϸ�Ͽ����ӵ���ʾ
	VOID RegisterDisConnectText();

	// ע���ɫְҵText
	VOID RegisterPlayerClassesText();

	// ע����Ʒ����������
	VOID RegisterItemDiscardName();

	// ע����Ʒ����
	VOID ReigisterItemName();

private:
	
};




#endif // !__GITBNSPROJECTS_BNSPROJECTS_TEXT_RESTEXT_H__
