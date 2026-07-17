#ifndef PD_PATH_DECL
#define PD_PATH_DECL

#include <stdint.h>

#ifdef BUILD_LINUX
    #include <sys/stat.h>
#endif

#include "string_view.h"

#define PD_TYPE_ERROR     0
#define PD_TYPE_FILE      1
#define PD_TYPE_DIRECTORY 2
#define PD_TYPE_OTHER     3

// will verify if a given path exists. Will return `PD_TYPE_ERROR`, if it does not
// exist, `PD_TYPE_FILE` or `PD_TYPE_DIRECTORY` if it does, and `PD_TYPE_OTHER` in any
// other case.
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

// lists all files in a directory and writes them into buf, returning a StringView.
// Separated by ';'. Not recursive.
StringView pdListFiles
(
    StringView directory,
    char       *buf
);
#endif

#ifdef PD_PATH_IMPL
#define STRING_VIEW_IMPL
#include "string_view.h"
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

StringView pdListFiles
(
    StringView directory
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

    String list = {0};
    list.data   = (char*)malloc(listSize + 1);
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

    char *sorted = calloc(4096, 1);
    sv_sort_by_delim(result, ';', sorted);

    free((void*)result.data);
    result.data = sorted;

    free(dir);

    return result;
}

// windows version of the old API
// StringView pdListFiles
// (
//     StringView directory,
       char       *buf
// ){
//     WIN32_FIND_DATAA fileData;
//     HANDLE           foundHandle;
//
//     char *winPath         = malloc(path.size + 3);
//     for(uint16_t i = 0; i < path.size; ++i)
//     {
//         winPath[i] = path[i];
//     }
//     winPath[path.size]     = '\\';
//     winPath[path.size + 1] = '*';
//     winPath[path.size + 2] = '\0';
//
//     foundHandle = FindFirstFileA(winPath, &fileData);
//     if(foundHandle == INVALID_HANDLE_VALUE)
//     {
//         fprintf(stderr, "\n\033[31;1;7mERROR: failed to open directory.\033[0m\n");
//         return 0;
//     }
//
//     uint32_t listSize = 0;
//
//     while(FindNextFileA(foundHandle, &fileData))
//     {
//         uint8_t length = 0;
//         for(; length < 255 && fileData.cFileName[length] != '\0'; ++length)
//         {
//         }
//         listSize += length + 1;
//     }
//
//     FindClose(foundHandle);
//     foundHandle = FindFirstFileA(winPath, &fileData);
//
//     char     *list  = (char*)malloc(listSize + 1);
//     uint32_t offset = 0;
//
//     while(FindNextFileA(foundHandle, &fileData))
//     {
//         uint8_t length = 0;
//         for(; length < 255 && fileData.cFileName[length] != '\0'; ++length)
//         {
//             list[offset + length] = fileData.cFileName[length];
//         }
//         list[offset + length] = ';';
//         offset += length + 1;
//     }
//
//     list[offset] = '\0';
//
//     FindClose(foundHandle);
//     free(winPath);
//     return(StringView)
//     {
//         .data = list,
//     };
// }

#endif
