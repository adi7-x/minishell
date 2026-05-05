<p align="center">
  <img src="https://img.shields.io/badge/Score-101%2F100-brightgreen?style=for-the-badge" />
    <img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white" />
      <img src="https://img.shields.io/badge/42_Network-000000?style=for-the-badge&logo=42&logoColor=white" />
        <img src="https://img.shields.io/badge/Shell-Bash_Compatible-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white" />
</p>p>

<h1 align="center">Minishell</h1>h1>

<p align="center">
  <i>As beautiful as a shell.</i>i>
    <br><br>
      A fully functional Unix shell written from scratch in C.<br>
        Handles pipes, redirections, environment variables, signals, and built-in commands --<br>
          replicating core bash behavior.
</p>p>

---

## Table of Contents

- [About](#about)
- - [Features](#features)
  - - [Built-in Commands](#built-in-commands)
    - - [Redirections and Pipes](#redirections-and-pipes)
      - - [Getting Started](#getting-started)
        - - [Architecture](#architecture)
          - - [Signal Handling](#signal-handling)
            - - [Testing](#testing)
              - - [Project Structure](#project-structure)
                - - [Author](#author)
                  -
                  - ---
                  -
                  - ## About
                  -
                  - **Minishell** is the most complex project in the 42 common core. It requires building a bash-like shell that handles tokenization, parsing, process creation with fork() and execve(), file descriptor management, signal handling, and environment variable expansion.
                  -
                  - ---
                  -
                  - ## Features
                  -
                  - - Interactive prompt with command history
                    - - Command execution via PATH resolution
                      - - Single and double quote handling
                        - - Environment variable expansion ($VAR, $?)
                          - - Input redirection ( < file )
                            - - Output redirection ( > file )
                              - - Append redirection ( >> file )
                                - - Here-document ( << DELIMITER )
                                  - - Pipes ( cmd1 | cmd2 | cmd3 )
                                    - - Exit status tracking ( $? )
                                      - - Signal handling ( Ctrl+C, Ctrl+D, Ctrl+\ )
                                        - - 7 built-in commands
                                          -
                                          - ---
                                          -
                                          - ## Built-in Commands
                                          -
                                          - | Command | Syntax | Description |
                                          - |:--------|:-------|:------------|
                                          - | echo | echo [-n] [text] | Print text; -n suppresses newline |
                                          - | cd | cd [path] | Change directory |
                                          - | pwd | pwd | Print working directory |
                                          - | export | export [KEY=value] | Set environment variable |
                                          - | unset | unset [KEY] | Remove environment variable |
                                          - | env | env | Display environment variables |
                                          - | exit | exit [status] | Exit the shell |
                                          -
                                          - ---
                                          -
                                          - ## Redirections and Pipes
                                          -
                                          - ```bash
                                            # Input redirection
                                            < infile cat

                                            # Output redirection
                                            echo "hello" > outfile

                                            # Append
                                            echo "world" >> outfile

                                            # Here-document
                                            cat << EOF
                                            multi-line input
                                            EOF

                                            # Pipes
                                            ls -la | grep ".c" | wc -l

                                            # Combined
                                            < infile grep "search" | sort > outfile
                                            ```

                                            ---

                                            ## Getting Started

                                            ### Prerequisites

                                            - GCC, GNU Make
                                            - - readline library
                                              -
                                              - ```bash
                                                sudo apt-get install libreadline-dev
                                                ```

                                                ### Compilation and Usage

                                                ```bash
                                                make
                                                ./minishell

                                                minishell$ ls -la
                                                minishell$ echo "Hello $USER"
                                                minishell$ cat Makefile | grep FLAGS
                                                minishell$ exit
                                                ```

                                                ---

                                                ## Architecture

                                                1. Lexer -- Breaks input into tokens
                                                2. 2. Parser -- Builds command structure
                                                   3. 3. Expander -- Resolves $VAR, $?, quotes
                                                      4. 4. Executor -- Forks processes, sets up pipes/redirections
                                                         5.
                                                         6. ---
                                                         7.
                                                         8. ## Signal Handling
                                                         9.
                                                         10. | Signal | Interactive Mode | During Execution |
                                                         11. |:-------|:----------------|:-----------------|
                                                         12. | Ctrl+C | New prompt | Interrupts child |
                                                         13. | Ctrl+D | Exits shell | Closes stdin |
                                                         14. | Ctrl+\ | Ignored | SIGQUIT to child |
                                                         15.
                                                         16. ---
                                                         17.
                                                         18. ## Testing
                                                         19.
                                                         20. ```bash
                                                             # Memory check (with readline suppression)
                                                             valgrind --leak-check=full --suppressions=readline.supp ./minishell
                                                             ```

                                                             ---

                                                             ## Project Structure

                                                             ```
                                                             minishell/
                                                             |-- include/
                                                             |-- parsing/
                                                             |-- execution/
                                                             |-- functions_utils1/
                                                             |-- minishell.c
                                                             |-- readline.supp
                                                             |-- Makefile
                                                             |-- README.md
                                                             ```

                                                             ---

                                                             ## Author

                                                             **Adil Bourji** -- [@adi7-x](https://github.com/adi7-x)

                                                             <p align="center">
                                                               <a href="https://github.com/adi7-x"><img src="https://img.shields.io/badge/GitHub-adi7--x-181717?style=flat-square&logo=github" /></a>a>
                                                                 <a href="https://linkedin.com/in/adil-bourji"><img src="https://img.shields.io/badge/LinkedIn-Adil_Bourji-0A66C2?style=flat-square&logo=linkedin" /></a>a>
                                                             </p>p>
                                                             
                                                             <p align="center"><sub>42 School - Common Core - Unix - Processes</sub>sub></p>p>
                                                             </sub></i>
