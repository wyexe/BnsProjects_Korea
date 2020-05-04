#include "stdafx.h"
#include "SortWindowService.h"
#include <thread>
#include <MyTools/Log.h>
#include "ConsoleVariable.h"

#define _SELF L"SortWindowService.cpp"
VOID CSortWindowService::Run()
{
	if (!_Run)
	{
		_Run = true;
		_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

		std::thread t([this] {SortBnsWindow(); });
		t.detach();
	}
}

VOID CSortWindowService::Stop()
{
	_Run = false;
	if (_hEvent != NULL)
	{
		LOG_CF_D(L"Wait [CSortWindowService] to die~");
		::WaitForSingleObject(_hEvent, INFINITE);
		LOG_CF_D(L"[CSortWindowService] is die!");
		::CloseHandle(_hEvent);
		_hEvent = NULL;
	}
}

VOID CSortWindowService::SetSortFlag(_In_ int nFlag)
{
	_nFlag = nFlag;
}

VOID CSortWindowService::SetPrintSortBnsWin(_In_ bool bValue)
{
	_bPrintSortBnsWin = bValue;
}

VOID CSortWindowService::SortBnsWindow()
{
	while(_Run)
	{
		if (_nFlag == 0)
		{
			::Sleep(1000);
			continue;
		}

		auto nIndex = 0;
		for (auto i = 0; i < MAX_GAME_COUNT && _Run; ++i)
		{
			auto pAccountGame = &g_pSharedInfo->arrGameInfo[i];
			if (!pAccountGame->AccountStatus.bExist)
				continue;
			if (pAccountGame->MyAcount_Info.bLogin)
				continue;
			if (pAccountGame->dwPid == NULL || pAccountGame->hGameWnd == NULL)
				continue;


			pAccountGame->MyAcount_Info.dwSortIndex = nIndex;
			auto CalRect = CalcBnsWindowRect_By_Index(nIndex++);

			RECT CurWinRect;
			::GetWindowRect(pAccountGame->hGameWnd, &CurWinRect);

			CON_LOG_CF_D(_bPrintSortBnsWin, L"窗口[%s] 计算的坐标:[%d,%d,%d,%d], 当前坐标:[%d,%d,%d,%d]",
				pAccountGame->MyAcount_Info.szUserName,
				CalRect.top, CalRect.right, CalRect.bottom, CalRect.left,
				CurWinRect.top, CurWinRect.right, CurWinRect.bottom, CurWinRect.left);

			if (CurWinRect.left == CalRect.left && CurWinRect.right == CalRect.right && CurWinRect.bottom == CalRect.bottom && CurWinRect.top == CalRect.top)
				continue;

			CON_LOG_CF_D(_bPrintSortBnsWin, L"正在排序窗口[%s]", pAccountGame->MyAcount_Info.szUserName);

			// 激活窗口
			::SwitchToThisWindow(pAccountGame->hGameWnd, TRUE);
			::Sleep(1000);

			// 置顶窗口
			::SetWindowPos(pAccountGame->hGameWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			::Sleep(1000);

			// 修改窗口大小
			::SetWindowPos(pAccountGame->hGameWnd, HWND_NOTOPMOST, 0, 0, static_cast<int>(CalRect.right - CalRect.left), static_cast<int>(CalRect.bottom - CalRect.top), SWP_NOMOVE);
			::Sleep(1000);

			// 设置窗口位置
			::MoveWindow(pAccountGame->hGameWnd, static_cast<int>(CalRect.left), static_cast<int>(CalRect.top), static_cast<int>(CalRect.right - CalRect.left), static_cast<int>(CalRect.bottom - CalRect.top), FALSE);
			::Sleep(1000);
		}
		::Sleep(5 * 1000);
	}
	::SetEvent(_hEvent);
}

RECT CSortWindowService::CalcBnsWindowRect_By_Index(_In_ int nIndex) CONST
{
	// 屏幕分辨率， 已经去掉任务栏
	auto nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	auto nHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	if (_nFlag == 1)
	{
		auto nRow = nIndex / 4;
		auto nColumn = nIndex % 4;

		RECT r;
		r.left = nColumn * (nWidth / 4);  // 一行分4个
		r.top = nRow * (nHeight / 3);     // 然而一列只能放2个  

		r.right = r.left + (nWidth / 4);
		r.bottom = r.top + (nHeight / 3);
		return r;
	}
	else if (_nFlag == 4)
	{
		auto nRow = nIndex / 4;
		auto nColumn = nIndex % 4;

		RECT r;
		r.left = nColumn * (nWidth / 4);  // 一行分4个
		r.top = nRow * (nHeight / 4);     // 

		r.right = r.left + (nWidth / 4);
		r.bottom = r.top + (nHeight / 4);
		return r;
	}
	else if (_nFlag == 15)
	{
		auto nRow = nIndex / 5;
		auto nColumn = nIndex % 5;

		RECT r;
		r.left = nColumn * (nWidth / 5);  // 一行分4个
		r.top = nRow * (nHeight / 3);     // 

		r.right = r.left + (nWidth / 5);
		r.bottom = r.top + (nHeight / 3);
		return r;
	}
	else if (_nFlag == 20)
	{
		auto nRow = nIndex / 5;
		auto nColumn = nIndex % 5;

		RECT r;
		r.left = nColumn * (nWidth / 5);  // 一行分4个
		r.top = nRow * (nHeight / 4);     // 

		r.right = r.left + (nWidth / 5);
		r.bottom = r.top + (nHeight / 4);
		return r;
	}
	else if (_nFlag == 6)
	{
		auto nRow = nIndex / 6;
		auto nColumn = nIndex % 6;

		RECT r;
		r.left = nColumn * 315;
		r.top = nRow * 250;
		r.right = r.left + 315;
		r.bottom = r.top + 250;
		return r;
	}
	else if (_nFlag == 8)
	{
		auto nRow = nIndex / 8;
		auto nColumn = nIndex % 8;

		RECT r;
		r.left = nColumn * 240;
		r.top = nRow * 310;
		r.right = r.left + 240;
		r.bottom = r.top + 310;
		return r;
	}
	else if (_nFlag == 7)
	{
		auto nRow = nIndex / 7;
		auto nColumn = nIndex % 7;

		RECT r;
		r.left = nColumn * 272;
		r.top = nRow * 200;
		r.right = r.left + 272;
		r.bottom = r.top + 200;
		return r;
	}
	else
	{
		LOG_MSG_CF(L"错误的窗口排序类型:%X", _nFlag);
	}
	return RECT();
}

