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
		Point VertexPoint;		// 坐标
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
		std::vector<Vertex> vList;	// 线段所包含的坐标
		float fDis;				// 线段的总长度
		int nLineId;			// 线段ID
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
		Line*			pLine1;		// 无向图的边
		Line*			pLine2;		// 无向图的边

		Vertex			Line1Link;	// 2个边的连接点(必须是2条线段距离最短)
		Vertex			Line2Link;	// ...

		float			nWeight;	// 权值
		struct Edge*	Root;		// 父节点, 用来做链表回溯!
		bool			bVisited;	// 是否访问过了
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
	// 添加寻路坐标
	//BOOL InitGameScript(_In_ CLScript& Script, _In_ std::function<VOID(CONST Point&)> Mover);

	// 清空寻路坐标
	VOID ClearGameScript();

	// 设置死亡坐标
	VOID SetDeadPoint(_In_ CONST Point& DeadPoint);

	// 清除死亡坐标
	VOID ClearDeadPoint();

	// 是否存在死亡坐标
	BOOL ExistDeadPoint() CONST;

	// 移动到死亡坐标
	BOOL MoveToDeadPoint(_In_ CONST Point& CurPoint);
private:
	// 获取脚本的所有的坐标
	//BOOL AnalysisPoint(_In_ CONST std::vector<CL_Script_TranCode>& ScriptCodeList);

	// 添加一个坐标到线上
	float CalcDis_By_List(_In_ CONST Line& Line_, _In_ CONST Vertex& Vertex_) CONST;

	// 线转边
	BOOL VertexToEdge(std::vector<Edge>& vEdge);

	// 判断2条线的坐标是否可以相交(第一线段的任意一个坐标与第二线段的任意一个坐标距离<10)
	BOOL Coincide_In_Line(Line& Line1, Line& Line2, _Out_ Vertex* pLine1Link, _Out_ Vertex* pLine2Link) CONST;

	// 寻找最近的线和点
	BOOL FindVertex_By_Point(_In_ CONST Point& CurPoint, _Out_opt_ int& nIndex, _Out_opt_ Point& RetPoint) CONST;

	// 开始寻路
	BOOL FindPath(_In_ std::vector<Edge>& vEdge, _In_ Line& StartLine, _In_ Line& EndLine, _In_ Point& StartPoint, _In_ Point& EndPoint);

	// 寻找可以通行的路线
	VOID FindLine(_In_ std::vector<Edge>& vEdge, _In_ CONST Line* pCurLine, _Out_ std::vector<Edge*>& SearchList);

	// A*估值
	int GetIndex_By_Heuristic_In_SearchList(_In_ std::vector<Edge*>& SearchList, _In_ CONST Point& EndPoint);

	// 输出线!
	BOOL PrintLine(Line* pNowLine, CONST Vertex& StartVertex, CONST Vertex& EndVertex);

	// 获取方向
	BOOL GetDir_By_Line(_In_ CONST Line* pLine, _In_ CONST Vertex& NowVertex, _In_ CONST Vertex& TarVertex, _Out_ LineDir& LineDir_);

	// 获取当前是在线的索引
	int GetIndex_By_Line(_In_ CONST Line* pLine, _In_ CONST Vertex& Vertex_) CONST;
private:
	// 死亡坐标
	Point					_TarPoint;
	BOOL					bExistDeadPoint;

	// 线
	std::vector<Line>			LineVec;

	// 走路坐标
	std::vector<Point>			vPoint;

	// 走路
	std::function<VOID(CONST Point&)> fnMove;
};



#endif // !__GITBNSPROJECTS_BNSPROJECTS_SCRIPT_FINDPATHSCRIPT_H__
