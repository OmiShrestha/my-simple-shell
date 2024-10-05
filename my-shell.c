/*
 Author: Omi Shrestha
 my-shell.c: My Simple Shell
 Program name: my-shell.c
 Compile: gcc -o my-shell my-shell.c
 Run: ./my-shell
 Description: This program is a simple shell program implemented in C, designed to mimic basic shell
 functionalities using system calls such as fork, execvp, and waitpid. The shell reads user input, executes
 commands, and handles built-in commands like cd, chdir, exit, and quit. It also supports running commands in the
 background and executing multiple commands in parallel.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARG_COUNT 100
#define MAX_COMMAND_COUNT 10

/**
 * Reads a line of input from the user and removes the newline character.
 * @param input Pointer to the buffer where the input will be stored.
 */
void read_input(char *input) {
    while (1) {  // Reads the command line
        if (fgets(input, MAX_INPUT_LENGTH, stdin) != NULL) {
            // Removes newline character from the input
            input[strcspn(input, "\n")] = 0;
            break;
        }
    }
}

/**
 * Changes the current working directory to the specified path.
 * @param path Pointer to the string representing the path to change to.
 */
void change_dir(char *path) {
    if (chdir(path) != 0) {
        perror("cd error");
    }
}

/**
 * Tokenizes the input string into separate commands based on the '#' delimiter.
 *
 * @param input Pointer to the input string to be tokenized.
 * @param cmds Array to store the tokenized commands.
 * @return The number of commands tokenized.
 */
int tok_cmds(char *input, char *cmds[]) {
    char *token;
    int cmd_count = 0;

    token = strtok(input, "#");
    while (token != NULL && cmd_count < MAX_COMMAND_COUNT) {
        cmds[cmd_count++] = token;
        token = strtok(NULL, "#");
    }

    return cmd_count;
}

/**
 * Tokenizes a command string into its individual arguments based on spaces.
 *
 * @param cmd Pointer to the command string to be tokenized.
 * @param args Array to store the tokenized arguments.
 * @return The number of arguments tokenized.
 */
int tok_args(char *cmd, char *args[]) {
    char *token;
    int arg_count = 0;

    token = strtok(cmd, " ");
    while (token != NULL && arg_count < MAX_ARG_COUNT) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    args[arg_count] = NULL; // Null-terminate the argument list
    return arg_count;
}

/**
 * Executes a command in a new process, with an option to wait for completion.
 *
 * @param args Array of arguments for the command to be executed.
 * @param last_cmd Flag indicating whether this is the last command in the input.
 */
void exec_cmd(char *args[], int last_cmd) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
    } else if (pid == 0) { // Child process, execute the command
        if (execvp(args[0], args) < 0) {
            perror("execvp error");
            exit(1);
        }
    } else { // Parent process
        if (last_cmd) { // Waits for last command to complete
            int status;
            waitpid(pid, &status, 0);
        } else { // Prints a message for the background process
            printf("Background process started with PID: %d\n", pid);
        }
    }
}

/**
 * Main function that initializes the shell, handles user input,
 * and executes commands.
 */
int main(void) {
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_ARG_COUNT];
    
    while (1) {
        printf("shrestha: "); // Display the prompt
        read_input(input);    // Reads the input
        
        // Handles "exit/quit" and "ch/chdir" commands
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            break;
        } else if (strncmp(input, "cd ", 3) == 0) {
            change_dir(input + 3); // changes directory
            continue;
        } else if (strncmp(input, "chdir ", 6) == 0) {
            change_dir(input + 6); // changes directory
            continue;
        }
        
        // Tokenizes the input into commands, handles parallel execution
        char *commands[MAX_COMMAND_COUNT];
        int cmd_count = tok_cmds(input, commands);
        
        // Executes commands
        for (int j = 0; j < cmd_count; j++) {
            char *cmd = commands[j];
            
            tok_args(cmd, args); // Tokenizes commands into arguments
            
            // Checks if the last command ends with #
            int last_cmd = (j == cmd_count - 1);
            if ((j == cmd_count - 1) && input[strlen(input)] == '#') {
                last_cmd = 0; // Does not wait for the last command if it ends with #
            }
            exec_cmd(args, last_cmd); // Call the execute_command function
        }
    }
    return 0;
}
