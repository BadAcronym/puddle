#include "pd_string_view.h"
#include "pd_print_macros.h"

void pdSVFree
(
    StringView *sv
){
    if(sv->data)
    {
        free((void*)sv->data);
    }

    sv->data = 0;
    sv->size = 0;
}

String pdCstrStr
(
    char *cstr
){
    if(!cstr)
    {
        PD_ERROR("passed nullptr as cstr.");
        return (String){0};
    }

    uint32_t i = 0;
    for(; cstr[i] != '\0'; ++i)
    {
    }

    return(String)
    {
        .data = cstr,
        .size = i
    };
}

String pdCstrStrCpy
(
    char *cstr
){
    if(!cstr)
    {
        PD_ERROR("passed nullptr as cstr.");
        return (String){0};
    }

    uint32_t i = 0;
    for(; cstr[i] != '\0'; ++i)
    {
    }

    char *buf = malloc(i + 2);
    memcpy((void*)buf, cstr, i);
    buf[i + 1] = '\0';

    return(String)
    {
        .data = buf,
        .size = i
    };
}

StringView pdCstrSV
(
    const char *cstr
){
    if(!cstr)
    {
        PD_ERROR("passed nullptr as cstr.");
        return (StringView){0};
    }

    uint32_t i = 0;
    for(; cstr[i] != '\0' && cstr[i] != '\n'; ++i)
    {
    }

    return(StringView)
    {
        .data = cstr,
        .size = i
    };
}

StringView pdCstrSVCpy
(
    const char *cstr,
    char       *buf
){
    if(!buf)
    {
        PD_ERROR("passed nullptr as buf.");
        return (StringView){0};
    }
    if(!cstr)
    {
        PD_ERROR("passed nullptr as cstr.");
        return (StringView){0};
    }

    uint32_t i = 0;
    for(; cstr[i] != '\0'; ++i)
    {
    }

    memcpy((void*)buf, cstr, i);
    buf[i + 1] = '\0';

    return(StringView)
    {
        .data = buf,
        .size = i
    };
}

StringView pdSVCpy
(
    StringView sv
){
    if(!sv.data)
    {
        PD_ERROR("passed nullptr as sv.data.");
        return (StringView){0};
    }

    char *buf = (char*)malloc(sv.size + 1);

    for(uint32_t i = 0; i < sv.size; ++i)
    {
        buf[i] = sv.data[i];
    }

    return(StringView)
    {
        .data = buf,
        .size = sv.size
    };
}

const char *pdSVCstr
(
    StringView sv,
    char       *buf
){
    if(!buf)
    {
        PD_ERROR("passed nullptr as buf.");
        return 0;
    }
    if(!sv.data)
    {
        PD_ERROR("passed nullptr as sv.data.");
        return 0;
    }

    memcpy(buf, sv.data, sv.size);
    buf[sv.size] = '\0';

    return buf;
}

void pdStrCstr
(
    String str,
    char   *buf
){
    if(!buf)
    {
        PD_ERROR("passed nullptr as buf.");
        return;
    }
    if(!str.data)
    {
        PD_ERROR("passed nullptr as str.data.");
        return;
    }

    memcpy(buf, str.data, str.size);
    buf[str.size] = '\0';
}

StringView pdStrCstrCpy
(
    String str,
    char   *buf
){
    if(!str.data)
    {
        PD_ERROR("passed nullptr as str.data.");
        return (StringView){0};
    }

    for(uint32_t i = 0; i < str.size; ++i)
    {
        buf[i] = str.data[i];
    }

    return(StringView)
    {
        .data = buf,
        .size = str.size
    };
}

StringView pdSVSubstr
(
    StringView *sv,
    size_t     startPos,
    size_t     endPos
){
    if(startPos > endPos)
    {
        PD_ERROR("end_pos cannot be smaller than start_pos.");
        return(StringView)
        {
            .data = 0,
            .size = 0
        };
    }

    if(startPos > sv->size)
    {
        PD_ERROR("start_pos cannot be larger than sv->size.");
        return(StringView)
        {
            .data = 0,
            .size = 0
        };
    }

    if(endPos > sv->size)
    {
        endPos = sv->size;
    }

    StringView result;
    result.data = sv->data + startPos;
    result.size = sv->size - startPos - (sv->size - endPos);

    if(startPos == endPos)
    {
        result.size = 1;
    }

    return result;
}

void pdSVTrim
(
    StringView *sv,
    size_t     count,
    uint8_t    direction
){
    if(direction > SV_BOTH)
    {
        PD_ERROR("unknown direction.");
        return;
    }

    if(direction == SV_LEFT || direction == SV_BOTH)
    {
        size_t i = count;
        if(i > sv->size)
        {
            i = sv->size;
        }
        sv->size -= i;
        sv->data += i;
    }
    if(direction == SV_RIGHT || direction == SV_BOTH)
    {
        size_t i = count;
        if(i > sv->size)
        {
            i = sv->size;
        }
        sv->size -= i;
    }
}

uint8_t pdSVSame
(
    StringView first,
    StringView second
){
    if(!first.data)
    {
        PD_WARN("passed nullptr as first.data.");
        if(second.data)
        {
            return SV_DIFFERENT;
        }
        return 0;
    }
    else if(!second.data)
    {
        PD_WARN("passed nullptr as second.data.");
        return SV_DIFFERENT;
    }

    if((first.size == second.size && first.data == second.data) ||
       (!first.size && !second.size)
    ){
        return SV_SAME;
    }
    else if(first.size == 0 || second.size == 0)
    {
        return SV_DIFFERENT;
    }

    size_t i = 0;
    for(; i < first.size && i < second.size; ++i)
    {
        if(first.data[i] != second.data[i])
        {
            return SV_DIFFERENT;
        }
    }

    if(i == second.size && i == first.size)
    {
        return SV_SAME;
    }
    return SV_DIFFERENT;
}

uint8_t pdSVIsSubstr
(
    StringView first,
    StringView second
){
    if((first.data == second.data) || (first.size == 0 && second.size == 0))
    {
        return SV_SAME;
    }

    for(size_t i = 0; i < second.size; ++i)
    {
        size_t j = 0;
        for(; j < first.size; ++j)
        {
            if(second.data[i + j] != first.data[j])
            {
                break;
            }
        }

        if(j == first.size)
        {
            return SV_IS_SUBSTR;
        }
    }

    return SV_DIFFERENT;
}

const char *pdSVFind
(
    StringView pattern,
    StringView sv
){
    if(sv.data == 0 || pattern.data == 0 || sv.size == 0 || pattern.size == 0)
    {
        return 0;
    }

    if(sv.size < pattern.size)
    {
        return 0;
    }

    for(size_t i = 0; i < sv.size - pattern.size + 1; ++i)
    {
        size_t j = 0;
        for(; j < pattern.size; ++j)
        {
            if(sv.data[i + j] != pattern.data[j])
            {
                break;
            }
        }

        if(j == pattern.size)
        {
            return(&sv.data[i]);
        }
    }

    return 0;
}

const char *pdSVFindLast
(
    StringView pattern,
    StringView sv
){
    if(sv.data == 0 || pattern.data == 0 || sv.size == 0 || pattern.size == 0)
    {
        return 0;
    }

    const char *current = 0;

    for(size_t i = 0; i < sv.size; ++i)
    {
        size_t j = 0;
        for(; j < pattern.size; ++j)
        {
            if(sv.data[i + j] != pattern.data[j])
            {
                break;
            }
        }

        if(j == pattern.size)
        {
            current = sv.data + i;
        }
    }

    return current;
}

StringView pdSVFindByDelim
(
    StringView sv,
    char       delim,
    uint32_t   index
){
    StringView result         = {0};
    uint32_t   delimCount     = 0;
    const char *nextwordStart = sv.data;

    if(sv.data == 0)
    {
        return result;
    }

    if(sv.data[0] == delim)
    {
        ++index;
    }

    for(uint64_t i = 0; i < sv.size; ++i)
    {
        size_t substrSize = 0;

        if(sv.data[i] == delim)
        {
            ++delimCount;

            for(; i < sv.size && sv.data[i] == delim; ++i)
            {
            }

            nextwordStart = sv.data + i;
        }

        if(delimCount == index)
        {
            for(; i < sv.size && sv.data[i] != delim; ++i)
            {
                ++substrSize;
            }

            return(StringView)
            {
                .data = nextwordStart,
                .size = substrSize
            };
        }
    }

    return (StringView){0};
}

uint32_t pdSVCountByDelim
(
    StringView sv,
    char       delim
){
    uint32_t delimCount = 0;

    if(sv.data == 0)
    {
        return 0;
    }

    for(uint32_t i = 0; i < sv.size; ++i)
    {
        if(sv.data[i] == delim)
        {
            ++delimCount;

            for(; i < sv.size && sv.data[i] == delim; ++i)
            {
            }
        }
    }

    if(sv.size && sv.data[sv.size - 1] != delim && sv.data[0] != delim)
    {
        return delimCount + 1;
    }
    else if(sv.size && sv.data[sv.size - 1] == delim && sv.data[0] == delim)
    {
        return delimCount - 1;
    }

    return delimCount;
}

uint8_t pdSVIsLesser
(
    StringView first,
    StringView second
){
    if(!first.data)
    {
        PD_WARN("passed nullptr as first.data.");
        return SV_GREATER;
    }
    else if(!second.data)
    {
        PD_WARN("passed nullptr as second.data.");
        return SV_GREATER;
    }

    for(uint32_t i = 0; i < first.size && second.size; ++i)
    {
        if(first.data[i] < second.data[i])
        {
            return SV_LESSER;
        }
        else if(second.data[i] < first.data[i])
        {
            return SV_GREATER;
        }

        if(i == first.size - 1 && first.size < second.size)
        {
            return SV_LESSER;
        }
        else if(i == second.size - 1 && second.size < first.size)
        {
            return SV_GREATER;
        }
    }

    return SV_LESSER;
}

void pdSVSortByDelim
(
    StringView sv,
    char       delim,
    char       *buf
){
    if(!buf)
    {
        PD_ERROR("passed nullptr as buf.");
        return;
    }

    uint32_t count = pdSVCountByDelim(sv, delim);
    if(!count)
    {
        return;
    }

    StringView *svBuf = malloc(count * sizeof(StringView));

    for(uint32_t i = 0; i < count; ++i)
    {
        svBuf[i] = pdSVFindByDelim(sv, delim, i);
    }

    for(uint32_t i = 0; i < count - 1; ++i)
    {
        uint8_t swapped = 0;
        for(uint32_t j = 0; j < count - i - 1; ++j)
        {
            if(!svBuf[j + 1].data)
            {
                continue;
            }

            if(!svBuf[j].data || pdSVIsLesser(svBuf[j + 1], svBuf[j]))
            {
                StringView tmp   = svBuf[j];
                svBuf[j]     = svBuf[j + 1];
                svBuf[j + 1] = tmp;

                swapped = 1;
            }
        }

        if(!swapped)
        {
            break;
        }
    }

    size_t offset = 0;
    for(uint32_t i = 0; i < count; ++i)
    {
        for(size_t j = 0; j < svBuf[i].size; ++j)
        {
            buf[offset + j] = svBuf[i].data[j];
        }

        if(offset + svBuf[i].size < sv.size)
        {
            buf[offset + svBuf[i].size] = delim;
        }
        offset += svBuf[i].size + 1;
    }

    buf[sv.size] = '\0';
    free(svBuf);
}

void pdSVSeparateByDelim
(
    StringView sv,
    StringView *buf,
    char       delim,
    uint64_t   bufsize
){
    uint64_t   index          = 0;
    const char *nextwordStart = sv.data;

    if(sv.data == 0)
    {
        return;
    }

    for(uint64_t i = 0; i < sv.size; ++i)
    {
        size_t substrSize = 0;

        if(sv.data[i] == delim)
        {
            for(; i < sv.size && sv.data[i] == delim; ++i)
            {
                ++nextwordStart;
            }
        }

        for(; i < sv.size && sv.data[i] != delim; ++i)
        {
            ++substrSize;
        }

        if(index >= bufsize)
        {
            return;
        }

        buf[index].data = nextwordStart;
        buf[index].size = substrSize;

        nextwordStart += substrSize + 1;

        ++index;
    }
}

StringView pdSVConcat
(
    StringView first,
    StringView second,
    char       *buf
){
    if(!first.data)
    {
        PD_WARN("passed nullptr as first.data.");
        return (StringView){0};
    }
    if(!second.data)
    {
        PD_WARN("passed nullptr as second.data.");
        return (StringView){0};
    }

    char firstData[first.size + 1];
    char secondData[second.size + 1];

    pdSVCstr(first, firstData);
    pdSVCstr(second, secondData);

    memcpy((void*)buf, (void*)firstData, first.size);
    memcpy((void*)(buf + first.size), (void*)secondData, second.size);
    buf[first.size + second.size] = '\0';

    StringView concat = {0};
    concat.data = buf;
    concat.size = first.size + second.size;
    return concat;
}
