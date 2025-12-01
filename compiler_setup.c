#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int index;
    char *name;
    char *command;
    char *version_flag;
    char *version;
    char **flags;
    char *description;
} Compiler;


int user_comp_num = 0;

static char *compilation_flags[] = {"-o", NULL};

static Compiler compilers[] = {
    {0, "gcc", "gcc", "--version", "0.0.0", compilation_flags, "GNU Compiler Collection"},
    {0, "clang", "clang", "--version", "0.0.0", compilation_flags, "LLVM Clang Compiler"},
    {0, "clang++", "clang++", "--version", "0.0.0", compilation_flags, "LLVM Clang C++ Compiler"},
    {0, "g++", "g++", "--version", "0.0.0", compilation_flags, "GNU C++ Compiler"},
    {0, "icc", "icc", "--version", "0.0.0", compilation_flags, "Intel C Compiler"},
    {0, "icpc", "icpc", "--version", "0.0.0", compilation_flags, "Intel C++ Compiler"},
    {0, "tcc", "tcc", "-v", "0.0.0", compilation_flags, "Tiny C Compiler"}
};

char *get_compiler_version(Compiler *compiler) {
    char command[512];
    snprintf(command, sizeof(command), "%s %s", compiler->command, compiler->version_flag);

    FILE *fp = popen(command, "r");
    if (!fp) return NULL;

    char *version = malloc(256);
    if (fgets(version, 256, fp)) {
        char *newline = strchr(version, '\n');
        if (newline) *newline = '\0';
    } else {
        free(version);
        version = strdup("0.0.0");
    }

    pclose(fp);
    return version;
}

bool is_compiler_available(Compiler *compiler) {
    char command[256];
    snprintf(command, sizeof(command), "which %s > /dev/null 2>&1", compiler->command);
    if (system(command) == 0) {
        char *new_version = get_compiler_version(compiler);
        if (new_version) {
            if (compiler->version && strcmp(compiler->version, "0.0.0") != 0) {
                free(compiler->version);
            }
            compiler->version = new_version;
        }
        return true;
    }
    return false;
}

void detect_compilers_from_list() {
    user_comp_num = 0;
    char command[256];

    printf("Compilers found:\n");

    for (int i = 0; compilers[i].name != NULL; i++) {
        bool is_comp_available = is_compiler_available(&compilers[i]);
        
        if (is_comp_available) {
            user_comp_num++;
            compilers[i].index = user_comp_num;
            printf("    [%d] %s (%s)\n", user_comp_num, compilers[i].version, compilers[i].description);
        } else {
            compilers[i].index = 0;
        }
    }
    printf("\n");
}

Compiler *select_compilers(char *file_name) {
    printf("Detecting compilers...\n");

    detect_compilers_from_list();

    printf("Select compilers to use during file compilation (1 - %d):\n", user_comp_num);

    char line[256];
    int selected_compilers[256] = {0};

    if (fgets(line, sizeof(line), stdin) != NULL) {
        int count = 0;

        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, " \t");
        while (token != NULL && count < 100) {
            selected_compilers[count++] = atoi(token);
            token = strtok(NULL, " \t");
        }
    }

    for (int i = 0; compilers[i].name != NULL; i++) {
        for (int j = 0; j < 256; j++) {
            if (selected_compilers[j] == compilers[i].index && compilers[i].index != 0) {
                printf("Using %s to compile %s.\n", compilers[i].name, file_name);
                return &compilers[i];
            }
        }
    }
    return NULL;
}