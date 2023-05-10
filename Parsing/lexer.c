/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:22:22 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/10 09:55:07 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	redirections(t_list *tmp)
// {
// }

void	lexer(t_list **list)
{
	t_list	*tmp;

	tmp = *list;
	while (tmp)
	{
		if (tmp->prev == NULL && ft_strcmp(tmp->content, ">>")
			&& ft_strcmp(tmp->content, ">") && ft_strcmp(tmp->content, "<<")
			&& tmp->content[0] != '\"' && tmp->content[0] != '\'')
			tmp->type = ft_strdup("COMMAND");
		else if (!ft_strcmp(tmp->content, "|"))
			tmp->type = ft_strdup("PIPE");
		else if (!ft_strcmp(tmp->content, " "))
			tmp->type = ft_strdup("SPACE");
		else if (tmp->content[0] == '-')
			tmp->type = ft_strdup("FLAG");
		else if (tmp->content[0] == '\"')
			tmp->type = ft_strdup("DOUBLE_Q");
		else if (tmp->content[0] == '\'')
			tmp->type = ft_strdup("SINGLE_Q");
		else if (!ft_strcmp(tmp->content, "<<"))
			tmp->type = ft_strdup("HEREDOC");
		else if (!ft_strcmp(tmp->content, ">>"))
			tmp->type = ft_strdup("APPEND");
		else if (!ft_strcmp(tmp->content, ">"))
			tmp->type = ft_strdup("OUTPUT");
		else if (!ft_strcmp(tmp->content, "<"))
			tmp->type = ft_strdup("INPUT");
		else if (tmp->prev && tmp->prev->prev && (!ft_strcmp(tmp->prev->content, "<")
				|| !ft_strcmp(tmp->prev->prev->content, "<")
				|| !ft_strcmp(tmp->prev->content, ">")
				|| !ft_strcmp(tmp->prev->prev->content, ">")
				|| !ft_strcmp(tmp->prev->content, ">>")
				|| !ft_strcmp(tmp->prev->prev->content, ">>")))
			tmp->type = ft_strdup("FILE");
		else if (tmp->prev && tmp->prev->prev && (!ft_strcmp(tmp->prev->content, "<<")
				|| !ft_strcmp(tmp->prev->prev->content, "<<")))
			tmp->type = ft_strdup("DELIMITER");
		else
			tmp->type = ft_strdup("COMMAND");
		tmp = tmp->link;
	}
}
