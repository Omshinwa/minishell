


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

what i am doing for non terminated command is:

tokenize everything, parse the tokens, then, when it realizes it's ending with an operator, call parse_rest_of_cmd()
ideally it would call a new prompt, read a new line, tokenize this newline, replace the current arr_token with the new one and keep going.

this line woud replace a reference
	*arr_token = tokenize(line);
	the problem is, it doesnt replace the arr_token for every stack right? only parse_simple_command() that called parse_rest_of_cmd() would get its reference of arr_token changed, if parse_command() has its own *arr_token, call parse_simple_command() (passing its *arr_token), then it gets changed inside parse_rest_of_cmd(), it will get changed in parse_simple_command() but not parse_command() right?

	I should create a struct instead that has a field for *arr_token (and also *index) and pass this struct around?