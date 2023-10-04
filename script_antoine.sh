#!/bin/bash
#Create the file for the bash result
touch bash_result.txt

#Create the file for the minishell result
touch minishell_result.txt

# echo "Résultat de 'ls -als' dans Bash :"
# ls -als > bash_result.txt

n=1
while read line; do
echo "Line No. $n : $line" > bash_result.txt
n=$((n+1))
done < tests.txt

# echo "Résultat de 'ls -als' dans Minishell :"
# echo "ls -als" | ./minishell > minishell_result.txt

n=1
while read line; do
echo "Line No. $n : $line" | ./minishell > minishell_result.txt
n=$((n+1))
done < tests.txt

diff bash_result.txt minishell_result.txt