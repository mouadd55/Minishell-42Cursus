/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 17:00:08 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/17 14:10:19 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	catching_signals(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 1;
		ft_printf("\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	exit_by_signal(void)
{
	int	status;

	while (wait(&status) != -1)
	{
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				g_exit_status = 130;
			else if (WTERMSIG(status) == SIGQUIT)
				g_exit_status = 131;
		}
	}
}