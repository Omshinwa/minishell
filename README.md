


if ( false || COND2 )


examples of commands to run:

cat README.md | grep "e" | wc -l
< logs.txt cat | grep "error" | wc > error_nb.txt -l
1    2      3  4  5     6     7  8 9   10         11

( echo "Hello World" || echo wesh ) && wc -l

< <<( echo 'test quotes' | cat 'like "Hello World " ' "ok" 'a' || echo wesh ) && wc > >>

< <<( echo 'test quotes' | cat 'like "Hello World " ' "ok" 'a' || echo

# command with in and out
grep foo < in.txt > out.txt
# multiple output redirections
echo hi > out1.txt >> out2.txt
command > out1 > out2

echo '$USER'
echo "$USER"
echo $USER


( echo "Hello World" || echo wesh ) && wc -l

cat 'like "Hello World"' && echo hahaha

( echo "Hello World" | echo wesh ) && wc -l

echo 1 && echo 2 && echo 3 || echo 4 && echo 5

( echo 1 && ( echo 2) && echo 3)|| echo 5

# logic operator
echo 1 && || echo 2

# redirections
echo 1 < in.txt
ls | grep "e">file
ls | grep "e">file <input.txt

ls | grep "e" && || echo 1
ls | grep "e" && >|| echo 1

><>

# check for () embedded in word, should throw error
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