# #!/bin/bash
# #Create the file for the bash result
# touch bash_result.txt

# #Create the file for the minishell result
# touch minishell_result.txt

# echo "Résultat de 'ls -als' dans Bash :"
# ls -als > bash_result.txt

# echo "Résultat de 'ls -als' dans Minishell :"
# echo "ls -als" | ./minishell > minishell_result.txt

# diff bash_result.txt minishell_result.txt





#!/bin/bash

touch bash_result.txt
touch minishell_result.txt

# echo caro > bash_result.txt

# echo caro | valgrind ../src/./minishell > minishell_result.txt

echo hola > bash_result.txt
cd ..
make re
echo hola | valgrind ./minishell > tester/minishell_result.txt
cd tester

diff bash_result.txt minishell_result.txt
