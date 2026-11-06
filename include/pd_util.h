#ifndef PD_UTIL_H
#define PD_UTIL_H

#include "pd_int_types.h"

// just a short-hand for returning a file via `pdReadBinFile()`, really.
// represents a pointer to the start of a file's data and the final length.
typedef struct BB
{
    u08  *data;
    size size;
}
BinBuffer;

// will read an entire file into buf and return a BinBuffer with the corresponding
// size. you will need to free the returned BinBuffer's data pointer yourself.
BinBuffer pdReadBinFile
(
    const char *filepath
);

#endif
