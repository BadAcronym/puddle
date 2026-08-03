#include "string_view.h"

String cstr_str
(
    char *cstr
){
    if(!cstr)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as cstr.\033[0m\n");
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

String cstr_str_cpy
(
    char *cstr
){
    if(!cstr)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as cstr.\033[0m\n");
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

StringView cstr_sv
(
    const char *cstr
){
    if(!cstr)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as cstr.\033[0m\n");
        return (StringView){0};
    }

    uint32_t i = 0;
    for(; cstr[i] != '\0'; ++i)
    {
    }

    return(StringView)
    {
        .data = cstr,
        .size = i
    };
}

StringView cstr_sv_cpy
(
    const char *cstr,
    char       *buf
){
    if(!buf)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as buf.\033[0m\n");
        return (StringView){0};
    }
    if(!cstr)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as cstr.\033[0m\n");
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

void sv_cstr
(
    StringView sv,
    char       *buf
){
    if(!buf)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as buf.\033[0m\n");
        return;
    }
    if(!sv.data)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as sv.data.\033[0m\n");
        return;
    }

    memcpy(buf, sv.data, sv.size);
    buf[sv.size] = '\0';
}

void str_cstr
(
    String str,
    char   *buf
){
    if(!buf)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as buf.\033[0m\n");
        return;
    }
    if(!str.data)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as str.data.\033[0m\n");
        return;
    }

    memcpy(buf, str.data, str.size);
    buf[str.size] = '\0';
}

StringView str_sv_cpy
(
    String str,
    char   *buf
){
    if(!str.data)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as str.data.\033[0m\n");
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

StringView sv_substr
(
    StringView *sv,
    size_t     start_pos,
    size_t     end_pos
){
    if(start_pos > end_pos)
    {
        fprintf(stderr, "\033[31;3;1mERROR: end_pos cannot be smaller than start_pos."
                "\033[0m\n");
        return(StringView)
        {
            .data = 0,
            .size = 0
        };
    }

    if(start_pos > sv->size)
    {
        fprintf(stderr, "\033[31;3;1mERROR: start_pos cannot be larger than sv->size."
                "\033[0m\n");
        return(StringView)
        {
            .data = 0,
            .size = 0
        };
    }

    if(end_pos > sv->size)
    {
        end_pos = sv->size;
    }

    StringView result;
    result.data = sv->data + start_pos;
    result.size = sv->size - start_pos - (sv->size - end_pos);

    if(start_pos == end_pos)
    {
        result.size = 1;
    }

    return result;
}

void sv_trim
(
    StringView *sv,
    size_t     count,
    uint8_t    direction
){
    if(direction > SV_BOTH)
    {
        fprintf(stderr, "\033[31;3;1mERROR: unknown direction.\033[0m\n");
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

uint8_t sv_same
(
    StringView first,
    StringView second
){
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

uint8_t sv_is_substr
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

const char *sv_find
(
    StringView pattern,
    StringView sv
){
    if(sv.data == 0 || pattern.data == 0 || sv.size == 0 || pattern.size == 0)
    {
        return 0;
    }

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
            return(&sv.data[i]);
        }
    }

    return 0;
}

StringView sv_find_by_delim
(
    StringView sv,
    char       delim,
    uint32_t   index
){
    StringView result          = {0};
    uint32_t   delim_count     = 0;
    const char *nextword_start = sv.data;

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
        size_t substr_size = 0;

        if(sv.data[i] == delim)
        {
            ++delim_count;

            for(; i < sv.size && sv.data[i] == delim; ++i)
            {
            }

            nextword_start = sv.data + i;
        }

        if(delim_count == index)
        {
            for(; i < sv.size && sv.data[i] != delim; ++i)
            {
                ++substr_size;
            }

            return(StringView)
            {
                .data = nextword_start,
                .size = substr_size
            };
        }
    }

    return (StringView){0};
}

uint32_t sv_count_by_delim
(
    StringView sv,
    char       delim
){
    uint32_t delim_count = 0;

    if(sv.data == 0)
    {
        return 0;
    }

    for(uint32_t i = 0; i < sv.size; ++i)
    {
        if(sv.data[i] == delim)
        {
            ++delim_count;

            for(; i < sv.size && sv.data[i] == delim; ++i)
            {
            }
        }
    }

    if(sv.size && sv.data[sv.size - 1] != delim && sv.data[0] != delim)
    {
        return delim_count + 1;
    }
    else if(sv.size && sv.data[sv.size - 1] == delim && sv.data[0] == delim)
    {
        return delim_count - 1;
    }

    return delim_count;
}

void sv_sort_by_delim
(
    StringView sv,
    char       delim,
    char       *buf
){
    if(!buf)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as buf.\033[0m\n");
        return;
    }

    uint32_t count = sv_count_by_delim(sv, delim);
    if(!count)
    {
        return;
    }

    StringView *sv_buffer = malloc(count * sizeof(StringView));

    for(uint32_t i = 0; i < count; ++i)
    {
        sv_buffer[i] = sv_find_by_delim(sv, delim, i);
    }

    for(uint32_t i = 0; i < count - 1; ++i)
    {
        uint8_t swapped = 0;
        for(uint32_t j = 0; j < count - i - 1; ++j)
        {
            if(!sv_buffer[j + 1].data)
            {
                continue;
            }

            if(!sv_buffer[j].data || sv_is_lesser(sv_buffer[j + 1], sv_buffer[j]))
            {
                StringView tmp   = sv_buffer[j];
                sv_buffer[j]     = sv_buffer[j + 1];
                sv_buffer[j + 1] = tmp;

                swapped = 1;
            }
        }

        if(!swapped)
        {
            break;
        }
    }

    uint32_t offset = 0;
    for(uint32_t i = 0; i < count; ++i)
    {
        for(uint32_t j = 0; j < sv_buffer[i].size; ++j)
        {
            buf[offset + j] = sv_buffer[i].data[j];
        }

        if(offset + sv_buffer[i].size < sv.size)
        {
            buf[offset + sv_buffer[i].size] = delim;
        }
        offset += sv_buffer[i].size + 1;
    }

    buf[sv.size] = '\0';
    free(sv_buffer);
}

void sv_separate_by_delim
(
    StringView sv,
    StringView *buf,
    char       delim
){
    // TODO:
}

uint8_t sv_is_lesser
(
    StringView first,
    StringView second
){
    if(!first.data)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as first.data.\033[0m\n");
        return SV_GREATER;
    }
    else if(!second.data)
    {
        fprintf(stderr, "\033[31;1mERROR: passed nullptr as second.data.\033[0m\n");
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

void sv_concat
(
    StringView first,
    StringView second,
    char       *buf
){
    if(!first.data || !second.data)
    {
        fprintf(stderr, "\033[31mERROR: bad stringview data pointer.\033[0m\n");
        return;
    }

    char first_data[first.size + 1];
    char second_data[second.size + 1];

    sv_cstr(first, first_data);
    sv_cstr(second, second_data);

    memcpy((void*)buf, (void*)first_data, first.size);
    memcpy((void*)(buf + first.size), (void*)second_data, second.size);
    buf[first.size + second.size] = '\0';
}
