#include "stdafx.h"
#include "ConsoleVariable.h"

PSHARED_INFO g_pSharedInfo = NULL;
BOOL		 g_bLockShareInfo = FALSE;
BOOL		 g_bRunConsoleDLL = TRUE;