#include "minishell.h"

int main(void)
{
	char *str;
	str = readline("👻");
	add_history(str);
	str = readline("👻");
}