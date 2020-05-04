#ifndef __GITBNSPROJECTS_BNSPROJECTS_SCRIPT_FINDPATHSCRIPT_H__
#define __GITBNSPROJECTS_BNSPROJECTS_SCRIPT_FINDPATHSCRIPT_H__

#include "GameVariable.h"

#define MAX_FINDPATH_DIS 35.0f
class CFindPathScript : public MyTools::CClassInstance<CFindPathScript>
{
public:
	CFindPathScript();
	~CFindPathScript() = default;
private:
	struct Vertex
	{
		Point VertexPoint;		// ����
		Vertex(CONST Point& VertexPoint_)
		{
			VertexPoint = VertexPoint_;
		}
		Vertex()
		{
			Clear();
		}
		bool EqualTo(_In_ CONST Vertex& Vertex_) CONST
		{
			return this->VertexPoint == Vertex_.VertexPoint;
		}
		void Clear()
		{
			VertexPoint.Clear();
		}
	};
	struct Line
	{
		std::vector<Vertex> vList;	// �߶�������������
		float fDis;				// �߶ε��ܳ���
		int nLineId;			// �߶�ID
		Line()
		{
			Clear();
		}
		VOID Clear()
		{
			vList.clear();
			fDis = 0.0;
			nLineId = 0;
		}
	};
	struct Edge
	{
		Line*			pLine1;		// ����ͼ�ı�
		Line*			pLine2;		// ����ͼ�ı�

		Vertex			Line1Link;	// 2���ߵ����ӵ�(������2���߶ξ������)
		Vertex			Line2Link;	// ...

		float			nWeight;	// Ȩֵ
		struct Edge*	Root;		// ���ڵ�, �������������!
		bool			bVisited;	// �Ƿ���ʹ���
		Edge()
		{
			pLine1 = nullptr;
			pLine2 = nullptr;
			nWeight = 0;
			Root = nullptr;
			bVisited = false;
			Line1Link.Clear();
			Line2Link.Clear();
		}
		void Init(_In_ Line* pLine1_, _In_ Line* pLine2_)
		{
			pLine1 = pLine1_;
			pLine2 = pLine2_;
		}
		void Release()
		{
			pLine1 = nullptr;
			pLine2 = nullptr;
			nWeight = 0;
			Root = nullptr;
			bVisited = false;
			Line1Link.Clear();
			Line2Link.Clear();
		}
		void SetWeight(_In_ CONST Point& TarPoint);

		bool operator < (CONST Edge& Edge_) CONST
		{
			return this->nWeight > Edge_.nWeight;
		}
	};

	enum em_Dir{ em_Dir_Order, em_Dir_Desc };

	struct LineDir
	{
		int nNowVertexIndex;
		int nTarVertexIndex;
		em_Dir GetDir()
		{
			return nNowVertexIndex < nTarVertexIndex ? em_Dir_Order : em_Dir_Desc;
		}
	};

public:
	// ���Ѱ·����
	//BOOL InitGameScript(_In_ CLScript& Script, _In_ std::function<VOID(CONST Point&)> Mover);

	// ���Ѱ·����
	VOID ClearGameScript();

	// ������������
	VOID SetDeadPoint(_In_ CONST Point& DeadPoint);

	// �����������
	VOID ClearDeadPoint();

	// �Ƿ������������
	BOOL ExistDeadPoint() CONST;

	// �ƶ�����������
	BOOL MoveToDeadPoint(_In_ CONST Point& CurPoint);
private:
	// ��ȡ�ű������е�����
	//BOOL AnalysisPoint(_In_ CONST std::vector<CL_Script_TranCode>& ScriptCodeList);

	// ���һ�����굽����
	float CalcDis_By_List(_In_ CONST Line& Line_, _In_ CONST Vertex& Vertex_) CONST;

	// ��ת��
	BOOL VertexToEdge(std::vector<Edge>& vEdge);

	// �ж�2���ߵ������Ƿ�����ཻ(��һ�߶ε�����һ��������ڶ��߶ε�����һ���������<10)
	BOOL Coincide_In_Line(Line& Line1, Line& Line2, _Out_ Vertex* pLine1Link, _Out_ Vertex* pLine2Link) CONST;

	// Ѱ��������ߺ͵�
	BOOL FindVertex_By_Point(_In_ CONST Point& CurPoint, _Out_opt_ int& nIndex, _Out_opt_ Point& RetPoint) CONST;

	// ��ʼѰ·
	BOOL FindPath(_In_ std::vector<Edge>& vEdge, _In_ Line& StartLine, _In_ Line& EndLine, _In_ Point& StartPoint, _In_ Point& EndPoint);

	// Ѱ�ҿ���ͨ�е�·��
	VOID FindLine(_In_ std::vector<Edge>& vEdge, _In_ CONST Line* pCurLine, _Out_ std::vector<Edge*>& SearchList);

	// A*��ֵ
	int GetIndex_By_Heuristic_In_SearchList(_In_ std::vector<Edge*>& SearchList, _In_ CONST Point& EndPoint);

	// �����!
	BOOL PrintLine(Line* pNowLine, CONST Vertex& StartVertex, CONST Vertex& EndVertex);

	// ��ȡ����
	BOOL GetDir_By_Line(_In_ CONST Line* pLine, _In_ CONST Vertex& NowVertex, _In_ CONST Vertex& TarVertex, _Out_ LineDir& LineDir_);

	// ��ȡ��ǰ�����ߵ�����
	int GetIndex_By_Line(_In_ CONST Line* pLine, _In_ CONST Vertex& Vertex_) CONST;
private:
	// ��������
	Point					_TarPoint;
	BOOL					bExistDeadPoint;

	// ��
	std::vector<Line>			LineVec;

	// ��·����
	std::vector<Point>			vPoint;

	// ��·
	std::function<VOID(CONST Point&)> fnMove;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_SCRIPT_FINDPATHSCRIPT_H__
