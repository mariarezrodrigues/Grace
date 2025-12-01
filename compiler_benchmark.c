#include <stdio.h>
#include <stdlib.h>
#include "compsetup.h"

void compile(Compiler *compiler, char *filename) {
    char command[256];

    snprintf(command, sizeof(command), "%s %s -o sourcefile_%s", compiler->command, filename, compiler->name);
    if (system(command) == 0) printf("Successfully compiled %s.\n", filename);
}

int main() {
    char *filename = "source_file.c";

    FILE *source_file;
    source_file = fopen(filename, "r");

    Compiler *selected_compilers = select_compilers(filename);

    if (selected_compilers != NULL) {
        for (int i = 0; i < 100; i++){
            compile(&selected_compilers[i], filename);
        }
        free(selected_compilers);
    } else {
        printf("No compiler selected or found.\n");
    }

    fclose(source_file);
    return 0;
}