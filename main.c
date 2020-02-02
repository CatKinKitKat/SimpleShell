#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>

#define BUFFER_SIZE 1024
#define MAX_SIZE 8
#define DELIMITER " \t\r\n\a"

static char cwd[BUFFER_SIZE];

void flush() {
    fflush(stdin);
    fflush(stdout);
}

void freeArgVector(char **argVector) {
    for(int i = 0; i < sizeof(argVector) / sizeof(argVector[0]); i++){
        free(argVector[i]);
    }
}

void changeCwd(char *where) {
    if (chdir(where) != 0) {
        printf("Unknown location. Is that Narnia?\n");
    } else {
        getcwd(cwd, sizeof(cwd));
    }
}

void Prompt() {

    char hostname[HOST_NAME_MAX];
    char username[HOST_NAME_MAX];
    char prompt[MAX_SIZE];

    strcpy(prompt, "\n");

    if (getlogin_r(username, HOST_NAME_MAX) == 0) {

        strcat(prompt, (char *) username);
        strcat(prompt, "@");

    }
    if (gethostname(hostname, HOST_NAME_MAX) == 0) {

        strcat(prompt, hostname);
        strcat(prompt, "> ");

    } else {
        strcpy(prompt, "> ");
    }

    printf("%s", prompt);

}

int cdFunc(char **argVector) {

    if (strcmp(argVector[1], "\0") == 0) {

        changeCwd(getenv("HOME"));
        return EXIT_SUCCESS;

    } else {

        changeCwd(argVector[1]);
        return EXIT_SUCCESS;

    }
}

//Task 1-d
int launcher(char **argVector) {

    const char quitCall[] = "quit";
    const char exitCall[] = "exit";
    const char cd[] = "cd";

    if (strcmp(argVector[0], quitCall) == 0 || strcmp(argVector[0], exitCall) == 0) {

        int children = getpid();
        for (int c = 0; c < children; c++) {
            kill(c, SIGKILL);
        }

        printf("I quit.\n");
        exit(EXIT_SUCCESS);

    } else if (strcmp(argVector[0], cd) == 0) {

        cdFunc(argVector);
        return EXIT_SUCCESS;

    }

    pid_t pid;

    pid = fork();

    if (pid == 0) {


        if (execvp(argVector[0], argVector) == -1) {

            printf("Unknown command. Is that elfish?\n");
            freeArgVector(argVector);
            return EXIT_FAILURE;
        } else {

            freeArgVector(argVector);
            return EXIT_SUCCESS;
        }


    } else if (pid < 0) {

        printf("Error forking.\n");
        exit(EXIT_FAILURE);

    } else {
        while (pid == wait(NULL));
        return EXIT_SUCCESS;
    }
}

//Task 1-c
void makeArgVector(char command[], char *argVector[]) {

    char *token;
    int arg = 0;
    token = strtok(command, DELIMITER);
    argVector[arg] = (char *) calloc(sizeof(token), sizeof(char));

    while (token != NULL) {

        strcpy(argVector[arg], token);

        arg++;
        token = strtok(NULL, DELIMITER);
        argVector[arg] = (char *) calloc(sizeof(token), sizeof(char));
    }

    strcpy(argVector[arg], (const char *) "\0");
    free(token);
}

//Task 1-b
int PrintArguments(char command[]) {

    char *token;
    int args = 0;

    token = strtok(command, DELIMITER);

    while (token != NULL) {
        printf("%s\n", token);
        args++;

        token = strtok(NULL, DELIMITER);
    }
    printf("%i\n", args);
    free(token);

    return EXIT_SUCCESS;
}

void parseCommand(char command[]) {

    const char ls[] = "ls";

    if (strcmp(command, ls) == 0) {

        strcat(command, " ");
        strcat(command, cwd);

    }

    char argVector[MAX_SIZE][BUFFER_SIZE];
    **argVector = *(char *) calloc(sizeof(argVector), sizeof(char));
    makeArgVector(command, (char **) argVector);

    launcher((char **) argVector);
    flush();
}

// Task 1-a
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

// Task 1-a
void GetCommand(char command[]) {

    scanLine(command);
    if ((strlen(command) > 0) && (command[strlen(command) - 1] == '\n')) {
        command[strcspn(command, "\n")] = '\0';
    }
}

int main() {

    changeCwd(getenv("HOME"));

    do {
        char command[BUFFER_SIZE];
        *command = calloc(sizeof(BUFFER_SIZE), sizeof(char));

        Prompt();

        GetCommand(command);

        if (strlen(command) <= 0) continue;

        parseCommand(command);

    } while (1);

}