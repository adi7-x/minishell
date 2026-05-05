<p align="center">
  <img src="https://img.shields.io/badge/Score-101%2F100-brightgreen?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white" />
  <img src="https://img.shields.io/badge/42_Network-000000?style=for-the-badge&logo=42&logoColor=white" />
  <img src="https://img.shields.io/badge/Shell-Bash_Compatible-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white" />
</p>

<h1 align="center">🐚 Minishell</h1>

<p align="center">
  <i>As beautiful as a shell.</i>
  <br><br>
  A fully functional Unix shell written from scratch in C.<br>
  Handles pipes, redirections, environment variables, signals, and built-in commands —<br>
  replicating core <b>bash</b> behavior with strict memory discipline.
</p>

---

## 📋 Table of Contents

- [About](#-about)
- [Features](#-features)
- [Built-in Commands](#-built-in-commands)
- [Redirections & Pipes](#-redirections--pipes)
- [Getting Started](#-getting-started)
- [Usage Examples](#-usage-examples)
- [Architecture](#-architecture)
- [Signal Handling](#-signal-handling)
- [Testing](#-testing)
- [Project Structure](#-project-structure)
- [Author](#-author)

---

## 💡 About

**Minishell** is the most complex project in the 42 common core before the final exams. It requires building a bash-like shell that handles:

- **Tokenization** and **parsing** of user input
- **Abstract Syntax Tree (AST)** construction for command pipelines
- **Process creation** with `fork()` and `execve()`
- **File descriptor management** for redirections and pipes
- **Signal handling** (Ctrl+C, Ctrl+D, Ctrl+\\)
- **Environment variable** expansion and management

> This is a **team project** — building it deepens your understanding of how Unix actually works under the hood.

---

## ✨ Features

```
┌──────────────────────────────────────────────────────┐
│                  MINISHELL FEATURES                   │
├──────────────────────────────────────────────────────┤
│                                                      │
│  ✅ Interactive prompt with command history           │
│  ✅ Command execution via PATH resolution             │
│  ✅ Single and double quote handling                  │
│  ✅ Environment variable expansion ($VAR, $?)         │
│  ✅ Input redirection    ( < file )                   │
│  ✅ Output redirection   ( > file )                   │
│  ✅ Append redirection   ( >> file )                  │
│  ✅ Here-document        ( << DELIMITER )             │
│  ✅ Pipes                ( cmd1 | cmd2 | cmd3 )       │
│  ✅ Exit status tracking ( $? )                       │
│  ✅ Signal handling      ( Ctrl+C, Ctrl+D, Ctrl+\ )   │
│  ✅ 7 built-in commands                               │
│                                                      │
└──────────────────────────────────────────────────────┘
```

---

## 🔧 Built-in Commands

| Command | Syntax | Description |
|:--------|:-------|:------------|
| `echo` | `echo [-n] [text]` | Print text; `-n` suppresses trailing newline |
| `cd` | `cd [path]` | Change working directory |
| `pwd` | `pwd` | Print current working directory |
| `export` | `export [KEY=value]` | Set environment variable |
| `unset` | `unset [KEY]` | Remove environment variable |
| `env` | `env` | Display all environment variables |
| `exit` | `exit [status]` | Exit the shell with optional status code |

### Built-in Examples

```bash
minishell$ echo "Hello, 42!"
Hello, 42!

minishell$ echo -n "no newline"
no newlineminishell$

minishell$ cd /tmp && pwd
/tmp

minishell$ export MY_VAR="hello world"
minishell$ echo $MY_VAR
hello world

minishell$ unset MY_VAR
minishell$ echo $MY_VAR

minishell$ env | grep PATH
PATH=/usr/local/bin:/usr/bin:/bin

minishell$ exit 42
```

---

## 🔀 Redirections & Pipes

### Redirections

```bash
# Input redirection — read from file
< infile cat

# Output redirection — write to file (overwrite)
echo "hello" > outfile

# Append redirection — write to file (append)
echo "world" >> outfile

# Here-document — read until delimiter
cat << EOF
This is a heredoc
Multiple lines supported
EOF
```

### Pipes

```bash
# Simple pipe
ls -la | grep ".c"

# Multi-pipe
cat file.txt | grep "pattern" | wc -l

# Pipes with redirections
< infile grep "search" | sort | uniq > outfile
```

### Complex Combinations

```bash
# Pipe + redirection + built-in
env | grep USER > user_info.txt

# Here-doc + pipe
cat << STOP | wc -l
line one
line two
line three
STOP

# Multiple redirections
< input.txt sort > sorted.txt
```

---

## 🚀 Getting Started

### Prerequisites

- GCC compiler
- GNU Make
- `readline` library (`libreadline-dev` on Debian/Ubuntu)

### Installation

```bash
# Install readline (if needed)
sudo apt-get install libreadline-dev   # Debian/Ubuntu
brew install readline                   # macOS

# Compile
make

# Run
./minishell
```

### Makefile Targets

```bash
make          # Build minishell
make clean    # Remove object files
make fclean   # Remove everything
make re       # Full rebuild
```

---

## 🎮 Usage Examples

```bash
# Start the shell
./minishell

# Execute commands like bash
minishell$ ls -la
minishell$ cat /etc/hostname
minishell$ echo "Current dir: $PWD"

# Pipes
minishell$ ls | wc -l
minishell$ cat Makefile | grep "FLAGS" | head -1

# Redirections
minishell$ echo "test" > output.txt
minishell$ cat < output.txt
minishell$ ls -la >> log.txt

# Environment variables
minishell$ export NAME="Adil"
minishell$ echo "Hello $NAME, exit status was $?"

# Exit
minishell$ exit
```

### Quote Handling

```bash
# Single quotes — no expansion
minishell$ echo '$USER is not expanded'
$USER is not expanded

# Double quotes — expansion happens
minishell$ echo "$USER is expanded"
adil is expanded

# Mixed quotes
minishell$ echo "Hello '$USER'"
Hello 'adil'
```

---

## 🏗️ Architecture

```
┌──────────────────────────────────────────────────┐
│                  MINISHELL PIPELINE               │
│                                                  │
│  ┌──────────┐    ┌──────────┐    ┌────────────┐ │
│  │  INPUT   │ →  │  LEXER   │ →  │  PARSER    │ │
│  │ readline │    │ tokenize │    │ build AST  │ │
│  └──────────┘    └──────────┘    └─────┬──────┘ │
│                                        │        │
│                                        ▼        │
│  ┌──────────┐    ┌──────────┐    ┌────────────┐ │
│  │  OUTPUT  │ ←  │ EXECUTOR │ ←  │  EXPANDER  │ │
│  │  stdout  │    │ fork+exec│    │  $VAR → val│ │
│  └──────────┘    └──────────┘    └────────────┘ │
└──────────────────────────────────────────────────┘
```

1. **Lexer** → Breaks input into tokens (words, operators, quotes)
2. **Parser** → Builds command structure with redirections
3. **Expander** → Resolves `$VAR`, `$?`, and quote removal
4. **Executor** → Forks processes, sets up pipes/redirections, calls `execve()`

---

## 📡 Signal Handling

| Signal | Interactive Mode | During Execution |
|:-------|:----------------|:-----------------|
| `Ctrl+C` (SIGINT) | New prompt on new line | Interrupts child process |
| `Ctrl+D` (EOF) | Exits the shell | Closes stdin for child |
| `Ctrl+\` (SIGQUIT) | Ignored | Sends SIGQUIT to child |

---

## 🧪 Testing

### Quick Functional Tests

```bash
# Basic commands
ls -la
echo hello world
cat /etc/hostname

# Pipes
ls | grep Makefile | wc -l

# Redirections
echo test > /tmp/test_minishell
cat < /tmp/test_minishell

# Exit status
ls nonexistent_file
echo $?    # Should print non-zero

# Edge cases
""         # Empty quotes
''         # Empty single quotes
|          # Error: syntax error
```

### Memory Check

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --suppressions=readline.supp ./minishell
```

> The `readline.supp` file suppresses known readline library leaks that are not our responsibility.

### Recommended Testers

- [`minishell_tester`](https://github.com/LucasKu662/42-minishell-tester)

---

## 📁 Project Structure

```
minishell/
├── include/            # Header files
│   └── minishell.h
├── parsing/            # Lexer, parser, token handling
│   ├── tokenizer.c
│   ├── parser.c
│   ├── expander.c
│   └── quotes.c
├── execution/          # Command execution engine
│   ├── executor.c
│   ├── pipes.c
│   ├── redirections.c
│   └── builtins.c
├── functions_utils1/   # Utility functions
├── minishell.c         # Main loop
├── readline.supp       # Valgrind suppressions
├── Makefile
└── README.md
```

---

## 👤 Author

**Adil Bourji** — [@adi7-x](https://github.com/adi7-x)

<p align="center">
  <a href="https://github.com/adi7-x"><img src="https://img.shields.io/badge/GitHub-adi7--x-181717?style=flat-square&logo=github" /></a>
  <a href="https://linkedin.com/in/adil-bourji"><img src="https://img.shields.io/badge/LinkedIn-Adil_Bourji-0A66C2?style=flat-square&logo=linkedin" /></a>
</p>

<p align="center"><sub>42 School · Common Core · Unix · Processes</sub></p>
