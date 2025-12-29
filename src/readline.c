#include "minishell.h"

int main(void)
{
	char *str = "salut";
	str = readline("👻");
	printf("%s", str);
}
