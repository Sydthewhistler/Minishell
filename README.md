# minishell

A minimal UNIX command-line interpreter written in C, designed to mimic the core behavior of **bash**.

---

## Overview

`minishell` implements the essential features of a POSIX shell:  
interactive command reading, variable expansion, pipe and redirection handling, and a complete set of built-in commands.

---

## Features

| Category | Details |
|---|---|
| **External commands** | `$PATH` lookup, execution via `execve` |
| **Built-ins** | `echo`, `cd`, `pwd`, `env`, `export`, `unset`, `exit` |
| **Pipes** | Multiple chaining with `\|` |
| **Redirections** | `<` `>` `>>` and heredoc `<<` |
| **Expansion** | `$VAR`, `$?`, local variables |
| **Quoting** | Single `'…'` (literal) and double `"…"` (with expansion) |
| **Signals** | `Ctrl+C` (new prompt), `Ctrl+D` (exit), `Ctrl+\` (ignored) |
| **History** | Arrow-key navigation via `readline` |

---

## Project Structure

```
minishell/
├── includes/
│   ├── minishell.h        # Core types, constants and data structures
│   └── exec.h             # Execution engine prototypes
├── srcs/
│   ├── main.c             # Entry point, REPL loop
│   ├── parsing/           # Lexer, parser, expansion, quoting, heredoc
│   ├── exec/
│   │   ├── built_in/      # Built-in command implementations
│   │   ├── exec.c         # External command execution
│   │   ├── exec_master.c  # Pipe and redirection orchestration
│   │   └── manage_redirect.c
│   └── utils/             # Error handling, memory management, signals
└── libft/                 # Custom C utility library
```

---

## Dependencies

- `gcc` / `cc`
- `make`
- `libreadline` (readline-dev)

On Debian/Ubuntu:

```bash
sudo apt-get install libreadline-dev
```

---

## Build

```bash
# Build the project
make

# Remove object files
make clean

# Remove everything (objects + binary)
make fclean

# Rebuild from scratch
make re
```

---

## Usage

```bash
./minishell
```

```
minishell$ echo "Hello $USER"
Hello ceylan

minishell$ ls -la | grep ".c" | wc -l
26

minishell$ cat << EOF
> line 1
> line 2
> EOF
line 1
line 2

minishell$ export MY_VAR=42 && echo $MY_VAR
42

minishell$ exit
```

---

## Exit Codes

| Code | Meaning |
|---|---|
| `0` | Success |
| `1` | General error |
| `2` | Syntax error |
| `126` | Permission denied |
| `127` | Command not found |
| `130` | Interrupted by signal (`Ctrl+C`) |

The exit code of the last command is accessible via `$?`.

---

## Authors

Developed by **Sydney Cavallin** and **Coraline Prot**.
