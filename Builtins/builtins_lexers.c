/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_lexers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:29:52 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/20 08:24:43 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_builtins(t_list *list, t_env *envr, t_cmd *f_list, int length)
{
	t_list	*tmp;
	t_env	*env_copy;

	tmp = list;
	if (f_list && !ft_strcmp(f_list->cmd[0], "cd"))
		change_dir(&envr, f_list);
	else if (list && !(list)->prev && (list)->link
		&& (list)->link->type[0] == 's' && !strcmp("unset", (list)->content))
		unset(&list, &envr);
	if (!ft_strcmp(f_list->cmd[0], "export") && f_list->cmd[1] == 0)
	{
		env_copy = ft_copy_env_list(envr);
		sort_env(env_copy, f_list->fd_out);
		ft_destroy_list_env(&env_copy);
	}
	if (export_parsing(&list, &envr, length))
		return ;
	if (f_list->cmd && f_list->cmd[0] && !ft_strcmp(f_list->cmd[0], "exit"))
		ft_exit(f_list->cmd, f_list);
	else if (f_list->cmd && f_list->cmd[0] && !ft_strcmp(f_list->cmd[0], "env"))
		env_parsing(f_list->cmd, envr, f_list->fd_out);
	echo(f_list);
	pwd(f_list, envr);
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
