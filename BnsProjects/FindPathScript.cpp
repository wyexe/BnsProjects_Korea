#include "stdafx.h"
#include "FindPathScript.h"
#include <MyTools/Log.h>
#include "SomeFun.h"
#include "BaseObject.h"

#define _SELF L"FindPathScript.cpp"

VOID CFindPathScript::Edge::SetWeight(_In_ CONST Point& TarPoint)
{
	nWeight = 100000.0f;
	for (auto& itm : pLine1->vList)
	{
		float fDis = CBaseObject::Get3DDis(itm.VertexPoint, TarPoint);
		if (fDis < nWeight)
			nWeight = fDis;
	}
	for (auto& itm : pLine2->vList)
	{
		float fDis = CBaseObject::Get3DDis(itm.VertexPoint, TarPoint);
		if (fDis < nWeight)
			nWeight = fDis;
	}
}

CFindPathScript::CFindPathScript()
{
	bExistDeadPoint = FALSE;
	fnMove = nullptr;
}

//BOOL CFindPathScript::InitGameScript(_In_ CLScript& Script, _In_ std::function<VOID(CONST Point&)> Mover)
//{
	//CONST std::vector<CL_Script_TranCode>& ScriptCodeList = Script.GetSourceList();

	// Point -> Line
	//if (!AnalysisPoint(ScriptCodeList))
	//{
	//	LOG_MSG_CF( L"收集坐标错误!");
	//	return FALSE;
	//}

	//fnMove = Mover;
	//return TRUE;
//}

/*
BOOL CFindPathScript::AnalysisPoint(_In_ CONST std::vector<CL_Script_TranCode>& ScriptCodeList)
{
	LineVec.clear();

	int		nGroupId		= 0;
	BOOL	bGroup			= FALSE;
	Line	Line_;
	Vertex	LastVertex;

	// Text
	static CONST std::vector<std::wstring>	HunterFunNameVec = { L"走到", L"设置坐标" };


	for (CONST auto& itm : ScriptCodeList)
	{
		if (itm.emCodeType != em_CL_Script_Code_Type::em_CL_Script_Code_Type_Function)
			continue;
		if (!GameRun)
			break;

		CONST auto pScriptFunction = reinterpret_cast<CL_Script_TranCode_Function *>(itm.pStruct);
		auto pwsExistFunName = CLPublic::Vec_find_if_Const(HunterFunNameVec, [&pScriptFunction](CONST  std::wstring& wsFunName){ return wsFunName == pScriptFunction->wsFunName; });
		if (pwsExistFunName == nullptr)
		{
			if (bGroup)
			{
				bGroup = FALSE;
				LineVec.push_back(Line_);
				nGroupId += 1;
				Line_.Clear();
				LastVertex.VertexPoint.Clear();
			}
			continue;
		}
		else
			bGroup = TRUE;

		if (pScriptFunction->ParmList.size() != 3)
			continue;

		Point Pt;
		Pt.X = pScriptFunction->ParmList.at(0).ConvertFLOAT();
		Pt.Y = pScriptFunction->ParmList.at(1).ConvertFLOAT();
		Pt.Z = pScriptFunction->ParmList.at(2).ConvertFLOAT();

		if (LastVertex.VertexPoint.IsZero())
			LastVertex.VertexPoint = Pt;
		else if (CBaseObject::Get3DDis(LastVertex.VertexPoint, Pt) >= MAX_FINDPATH_DIS)
		{
			LineVec.push_back(Line_);
			nGroupId += 1;
			Line_.Clear();
		}

		LastVertex.VertexPoint = Pt;
		Line_.fDis += CalcDis_By_List(Line_, LastVertex);
		Line_.vList.push_back(Pt);
		Line_.nLineId = nGroupId;
	}
	LOG_CF_D( L"线:%d", LineVec.size());
	return TRUE;
}*/

BOOL CFindPathScript::MoveToDeadPoint(_In_ CONST Point& CurPoint)
{
	// Line -> Edge
	std::vector<Edge> EdgeVec;
	VertexToEdge(EdgeVec);

	LOG_CF_D( L"无向图 边的数量:%d", EdgeVec.size());

	int nStartIndex = 0;
	Point NearStartPoint;
	if (!FindVertex_By_Point(CurPoint, nStartIndex, NearStartPoint))
	{
		LOG_MSG_CF( L"找不到离起点最近的坐标!");
		return FALSE;
	}

	int nEndIndex = 0;
	Point NearEndPoint;
	if (!FindVertex_By_Point(_TarPoint, nEndIndex, NearEndPoint))
	{
		LOG_MSG_CF( L"找不到离终点最近的坐标!");
		return FALSE;
	}

	if (!FindPath(EdgeVec, LineVec.at(nStartIndex), LineVec.at(nEndIndex), NearStartPoint, NearEndPoint))
		return FALSE;

	for (int i = static_cast<int>(vPoint.size()) - 1; i >= 0; --i)
	{
		fnMove(vPoint.at(i));
	}

	return TRUE;
}

VOID CFindPathScript::ClearGameScript()
{
	LineVec.clear();
}

VOID CFindPathScript::SetDeadPoint(_In_ CONST Point& DeadPoint)
{
	_TarPoint = DeadPoint;
	bExistDeadPoint = TRUE;
}

VOID CFindPathScript::ClearDeadPoint()
{
	bExistDeadPoint = FALSE;
}

BOOL CFindPathScript::ExistDeadPoint() CONST
{
	return bExistDeadPoint;
}

float CFindPathScript::CalcDis_By_List(_In_ CONST Line& Line_, _In_ CONST Vertex& Vertex_) CONST
{
	return Line_.vList.size() == 0 ? 0 : (Line_.fDis + CBaseObject::Get3DDis(Vertex_.VertexPoint, Line_.vList.back().VertexPoint));
}

BOOL CFindPathScript::VertexToEdge(std::vector<Edge>& vEdge)
{
	Edge Edge_;
	for (UINT i = 0; i < LineVec.size(); ++i)
	{
		for (UINT j = i + 1; j < LineVec.size(); ++j)
		{
			if (Coincide_In_Line(LineVec.at(i),LineVec.at(j), &Edge_.Line1Link, &Edge_.Line2Link))
			{
				Edge_.Init(&LineVec.at(i), &LineVec.at(j));
				vEdge.push_back(Edge_);
				Edge_.Release();
			}
		}
	}
	return TRUE;
}

BOOL CFindPathScript::Coincide_In_Line(Line& Line1, Line& Line2, _Out_ Vertex* pLine1Link, _Out_ Vertex* pLine2Link) CONST
{
	BOOL bRetCode = FALSE;
	float fMinDis = MAX_FINDPATH_DIS;
	for (auto& v1 : Line1.vList)
	{
		for (auto& v2 : Line2.vList)
		{
			float fDis = CBaseObject::Get3DDis(v1.VertexPoint, v2.VertexPoint);
			if (fDis <= fMinDis)
			{
				*pLine1Link = v1;
				*pLine2Link = v2;

				fMinDis = fDis;
				bRetCode = TRUE;
			}
		}
	}
	return bRetCode;
}

BOOL CFindPathScript::FindVertex_By_Point(_In_ CONST Point& CurPoint, _Out_opt_ int& nIndex, _Out_opt_ Point& RetPoint) CONST
{
	nIndex = -1;
	float fMinDis = 9999999.0f;
	for (UINT i = 0; i < LineVec.size(); ++i)
	{
		for (auto& itm : LineVec.at(i).vList)
		{
			float fDis = CBaseObject::Get3DDis(CurPoint, itm.VertexPoint);
			if (fDis < fMinDis)
			{
				RetPoint = itm.VertexPoint;
				fMinDis = fDis;
				nIndex = i;
			}
		}
	}

	return nIndex != -1;
}

BOOL CFindPathScript::FindPath(_In_ std::vector<Edge>& vEdge, _In_ Line& StartLine, _In_ Line& EndLine, _In_ Point& StartPoint, _In_ Point& EndPoint)
{
	// 搜索列表
	std::vector<Edge*> SearchList;
	Edge* pEdgeRoot = nullptr;
	auto ulTick = ::GetTickCount64();
	FindLine(vEdge, &StartLine, SearchList);
	BOOL bFind = FALSE;
	while (SearchList.size() > 0)
	{
		// 启动A*的 估值函数, 估值最短的排前面
		int nIndex = GetIndex_By_Heuristic_In_SearchList(SearchList, EndPoint);
		if (nIndex == -1)
			break;

		Edge* pEdge = SearchList.at(nIndex);
		if (pEdge->pLine1->nLineId == EndLine.nLineId || pEdge->pLine2->nLineId == EndLine.nLineId)
		{
			pEdge->Root = pEdgeRoot; // 保存回溯路径
			pEdgeRoot = pEdge;
			bFind = TRUE;
			break;
		}

		SearchList.erase(SearchList.begin() + nIndex);
		pEdge->Root = pEdgeRoot; // 保存回溯路径
		pEdgeRoot = pEdge;
		FindLine(vEdge, pEdge->pLine1, SearchList);
		FindLine(vEdge, pEdge->pLine2, SearchList);
	}

	if (!bFind)
	{
		LOG_CF_D( L"花费时间:%dms", static_cast<DWORD>(::GetTickCount64() - ulTick));
		LOG_CF_E( L"无法找到路径!");
		return FALSE;
	}

	Line* pLastLine = &EndLine;
	Vertex TarPoint = EndPoint;
	Vertex* pLink = nullptr;
	Line* pNowLine = nullptr;
	while (pEdgeRoot != nullptr)
	{
		if (pEdgeRoot->pLine1->nLineId == pLastLine->nLineId)
		{
			pLink = &pEdgeRoot->Line1Link;
			pNowLine = pEdgeRoot->pLine1;
			pLastLine = pEdgeRoot->pLine2;
		}
		else
		{
			pLink = &pEdgeRoot->Line2Link;
			pNowLine = pEdgeRoot->pLine2;
			pLastLine = pEdgeRoot->pLine1;
		}
		//cout << "ID:" << pNowLine->nLineId << endl;
		PrintLine(pNowLine, TarPoint, *pLink);
		pEdgeRoot = pEdgeRoot->Root;
		TarPoint = *pLink;
	}

	PrintLine(pLastLine, *pLink, Vertex(StartPoint));

	LOG_CF_D( L"寻路完毕!, 花费时间:%ds", static_cast<DWORD>(::GetTickCount64() - ulTick) / 1000);
	//wcout << L"花费时间:" << dwEndTick - dwStartTick << L"ms" << endl;
	return TRUE;
}

VOID CFindPathScript::FindLine(_In_ std::vector<Edge>& vEdge, _In_ CONST Line* pCurLine, _Out_ std::vector<Edge*>& SearchList)
{
	for (auto& Edge_ : vEdge)
	{
		if (Edge_.pLine1 == pCurLine && !Edge_.bVisited)
		{
			Edge_.bVisited = true;
			SearchList.push_back(&Edge_);
		}
		else if (Edge_.pLine2 == pCurLine && !Edge_.bVisited)
		{
			Edge_.bVisited = true;
			SearchList.push_back(&Edge_);
		}
	}
}

int CFindPathScript::GetIndex_By_Heuristic_In_SearchList(_In_ std::vector<Edge*>& SearchList, _In_ CONST Point& EndPoint)
{
	// 先按照离上一个点的距离排序, 查询查询排名前面的那2个点, 哪个近!
	for (int i = 0; i < (int)SearchList.size(); ++i)
	{
		auto& itm = SearchList.at(i);
		itm->SetWeight(EndPoint);
	}
	std::sort(SearchList.begin(), SearchList.end(), [](CONST Edge* pEdge1, CONST Edge* pEdge2){return pEdge1->nWeight < pEdge2->nWeight;});
	return 0;
}

BOOL CFindPathScript::PrintLine(Line* pNowLine, CONST Vertex& StartVertex, CONST Vertex& EndVertex)
{
	// 搜索'当前'线的结合点, 从TarPoint -> pNowLine.TarPoint, 要判断倒序还是顺序!( 判断方向的方法是-> 判断TarPoint 在pNowLine的List的结合点的前面 还是 后面 )
	LineDir LineDir_;
	if (!GetDir_By_Line(pNowLine, StartVertex, EndVertex, LineDir_)) // Err
	{
		LOG_MSG_CF( L"Err GetDir_By_Line");
		return FALSE;
	}

	if (LineDir_.GetDir() == em_Dir_Order)
	{
		for (int i = LineDir_.nNowVertexIndex; i <= LineDir_.nTarVertexIndex; ++i)
		{
			auto Pt = pNowLine->vList.at(i).VertexPoint;
			LOG_CF_D( L"dis=%.2f,Point:[%d,%d,%d]", CBaseObject::GetPersonDis(Pt), Pt.GetX(), Pt.GetY(), Pt.GetZ());
			vPoint.push_back(pNowLine->vList.at(i).VertexPoint);
		}
	}
	else
	{
		for (int i = LineDir_.nNowVertexIndex; i >= LineDir_.nTarVertexIndex; --i)
		{
			auto Pt = pNowLine->vList.at(i).VertexPoint;
			LOG_CF_D( L"dis=%.2f,Point:[%d,%d,%d]", CBaseObject::GetPersonDis(Pt), Pt.GetX(), Pt.GetY(), Pt.GetZ());
			vPoint.push_back(pNowLine->vList.at(i).VertexPoint);
		}
	}

	//cout << "---------------" << endl;
	return TRUE;
}

BOOL CFindPathScript::GetDir_By_Line(_In_ CONST Line* pLine, _In_ CONST Vertex& NowVertex, _In_ CONST Vertex& TarVertex, _Out_ LineDir& LineDir_)
{
	LineDir_.nNowVertexIndex = GetIndex_By_Line(pLine, NowVertex);
	LineDir_.nTarVertexIndex = GetIndex_By_Line(pLine, TarVertex);
	return LineDir_.nNowVertexIndex != -1 && LineDir_.nTarVertexIndex != -1;
}

int CFindPathScript::GetIndex_By_Line(_In_ CONST Line* pLine, _In_ CONST Vertex& Vertex_) CONST
{
	float fMinDis = MAX_FINDPATH_DIS;
	int nIndex = -1;
	for (int i = 0; i < (int)pLine->vList.size(); ++i)
	{
		float fDis = CBaseObject::Get3DDis(Vertex_.VertexPoint, pLine->vList.at(i).VertexPoint);
		if (fDis < fMinDis)
		{
			fMinDis = fDis;
			nIndex = i;
		}
	}
	return nIndex;
}


