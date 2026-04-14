#include "string_view.h"

#include <stdio.h>

int main
(
    void
){
    StringView test = cstr_sv("Hello, World!");
    printf("\n");
    return 0;
}
