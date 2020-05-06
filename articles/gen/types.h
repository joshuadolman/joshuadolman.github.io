#ifndef TYPES_H
#define TYPES_H

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

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define ArrayCount(array) (sizeof((array)) / sizeof((array)[0]))

#define ByteSwap4(V) (((V>>24)&0xff) | ((V<<8)&0xff0000) | ((V>>8)&0xff00) | ((V<<24)&0xff000000))

#define PI32 3.14159265359f
#define TAU32 6.28318530717958647692f

#define PI64 3.141592653589793115997963468544185161590576171875

#define U16MAX 65535
#define S16MAX ((s16)0x7FFF)
#define S16MIN (((s16)((S16MAX)+1)))

#define S32MIN ((s32)0x80000000)
#define S32MAX ((s32)0x7fffffff)
#define U32MIN 0
#define U32MAX (((u32)U32MIN)-(u32)1)
#define U64MIN ((u64)U32MIN)
#define U64MAX (((u64)U64MIN)-(u64)1)
#define F32MAX FLT_MAX
#define F32MIN -FLT_MAX

#define TRUE 1
#define FALSE 0

#if ASSERTS
#define Assert(Expression) if(!(Expression)) { *(u8*)0 = 0; }
#else 
#define Assert(Expression)
#endif

inline u32
SafeTruncateU64ToU32(u64 v) {
	Assert(v <= U32MAX);
	u32 result = (u32)v;
	return result;
}

inline s16
SafeTruncateS32ToS16(s32 v) {
	Assert(v <= S16MAX);
	s16 result = (s16)v;
	return result;
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

#define RELEASEMODE_RELEASE 0
#define RELEASEMODE_INTERNAL 1

#define DISABLED 0
#define ENABLED 1


#endif //TYPES_H