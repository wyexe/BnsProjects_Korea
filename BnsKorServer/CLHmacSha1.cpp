#include "CLHmacSha1.h"
#include <time.h>
#include <cmath>
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include "TotpCounter.h"

#define _SELF L"CLHmacSha1.cpp"
CLHmacSha1::CLHmacSha1()
{
	InitCharMap();
}

CLHmacSha1::~CLHmacSha1()
{

}

BOOL CLHmacSha1::CalcOTPValue(_In_ CONST std::wstring& wsKey, _Out_ std::wstring& wsValue) CONST
{
	UINT uKeyBufferLen = 0;
	auto KeyBuffer = EncodeKey(CCharacter::UnicodeToASCII(wsKey), uKeyBufferLen);
	auto OtpState = GetOTPState();

	BYTE CounterBuffer[sizeof(OtpState)] = { 0 };
	for (decltype(OtpState) i = 0; i < sizeof(OtpState); ++i)
		CounterBuffer[sizeof(OtpState) - i - 1] = static_cast<BYTE>(OtpState >> (i * 8));

	BYTE ResultBuffer[20] = { 0 };
	ComputerHash(reinterpret_cast<CONST CHAR*>(CounterBuffer), sizeof(OtpState), reinterpret_cast<CONST CHAR*>(KeyBuffer.get()), uKeyBufferLen, reinterpret_cast<CHAR*>(ResultBuffer));

	int nOffset = ResultBuffer[20 - 1] & 0xF;
	int nBinary = ((ResultBuffer[nOffset] & 0x7f) << 24) | (ResultBuffer[nOffset + 1] << 16) | (ResultBuffer[nOffset + 2] << 8) | (ResultBuffer[nOffset + 3]);
	int nPass = nBinary % static_cast<int>(pow(10, 6));

	wsValue = std::to_wstring(nPass);
	return TRUE;
}

BOOL CLHmacSha1::ComputerHash(_In_ CONST CHAR* szEncodeText, _In_ UINT uTextLen, _In_ CONST CHAR* szEncodeKey, _In_ UINT uKeyLen, _Out_ CHAR* ResultBuffer) CONST
{
	Sha1_ctx Inner;
	Sha1_ctx Outer;
	char szOptKeyBuffer[20] = { 0 };
	char szBlock[64] = { 0 };
	char InnerHash[20] = { 0 };

	if (uKeyLen > 64)
	{
		Sha1_ctx KeyHash;
		Hash1_Bytes(szEncodeKey, uKeyLen, KeyHash);
		Hash1_Finish(KeyHash, szOptKeyBuffer);

		szEncodeKey = szOptKeyBuffer;
		uKeyLen = 20;
	}

	memset(szBlock, IPAD, sizeof(szBlock));
	MemXor(szBlock, szEncodeKey, uKeyLen);

	Hash1_Block(szBlock, 64, Inner);
	Hash1_Bytes(szEncodeText, uTextLen, Inner);
	Hash1_Finish(Inner, InnerHash);



	memset(szBlock, OPAD, sizeof(szBlock));
	MemXor(szBlock, szEncodeKey, uKeyLen);

	Hash1_Block(szBlock, 64, Outer);
	Hash1_Bytes(InnerHash, 20, Outer);
	Hash1_Finish(Outer, ResultBuffer);

	return TRUE;

}

VOID CLHmacSha1::Hash1_Block(_In_ CONST CHAR* szEcodeKey, _In_ UINT uKeyLen, _Out_ Sha1_ctx& Sha1Ctx) CONST
{
	const uint32_t *words = (const uint32_t*)szEcodeKey;
	size_t nwords = uKeyLen / sizeof(uint32_t);
	const uint32_t *endp = words + nwords;
	uint32_t x[16];
	uint32_t a = Sha1Ctx.A;
	uint32_t b = Sha1Ctx.B;
	uint32_t c = Sha1Ctx.C;
	uint32_t d = Sha1Ctx.D;
	uint32_t e = Sha1Ctx.E;

	Sha1Ctx.Total[0] += uKeyLen;
	if (Sha1Ctx.Total[0] < uKeyLen)
		++Sha1Ctx.Total[1];

#define rol(x, n) (((x) << (n)) | ((uint32_t) (x) >> (32 - (n))))

#define M(I) ( tm =   x[I&0x0f] ^ x[(I-14)&0x0f] \
		    ^ x[(I-8)&0x0f] ^ x[(I-3)&0x0f] \
	       , (x[I&0x0f] = rol(tm, 1)) )

#define R(A,B,C,D,E,F,K,M)   { E += rol( A, 5 )     \
				      + F( B, C, D )  \
				      + K	      \
				      + M;	      \
				 B = rol( B, 30 );    \
					       }

	while (words < endp)
	{
		uint32_t tm;
		int t;
		for (t = 0; t < 16; t++)
		{
			x[t] = SWAP(*words);
			words++;
		}

		R(a, b, c, d, e, F1, K1, x[0]);
		R(e, a, b, c, d, F1, K1, x[1]);
		R(d, e, a, b, c, F1, K1, x[2]);
		R(c, d, e, a, b, F1, K1, x[3]);
		R(b, c, d, e, a, F1, K1, x[4]);
		R(a, b, c, d, e, F1, K1, x[5]);
		R(e, a, b, c, d, F1, K1, x[6]);
		R(d, e, a, b, c, F1, K1, x[7]);
		R(c, d, e, a, b, F1, K1, x[8]);
		R(b, c, d, e, a, F1, K1, x[9]);
		R(a, b, c, d, e, F1, K1, x[10]);
		R(e, a, b, c, d, F1, K1, x[11]);
		R(d, e, a, b, c, F1, K1, x[12]);
		R(c, d, e, a, b, F1, K1, x[13]);
		R(b, c, d, e, a, F1, K1, x[14]);
		R(a, b, c, d, e, F1, K1, x[15]);
		R(e, a, b, c, d, F1, K1, M(16));
		R(d, e, a, b, c, F1, K1, M(17));
		R(c, d, e, a, b, F1, K1, M(18));
		R(b, c, d, e, a, F1, K1, M(19));
		R(a, b, c, d, e, F2, K2, M(20));
		R(e, a, b, c, d, F2, K2, M(21));
		R(d, e, a, b, c, F2, K2, M(22));
		R(c, d, e, a, b, F2, K2, M(23));
		R(b, c, d, e, a, F2, K2, M(24));
		R(a, b, c, d, e, F2, K2, M(25));
		R(e, a, b, c, d, F2, K2, M(26));
		R(d, e, a, b, c, F2, K2, M(27));
		R(c, d, e, a, b, F2, K2, M(28));
		R(b, c, d, e, a, F2, K2, M(29));
		R(a, b, c, d, e, F2, K2, M(30));
		R(e, a, b, c, d, F2, K2, M(31));
		R(d, e, a, b, c, F2, K2, M(32));
		R(c, d, e, a, b, F2, K2, M(33));
		R(b, c, d, e, a, F2, K2, M(34));
		R(a, b, c, d, e, F2, K2, M(35));
		R(e, a, b, c, d, F2, K2, M(36));
		R(d, e, a, b, c, F2, K2, M(37));
		R(c, d, e, a, b, F2, K2, M(38));
		R(b, c, d, e, a, F2, K2, M(39));
		R(a, b, c, d, e, F3, K3, M(40));
		R(e, a, b, c, d, F3, K3, M(41));
		R(d, e, a, b, c, F3, K3, M(42));
		R(c, d, e, a, b, F3, K3, M(43));
		R(b, c, d, e, a, F3, K3, M(44));
		R(a, b, c, d, e, F3, K3, M(45));
		R(e, a, b, c, d, F3, K3, M(46));
		R(d, e, a, b, c, F3, K3, M(47));
		R(c, d, e, a, b, F3, K3, M(48));
		R(b, c, d, e, a, F3, K3, M(49));
		R(a, b, c, d, e, F3, K3, M(50));
		R(e, a, b, c, d, F3, K3, M(51));
		R(d, e, a, b, c, F3, K3, M(52));
		R(c, d, e, a, b, F3, K3, M(53));
		R(b, c, d, e, a, F3, K3, M(54));
		R(a, b, c, d, e, F3, K3, M(55));
		R(e, a, b, c, d, F3, K3, M(56));
		R(d, e, a, b, c, F3, K3, M(57));
		R(c, d, e, a, b, F3, K3, M(58));
		R(b, c, d, e, a, F3, K3, M(59));
		R(a, b, c, d, e, F4, K4, M(60));
		R(e, a, b, c, d, F4, K4, M(61));
		R(d, e, a, b, c, F4, K4, M(62));
		R(c, d, e, a, b, F4, K4, M(63));
		R(b, c, d, e, a, F4, K4, M(64));
		R(a, b, c, d, e, F4, K4, M(65));
		R(e, a, b, c, d, F4, K4, M(66));
		R(d, e, a, b, c, F4, K4, M(67));
		R(c, d, e, a, b, F4, K4, M(68));
		R(b, c, d, e, a, F4, K4, M(69));
		R(a, b, c, d, e, F4, K4, M(70));
		R(e, a, b, c, d, F4, K4, M(71));
		R(d, e, a, b, c, F4, K4, M(72));
		R(c, d, e, a, b, F4, K4, M(73));
		R(b, c, d, e, a, F4, K4, M(74));
		R(a, b, c, d, e, F4, K4, M(75));
		R(e, a, b, c, d, F4, K4, M(76));
		R(d, e, a, b, c, F4, K4, M(77));
		R(c, d, e, a, b, F4, K4, M(78));
		R(b, c, d, e, a, F4, K4, M(79));

		a = Sha1Ctx.A += a;
		b = Sha1Ctx.B += b;
		c = Sha1Ctx.C += c;
		d = Sha1Ctx.D += d;
		e = Sha1Ctx.E += e;
	}
}

VOID CLHmacSha1::Hash1_Bytes(_In_ CONST CHAR* szEcodeText, _In_ UINT uTextLen, _Out_ Sha1_ctx& Sha1Ctx) CONST
{
	if (Sha1Ctx.BufferLen != 0)
	{
		size_t left_over = Sha1Ctx.BufferLen;
		size_t add = 128 - left_over > uTextLen ? uTextLen : 128 - left_over;

		memcpy(&((char *)Sha1Ctx.Buffer)[left_over], szEcodeText, add);
		Sha1Ctx.BufferLen += add;

		if (Sha1Ctx.BufferLen > 64)
		{
			Hash1_Block(reinterpret_cast<CONST CHAR*>(Sha1Ctx.Buffer), Sha1Ctx.BufferLen & ~63, Sha1Ctx);

			Sha1Ctx.BufferLen &= 63;
			/* The regions in the following copy operation cannot overlap.  */
			memcpy(Sha1Ctx.Buffer, &(reinterpret_cast<char *>(Sha1Ctx.Buffer))[(left_over + add) & ~63], Sha1Ctx.BufferLen);
		}

		szEcodeText += add;
		uTextLen -= add;
	}

	/* Process available complete blocks.  */
	if (uTextLen >= 64)
	{
#if !_STRING_ARCH_unaligned

//# define alignof(type) offsetof (struct { char c; type x; }, x)
# define UNALIGNED_P(p) (((size_t) p) % sizeof(uint32_t) != 0)
		if (UNALIGNED_P(szEcodeText))
		{
			while (uTextLen > 64)
			{
				memcpy(reinterpret_cast<LPVOID>(Sha1Ctx.Buffer), reinterpret_cast<LPCVOID>(szEcodeText), 64);
				Hash1_Block(reinterpret_cast<CONST CHAR*>(Sha1Ctx.Buffer), 64, Sha1Ctx);
				szEcodeText += 64;
				uTextLen -= 64;
			}
		}
		else
#endif
		{
			Hash1_Block(szEcodeText, uTextLen & ~63, Sha1Ctx);
			szEcodeText += (uTextLen & ~63);
			uTextLen &= 63;
		}
	}
	 
	/* Move remaining bytes in internal buffer.  */
	if (uTextLen > 0)
	{
		size_t left_over = Sha1Ctx.BufferLen;

		memcpy(&((char *)Sha1Ctx.Buffer)[left_over], szEcodeText, uTextLen);
		left_over += uTextLen;
		if (left_over >= 64)
		{
			Hash1_Block(reinterpret_cast<CONST CHAR*>(Sha1Ctx.Buffer), 64, Sha1Ctx);
			left_over -= 64;
			memcpy(Sha1Ctx.Buffer, &Sha1Ctx.Buffer[16], left_over);
		}
		Sha1Ctx.BufferLen = left_over;
	}
}

VOID CLHmacSha1::Hash1_Finish(_In_ Sha1_ctx& Sha1Ctx, _Out_ CHAR* ResultBuffer) CONST
{
	uint32_t bytes = Sha1Ctx.BufferLen;
	size_t size = (bytes < 56) ? 64 / 4 : 64 * 2 / 4;

	/* Now count remaining bytes.  */
	Sha1Ctx.Total[0] += bytes;
	if (Sha1Ctx.Total[0] < bytes)
		++Sha1Ctx.Total[1];

	/* Put the 64-bit file length in *bits* at the end of the buffer.  */
	Sha1Ctx.Buffer[size - 2] = SWAP((Sha1Ctx.Total[1] << 3) | (Sha1Ctx.Total[0] >> 29));
	Sha1Ctx.Buffer[size - 1] = SWAP(Sha1Ctx.Total[0] << 3);

	static const unsigned char FillBufer[64] = { 0x80, 0 /* , 0, 0, ...  */ };
	memcpy(&((char *)Sha1Ctx.Buffer)[bytes], FillBufer, (size - 2) * 4 - bytes);

	Hash1_Block(reinterpret_cast<CHAR*>(Sha1Ctx.Buffer), size * 4, Sha1Ctx);

	SetValue(ResultBuffer + 0 * sizeof(Sha1Ctx.A), SWAP(Sha1Ctx.A));
	SetValue(ResultBuffer + 1 * sizeof(Sha1Ctx.B), SWAP(Sha1Ctx.B));
	SetValue(ResultBuffer + 2 * sizeof(Sha1Ctx.C), SWAP(Sha1Ctx.C));
	SetValue(ResultBuffer + 3 * sizeof(Sha1Ctx.D), SWAP(Sha1Ctx.D));
	SetValue(ResultBuffer + 4 * sizeof(Sha1Ctx.E), SWAP(Sha1Ctx.E));
}

VOID CLHmacSha1::SetValue(_In_ CHAR* Buffer, uint32_t Value) CONST
{
	memcpy(Buffer, &Value, sizeof(Value));
}

LPVOID CLHmacSha1::MemXor(_Out_ LPVOID Dest, _In_ LPCVOID Src, _In_ UINT uSize) CONST
{
	CONST CHAR *s = (char const*)Src;
	char *d = (char*)Dest;

	for (; uSize > 0; uSize--)
		*d++ ^= *s++;

	return Dest;
}

std::shared_ptr<BYTE> CLHmacSha1::EncodeKey(_In_ CONST std::string& strKey, _Out_ UINT& uBufferLen) CONST
{
	int outLength = strKey.length() * _SHIFT / 8;

	std::shared_ptr<BYTE> result(new BYTE[outLength], [](BYTE* p) { delete[] p; });

	int buffer = 0;
	int next = 0;
	int bitsleft = 0;
	for (UINT i = 0; i < strKey.length(); ++i)
	{
		BYTE c = static_cast<BYTE>(strKey[i]);
		auto itr = _CharMap.find(c);
		if (itr == _CharMap.end())
		{
			LOG_MSG_CF(CLog::em_Log_Type_Exception, L"“Ï≥£OTP Key=%s", CCharacter::ASCIIToUnicode(strKey).c_str());
			return result;
		}

		buffer <<= _SHIFT;
		buffer |= itr->second & _MASK;
		bitsleft += _SHIFT;
		if (bitsleft >= 8)
		{
			result.get()[next++] = static_cast<BYTE>(buffer >> (bitsleft - 8));
			bitsleft -= 8;
		}
	}

	uBufferLen = static_cast<UINT>(outLength);
	return result;
}

VOID CLHmacSha1::InitCharMap()
{
	std::string wsAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
	std::shared_ptr<BYTE> DIGITS_(new BYTE[wsAlphabet.length()], [](BYTE* p) { delete[] p; });
	for (UINT i = 0; i < wsAlphabet.length(); ++i)
		DIGITS_.get()[i] = static_cast<BYTE>(wsAlphabet[i]);

	_MASK = wsAlphabet.length() - 1;
	_SHIFT = NumberOfTrailingZeros(wsAlphabet.length());

	for (UINT i = 0; i < wsAlphabet.length(); ++i)
		_CharMap.insert(std::make_pair(DIGITS_.get()[i], i));
}

int CLHmacSha1::NumberOfTrailingZeros(int i) CONST
{
	static CONST int _lookup[] =
	{
		32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4, 7, 17,
		0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5, 20, 8, 19, 18
	};
	return _lookup[(i & -i) % 37];
}

LONGLONG CLHmacSha1::GetOTPState() CONST _NOEXCEPT
{
	static CTotpCounter TotpCounter(30, 0);
	return TotpCounter.GetValueAtTime(static_cast<LONGLONG>(time(nullptr)));
}
