/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 12:48:36 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/13 10:39:01 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	join_nodes_of_type_var(t_vars *v)
{
	while (v->tmp1 && v->tmp1->type[0] == 's')
		v->tmp1 = v->tmp1->link;
	while (v->tmp1 && v->tmp1->type[0] == 'V' && check_type(v->tmp1->type))
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

void	delete_node_in_middle(int position, t_vars *v)
{
	while (position > 1)
	{
		v->temp1 = v->temp1->link;
		position--;
	}
	v->temp2 = v->temp1->prev;
	v->temp2->link = v->temp1->link;
	v->temp1->link->prev = v->temp2;
	free(v->temp1->key);
	if (v->temp1->value)
		free(v->temp1->value);
	free(v->temp1);
	v->temp1 = NULL;
}

void	delete_node(t_env **env, int position)
{
	t_vars	v;

	v.temp1 = *env;
	if (position == 1)
	{
		*env = (*env)->link;
		free((*env)->prev->key);
		if ((*env)->prev->value)
			free((*env)->prev->value);
		free((*env)->prev);
		(*env)->prev = NULL;
	}
	else if (position > 1 && position < ft_lstsize_env(*env))
		delete_node_in_middle(position, &v);
	else if (position == ft_lstsize_env(*env))
	{
		v.temp1 = ft_lstlast_env(*env);
		v.temp2 = v.temp1->prev;
		free(v.temp1->key);
		if (v.temp1->value)
			free(v.temp1->value);
		free(v.temp1);
		v.temp2->link = NULL;
	}
}

void	remove_variable(t_vars *v, t_env **env)
{
	v->i = 0;
	v->temp1 = *env;
	if (ft_lstsize_env(*env) == 1)
	{
		ft_destroy_list_env(env);
		return ;
	}
	while (v->temp1)
	{
		v->i++;
		if (v->temp1 && !ft_strcmp(v->var, v->temp1->key))
		{
			delete_node(env, v->i);
			return ;
		}
		v->temp1 = v->temp1->link;
	}
}

void	unset(t_list **list, t_env **env)
{
	t_vars	v;

	v.vars = 0;
	v.tmp1 = *list;
	if (search_for_pipe(&v))
		return ;
	if (!env || !(*env) || ft_lstsize_env(*env) == 0)
		return ;
	v.tmp1 = lexer_for_unset(list);
	while (v.tmp1)
	{
		v.var = NULL;
		join_nodes_of_type_var(&v);
		if (check_valid_var(v.var) || (v.var && v.var[0] == '-'))
		{
			printf("minishell: unset: `%s': not a valid identifier\n", v.var);
			if (v.vars == 1 && v.var[0] == '-')
				return ;
		}
		else
			remove_variable(&v, env);
		free(v.var);
	}
}
