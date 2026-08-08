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
// Returns a StringView created from buf.
StringView pdExpandPath
(
    StringView path,
    char       *buf
);

// returns parent path of the passed path. If an absolute path was passed, it will
// simply return a trimmed StringView from the same pointer. If the path needs to be
// resolved first, that resolved parent path will be written to `buf`.
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
