# my-simple-shell

This project is a simple shell program implemented in C, designed to mimic basic shell functionalities using
system calls such as fork, execvp, and waitpid. The shell reads user input, executes commands, and handles
built-in commands like cd, chdir, exit, and quit. It also supports running commands in the background and executing
multiple commands in parallel.

## How to Run

1. Compile the program:
   gcc -o my-shell my-shell.c
   
2. Run the program:
   ./my-shell

## Features Implemented

### Required Features
1. **Prompt Display**: The shell displays a prompt in the format `<last name>:`.
2. **Read Command Line**: The shell reads a command line from standard input and processes it.
3. **Execute Commands**: The shell starts a process to execute the command and waits for it to complete.
4. **Built-in Commands**: 
   - `cd`: Changes the current working directory.
   - `chdir`: Also changes the current working directory.
   - `exit`: Terminates the shell.
   - `quit`: Also terminates the shell.

### Advanced Features
1. **Background Execution**: The shell can execute commands in the background using the `#` symbol.
2. **Parallel Command Execution**: The shell can execute multiple commands separated by the `#` symbol, with the shell waiting only for the last command to complete.

## Features Not Completed

- **Parallel Execution**: While the shell starts multiple commands separated by `#`, it currently does execute all of them correctly in parallel. The  issue is that when the command ends in a '#' (e.g. sleep 5 #), it waits for a program before the '#' to complete, and then finally goes to accept next user input. It does not have that issue when there are multiple #'s between commands and the last command ends in a '#'.

