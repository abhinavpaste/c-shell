# navsh — A POSIX Shell for Linux

A Unix shell built from scratch in C using low-level POSIX system calls.
Built to understand how shells actually work under the hood — process creation,
inter-process communication, signal handling, and job control.

## Features

- **Command execution** — runs any binary on `$PATH` via `fork` and `execvp`
- **Pipes** — connects commands via `pipe()` and `dup2()` e.g. `ls | grep foo`
- **Built-in commands** — `cd`, `exit` run directly inside the shell process
- **Signal handling** — shell survives `Ctrl+C` and `Ctrl+Z`, child processes respond normally
- **Dynamic prompt** — shows current working directory e.g. `navsh:~/projects>`
- **Modular architecture** — signal handling, builtins, and pipe logic each in separate modules

## Build
```bash
git clone https://github.com/yourusername/navsh.git
cd navsh
make
```

## Run
```bash
./navsh
```

## Usage
```bash
# Run any command
navsh:~> ls -la
navsh:~> pwd

# Navigate directories
navsh:~> cd /tmp
navsh:/tmp> cd ~
navsh:~>

# Pipes
navsh:~> ls | grep .c
navsh:~> cat main.c | grep include
navsh:~> echo hello world | grep hello

# Exit
navsh:~> exit
```

## Project Structure
```
navsh/
├── main.c              # shell loop — read, parse, fork, exec
├── builtin.c           # built-in commands: cd, exit
├── builtin.h
├── pipe.c              # pipe parsing and execution
├── pipe.h
├── signal_handler.c    # signal setup for parent and child
├── signal_handler.h
└── Makefile
```

## How It Works

### Command Execution
Every external command follows the same flow:
1. `fgets` reads raw input from the user
2. `strtok` splits it into an args array
3. `fork()` creates a child process
4. Child calls `execvp()` to replace itself with the target program
5. Parent calls `waitpid()` to block until the child finishes

### Pipes
For a command like `ls | grep foo`:
1. `pipe()` creates a file descriptor pair — a read end and a write end
2. Child 1 (`ls`) has its stdout rewired to the write end via `dup2`
3. Child 2 (`grep`) has its stdin rewired to the read end via `dup2`
4. Both unused ends are closed to signal EOF correctly
5. Parent waits for both children to finish

### Signal Handling
The shell and its child processes need different signal behavior:
- **Shell** ignores `SIGINT` (Ctrl+C) and `SIGTSTP` (Ctrl+Z) so it cannot be accidentally killed
- **Children** have signals reset to default after `fork` but before `exec` so Ctrl+C correctly kills the running command
- This reset is necessary because children inherit the parent's signal handlers on fork

### Built-in Commands
`cd` and `exit` cannot be forked because:
- `cd` must change the shell's own working directory — a child changing its directory has no effect on the parent
- `exit` must terminate the shell process itself — a child exiting just exits the child
Both are intercepted before the fork and executed directly inside the shell process.

## System Calls Used

| Call | Purpose |
|------|---------|
| `fork` | Create child process |
| `execvp` | Replace child with target program |
| `waitpid` | Parent waits for child to finish |
| `pipe` | Create inter-process communication channel |
| `dup2` | Rewire file descriptors for pipes |
| `sigaction` | Register signal handlers |
| `chdir` | Change working directory (cd builtin) |
| `getcwd` | Get current directory for prompt |

## What I Learned

- How Unix process creation and execution works at the system call level
- How pipes are implemented using file descriptors and why closing unused ends matters
- Why certain commands must be shell builtins and cannot be forked
- How signal inheritance works between parent and child processes
- How to structure a C project across multiple modules with clear separation of concerns

## Future plans -

## Reference

Built with reference to:
- [Writing a Unix Shell — Indradhanush Gupta](https://igupta.in/blog/writing-a-unix-shell-part-1/)
- [Linux Man Pages](https://linux.die.net/man/)


## Prerequisites

Make sure you have the following installed:
```bash
sudo apt install build-essential    # gcc and make
```

> **Note:** navsh is built for Linux. It works on any Linux system or WSL (Windows Subsystem for Linux). It will not work on Windows natively.

## Installation
```bash
# Clone the repo
git clone https://github.com/abhinavpaste/navsh.git

# Enter the project directory
cd navsh

# Build
make

# Run
./navsh
```

## Uninstall
```bash
# Remove the compiled binary and object files
make clean
```
```
