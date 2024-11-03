<a id="top"></a>

# Minishell

Minishell is a simple Unix shell implementation, the goal of this project is to create a functional shell that can execute commands, manage processes, and handle various shell features while demonstrating proficiency in system calls and process management.

## Table of Contents
- [About](#about)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

# About

Minishell allows users to execute commands similar to a standard shell environment. It demonstrates the core functionalities of a shell, including:
- Command parsing
- Execution of built-in commands
- Managing environment variables
- Handling input/output redirection
- Supporting piping between commands

# Features
- **Command Execution:** Execute both built-in and external commands.
- **Environment Variables:** Access and modify environment variables.
- **Input/Output Redirection:** Redirect input and output to and from files.
- **Pipes:** Connect commands using pipes to enable command chaining.
- **Signal Handling:** Gracefully handle signals like SIGINT.

# Installation

To set up this project locally, follow these steps:

1. **Clone the repository to your local machine:**
	```sh
	git clone https://github.com/KarlQuerel/42-minishell
	```

2. **Navigate to the project directory:**
	```sh
	cd 42-minishell
	```

3. **Compile the project:**
	```sh
	make
	```

4. **Run the shell:**
	```sh
	./minishell
	```

# Usage

Once the shell is running, you can start entering commands. Here are some examples:

- To execute a built-in command:
	```sh
	echo "Hello, Minishell!"
	```

- To set an environment variable:
	```sh
	export MY_VAR="Hello World"
	```

- To run an external command:
	```sh
	ls -l
	```

- To redirect output to a file:
	```sh
	echo "This is a test." > output.txt
	```

- To pipe commands:
	```sh
	cat file.txt | grep "search_term"
	```

- HEREDOC with variable expansion:
	```sh
	MY_VAR="world"
	cat << EOF
	Hello, $MY_VAR!
	EOF
	```

For more information, refer to the [subject PDF](https://github.com/KarlQuerel/42-minishell/blob/main/docs/en.subject.pdf).

[Back to Top](#top)
