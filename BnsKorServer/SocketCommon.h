#ifndef __MYTOOLS_SOCKET_SOCKETCOMMON_H__
#define __MYTOOLS_SOCKET_SOCKETCOMMON_H__

#include <MyTools/ToolsPublic.h>
#include <queue>
#include <MyTools/ByteBuffer.h>
#include <MyTools/CLLock.h>
#include <MyTools/ClassInstance.h>
#pragma  comment (lib,"wsock32.lib") 


typedef struct _SocketByteBuffer
{
	DWORD dwMsg;
	ByteBuffer ByteBuffer_;
}SocketByteBuffer;

#define SOCKET_MESSAGE_ERROR		0x0
#define SOCKET_MESSAGE_MESSAGEBOX	0x1


#define MAX_SOCKET_PACKET_SIZE		3096

class CSocketCommon
{
public:
	CSocketCommon();
	~CSocketCommon();

private:

};



#endif