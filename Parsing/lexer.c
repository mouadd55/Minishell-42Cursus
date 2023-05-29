/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:22:22 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/29 17:39:30 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_lstsize_env(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->link;
	}
	return (count);
}

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

void	more_lexer_conditions(t_list *tmp)
{
	if (!ft_strcmp(tmp->content, "|"))
		tmp->type = ft_strdup("PIPE");
	else if (!ft_strcmp(tmp->content, " "))
		tmp->type = ft_strdup("space");
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
		tmp->type = ft_strdup("OUTFILE");
	else if (!ft_strcmp(tmp->content, "<"))
		tmp->type = ft_strdup("INFILE");
}

void	moooore_conditions(t_list *tmp)
{
	if (tmp->prev && tmp->prev->prev && (!ft_strcmp(tmp->prev->content, "<")
			|| !ft_strcmp(tmp->prev->prev->content, "<")
			|| !ft_strcmp(tmp->prev->content, ">")
			|| !ft_strcmp(tmp->prev->prev->content, ">")
			|| !ft_strcmp(tmp->prev->content, ">>")
			|| !ft_strcmp(tmp->prev->prev->content, ">>")))
		tmp->type = ft_strdup("FILE");
	else
		tmp->type = ft_strdup("COMMAND");
}

void	conditions(t_list *tmp)
{
	if ((tmp->prev && tmp->prev->prev && ft_strcmp(tmp->type, "PIPE") && (!ft_strcmp(tmp->type, "DOUBLE_Q")
				|| !ft_strcmp(tmp->type, "SINGLE_Q") || !ft_strcmp(tmp->type, "FLAG"))
			&& ((!ft_strcmp(tmp->prev->content, "<")
					|| !ft_strcmp(tmp->prev->prev->content, "<"))
				|| (!ft_strcmp(tmp->prev->content, ">")
					|| !ft_strcmp(tmp->prev->prev->content, ">"))
				|| (!ft_strcmp(tmp->prev->content, ">>")
					|| !ft_strcmp(tmp->prev->prev->content, ">>"))))
		|| ((tmp->prev && ft_strcmp(tmp->type, "PIPE") && !ft_strcmp(tmp->prev->type, "FILE")
					&& ft_strcmp(tmp->content, ">") && ft_strcmp(tmp->content,
						"<") && ft_strcmp(tmp->content, ">>")
					&& ft_strcmp(tmp->content, "<<") && ft_strcmp(tmp->content,
						" "))))
	{
		free(tmp->type);
		tmp->type = ft_strdup("FILE");
	}
}

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
		else if (!ft_strcmp(tmp->content, "|") || !ft_strcmp(tmp->content, " ")
				|| tmp->content[0] == '-' || tmp->content[0] == '\"'
				|| tmp->content[0] == '\'' || !ft_strcmp(tmp->content, "<<")
				|| !ft_strcmp(tmp->content, ">>") || !ft_strcmp(tmp->content,
				">") || !ft_strcmp(tmp->content, "<"))
			more_lexer_conditions(tmp);
		else if (tmp->prev && tmp->prev->prev && (!ft_strcmp(tmp->prev->content,
						"<<") || !ft_strcmp(tmp->prev->prev->content, "<<")))
			tmp->type = ft_strdup("DELIMITER");
		else
			moooore_conditions(tmp);
		conditions(tmp);
		tmp = tmp->link;
	}
}