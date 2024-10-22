/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 17:00:08 by moudrib           #+#    #+#             */
/*   Updated: 2023/07/09 11:07:46 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	catching_signals(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 1;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	other_signals(int status)
{
	if (WTERMSIG(status) == SIGSEGV)
	{
		write(1, "Segmentation fault: 11\n", 24);
		g_exit_status = WTERMSIG(status) + 128;
	}
	else if (WTERMSIG(status) == SIGTERM)
	{
		write(1, "Terminated: 15\n", 16);
		g_exit_status = WTERMSIG(status) + 128;
	}
	else if (WTERMSIG(status) == SIGKILL)
	{
		write(1, "Killed: 9\n", 11);
		g_exit_status = WTERMSIG(status) + 128;
	}
	else if (WTERMSIG(status) == SIGBUS)
	{
		write(1, "Bus error: 10\n", 15);
		g_exit_status = WTERMSIG(status) + 128;
	}
	else if (WTERMSIG(status) == SIGQUIT)
	{
		write(1, "Quit: 3\n", 9);
		g_exit_status = WTERMSIG(status) + 128;
	}
}

void	exit_by_signal(void)
{
	int	status;

	while (wait(&status) != -1)
	{
	}
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			g_exit_status = WTERMSIG(status) + 128;
		}
		else if (WTERMSIG(status) == SIGABRT)
		{
			write(1, "Abort trap: 6\n", 15);
			g_exit_status = WTERMSIG(status) + 128;
		}
		else
			other_signals(status);
	}
}
