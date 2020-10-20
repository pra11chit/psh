#include <stdlib.h>
#include <stdio.h>

#define PSH_RL_BUFSIZE 1024
char *psh_read_line() {
    int bufsize = PSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    if (!buffer) {
        fprintf(stderr, "psh : allocation error\n");
        exit(EXIT_FAILURE);
    }

    int c;
    while(true) {
        c = getchar();
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        }
        else {
            buffer[position] = c;
        }
        position++;

        if (position >= bufsize) {
            bufsize += PSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "psh : allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}


char *psh_loop() {
    char *line; //input
    char **args; //array of split arguments
    int status;

    do {
        printf("> ");
        line = psh_read_line();
        args = psh_split_line(line);
        status = psh_execute(args);

        free(line);
        free(args);
    } while(status);
}

int main(int argc, char *argv[]) {
    psh_loop();
    return EXIT_SUCCESS;
}
