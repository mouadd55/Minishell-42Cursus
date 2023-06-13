/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_lexers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:29:52 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/13 10:30:32 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*lexer_for_unset(t_list **list)
{
	t_list	*tmp;

	tmp = *list;
	tmp = tmp->link->link;
	while (tmp && check_type(tmp->type))
	{
		if (tmp->type[0] != 's')
		{
			free(tmp->type);
			tmp->type = ft_strdup("VAR");
		}
		tmp = tmp->link;
	}
	return ((*list)->link->link);
}

void	lexer(t_list **list)
{
	t_list	*tmp;

	tmp = *list;
	while (tmp)
	{
		first_conditions(tmp);
		conditions_for_files(tmp);
		conditions_for_delimiter(tmp);
		tmp = tmp->link;
	}
}
