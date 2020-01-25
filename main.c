#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


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

void GetCommand(char command[]) {

    char * envp[] = { (char *) "PATH=/usr/bin", 0};

    const char exitCall[] = "quit";

    if (strcmp(command,exitCall) == 0) {

        printf("I quit.\n");
        exit(EXIT_SUCCESS);

    } else {
        printf("I am not a quitter.\n");
        //execve(program,parameters,envp);
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

/*
 * if (fork() != 0) {
 *      wait(NULL);
 * } else
 */