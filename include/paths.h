#ifndef PATH_DECL
#define PATH_DECL

#include <stdint.h>

#ifdef BUILD_LINUX
    #include <sys/stat.h>
#endif

#include "string_view.h"

#define PD_TYPE_ERROR     0
#define PD_TYPE_FILE      1
#define PD_TYPE_DIRECTORY 2
#define PD_TYPE_OTHER     3

uint8_t pdVerifyPath
(
    StringView path
);

// will expand the given path into buf, resolving '.', '$HOME' and '~', but currently no
// other symbols.
void pdExpandPath
(
    const char *path,
    char       *buf
);
#endif

#ifdef STRING_VIEW_IMPL
uint8_t pdVerifyPath
(
    StringView path
){
#ifdef BUILD_LINUX
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
#endif
#ifdef BUILD_WINDOWS
#endif
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
#endif
