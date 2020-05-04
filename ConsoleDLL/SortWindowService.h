#ifndef __GITBNSPROJECTS_CONSOLEDLL_SERVICE_SORT_SORTWINDOWSERVICE_H__
#define __GITBNSPROJECTS_CONSOLEDLL_SERVICE_SORT_SORTWINDOWSERVICE_H__

#include <atomic>
#include <MyTools/RelfexClass.h>

class CSortWindowService : public MyTools::CRelfexBaseClass
{
public:
	CSortWindowService() = default;
	~CSortWindowService() = default;
	
	VOID Run();
	
	VOID Stop();

	VOID SetSortFlag(_In_ int nFlag);

	VOID SetPrintSortBnsWin(_In_ bool bValue);
private:
	//
	VOID SortBnsWindow();

	//
	RECT CalcBnsWindowRect_By_Index(_In_ int nIndex) CONST;

private:
	std::atomic<bool>   _Run    = false;
	HANDLE _hEvent = NULL;
	int    _nFlag  = 0;
	std::atomic<bool> _bPrintSortBnsWin = false;
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CSortWindowService*>(lpObjectAddr);
	}
	
	CSortWindowService(CONST CSortWindowService&) = delete;
	
	CSortWindowService& operator=(CONST CSortWindowService&) = delete;
};


#endif // !__GITBNSPROJECTS_CONSOLEDLL_SERVICE_SORTWINDOW_SORTWINDOWSERVICE_H__
