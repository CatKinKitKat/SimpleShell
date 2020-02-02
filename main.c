#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>

#define BUFFER_SIZE 1024
#define MAX_SIZE 8
#define DELIMITER " \t\r\n\a" //delimiters of strtok

/*
 * Current pid
 *      +
 * Current Directory
 */
pid_t pid;
static char cwd[BUFFER_SIZE];

/*
 * Change cureent Directory
 */
int changeCwd(char *where) {
    if (chdir(where) != 0) { //change
        return EXIT_FAILURE; //if cnt change
    } else {
        getcwd(cwd, sizeof(cwd)); //alter our var with dir
        return EXIT_SUCCESS;
    }
}

/*
 * Sets prompt with user@machine
 */
void Prompt() {

    char hostname[HOST_NAME_MAX];
    char username[HOST_NAME_MAX];
    char prompt[MAX_SIZE];

    strcpy(prompt, "\n");

    if (getlogin_r(username, HOST_NAME_MAX) == 0) {

        strcat(prompt, (char *) username); //get username if successful
        strcat(prompt, "@");

    }
    if (gethostname(hostname, HOST_NAME_MAX) == 0) {

        strcat(prompt, hostname);   //get machine if successful

    }
        strcat(prompt, "> "); //default


    printf("%s", prompt); //print

}

//Task 1-d
/*
 * Launches commands and takes care of processes (parent/children)
 */
int launcher(char **argVector) {

    const char quitCall[] = "quit";
    const char exitCall[] = "exit";
    const char cd[] = "cd";

    if (strcmp(argVector[0], quitCall) == 0 || strcmp(argVector[0], exitCall) == 0) { // exits quits, you know

        printf("Goodbye.\n"); //
        exit(EXIT_SUCCESS);

    } else if (strcmp(argVector[0], cd) == 0) {

        if (changeCwd(argVector[1])){
            changeCwd(getenv("HOME")); // if error return home, home sweet home
        }
        return EXIT_SUCCESS;

    }

    pid = fork(); // new process?

    if (pid == 0) { // if not execute command

        if (execvp(argVector[0], argVector) == -1) {

            printf("Unknown command. Is that elfish?\n"); //if our command fails to execute it most likely is missing or a typo
            return EXIT_FAILURE;
        }

    } else if (pid < 0) {

        printf("Error forking.\n"); // a colleague advised
        exit(EXIT_FAILURE);

    } else {
        while (pid == wait(NULL)); // waits indefinitely for our child to finish
        return EXIT_SUCCESS;
    }
}

//Task 1-c
/*
 * Creates a vector of arguments for our command
 */
void makeArgVector(char *command, char **argVector) {

    char *token;
    int arg = 0;
    token = strtok(command, DELIMITER); //first token
    argVector[arg] = (char *) calloc(sizeof(token), sizeof(char)); //calloc is so much better than malloc! it allocates and clears mem

    while (token != NULL) {

        strcpy(argVector[arg], token); //copy token

        arg++;
        token = strtok(NULL, DELIMITER); // next
        argVector[arg] = (char *) calloc(sizeof(token), sizeof(char)); // allocate next
    }

    argVector[arg] = (char *) NULL; // THIS WAS THE CULPRIT (I WAS USING STRCPY AND IT WAS A ****STORM)
    free(token); //frees mem
}

//Task 1-b
/*
 * Prints our arguments. It is the base of our makeArgVect
 */
int PrintArguments(char *command) {

    char *token;
    int args = 0;

    token = strtok(command, DELIMITER); //first token

    while (token != NULL) {
        printf("%s\n", token);
        args++;

        token = strtok(NULL, DELIMITER); //next
    }
    printf("%i\n", args);
    free(token); //frees mem

    return EXIT_SUCCESS;
}

/*
 * Parses our command to take care of ls, although we could have used launcher or cd here this was easier
 */
void parseCommand(char *command) {

    const char ls[] = "ls";

    if (strcmp(command, ls) == 0) { //if ls is empty -> ls the current directory

        strcat(command, " ");
        strcat(command, cwd);

    }

    char *argVector[BUFFER_SIZE]; //create our argVect
    makeArgVector(command, (char **) argVector);

    launcher((char **) argVector); //executes
}

// Task 1-a
/*
 * scanf hates spaces, fgets hates special chars, this was a ugly solution for a ugly problem
 */
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
/*
 * Gets our command and removes any \n if somehow it passed through my "scanLine great wall of trump"
 */
void GetCommand(char *command) {

    scanLine(command);
    if ((strlen(command) > 0) && (command[strlen(command) - 1] == '\n')) {
        command[strcspn(command, "\n")] = '\0';
    }
}

/*
 * Starts here, infinite loop like a good shell
 */
int main() {

    changeCwd(getenv("HOME")); //Start at home
    char *command = calloc(BUFFER_SIZE, sizeof(char)); //Allocates mem like malloc but better, it clears it

    do {
        *command = realloc(command, BUFFER_SIZE * sizeof(char)); //Reallocates every iteration

        Prompt(); //user@machine>

        GetCommand(command);

        if (strlen(command) <= 0) continue; //If it is empty skip

        parseCommand(command); //Parses and executes

    } while (1);

}