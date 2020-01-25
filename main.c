#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void Prompt(int cycle) {

    const char *clc = (const char *) '\033[2J';
    char newline = '\n';

    if (cycle == 1) {

        write(STDOUT_FILENO, clc, 12);

    } else {

        printf("%c", newline);

    }

    printf("> ");
}

void GetCommand(char command[]) {

    char * envp[] = { (char *) "PATH=/bin", 0};

    const char exitCall[] = "quit";

    if (fork() != 0) {
        wait(NULL);
    } else {

        //execve(program,parameters,envp);
    }

    if (strcmp(command,exitCall) == 0) {
        exit(EXIT_SUCCESS);
    }

}

int PrintArguments(char command[]) {

    return 0;
}

void makeArgVector(char command[], char *argVector[]) {

}

int main() {

    int cycle = 0;
    char command[1024];

    do {
        cycle++;

        Prompt(cycle);
        scanf("%s", command);
        GetCommand(command);

    } while (cycle <= 1);

    return EXIT_SUCCESS;
}
