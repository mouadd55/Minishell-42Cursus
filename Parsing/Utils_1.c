/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 15:38:39 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/21 17:08:46 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_alpha_num(char c)
{
	if (ft_isalpha(c) || check_char("0123456789", c))
		return (1);
	return (0);
}

int	is_space(int c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

void	index_list(t_list **list)
{
	t_list	*tmp;
	int		count;

	tmp = *list;
	count = 0;
	while (tmp)
	{
		tmp->pos = count;
		count++;
		tmp = tmp->link;
	}
}

char	*strlower(char *str)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strdup(str);
	while (tmp[i])
	{
		if (tmp[i] >= 'A' && tmp[i] <= 'Z')
			tmp[i] += 32;
		i++;
	}
	return (tmp);
}
