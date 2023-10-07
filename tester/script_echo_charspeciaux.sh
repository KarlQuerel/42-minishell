#!/bin/bash
#Create the file for the bash result
touch bash_echo_charspeciaux.txt

# echo "a#b" > bash_echo_charspeciaux.txt
#echo a#b > bash_echo_charspeciaux.txt
# printf "\n" > bash_echo_charspeciaux.txt

# syntax error near unexpected token `('
# echo "a(b" > bash_echo_charspeciaux.txt
# echo a(b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt

# syntax error near unexpected token `)'
# echo "a)b" > bash_echo_charspeciaux.txt
# echo a)b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt

# echo "a*b" > bash_echo_charspeciaux.txt
# echo a*b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt

#a
#b: command not found
# echo "a;b" > bash_echo_charspeciaux.txt
#echo a;b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt

#a puis attend
# echo "a<b" > bash_echo_charspeciaux.txt
echo a<b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt

#attend
# echo "a>b" > bash_echo_charspeciaux.txt
echo a>b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt

#>
# echo "a`b" > bash_echo_charspeciaux.txt
echo a`b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt

# echo "a~b" > bash_echo_charspeciaux.txt
echo a~b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt

#>
# echo "a"b" > bash_echo_charspeciaux.txt
echo a"b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt

#ab
# echo "a\b" > bash_echo_charspeciaux.txt
echo a\b > bash_echo_charspeciaux.txt
# echo "\n" > bash_echo_charspeciaux.txt