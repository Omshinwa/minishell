
echo $USER >> out && cat out | grep		1
echo $USER >> out && cat out | grep $USER	wiwu\nwiwu\nwiwu	0

CONCERNANT LES SIGNAUX:
execve reset les customs signal handlers, mais il n'undo pas SIG_IGN.
Si on fait signal(machin, SIG_IGN), on doit le reset par SIG_DFL avant de execve.

Q: Tu peux me montrer ton Abstract syntax tree (un printer?).

valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp --track-fds=yes --trace-children=yes ./minishell

ulimit -n 10 to test a reasonable amount of pipes / redirections that fit under the limit


f1r4s7% cat << delimiter_1 &&
cmdand heredoc> this is the first cat
cmdand heredoc> delimiter_1
cmdand> cat << delimiter_2 && echo 'hold on this isnt all' ||       
cmdand cmdand cmdor heredoc> second cat
cmdand cmdand cmdor heredoc> delimiter_2
cmdand cmdand cmdor> echo "last"    

test just entering newlines for the trailing && / ||

# check which chars are valid or not for words / args
ex: / is like an alphanumerical char
