/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:22:22 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/13 10:33:50 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (tmp->prev && !tmp->prev->prev && (!ft_strcmp(tmp->prev->content, "<")
			|| !ft_strcmp(tmp->prev->content, ">")
			|| !ft_strcmp(tmp->prev->content, ">>")))
		tmp->type = ft_strdup("FILE");
	else if (tmp->prev && tmp->prev->prev && (!ft_strcmp(tmp->prev->content,
				"<") || !ft_strcmp(tmp->prev->prev->content, "<")
			|| !ft_strcmp(tmp->prev->content, ">")
			|| !ft_strcmp(tmp->prev->prev->content, ">")
			|| !ft_strcmp(tmp->prev->content, ">>")
			|| !ft_strcmp(tmp->prev->prev->content, ">>")))
		tmp->type = ft_strdup("FILE");
	else
		tmp->type = ft_strdup("COMMAND");
}

void	conditions_for_files(t_list *tmp)
{
	if (((tmp->prev && tmp->prev->prev && ft_strcmp(tmp->type, "PIPE")
				&& (!ft_strcmp(tmp->type, "DOUBLE_Q") || !ft_strcmp(tmp->type,
						"SINGLE_Q") || !ft_strcmp(tmp->type, "FLAG"))
				&& ((!ft_strcmp(tmp->prev->content, "<")
						|| !ft_strcmp(tmp->prev->prev->content, "<"))
					|| (!ft_strcmp(tmp->prev->content, ">")
						|| !ft_strcmp(tmp->prev->prev->content, ">"))
					|| (!ft_strcmp(tmp->prev->content, ">>")
						|| !ft_strcmp(tmp->prev->prev->content, ">>"))))
			|| ((tmp->prev && ft_strcmp(tmp->type, "PIPE")
					&& !ft_strcmp(tmp->prev->type, "FILE")
					&& ft_strcmp(tmp->content, ">") && ft_strcmp(tmp->content,
						"<") && ft_strcmp(tmp->content, ">>")
					&& ft_strcmp(tmp->content, "<<") && ft_strcmp(tmp->content,
						" "))) || (tmp->prev && !tmp->prev->prev
				&& (!ft_strcmp(tmp->prev->content, "<")
					|| !ft_strcmp(tmp->prev->content, ">")
					|| !ft_strcmp(tmp->prev->content, ">>"))))
		&& ft_strcmp(tmp->type, "space"))
	{
		free(tmp->type);
		tmp->type = ft_strdup("FILE");
	}
}

void	conditions_for_delimiter(t_list *tmp)
{
	if (((tmp->prev && tmp->prev->prev && ft_strcmp(tmp->type, "PIPE")
				&& (!ft_strcmp(tmp->type, "DOUBLE_Q") || !ft_strcmp(tmp->type,
						"SINGLE_Q") || !ft_strcmp(tmp->type, "FLAG"))
				&& ((!ft_strcmp(tmp->prev->content, "<<")
						|| !ft_strcmp(tmp->prev->prev->content, "<<"))))
			|| ((tmp->prev && ft_strcmp(tmp->type, "PIPE")
					&& !ft_strcmp(tmp->prev->type, "DELIMITER")
					&& ft_strcmp(tmp->content, ">") && ft_strcmp(tmp->content,
						"<") && ft_strcmp(tmp->content, ">>")
					&& ft_strcmp(tmp->content, "<<") && ft_strcmp(tmp->content,
						" "))) || (tmp->prev && !tmp->prev->prev
				&& !ft_strcmp(tmp->prev->content, "<<")))
		&& ft_strcmp(tmp->type, "space"))
	{
		free(tmp->type);
		tmp->type = ft_strdup("DELIMITER");
	}
}

void	first_conditions(t_list *tmp)
{
	if (tmp->prev == NULL && ft_strcmp(tmp->content, ">>")
		&& ft_strcmp(tmp->content, ">") && ft_strcmp(tmp->content, "<")
		&& ft_strcmp(tmp->content, "<<") && tmp->content[0] != '\"'
		&& tmp->content[0] != '\'')
		tmp->type = ft_strdup("COMMAND");
	else if (!ft_strcmp(tmp->content, "|") || !ft_strcmp(tmp->content, " ")
		|| tmp->content[0] == '-' || tmp->content[0] == '\"'
		|| tmp->content[0] == '\'' || !ft_strcmp(tmp->content, "<<")
		|| !ft_strcmp(tmp->content, ">>") || !ft_strcmp(tmp->content,
			">") || !ft_strcmp(tmp->content, "<"))
		more_lexer_conditions(tmp);
	else if ((tmp->prev && tmp->prev->prev
			&& (!ft_strcmp(tmp->prev->content, "<<")
				|| !ft_strcmp(tmp->prev->prev->content, "<<")))
		|| (tmp->prev && !ft_strcmp(tmp->prev->content, "<<")))
		tmp->type = ft_strdup("DELIMITER");
	else
		moooore_conditions(tmp);
}
