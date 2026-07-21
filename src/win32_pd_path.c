#include "pd_path.h"

#include <sys/stat.h>
#include <windows.h>
#include <direct.h>

uint8_t pdVerifyPath
(
    StringView path
){
    struct _stat pathInfo;

    char path_cstr[4096];
    sv_cstr(path, path_cstr);

    if(_stat(path_cstr, &pathInfo))
    {
        return PD_TYPE_ERROR;
    }

    if(_S_IFDIR & pathInfo.st_mode)
    {
        return PD_TYPE_DIRECTORY;
    }

    if(_S_IFREG & pathInfo.st_mode)
    {
        return PD_TYPE_FILE;
    }

    return PD_TYPE_OTHER;
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
    if(!home)
    {
        home = getenv("USERPROFILE");
        if(!home)
        {
            fprintf(stderr, "\033[31mERROR: couldn't resolve HOME or USERPROFILE."
                    "\033[0m\n");
            return;
        }
    }
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
        char cwd[4096];
        _getcwd(cwd, 4096);

        StringView cwd_sv = cstr_sv(cwd);

        uint32_t i = 0;
        for(; i < cwd_sv.size; ++i)
        {
            buf[i] = cwd[i];
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

    char *winPath = malloc(directory.size + 3);
    for(uint16_t i = 0; i < directory.size; ++i)
    {
        winPath[i] = directory.data[i];
    }
    winPath[directory.size]     = '\\';
    winPath[directory.size + 1] = '*';
    winPath[directory.size + 2] = '\0';

    foundHandle = FindFirstFileA(winPath, &fileData);
    if(foundHandle == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "\n\033[31;1;7mERROR: failed to open directory.\033[0m\n");
        return (StringView){0};
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

    uint32_t offset = 0;

    while(FindNextFileA(foundHandle, &fileData))
    {
        uint8_t length = 0;
        for(; length < 255 && fileData.cFileName[length] != '\0'; ++length)
        {
            buf[offset + length] = fileData.cFileName[length];
        }
        buf[offset + length] = ';';
        offset += length + 1;
    }

    buf[offset] = '\0';

    FindClose(foundHandle);
    free(winPath);
    return cstr_sv(buf);
}
