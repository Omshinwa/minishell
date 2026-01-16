// Note: this only works if the file already exists, otherwise you gotta change the flags of open
// to O_CREAT | O_WRONLY or whatever

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

int main(void)
{
	int fd = open("newfile.txt", O_WRONLY);
	dup2(fd, 1);
	printf("La chance favorise les audacieux");
}