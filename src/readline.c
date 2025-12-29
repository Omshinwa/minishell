#include "minishell.h"

int main(void)
{
	char *str;
	str = readline("👻");
	printf("%s", str);
}