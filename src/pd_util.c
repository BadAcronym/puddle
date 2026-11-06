#include "pd_util.h"
#include "pd_print_macros.h"

#include <stdio.h>
#include <inttypes.h>

BinBuffer pdReadBinFile
(
    const char *filepath
){
    FILE *file = fopen(filepath, "rb");
    if(!file)
    {
        PD_ERROR("failed to open file for reading: '%s'", filepath);
        goto error;
    }

    fseek(file, 0, SEEK_END);
    size filesize = (size)ftell(file);
    fseek(file, 0, SEEK_SET);

    u08 *fileBuf = malloc(filesize);
    if(!fileBuf)
    {
        PD_ERROR("failed to allocate fileBuf of size %"PRIu64, filesize);
        goto error;
    }

    if(fread(fileBuf, 1, filesize, file) != filesize)
    {
        PD_ERROR("failed to read entire file into fileBuf.");
        fclose(file);
        goto error;
    }
    fclose(file);

    BinBuffer buf = {0};
    buf.data = fileBuf;
    buf.size = filesize;
    return buf;

error:
    return (BinBuffer){0};
}
