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

// directly from stdbool.h
#if __STDC_VERSION__ < 202311l
    #define bool  _Bool
    #define true  1
    #define false 0
#endif
#define __bool_true_false_are_defined true

#endif
