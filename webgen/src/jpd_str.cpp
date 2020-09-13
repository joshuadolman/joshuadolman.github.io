#ifndef JPD_STR_CPP
#define JPD_STR_CPP

#include "jpd.h"
#include "jpd_str.h"

namespace CStr {

	internal s32 Length(const char* String) {
		int Length = 0;
		while (*String!=0)
		{
			Length++;
			String++;
		}
		return Length;
	}

	internal b32 Compare(const char* A, const char* B) {
		int At = 0;
		while(A[At] != 0 || B[At] != 0)
		{
			if (A[At] != B[At])
			{
				return false;
			}
			if (A[At+1] == 0 && B[At+1] != 0)
			{
				return false;
			}
			else if (A[At+1] != 0 && B[At+1] == 0)
			{
				return false;
			}
			At++;
		}
		return true;
	}

	internal void ToLower(char* String) {
		char* At = String;
		while (*At!=0)
		{
			if (*At >= 'A' && *At <= 'Z')
			{
				*At += 32;
			}
			At++;
		}
	}

	internal void ToUpper(char* String)  {
		char* At = String;
		while (*At!=0)
		{
			if (*At >= 'a' && *At <= 'z')
			{
				*At -= 32;
			}
			At++;
		}
	}

	internal string Copy(char* CString) {
		string Result;
		Result.Length = CStr::Length(CString);
		Result.DataSize = Result.Length + 1;
		Result.Data = (char*)Allocate(Result.DataSize);
		MemoryCopy(CString, Result.Data, Result.Length);

		return Result;
	}

	internal b32 StartsWith(const char* A, const char* Prefix) {
		if (*A == 0) { return false; }
		int At = 0;
		while(A[At] != 0 && Prefix[At] != 0)
		{
			if (A[At] != Prefix[At])
			{
				return false;
			}
			At++;
		}
		return true;
	}

}

internal b32 Compare(string A, string B) {
	if (A.Length != B.Length)
	{
		return false;
	}
	for (int i = 0; i < A.Length; i++)
	{
		if (A.Data[i] != B.Data[i])
		{
			return false;
		}
	}

	return true;
}

internal b32 Compare(string A, const char* B) {
	string BPrime = {};
	BPrime.Data = (char*)B;
	BPrime.Length = CStr::Length(B);
	BPrime.DataSize = BPrime.Length + 1;

	return Compare(A, BPrime);
}

char* string_builder::At()
{
	return this->String.Data + this->String.Length;
}

void string_builder::NullTerminate()
{
	*(this->At() + 1) = 0;
}

void string_builder::Init(s64 Size)
{
	if (this->String.Data)
	{
		Free(this->String.Data);
	}

	this->String.Length = 0;
	if (Size == 0)
	{
		Size = 8;
	} else {
		Size += ( 8 - (Size % 8) );
	}
	this->String.DataSize = Size;
	this->String.Data = (char*)Allocate(this->String.DataSize);
	
};

void string_builder::AppendCharacter(u8 Character)
{
	if (!this->String.Data)
	{
		this->Init(0);
	}
	if (this->String.Length + 1 >= this->String.DataSize)
	{
		s64 NewSize = this->String.DataSize * 2;
		this->String.Data = (char*)ReAllocate(this->String.Data, this->String.DataSize, NewSize);
		this->String.DataSize = NewSize;
	}
	*(this->At()    ) = Character;
	this->String.Length += 1;
	this->NullTerminate();
}

void string_builder::AppendCStr(const char* CStr)
{
	if (!this->String.Data)
	{
		this->Init(0);
	}
	auto CStrLen = CStr::Length(CStr);
	s64 AppendedSize = this->String.Length + 1 + CStrLen;
	if (AppendedSize >= this->String.DataSize)
	{
		s64 NewSize = this->String.DataSize * 2;
		while(NewSize < AppendedSize)
		{
			NewSize *= 2;
		}
		this->String.Data = (char*)ReAllocate(this->String.Data, this->String.DataSize, NewSize);
		this->String.DataSize = NewSize;
	}

	MemoryCopy((void*)CStr, this->At(), CStrLen);
	this->String.Length += CStrLen;
	this->NullTerminate();
}

void string_builder::AppendStr(string S)
{
	if (!String.Data)
	{
		Init(S.DataSize);
	}
	s64 AppendedSize = String.Length + 1 + S.Length;
	if (AppendedSize >= String.DataSize)
	{
		s64 NewSize = String.DataSize * 2;
		while(NewSize < AppendedSize)
		{
			NewSize *= 2;
		}
		String.Data = (char*)ReAllocate(String.Data, String.DataSize, NewSize);
		String.DataSize = NewSize;
	}

	MemoryCopy(S.Data, this->At(), S.Length);
	String.Length += S.Length;
	this->NullTerminate();
}

void string_builder::ToLower()
{
	CStr::ToLower(this->String.Data);
}

void string_builder::ToUpper()
{
	CStr::ToUpper(this->String.Data);
}

char string_builder::GetChar(s32 Index)
{
	Assert(Index >= 0)
	Assert(Index < this->String.Length)
	if (Index > this->String.Length)
	{
		return 0;
	}
	return this->String.Data[Index];
}

void string_builder::PutChar(s32 Index, char Character)
{
	Assert(Index >= 0)
	Assert(Index < this->String.Length)
	if (Index > this->String.Length)
	{
		return;
	}
	this->String.Data[Index] = Character;
}

#endif // JPD_STR_CPP