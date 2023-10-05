#!/bin/bash
#Create the file for the bash result
touch bash_result.txt

#Create the file for the minishell result
touch minishell_result.txt

# echo "Résultat de 'ls -als' dans Bash :"
# echo caro > bash_result.txt

n=1
while read line; do
echo "Line No. $n : " > bash_result.txt
$line > bash_result.txt
n=$((n+1))
done < tests.txt

cd ..
make re

# echo "Résultat de 'ls -als' dans Minishell :"
# echo caro | ./minishell > tester/minishell_result.txt

n=1
while read line; do
echo "Line No. $n : $line" | ./minishell > tester/minishell_result.txt
n=$((n+1))
done < tester/tests.txt

cd tester

diff bash_result.txt minishell_result.txt