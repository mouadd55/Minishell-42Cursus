/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 17:00:08 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/01 18:59:19 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	catching_signals(int sig)
{
	if (sig == SIGINT)
	{
	    printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();	
	}
}