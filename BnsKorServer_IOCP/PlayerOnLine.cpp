#include "PlayerOnLine.h"

VOID CPlayerOnLine::SetKeepALiveContent(_In_ CONST std::wstring& wsPlayerName, _In_ CONST KeepALiveContent& Content)
{
	std::lock_guard<std::mutex> LockMapPlayerOnLineContent(_MtxPlayerOnLineContent);

	auto itr = _MapPlayer.find(wsPlayerName);
	if (itr != _MapPlayer.end())
	{
		itr->second.Gold = Content.Gold;
		itr->second.Level = Content.Level;
		if (Content.dwTaskId != NULL)
		{
			itr->second.dwTaskId = Content.dwTaskId;
			itr->second.dwScheduleId = Content.dwScheduleId;
		}
	}
	else
		_MapPlayer.insert(std::make_pair(wsPlayerName, Content));
}

VOID CPlayerOnLine::SetKeepALiveContent(_In_ CONST std::wstring& wsPlayerName, _In_ CONST std::wstring& wsContent)
{
	std::lock_guard<std::mutex> LockMapPlayerOnLineContent(_MtxPlayerOnLineContent);

	auto itr = _MapPlayer.find(wsPlayerName);
	if (itr != _MapPlayer.end())
		itr->second.wsContent = wsContent;
	else
	{
		KeepALiveContent Content;
		Content.wsContent = wsContent;
		_MapPlayer.insert(std::make_pair(wsPlayerName, std::move(Content)));
	}
}

BOOL CPlayerOnLine::GetKeepALiveContent(_In_ CONST std::wstring& wsPlayerName, _Out_ KeepALiveContent& Content)
{
	std::lock_guard<std::mutex> LockMapPlayerOnLineContent(_MtxPlayerOnLineContent);

	auto itr = _MapPlayer.find(wsPlayerName);
	if (itr == _MapPlayer.end())
		return FALSE;


	Content = itr->second;
	return TRUE;
}
