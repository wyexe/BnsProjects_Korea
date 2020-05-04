#include "stdafx.h"
#include "Cat.h"

CCat::CCat()
{
}

CCat::CCat(_In_ DWORD dwNodeBase_) : CPlayer(dwNodeBase_)
{

}

CCat::~CCat()
{
}

DWORD CCat::GetMasterId() CONST
{
	return GetId() & 0xFFFF;
}
