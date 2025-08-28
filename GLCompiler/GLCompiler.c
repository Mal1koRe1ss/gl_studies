#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char command[512];
    char extraFlags[128] = "";  

    if (argc < 2 || argc > 3) { 
        printf("Usage: %s filename [extra_flags]\n", argv[0]);
        exit(EXIT_FAILURE); 
    }

    // Handle optional extra flags
    if (argc == 3) {
        snprintf(extraFlags, sizeof(extraFlags), " %s", argv[2]);
    }

    printf("Compiling...");
    fflush(stdout);
    
    snprintf(command, sizeof(command), 
             "gcc %s.c -o%s glad.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl %s",
             argv[1], argv[1], extraFlags);
    
    int result = system(command);

    if (result != 0) {
        fprintf(stderr, "\nCompilation failed (error %d)\n", result);
        exit(EXIT_FAILURE);
    }

    printf("\nCompilation successful\n");
    return 0;
}
