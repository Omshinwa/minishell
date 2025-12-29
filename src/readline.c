#include "minishell.h"
#include "libft.h"

int main(void)
{
	char *str;
	while(1)
	{
		str = readline("👻>");
		if (ft_strlen(str))
			add_history(str);
	}
	// int rl_on_new_line(void);
	// str = readline("👻>");
}