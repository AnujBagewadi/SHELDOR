# SHELDOR - Simple Shell Program
### Author: Anuj Bagewadi
### Date: 4 September 2024

## Overview
- SHELDOR is a simple shell program, inspired by the fictional "Sheldor the Conqueror." It is designed as a minimalistic command-line interface that allows users to execute basic shell commands. This project is intended as a foundational example of a custom shell written in C, without advanced features such as pipelines or redirection.

## Features
- Built-in Commands:

  1. cd - Change the current directory.
  2. help - Display information about SHELDOR's built-in commands.
  3. exit - Exit the SHELDOR shell.
- External Command Execution: Executes standard system commands by forking a child process and using "execvp" for command execution.

- Simple Parsing and Tokenization: Reads and tokenizes input from the user, enabling basic command parsing.

## Limitations
- SHELDOR is a basic shell and does not support advanced shell functionalities such as:

  - Pipelines (e.g., ls | grep txt)
  - Redirection (e.g., ls > output.txt)

## Usage
- Clone the repository and compile the code:
  - gcc SHELL.c -o sheldor

- Run the shell:
  - ./sheldor

## Within the shell, you can use both built-in and external commands by typing them and pressing Enter. To exit, type exit.
