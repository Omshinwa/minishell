CONCERNANT LES SIGNAUX:
execve reset les customs signal handlers, mais il n'undo pas SIG_IGN.
Si on fait signal(machin, SIG_IGN), on doit le reset par SIG_DFL avant de execve.

Q: Tu peux me montrer ton Abstract syntax tree (un printer?).

valgrind --leak-check=full --show-leak-kinds=all --suppressions=.readline.supp --track-fds=yes --trace-children=yes ./minishell

ulimit -n 10 to test a reasonable amount of pipes / redirections that fit under the limit

# command with in and out
grep foo < in.txt > out.txt

# multiple output redirections
echo hi > out1.txt >> out2.txt
command > out1 > out2

# quotation marks
echo '$USER'
echo "$USER"
echo $USER
echo mygod'$USER'whythis
echo 'like "Hello World"' && echo hahaha

# OPERATORS
echo 1 && echo 2 && echo 3 || echo 4 && echo 5
( echo 1 && ( echo 2) && echo 3)|| echo 4
# fail
echo 1 && || echo 2

# redirections
echo 1 < in.txt
ls | grep "e">file
ls | grep "e">file <input.txt

#fail
ls | grep "e" && >|| echo 1

><>

# pipe

cat README.md | grep "e" | wc -l

< logs.txt cat | grep "error" | wc > error_nb.txt -l

# parenthesis
( echo "Hello World" || echo wesh ) && ls -l

( echo "Hello World" || echo wesh ) && wc -l

# check for () embedded in word, should throw error

echo ( 1
echo ) 1
echo ( 1 )

# this is not valid
ls | | ls

# this is valid
ls | < TODO | ls

wtf is a stdin && multiline (sorry) > what
wtf is a stdin && echo e(sorry)e > what
wtf is a stdin && echo '(sorry" > what
test && echo | ( )

# multiline

cat << EOF || cat << . &&

> 1
> EOF
> 2
> .
> cat << ??
> why pain
> ??
1
why pain

echo 1 || echo 2 && echo 3 || echo 4 && echo 5
vs
echo 1 || echo 2 &&                                 
cmdor cmdand> echo 3 || echo 4 &&       
cmdor cmdand cmdor cmdand> echo 5

f1r4s7% cat << delimiter_1 &&
cmdand heredoc> this is the first cat
cmdand heredoc> delimiter_1
cmdand> cat << delimiter_2 && echo 'hold on this isnt all' ||       
cmdand cmdand cmdor heredoc> second cat
cmdand cmdand cmdor heredoc> delimiter_2
cmdand cmdand cmdor> echo "last"    

test just entering newlines for the trailing && / ||


echo 1 && (
	echo 2)

echo 1 &&
	(echo 2)

# check which chars are valid or not for words / args
ex: / is like an alphanumerical char

# variable expansion

bash-5.1$ cat << $USER
> $PATH
> $USER