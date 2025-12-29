#include "minishell.h"
#include "libft.h"

// History Storage
// 	The history list is an array of history entries.  A history entry is declared as follows:

// 	typedef void * histdata_t;

// 	typedef struct _hist_entry {
// 		char *line;
// 		char *timestamp;
// 		histdata_t data;
// 	} HIST_ENTRY;

// 	The history list itself might therefore be declared as

// 	HIST_ENTRY ** the_history_list;

// 	The state of the History library is encapsulated into a single structure:

// 	/*
// 	* A structure used to pass around the current state of the history.
// 	*/
// 	typedef struct _hist_state {
// 		HIST_ENTRY **entries; /* Pointer to the entries themselves. */
// 		int offset;           /* The location pointer within this array. */
// 		int length;           /* Number of elements within this array. */
// 		int size;             /* Number of slots allocated to this array. */
// 		int flags;
// 	} HISTORY_STATE;

// 	If the flags member includes HS_STIFLED, the history has been stifled.


volatile sig_atomic_t	g_ackowledge = 0;
// an atomic operation guarantees that every read/write from g_acknoledge are
// done in full before handling a signal.
// volatile prevents the compiler from making some optimizations

static void	acknowledge_sig(int signum)
{
	g_ackowledge = signum;
	// printf("%i received %i \n", getpid(), signum);
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int main(void)
{
	char *str;

	signal(SIGINT, acknowledge_sig);
	signal(SIGQUIT, SIG_IGN);
	// fork();
	while(1)
	{
		str = readline("👻>");
		if (!str)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (ft_strlen(str))
			add_history(str);
		free(str);
	}
	return (0);
}