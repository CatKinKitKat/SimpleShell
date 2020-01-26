#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


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

int PrintArguments(char command[]) {

    const char space[] = " ";
    char * token;

    token = strtok(command, space);

    while (token != NULL) {
        printf("%s\n", token);

        token = strtok(NULL, space);
    }

    return EXIT_SUCCESS;
}

void GetCommand(char command[]) {

    char * envp[] = { (char *) "PATH=/usr/bin", 0};

    const char exitCall[] = "quit";

    if (strcmp(command,exitCall)==0) {

        printf("I quit.\n");
        exit(EXIT_SUCCESS);

    } else {
        //printf("%s", command);
        PrintArguments(command);
        //execve(command,"",envp);
    }
}

void makeArgVector(char command[], char *argVector[]) {

}

int main() {

    int cycle = 0;
    char command[1024];

    do {
        cycle++;

        Prompt(cycle);

        fgets(command,1024,stdin);                                               // WHY NOT A PRINTF -> BECAUSE PRINTF SUCKS, IT IGNORES SPACES; my poor cin >> var;
        if ((strlen(command)>0) && (command[strlen(command)-1] == '\n')) {
            command[strlen(command) - 1] = '\0';                                    // WHY THIS -> BECAUSE FGETS SUCKS, IT DOES NOT ADD A \0 (null); my poor cin >> var;
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