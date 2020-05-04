#ifndef __GITBNSPROJECTS_BNSKORSERVER_OTP_CLHMACSHA1_H__
#define __GITBNSPROJECTS_BNSKORSERVER_OTP_CLHMACSHA1_H__

#include <MyTools/ClassInstance.h>

class CLHmacSha1 : public CClassInstance<CLHmacSha1>
{
public:
	CLHmacSha1();
	~CLHmacSha1();

	BOOL CalcOTPValue(_In_ CONST std::wstring& wsKey, _Out_ std::wstring& wsValue) CONST;

	BOOL ComputerHash(_In_ CONST CHAR* szEncodeText, _In_ UINT uTextLen, _In_ CONST CHAR* szEncodeKey, _In_ UINT uKeyLen, _Out_ CHAR* ResultBuffer) CONST;
private:
#define SHA1_DIGEST_SIZE 20
	struct Sha1_ctx
	{
		uint32_t A = 0x67452301;
		uint32_t B = 0xEFCDAB89;
		uint32_t C = 0x98BADCFE;
		uint32_t D = 0x10325476;
		uint32_t E = 0xC3D2E1F0;

		uint32_t Total[2];
		uint32_t BufferLen = 0;
		uint32_t Buffer[32];

		Sha1_ctx()
		{
			ZeroMemory(Total, sizeof(Total));
			ZeroMemory(Buffer, sizeof(Buffer));
		}
	};

# define SWAP(n) (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))
#define K1 0x5a827999
#define K2 0x6ed9eba1
#define K3 0x8f1bbcdc
#define K4 0xca62c1d6
#define F1(B,C,D) ( D ^ ( B & ( C ^ D ) ) )
#define F2(B,C,D) (B ^ C ^ D)
#define F3(B,C,D) ( ( B & C ) | ( D & ( B | C ) ) )
#define F4(B,C,D) (B ^ C ^ D)
#define IPAD 0x36
#define OPAD 0x5C

private:
	VOID Hash1_Block(_In_ CONST CHAR* szEcodeKey, _In_ UINT uKeyLen, _Out_ Sha1_ctx& Sha1Ctx) CONST;

	VOID Hash1_Bytes(_In_ CONST CHAR* szEcodeText, _In_ UINT uTextLen, _Out_ Sha1_ctx& Sha1Ctx) CONST;

	VOID Hash1_Finish(_In_ Sha1_ctx& Sha1Ctx, _Out_ CHAR* ResultBuffer) CONST;

	VOID SetValue(_In_ CHAR* Buffer, uint32_t Value) CONST;

	LPVOID MemXor(_Out_ LPVOID Dest, _In_ LPCVOID Src, _In_ UINT uSize) CONST;

	std::shared_ptr<BYTE> EncodeKey(_In_ CONST std::string& strKey, _Out_ UINT& uBufferLen) CONST;

	VOID InitCharMap();

	int NumberOfTrailingZeros(int i) CONST;

	LONGLONG GetOTPState() CONST _NOEXCEPT;
private:
	std::map<BYTE, int> _CharMap;
	int _MASK;
	int _SHIFT;
};

#endif // !__GITBNSPROJECTS_BNSKORSERVER_OTP_CLHMACSHA1_H__
