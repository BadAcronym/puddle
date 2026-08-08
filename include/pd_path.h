#ifndef PD_PATH_H
#define PD_PATH_H

#include <stdint.h>

#ifdef BUILD_LINUX
    #include <sys/stat.h>
    #include <dirent.h>
#endif

#include "string_view.h"

#define PD_TYPE_ERROR     0
#define PD_TYPE_FILE      1
#define PD_TYPE_DIRECTORY 2
#define PD_TYPE_OTHER     3

// will verify if a given path exists. Will return `PD_TYPE_ERROR`, if it does not
// exist, `PD_TYPE_FILE` or `PD_TYPE_DIRECTORY` if it does, and `PD_TYPE_OTHER` in
// any other case.
uint8_t pdVerifyPath
(
    StringView path
);

// will expand the given path into buf, resolving '.', '$HOME' and '~', but
// currently no other symbols.
StringView pdExpandPath
(
    const char *path,
    char       *buf
);

StringView pdParentPath
(
    StringView path,
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
