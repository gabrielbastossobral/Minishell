# Minishell

As beautiful as a shell

## About

Minishell is the second project of the Unix branch at 42.
It's a minimum viable version of a real shell.
The main goal is to have a good understanding of process creation and
synchronisation using the C programmming language.
We build this in pairs as a team.

## Installation & Usage

### Building the program

1. Download/Clone this repo

        git clone https://github.com/gabrielbastossobral/minishell
2.  Git submodule the libft

        cd Minishell
        make set
3.  Run `make`

        make

### Running the program

After building the source, run `./minishell` from the project root.

## Main Project Instructions

### Mandatory

- Can only use C
- Must respect the school imposed coding style ([The Norme][norme-pdf])
- No memory leaks
- Implement a series of builtins: `echo`, `cd`, `setenv`, `unsetenv`, `env`, `exit`
- Manage the errors without using `errno`, by displaying a message adapted
to the error output
- Can only use these standard library functions:
    - malloc, free
    - access
    - open, close, read, write
    - opendir, readdir, closedir
    - getcwd, chdir
    - stat, lstat, fstat
    - fork, execve
    - wait, waitpid, wait3, wait4
    - signal, kill
    - exit
- Must have a Makefile to build the program
- The binary file must be named `minishell`
- Can use [Libft][libft-url]
- Handle program interruption (Ctrl + D)
