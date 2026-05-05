# Minishell - As Beautiful as a Shell

<p align="center">
  <img src="https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=black" />
    <img src="https://img.shields.io/badge/Shell-000000?style=for-the-badge" />
      <img src="https://img.shields.io/badge/POSIX-FF6F00?style=for-the-badge" />
        <img src="https://img.shields.io/badge/Concurrency-00C853?style=for-the-badge" />
</p>p>

## Overview
**Minishell** is a collaborative project from the 42 School curriculum that involves building a fully functional command-line interpreter from scratch. It serves as an introduction to the inner workings of a shell, focusing on process management, system calls, and complex string parsing.

The project mimics the behavior of Bash, supporting essential features like pipes, redirections, and environment variable expansion.

---

## Key Features
- **Interactive Prompt**: Displays a prompt and waits for user input.
- - **Command Execution**: Handles absolute, relative, and PATH-based executables.
  - - **Pipes (`|`)**: Connects the output of one command to the input of the next.
    - - **Redirections**:
      -   - Input (`<`) and Output (`>`).
          -   - Append Mode (`>>`).
              -   - Here-doc (`<<`).
                  - - **Environment Variables**: Expansion of `$VAR` and `$?`.
                    - - **Signal Handling**: Correct behavior for `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`.
                     
                      - ---

                      ## Built-in Commands
                      - `echo` (with `-n` option)
                      - - `cd` (with relative or absolute paths)
                        - - `pwd`
                          - - `export`
                            - - `unset`
                              - - `env`
                                - - `exit`
                                 
                                  - ---

                                  ## Usage

                                  ### Compilation
                                  ```bash
                                  make
                                  ```

                                  ### Execution
                                  ```bash
                                  ./minishell
                                  ```

                                  ---

                                  ## Technical Skills
                                  - Process creation (`fork`)
                                  - - Inter-process communication (`pipe`, `dup2`)
                                    - - Signal management (`sigaction`)
                                      - - Complex parsing and tokenization
                                        - - Memory management (preventing leaks in long-running processes)
                                         
                                          - ---

                                          ## Author
                                          **Adil Bourji (adi7-x)**
                                          - GitHub: [@adi7-x](https://github.com/adi7-x)
                                          - - LinkedIn: [Adil Bourji](https://linkedin.com/in/adil-bourji)
                                           
                                            - ---
                                            <p align="center">Made for the 42 Network</p>p>
                                            
