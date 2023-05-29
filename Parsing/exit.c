/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:15:46 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/29 19:31:25 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	if (str[i] && (str[i] == '-' || str[i] == '+'))
		if (!str[++i])
			return (0);
	while (str[i])
		if (!(str[i] >= '0' && str[i++] <= '9'))
			return (0);
	return (1);
}

void	ft_exit(char **cmd, t_list *list)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (cmd[++i])
	{
		if (ft_isdigit(cmd[i]))
			flag++;
		if (flag == 0)
		{
			printf("exit\nminishell: exit: %s: numeric argument required\n", cmd[i]);
			exit (0);
		}
		else if (flag == 2 && i == 2)
			printf("exit\nminishell: exit: too many arguments\n");
	}
}
