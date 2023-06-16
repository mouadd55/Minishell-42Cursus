/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:15:46 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/16 13:14:17 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long long	ft_atoi(const char *str)
{
	long long	i;
	long long	sign;
	long long	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9' && str[i])
	{
		res *= 10;
		res += str[i] - 48;
		i++;
	}
	return (res * sign);
}

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

int	sign_case(t_vars *v, char **cmd)
{
	if (!ft_strcmp(cmd[1], "-0") && cmd[2] == 0)
	{
		printf("exit\n");
		exit(0);
	}
	else if (cmd[2] && ft_atoi(cmd[1]) == 0 && ft_atoi(cmd[2]) == 0)
	{
		ft_printf("exit\nminishell: exit: too many arguments\n", 2);
		return (1);
	}
	else if (cmd[v->i][0] == '+')
	{
		v->str = ft_itoa(ft_atoi(cmd[v->i]));
		if (ft_strcmp(cmd[v->i] + 1, v->str))
		{
			ft_printf("exit\nminishell: exit: %s: numeric argument required\n", 2,
				cmd[v->i]);
			free(v->str);
			exit(255);
		}
		if (!cmd[v->i + 1])
		{
			printf("exit\n");
			free(v->str);
			exit(ft_atoi(cmd[v->i]));
		}
		free(v->str);
	}
	return (0);
}

int	exit_errors(t_vars *v, char **cmd)
{
	if (ft_isdigit(cmd[v->i]))
	{
		v->flag++;
		v->str = ft_itoa(ft_atoi(cmd[v->i]));
		if (sign_case(v, cmd))
		{
			free(v->str);
			v->str = NULL;
			return (1);
		}
		else if (ft_strcmp(cmd[v->i], v->str))
		{
			ft_printf("exit\nminishell: exit: %s: numeric argument required\n", 2,
				cmd[v->i]);
			free(v->str);
			v->str = NULL;
			exit(255);
		}
	}
	if (v->flag == 0)
	{
		ft_printf("exit\nminishell: exit: %s: numeric argument required\n", 2,
			cmd[v->i]);
		if (v->str)
		{
			free(v->str);
			v->str = NULL;
		}
		exit(255);
	}
	else if ((v->flag == 2 && v->i == 2) || (v->flag == 1 && v->i == 2))
	{
		v->j++;
		ft_printf("exit\nminishell: exit: too many arguments\n", 2);
	}
	if (v->str)
	{
		free(v->str);
		v->str = NULL;
	}
	return (0);
}

void	ft_exit(char **cmd, t_cmd *final)
{
	t_vars	v;

	v.i = 0;
	v.j = 0;
	v.flag = 0;
	while (cmd && cmd[++v.i])
	{
		if (!ft_strcmp("-9223372036854775808", cmd[v.i]))
		{
			printf("exit\n");
			exit(255);
		}
		if (exit_errors(&v, cmd))
			return ;
	}
	if (final && !final->link && !final->prev && v.j == 0)
	{
		if (v.flag == 1)
		{
			printf("exit\n");
			exit(ft_atoi(cmd[1]));
		}
		printf("exit\n");
		exit(255);
	}
}
