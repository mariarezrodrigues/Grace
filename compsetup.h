#ifndef COMP_H_
#define COMP_H_
#include <stdbool.h>

typedef struct {
    char *name;
    char *command;
    char *version_flag;
    char *version;
    char *description;
} Compiler;

char *get_compiler_version(Compiler *compiler);
bool is_compiler_available(Compiler *compiler);
void detect_compilers_from_list();
Compiler *select_compilers();

#endif