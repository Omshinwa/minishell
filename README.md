examples of commands to run:

cat README.md | grep "e" | wc -l
< logs.txt cat | grep "error" | wc > error_nb.txt -l
1    2      3  4  5     6     7  8 9   10         11

( echo "Hello World" || echo wesh ) && wc -l

< <<( echo 'test quotes' | cat 'like "Hello World " ' "ok" 'a' || echo wesh ) && wc > >>

< <<( echo 'test quotes' | cat 'like "Hello World " ' "ok" 'a' || echo