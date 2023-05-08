/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:52:52 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/07 20:25:55 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_char(char *input, int c)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == (char)c)
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_count_char(char *input, char c)
{
	size_t	i;
	size_t	count;

	i = -1;
	count = 0;
	while (input[++i])
	{
		if (input[i] == c)
			count++;
	}
	return (count);
}

int check_syntax(t_list *lst)
{
	while (lst)
	{
		if ((ft_lstsize(lst) == 1 && check_char("><|", lst->content[0]))
			|| check_char("><|", ft_lstlast(lst)->content[0]))
			return(syntax_error("`newline'", 0), 1);
		else if (check_char("><|", lst->content[0])
			&& (check_char("><|", lst->link->content[0])))
			return(syntax_error(lst->link->content, 0), 1);
		else if(check_char("><|", lst->content[0]) &&
			lst->link->content[0] == 32 && check_char("><|", lst->link->link->content[0]))
			return(syntax_error(lst->link->link->content, 0), 1);
		else if (check_char(lst->content, '(') || check_char(lst->content, ')'))
			return(syntax_error(lst->content, 0), 1);
		lst = lst->link;
	}
	return (0);
}