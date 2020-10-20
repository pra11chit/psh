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
    while(1) {
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

#define PSH_TOK_BUFSIZE 64
#define PSH_TOK_DELIM " \t\n"
char **psh_split_line(char *line) {
    int bufsize = PSH_TOK_BUFSIZE;
    char *token;
    char **tokens = malloc(sizeof(char*) * PSH_TOK_BUFSIZE);
    if (!tokens) {
        fprintf(stderr, "lsh : allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int position = 0;

    token = strtok(line,PSH_TOK_DELIM);
    while(token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += PSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
            fprintf(stderr, "lsh : allocation failed\n");
            exit(EXIT_FAILURE);
            }
        }
    }
    tokens[position] = NULL;
    return tokens;
}

int psh_launch(char **args) {
}

char *psh_loop() {
    char *line; //input
    char **args; //array of split arguments
    int status;

    do {
        printf("> ");
        line = psh_read_line();
        args = psh_split_line(line);
        //status = psh_execute(args);

        //free(line);
        //free(args);
    } while(status);
}

int main(int argc, char *argv[]) {
    psh_loop();
    return EXIT_SUCCESS;
}
