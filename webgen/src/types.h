#ifndef JPD_H
#define JPD_H

#include <stdint.h>

#define internal static
#define local_persist static
#define global static

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float  f32;
typedef double f64;

/* fixedpoint
typedef u32 fp32;
typedef u64 fp64;
typedef s32 sfp32;
typedef s64 sfp64;
*/

typedef s8  b8;
typedef s16 b16;
typedef s32 b32;
typedef s64 b64;

#define TRUE 1
#define FALSE 0

/* bitfields 
typedef u8  bf8;
typedef u16 bf16;
typedef u32 bf32;
typedef u64 bf64;
*/

#define Kilo(Value) ((Value)*1024LL)
#define Mega(Value) (Kilobytes(Value)*1024LL)
#define Giga(Value) (Megabytes(Value)*1024LL)
#define Tera(Value) (Gigabytes(Value)*1024LL)

#define ArrayCount(array) (sizeof((array)) / sizeof((array)[0]))

#define EndianByteSwap4(V) (((V>>24)&0xff) | ((V<<8)&0xff0000) | ((V>>8)&0xff00) | ((V<<24)&0xff000000))
// #define EndianByteSwap8(V) (((V>>24)&0xff) | ((V<<8)&0xff0000) | ((V>>8)&0xff00) | ((V<<24)&0xff000000))

#define PI32 3.14159265359f
#define TAU32 6.28318530717958647692f

#define PI64 3.141592653589793115997963468544185161590576171875

#define U16MAX (65535)
#define S16MAX ((s16)0x7FFF)
#define S16MIN (((s16)((S16MAX)+1)))

#define S32MIN ((s32)0x80000000)
#define S32MAX ((s32)0x7fffffff)
#define U32MIN (0)
#define U32MAX (((u32)U32MIN)-(u32)1)
#define U64MIN (0)
#define U64MAX (((u64)U64MIN)-(u64)1)
#define S64MIN (-9,223,372,036,854,775,808)
#define S64MAX (9,223,372,036,854,775,807)
#define F32MAX FLT_MAX
#define F32MIN -FLT_MAX

#if JPD_ASSERTS
#define Assert(Expression) if(!(Expression)) { *(u8*)0 = 0; }
#else 
#define Assert(Expression)
#endif

internal inline u32
SafeTruncateU64ToU32(u64 v) {
	Assert(v <= U32MAX);
	u32 result = (u32)v;
	return result;
}

internal inline s16
SafeTruncateS32ToS16(s32 v) {
	Assert(v <= S16MAX);
	s16 result = (s16)v;
	return result;
}

internal inline u64
SafeConvertS64ToU64(s64 v)
{
	Assert(v > U64MIN)
	return (u64)v;
}

inline u32
Maximum(u32 a, u32 b)
{
    if (a > b)
    {
        return a;
    } else {
        return b;
    }
}

inline u32
Minimum(u32 a, u32 b)
{
    if (a > b)
    {
        return b;
    } else {
        return a;
    }
}

inline f32
Maximum(f32 a, f32 b)
{
    if (a > b)
    {
        return a;
    } else {
        return b;
    }
}

inline f32
Minimum(f32 a, f32 b)
{
    if (a > b)
    {
        return b;
    } else {
        return a;
    }
}

inline f32
Average(f32 a, f32 b)
{
    f32 Result = a+b;
    Result /= 2;
    return Result;
}

internal u32
Power(u32 a, u32 b)
{
    u32 Result = a;
    for(u32 i = 0; i < b; i++)
    {
        Result = Result * a;
    }
    return Result;
}

internal f32
Power(f32 a, f32 b)
{
	f32 Result = a;
	for(f32 i = 0; i < b; i++)
	{
		Result *= a;
	}
	return Result;
}

#define ENABLED 1
#define DISABLED 0

internal void* Allocate(s64 Size);
internal void* ReAllocate(void* Memory, s64 CurrentSize, s64 NewSize);
internal void Free(void *Memory);
internal b32 SafeFree(void *Memory);

internal void
MemorySet(void *Buffer, u8 Value, u64 Length)
{
	for (int At = 0; At < Length; At++)
	{
		*(((u8 *)Buffer) + At) = Value;
	}
}

internal void
MemoryCopy32(u32* Src, void* Dst, s64 Length)
{
	u32 *CopyDst;
	for (s64 At = 0; At < Length; At++)
	{
		CopyDst = (u32 *)Dst + At;
		*CopyDst = *((u32 *)Src + At);
	}
}

internal void
MemoryCopy(void* Src, void* Dst, s64 Length)
{
	for (s64 At = 0; At < Length; At++)
	{
		*((u8*)Dst + At) = *((u8*)Src + At); 
	}
}

struct entire_file {
	void *Data;
	u64 Size;
};

internal entire_file ReadEntireFile(const char* Filename);

#ifdef OS_WIN32
internal entire_file
ReadEntireFile(const char* Filename)
{
	entire_file Result = {};
	b32 Success = false;
	HANDLE fileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER fileSize;
		if (GetFileSizeEx(fileHandle, &fileSize))
		{

			Result.Size = fileSize.QuadPart + 1;
			Assert(Result.Size > SafeConvertS64ToU64(fileSize.QuadPart))
			Result.Data = Allocate(Result.Size);
			if (Result.Data)
			{
				DWORD bytesRead = 0;
				if (ReadFile(fileHandle, Result.Data, (DWORD)Result.Size, &bytesRead, 0) && (Result.Size == bytesRead))
				{
					OutputDebugStringA("read success"); // win32_OutputDebug("FILE READ SUCCESS!");
					Success = true;
				}
				else
				{
					OutputDebugStringA("read failed"); // win32_OutputDebug("FILE READ FAILED!");
				}
			}
			else
			{
				OutputDebugStringA("alloc failed"); // win32_OutputDebug("ALLOC FAILED!");
			}
		}
		else
		{
			OutputDebugStringA("getfilesize failed"); // win32_OutputDebug("GETFILESIZE FAILED!");
		}
		CloseHandle(fileHandle);
	}
	else
	{
		OutputDebugStringA("could not find file"); // win32_OutputDebug("COULD NOT FIND FILE!");
	}

	if (!Success)
	{
		Result.Size = 0;
		SafeFree(Result.Data);
		Result.Data = 0;
	}

	return Result;
}

internal void*
Allocate(s64 Size)
{
	return VirtualAlloc(NULL, Size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

// internal void
// ReAllocate(void** Memory, s64 CurrentSize, s64 NewSize)
// {
// 	auto Temp = Allocate(CurrentSize);
// 	MemoryCopy(*Memory, Temp, CurrentSize);
// 	Free(*Memory);
// 	*Memory = Allocate(NewSize);
// 	MemoryCopy(Temp, *Memory, CurrentSize);
// 	Free(Temp);
// }

internal void*
ReAllocate(void* Memory, s64 CurrentSize, s64 NewSize)
{
	void* Temp = Allocate(CurrentSize);
	MemoryCopy(Memory, Temp, CurrentSize);
	Free(Memory);
	void* Result = Allocate(NewSize);
	MemoryCopy(Temp, Result, CurrentSize);
	Free(Temp);

	return Result;
}

internal void
Free(void* Memory)
{
	Assert(Memory)
	VirtualFree(Memory, 0, MEM_DECOMMIT | MEM_RELEASE);
}

internal b32
SafeFree(void* Memory)
{
	if (Memory)
	{
		VirtualFree(Memory, 0, MEM_DECOMMIT | MEM_RELEASE);
		return true;
	}
	return false;
}
#endif


#endif //JPD_H