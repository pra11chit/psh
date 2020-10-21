#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* shell built-ins forward declaration */
int psh_cd(char **args);
int psh_help(char **args);
int psh_exit(char **args);

/* */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[])(char **) = {
    &psh_cd,
    &psh_help,
    &psh_exit
};

/*      shell built-ins implementation     */
int psh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "psh : expected argument to \"cd\"\n");
    }
    else {
        if (chdir(args[1]) != 0) {
            perror("psh");
        }
    }
    return 1;
}

int psh_exit(char **args) {
    return 0;
}

int psh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int psh_help(char **args) {
    printf("Welcome to Pratik's psh shell\n");
    printf("Following are builtins : \n");
    for (int i = 0; i < psh_num_builtins(); i++) {
        printf(" %s\n", builtin_str[i]);
    }
}

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
            fprintf(stderr, "psh : allocation failed\n");
            exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, PSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int psh_launch(char **args) {
    pid_t pid, wpid;
    int status;
    pid = fork();

    if (pid == 0) {
        //child process
        if (execvp(args[0], args) == -1) {
            perror("psh");
        }   exit(EXIT_FAILURE);
    }
    else if (pid < 0) {
        perror("psh");
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

int psh_execute(char **args) {
    if (args[0] == NULL) {
        return 1;
    }

    for (int i = 0; i < psh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    return psh_launch(args);
}

void psh_loop() {
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
