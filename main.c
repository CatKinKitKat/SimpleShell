#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define MAX_SIZE 16
#define DELIMITER " \t\r\n\a"

void Prompt(int cycle) {

    char newline = '\n';
    const char *clc = (const char *) '\033[2J';

    if (cycle == 1) {

        write(STDOUT_FILENO, clc, 12);

    } else {

        printf("%c", newline);

    }

    printf("> ");
}

int cdFunc(char **argVector) {
    if (strcmp(argVector[1],"\0") == 0) {
        printf("Specify where.\n");
    } else {
        if (chdir(argVector[1]) != 0) {
            printf("Command Failed.\n");
        }
    }
    return 1;
}

//Task 1-d
int forkLauncher(char **argVector) {

    char *envp[] = {(char *) "PATH=/bin", 0};
    char path[MAX_SIZE] = "/bin/";

    strcat(path,argVector[0]);

    pid_t pid;

    pid = fork();
    if (pid == 0) {
        if (execv(path, argVector) == -1) {
            printf("No such command.");
        }
        return EXIT_FAILURE;
    } else if (pid < 0) {
        printf("Error Forking.\n");
    } else {
        wait(NULL);
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
        //printf("%s\n", token);
        strcpy(argVector[arg], token);
        printf("%s\n", argVector[arg]);
        arg++;
        token = strtok(NULL, DELIMITER);
        argVector[arg] = malloc(sizeof(token));
    }

    argVector[arg + 1] = malloc(sizeof(token));
    strcpy(argVector[arg + 1], "\0");
    printf("%s\n", argVector[arg+1]);

    //printf("%i\n", arg);


}

//Task 1-b
int PrintArguments(char command[]) {
    //Not a good name

    char *token;
    int args = 0;

    token = strtok(command, DELIMITER);

    while (token != NULL) {
        printf("%s\n", token);
        args++;

        token = strtok(NULL, DELIMITER);
    }
    printf("%i\n", args);

    return EXIT_SUCCESS;
}

//Task 1-a
void GetCommand(char command[]) {

    const char exitCall[] = "quit";

    if (strcmp(command, exitCall) == 0) {
        int children = getpid();
        printf("%i\n", children);

        for (int c = 0; c < children; c++) {
            kill(c, SIGKILL);
        }

        printf("I quit.\n");
        exit(EXIT_SUCCESS);

    } else {

        char *argVector[MAX_SIZE];
        makeArgVector(command, argVector);
        forkLauncher(argVector);
    }
}

int main() {

    int cycle = 0;
    char command[BUFFER_SIZE];

    do {
        cycle++;

        Prompt(cycle);

        fgets(command,1024,stdin);
        if ((strlen(command)>0) && (command[strlen(command)-1] == '\n')) {
            command[strlen(command) - 1] = '\0';
        }

        GetCommand(command);

    } while (cycle > 0);

    return EXIT_SUCCESS;
}

/*
 * if (fork() != 0) {
 *      wait(NULL);
 * } else
 */