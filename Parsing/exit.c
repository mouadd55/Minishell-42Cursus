/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:15:46 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/22 20:51:13 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_typee(char *type)
{
	if (ft_strcmp(type, "APPEND") && ft_strcmp(type, "INPUT")
		&& ft_strcmp(type, "OUTPUT") && ft_strcmp(type, "HEREDOC")
		&& ft_strcmp(type, "FILE") && ft_strcmp(type, "DELIMITER")
		&& ft_strcmp(type, "space"))
		return (1);
	return (0);
}

void	join_nodes_of_type_arg(t_vars *v)
{
	while (v->tmp1 && v->tmp1->type[0] == 's')
		v->tmp1 = v->tmp1->link;
	while (v->tmp1 && v->tmp1->type[0] == 'a' && check_type(v->tmp1->type))
	{
		v->vars++;
		if (v->tmp1->type[0] == 'D')
			v->var = ft_strjoin(v->var, ft_strtrim(v->tmp1->content, "\""));
		else if (v->tmp1->type[0] == 'S')
			v->var = ft_strjoin(v->var, ft_strtrim(v->tmp1->content, "\'"));
		else
			v->var = ft_strjoin(v->var, v->tmp1->content);
		v->tmp1 = v->tmp1->link;
	}
}

t_list	*lexer_for_exit(t_list **list)
{
	t_list	*tmp;

	tmp = *list;
	tmp = tmp->link->link;
	while (tmp && check_type(tmp->type))
	{
		if (tmp->type[0] != 's')
		{
			free(tmp->type);
			tmp->type = ft_strdup("arg");
		}
		tmp = tmp->link;
	}
	return ((*list)->link->link);
}


void	ft_exit(t_list **list)
{
	t_vars	v;

	v.tmp1 = *list;
	v.vars = 0;
	while (v.tmp1)
	{
		if (!ft_strcmp(v.tmp1->type, "PIPE"))
			return ;
		v.tmp1 = v.tmp1->link;
	}
	v.tmp1 = *list;
	while (v.tmp1 && ft_strcmp(v.tmp1->content, "exit"))
		v.tmp1 = v.tmp1->link;
	v.tmp1 = v.tmp1->link;		
	while (v.tmp1 && check_typee(v.tmp1->type))
		v.tmp1 = v.tmp1->link;
	// lexer_for_exit(&v.tmp1);
	// while (v.tmp1)
	// {
	// 	v.var = NULL;
	// 	join_nodes_of_type_arg(&v);
	// }
}

// if (v.tmp1->link && v.tmp1->link->link
// 	&& (!check_type(v.tmp1->link->type) || !check_type(v.tmp1->link->link->type)))
// 	printf("exit\n");
// else if (v.tmp1->link && v.tmp1->link->link)
// 	printf("exit\nminishell: exit: %s: numeric argument required\n",
// 		v.tmp1->link->link->content);
// else
// 	printf("exit\n");
// exit(0);