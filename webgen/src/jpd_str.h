#ifndef JPD_STR_h
#define JPD_STR_H

#include "jpd.h"

struct string {
	s64 Length;
	s64 DataSize; 
	char* Data;
};

struct string_builder {
	string String;

	void Init(s64 Size);

	void AppendStr(string Str);
	void AppendCharacter(u8 Character);
	void AppendCStr(const char* CStr);

	void ToLower();
	void ToUpper();
	
	void NullTerminate();

	char GetChar(s32 Index);
	void PutChar(s32 Index, char Character);

	char* At();
};

namespace CStr {
	internal s32 Length(const char* String);
	internal b32 Compare(const char* A, const char* B);
	internal void ToLower(char* String);
	internal void ToUpper(char* String);
	internal string Copy(char* CString);
	internal b32 StartsWith(const char* A, const char* Prefix);
}

#endif // JPD_STR_H