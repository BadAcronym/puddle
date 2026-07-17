#include "pd_path.h"

uint8_t pdVerifyPath
(
    StringView path
){
    struct _stat pathInfo;

    if(_stat(path, &pathInfo))
    {
        return RV_TYPE_ERROR;
    }

    if(_S_IFDIR & pathInfo.st_mode)
    {
        return RV_TYPE_DIRECTORY;
    }

    if(_S_IFREG & pathInfo.st_mode)
    {
        return RV_TYPE_FILE;
    }

    return RV_TYPE_OTHER;
}

void pdExpandPath
(
    const char *path,
    char*      buf
){
    StringView path_sv = cstr_sv(path);
    StringView dot_sv  = cstr_sv(".");

    StringView homevar = cstr_sv("$HOME");
    const char *home   = getenv("HOME");
    StringView home_sv = cstr_sv(home);

    if(home && path_sv.size > 0 && path[0] == '~')
    {
        uint32_t i = 0;
        for(; i < home_sv.size; ++i)
        {
            buf[i] = home[i];
        }

        uint32_t j = 0;
        for(; j < path_sv.size && j < 4096 - i; ++j)
        {
            buf[i + j] = path[j + 1];
        }

        buf[i + j] = '\0';
    }
    else if(home && path_sv.size > 4 && sv_find(homevar, path_sv) == path_sv.data)
    {
        uint32_t i = 0;
        for(; i < home_sv.size; ++i)
        {
            buf[i] = home[i];
        }

        uint32_t j = 0;
        for(; j < path_sv.size && j < 4096 - i - homevar.size; ++j)
        {
            buf[i + j] = path[j + homevar.size];
        }

        buf[i + j] = '\0';
    }
    else if(sv_same(path_sv, dot_sv))
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
        for(; i < path_sv.size && i < 4096; ++i)
        {
            buf[i] = path[i];
        }
        buf[i] = '\0';
    }
}

StringView pdListFiles
(
    StringView directory,
    char       *buf
){
    WIN32_FIND_DATAA fileData;
    HANDLE           foundHandle;

    char *winPath         = malloc(path.size + 3);
    for(uint16_t i = 0; i < path.size; ++i)
    {
        winPath[i] = path[i];
    }
    winPath[path.size]     = '\\';
    winPath[path.size + 1] = '*';
    winPath[path.size + 2] = '\0';

    foundHandle = FindFirstFileA(winPath, &fileData);
    if(foundHandle == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to open directory.\033[0m\n");
        return 0;
    }

    uint32_t listSize = 0;

    while(FindNextFileA(foundHandle, &fileData))
    {
        uint8_t length = 0;
        for(; length < 255 && fileData.cFileName[length] != '\0'; ++length)
        {
        }
        listSize += length + 1;
    }

    FindClose(foundHandle);
    foundHandle = FindFirstFileA(winPath, &fileData);

    char     *list  = (char*)malloc(listSize + 1);
    uint32_t offset = 0;

    while(FindNextFileA(foundHandle, &fileData))
    {
        uint8_t length = 0;
        for(; length < 255 && fileData.cFileName[length] != '\0'; ++length)
        {
            list[offset + length] = fileData.cFileName[length];
        }
        list[offset + length] = ';';
        offset += length + 1;
    }

    list[offset] = '\0';

    FindClose(foundHandle);
    free(winPath);
    return(StringView)
    {
        .data = list,
    };
}
