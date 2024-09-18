#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void processName(char **name) {
    // Create a copy of the name
    *name = "hh";
    printf("Name after processName: %s\n", *name);
}

int main() {
    char *name = "John Doe";
    processName(&name);
    printf("Original name after processName: %s\n", name);
    return 0;
}