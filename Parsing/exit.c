/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:15:46 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/12 16:27:15 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	nbr_len(long nbr)
{
	int	len;

	len = 0;
	if (nbr <= 0)
		len++;
	while (nbr != 0)
	{
		len++;
		nbr /= 10;
	}
	return (len);
}

char	*ft_itoa(long long n)
{
	int			len;
	char		*str;
	long long	nb;

	nb = n;
	len = nbr_len(nb);
	str = malloc(len * sizeof(char) + 1);
	if (!str)
		return (NULL);
	if (nb < 0)
	{
		str[0] = '-';
		nb *= (-1);
	}
	if (nb == 0)
		str[0] = '0';
	str[len--] = '\0';
	while (nb != 0)
	{
		str[len--] = nb % 10 + 48;
		nb /= 10;
	}
	return (str);
}

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
		if (ft_isdigit(cmd[v.i]))
		{
			v.flag++;
			if (ft_strcmp(cmd[v.i], ft_itoa(ft_atoi(cmd[v.i]))))
			{
				ft_printf_fd("exit\nminishell: exit: %s: numeric argument required\n", 2,
					cmd[v.i]);
				exit(255);
			}
		}
		if (v.flag == 0)
		{
			ft_printf_fd("exit\nminishell: exit: %s: numeric argument required\n", 2,
				cmd[v.i]);
			exit(255);
		}
		else if ((v.flag == 2 && v.i == 2) || (v.flag == 1 && v.i == 2))
		{
			v.j++;
			ft_printf_fd("exit\nminishell: exit: too many arguments\n", 2);
		}
	}
	if (final && !final->link && !final->prev && v.j == 0)
	{
		if (v.flag == 1)
			exit(ft_atoi(cmd[1]));
		printf("exit\n");
		exit(255);
	}
}
