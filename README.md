# TurtleShell

*People say that there is no place like home. Well, for that reason I carry my $HOME with me.* - Turtle

## Build Instructions
After entering the directory :
```
make
./TurtleShell
```

## Features
- Supports `;` separated multiple commands in a single line.
- Stores history of the last 20 commands.
- Supports the following environment variables - `SHELL` `HOME` `PWD` `HOST` `USER`
- Supports background processes with by appending ` &` at the end of the command.
- Commands:
  - `ls` - Can take `[-al]` as the flags and `[directory names]` as arguments.
  - `echo` - Can take `[$ENV_VAR]` or `[custom_text]` as the argument.
  - `pwd`- Does not take any arguments. Prints the Current Working Directory.
  - `cd` - Takes the absolute path or the relative path as an argument.
  - `pinfo` - Can or cannot take arguments. When no arguments are given, it prints the details of the process of the shell. When a valid process id is given as the argument, it prints the details of that process.
  - `nightswatch` - Syntax is `nightswatch -n [number_of_seconds] [interrupt/newborn]`. It prints the given argument every `number_of_seconds` in a man-page like style.
  - `history` - Prints the last 10 commands if no argument is given. Else prints the last `arg` commands.
  - `exit` or `quit` - Exits the shell.
  - Input/Output redirection alongside Piping for commands is supported. 
  - `setenv var [value]` - Sets the environment variable that lasts till the shell is running.
  - `unsetenv var` - Unsets the environment variable.
  - `jobs` - Lists all the jobs that the shell has spawned that are running in the background.
  - `kjobs <job number> <signal number>` - Used to send a signal to a background process.
  - `fg <job number>` - Brings a background executing process to foreground.
  - `bg <job number>` - Resuemes a stopped background process in background itself.
  - `overkill` - Terminates all the background jobs.
  - Can handle signals like `SIGINT` and `SIGSTP`.
  - `cd -` - Supports the last working directory.
  - Prints a coloured prompt indicating the successful/unsuccesful run of the previous command.

All the `.c` files have a corresponding `.h` file (except `main.c`) which contain all the function declarations.

#### `bg_proc_list.c`
Contains a simple array like data structure to keep track of the names and process ids of the process that are being executed in background (which is invoked with an `&` at the end of a command).

#### `cd.c`
Contatins the code for changing the present working directory in the shell.

#### `ctrl_c.c`
Contains the code to handle an interrupt signal.

#### `echo.c`
Contains the code print out the arguments passed in echo command. It supports the common environment variables as well - HOME, SHELL, PWD, USER, HOST.

#### `execute.c`
Contains all the code for executing the commands that haven't been explicitly coded i.e all except `cd`, `ls`, `pwd`, `echo`, etc. Also contains the code for executing the commands in background.

#### `header.h`
The header file that includes all the Library Inclusions at one place. By including this, there isn't a need to include the libraries again and again in every file.

#### `history.c`
Contains the code for the execution of `history` command which stores the history accross multiple sessions of the shell in a local file `history.txt`. It stores only the latest 20 commands.

#### `ls.c`
Contains the code to execute the `ls` command along with inclusions of `-a` and `-l` flags.

#### `main.c`
Contains the main function and the basic functions used in the shell loop.

#### `nightswatch.c`
Contains a man-page like implemntation of the nightswatch command. Upon execution, terminal switches to non-canonical mode for the execution of this command and then switches back. `q` needs to be pressed to exit. There isn't a need to press `Enter` after `q`.

#### `pinfo.c`
Contains the code to execute the `pinfo` command which tells about the information of an ongoing process when the process id is passed as an argument.

#### `prompt.c`
Contains code to send a prompt which has the username, hostname and the current working directory.

#### `pwd.c`
Contains the code to change the present working directory.
