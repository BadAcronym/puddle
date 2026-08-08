#include "pd_path.h"

uint8_t pdVerifyPath
(
    StringView path
){
    struct stat pathInfo;
    char path_cstr[4096];
    sv_cstr(path, path_cstr);

    if(stat(path_cstr, &pathInfo))
    {
        return PD_TYPE_ERROR;
    }

    if(S_ISDIR(pathInfo.st_mode))
    {
        return PD_TYPE_DIRECTORY;
    }

    if(S_ISREG(pathInfo.st_mode))
    {
        return PD_TYPE_FILE;
    }
    return PD_TYPE_OTHER;
}

StringView pdExpandPath
(
    StringView path,
    char*      buf
){
    if(!path.data)
    {
        return (StringView){0};
    }

    StringView dot_sv  = cstr_sv(".");

    StringView homevar = cstr_sv("$HOME");
    const char *home   = getenv("HOME");
    StringView home_sv = cstr_sv(home);

    if(home && path.size > 0 && path.data[0] == '~')
    {
        uint32_t i = 0;
        for(; i < home_sv.size; ++i)
        {
            buf[i] = home[i];
        }

        uint32_t j = 0;
        for(; j < path.size && j < 4096 - i; ++j)
        {
            buf[i + j] = path.data[j + 1];
        }

        buf[i + j] = '\0';
    }
    else if(home && path.size > 4 && sv_find(homevar, path) == path.data)
    {
        uint32_t i = 0;
        for(; i < home_sv.size; ++i)
        {
            buf[i] = home[i];
        }

        uint32_t j = 0;
        for(; j < path.size && j < 4096 - i - homevar.size; ++j)
        {
            buf[i + j] = path.data[j + homevar.size];
        }

        buf[i + j] = '\0';
    }
    else if(sv_same(path, dot_sv))
    {
        const char *pwd   = getenv("PWD");
        StringView pwd_sv = cstr_sv(pwd);

        uint32_t i = 0;
        for(; i < pwd_sv.size; ++i)
        {
            buf[i] = pwd[i];
        }
        buf[i] = '\0';
    }
    else
    {
        uint32_t i = 0;
        for(; i < path.size && i < 4096; ++i)
        {
            buf[i] = path.data[i];
        }
        buf[i] = '\0';
    }

    return cstr_sv(buf);
}

StringView pdParentPath
(
    StringView path,
    char       *buf
){
    pdExpandPath(path, buf);
    StringView expanded = cstr_sv(buf);

    const char *lastdir = sv_find_last("/");

    expanded.size -= (size_t)(expanded.data - lastdir);

    uint8_t code = pdVerifyPath(expanded);
    if(code != PD_TYPE_FILE && code != PD_TYPE_DIRECTORY)
    {
        return (StringView){0};
    }
}

StringView pdListFiles
(
    StringView directory,
    char       *buf
){
    DIR           *dir;
    struct dirent *ent;
    uint32_t      listSize = 0;

    char path[4096] = {0};

    sv_cstr(directory, path);

    if((dir = opendir(path)))
    {
        while((ent = readdir(dir)))
        {
            uint8_t length = 0;
            for(; length < 255 && ent->d_name[length] != '\0'; ++length)
            {
            }
            listSize += length + 1;
        }
    }
    else
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to open directory.\033[0m\n");
        return(StringView){0};
    }

    char listBuffer[listSize + 1];

    String list = {0};
    list.data   = listBuffer;
    list.size   = 0;
    uint32_t offset = 0;

    free(dir);
    dir = opendir(path);

    while((ent = readdir(dir)))
    {
        uint8_t curr_length = 0;
        for(; curr_length >= 0 && ent->d_name[curr_length] != '\0'; ++curr_length)
        {
            list.data[offset + curr_length] = ent->d_name[curr_length];
        }
        list.data[offset + curr_length] = ';';
        list.size += curr_length + 1;
        offset    += curr_length + 1;
    }

    list.data[offset] = '\0';

    StringView result =
    {
        .data = list.data,
        .size = list.size
    };

    sv_sort_by_delim(result, ';', buf);

    result.data = buf;

    free(dir);

    return result;
}
