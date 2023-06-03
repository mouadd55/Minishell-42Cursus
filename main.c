#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	main(void)
{
	int pipefd[2];
	int childPid;

	pipe(pipefd);
	/* child process, executes echo */
	childPid = fork();
	if (childPid == 0)
    {
	    char *av[] = {"/bin/echo", "h_e_l_l_o_ _w_o_r_l_d", NULL,};
		close(pipefd[0]); // closing unused read end
		dup2(pipefd[1], 1);
		close(pipefd[1]); // closing original write end, because it has been duplicated
		if (execv(av[0], av) == -1)
			perror("execv: ");
		exit(0);
	}
	/* parent process, wires up echo's output to tr's input */
	char *av[] = {"/usr/bin/tr", "-d", "_", NULL,};
	close(pipefd[1]); // closing unused write end
	dup2(pipefd[0], 0);
	close(pipefd[0]); // closing original read end, because it has been duplicated
	if (execv(av[0], av) == -1)
		perror("execv: ");
	return (0);
}
