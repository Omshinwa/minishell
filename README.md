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


# redirections
echo 1 < in.txt
ls | grep "e">file
ls | grep "e">file <input.txt

test in bash: > not followed by a word, followed by && or || etc
ls | grep "e" && || echo 1

><>