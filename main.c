#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>

#define BUFFER_SIZE 64
#define MAX_SIZE 16
#define DELIMITER " \t\r\n\a"

pid_t pid, old_pid;

static char cwd[BUFFER_SIZE];

void flush() {
    fflush(stdin);
    fflush(stdout);
}

void changeCwd(char *where) {
    chdir(where);
    getcwd(cwd, sizeof(cwd));
}

void Prompt() {

    char hostname[HOST_NAME_MAX];
    char prompt[MAX_SIZE];

    if (gethostname(hostname, HOST_NAME_MAX) == 0) {

        strcpy(prompt, "\n");
        strcat(prompt, hostname);
        strcat(prompt, "> ");

    } else {
        strcpy(prompt, "\n> ");
    }

    printf("%s", prompt);

}

int cdFunc(char **argVector) {

    if (strcmp(argVector[1], "\0") == 0) {

        printf("Specify where.\n");
        return EXIT_FAILURE;

    } else {

        if (chdir(argVector[1]) != 0) {

            printf("Command Failed.\n");
            return EXIT_FAILURE;
        } else {
            changeCwd(argVector[1]);
            return EXIT_SUCCESS;
        }
    }
}

//Task 1-d
int launcher(char **argVector) {

    char *envp[] = {(char *) "PATH=/bin", 0};
    const char cd[] = "cd";

    if (strcmp(argVector[0], cd) == 0) {

        cdFunc(argVector);
        flush();
        return EXIT_SUCCESS;

    } else {
        if (execvp(argVector[0], argVector) == -1) {

            printf("?\n");
            flush();
            return EXIT_SUCCESS;
        }
    }

    return EXIT_SUCCESS;
}

//Task 1-c
void makeArgVector(char command[], char *argVector[]) {

    char *token;
    int arg = 0;
    token = strtok(command, DELIMITER);
    argVector[arg] = malloc(sizeof(token));

    while (token != NULL) {

        strcpy(argVector[arg], token);
        //printf("%s\n", argVector[arg]);

        arg++;
        token = strtok(NULL, DELIMITER);
        argVector[arg] = malloc(sizeof(token));
    }

    //argVector[arg + 1] = malloc(sizeof(token));
    strcpy(argVector[arg], "\0");
    //printf("%i\n%lu\n", arg, sizeof(argVector));
}

//Task 1-b
int PrintArguments(char command[]) {
    //Not a good name

    char *token;
    int args = 0;

    token = strtok(command, DELIMITER);

    while (token != NULL) {
        //printf("%s\n", token);
        args++;

        token = strtok(NULL, DELIMITER);
    }
    printf("%i\n", args);

    return args;
}

//Task 1-a
void GetCommand(char command[]) {

    const char quitCall[] = "quit";
    const char exitCall[] = "exit";
    const char ls[] = "ls";

    if (strcmp(command, quitCall) == 0 || strcmp(command, exitCall) == 0) {
        int children = getpid();
        printf("%i\n", children);

        for (int c = 0; c < children; c++) {
            kill(c, SIGKILL);
        }

        printf("I quit.\n");
        exit(EXIT_SUCCESS);

    } else if (strcmp(command, ls) == 0) {

        char dir[BUFFER_SIZE];
        getcwd(dir, sizeof(dir));

        strcat(command, " ");
        strcat(command, dir);

    }

    //int size = PrintArguments(command) + 1;
    char *argVector[MAX_SIZE];
    makeArgVector(command, argVector);
    launcher(argVector);
    //free(*argVector);

}

char transformer(char c) {

    if (c == '-') {
        c = '-';
    } else if (c == '/') {
        c = '/';
    }

    return c;
}

char *scanLine(char *buffer) {
    int size = BUFFER_SIZE;
    char *p = buffer;
    int count = 0;
    do {
        char c;
        scanf("%c", &c);
        if (c == '\r' || c == '\n' || c == '\0' || c == EOF) {
            *p = '\0';
            break;
        }
        *p++ = c;
        count++;
        if (count >= size) {
            size += BUFFER_SIZE;
            buffer = realloc(buffer, size);
        }
    } while (1);
    return buffer;
}

int main() {

    int status;

    changeCwd(getenv("HOME"));

    do {
        char command[BUFFER_SIZE];
        *command = malloc(BUFFER_SIZE);

        Prompt();

        //fgets(command, sizeof(command), stdin);
        scanLine(command);
        if ((strlen(command) > 0) && (command[strlen(command) - 1] == '\n')) {
            command[strlen(command) - 1] = '\0';
        }

        pid = fork();
        if (pid == 0) {
            GetCommand(command);
        } else if (pid < 0) {
            printf("Error Forking.\n");
            return EXIT_FAILURE;
        } else {
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

    } while (1);

}