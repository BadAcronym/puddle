#ifndef PUDDLE_INT_TYPES_H
#define PUDDLE_INT_TYPES_H

typedef __UINT8_TYPE__  u08;
typedef __UINT16_TYPE__ u16;
typedef __UINT32_TYPE__ u32;
typedef __UINT64_TYPE__ u64;

typedef __INT8_TYPE__   i08;
typedef __INT16_TYPE__  i16;
typedef __INT32_TYPE__  i32;
typedef __INT64_TYPE__  i64;

typedef __SIZE_TYPE__   size;

typedef float           f32;
typedef double          f64;

#define U08MAX (255)
#define U16MAX (65535)
#define U32MAX (4294967295U)
#define U64MAX (18446744073709551615UL)

#define I08MIN (-128)
#define I16MIN (-32767-1)
#define I32MIN (-2147483647-1)
#define I64MIN (-__INT64_C(9223372036854775807)-1)
#define I08MAX (127)
#define I16MAX (32767)
#define I32MAX (2147483647)
#define I64MAX (__INT64_C(9223372036854775807))

#define F32MIN (-3.402823466e+38f)
#define F64MIN (-1.7976931348623157e+308)
#define F32MAX (3.402823466e+38f)
#define F64MAX (1.7976931348623157e+308)

// directly from stdbool.h
#if __STDC_VERSION__ < 202311l
    #define bool  _Bool
    #define true  1
    #define false 0
#endif
#define __bool_true_false_are_defined true

#endif
