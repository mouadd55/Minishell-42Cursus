/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 15:47:40 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/16 19:02:43 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	join_variable_values(t_vars *v)
{
	if (v->tmp1 && v->tmp1->type[0] == 'P')
	{
		v->flag = 1;
		v->tmp1 = v->tmp1->link;
	}
	if (v->tmp1 && v->tmp1->link && v->tmp1->link->type[0] == 'v')
		v->tmp1 = v->tmp1->link;
	while (v->tmp1 && v->tmp1->type[0] == 'v' && check_type(v->tmp1->type))
	{
		v->tmp = ft_strtrim(v->tmp1->content, "\"\'");
		v->val = ft_strjoin(v->val, v->tmp);
		v->tmp1 = v->tmp1->link;
		free(v->tmp);
		v->tmp = NULL;
	}
	if (v->tmp1 && v->tmp1->type[0] == 'E' && check_type(v->tmp1->type))
		v->tmp1 = v->tmp1->link;
}

void	existed_variable(t_vars *v)
{
	if (v->tmp3 && v->tmp3->type[0] == 'E' && !v->val)
	{
		free(v->temp2->value);
		v->temp2->value = ft_strdup("");
	}
	else if ((v->tmp3 && v->tmp3->link && v->tmp3->type[0] == 'E'
			&& v->tmp3->link->type[0] == 's')
		|| (v->tmp3 && !v->tmp3->link && v->tmp3->type[0] == 'E'))
		v->temp2->value = ft_strjoin(v->temp2->value, "");
	else if (v->flag == 1 && v->val)
		v->temp2->value = ft_strjoin(v->temp2->value, v->val);
	else if (v->val)
	{
		free(v->temp2->value);
		v->temp2->value = ft_strdup(v->val);
	}	
}

void	add_variable_to_env(t_env **env, t_vars *v)
{
	if (v->flag == 1)
	{
		if (v->val)
			ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(v->var),
					ft_strdup(v->val)));
		else
			ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(v->var),
					ft_strdup("")));
	}
	else if (v->tmp3 && v->tmp3->type[0] == 'E' && !v->val)
		ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(v->var),
				ft_strdup("")));
	else if (!check_if_variable_exist(*env, v->var, &v->temp2))
	{
		if (v->tmp3 && v->tmp3->link && v->tmp3->type[0] == 'E'
			&& v->tmp3->link->type[0] == 'v')
			ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(v->var),
					ft_strdup(v->val)));
		else
			ft_lstadd_back_env(env, ft_lstnew_env(ft_strdup(v->var), NULL));
	}
}

void	mooooore_steps(t_vars *v, t_env **env, int length)
{
	v->tmp3 = v->tmp1;
	join_variable_values(v);
	if (ft_strlen(v->var) == 0 && v->val)
	{
		ft_printf("minishell: export: `=%s': not a valid identifier\n",
			2, v->val);
		g_exit_status = 1;
	}
	else if (ft_strlen(v->var) == 0 && !length)
	{
		ft_printf("minishell: export: `': not a valid identifier\n", 2);
		g_exit_status = 1;
	}
	else if (!length && check_if_variable_exist(*env, v->var, &v->temp2))
		existed_variable(v);
	else if (!length)
		add_variable_to_env(env, v);
	v->flag = 0;
}

int	export_parsing(t_list **list, t_env **env, int length)
{
	t_vars	v;

	v.vars = 0;
	v.flag = 0;
	v.var = NULL;
	v.val = NULL;
	v.temp2 = NULL;
	v.tmp3 = NULL;
	v.tmp1 = skip_whats_before_the_first_var(*list, *list);
	lexer_for_export(&v.tmp1);
	while (v.tmp1 && check_type(v.tmp1->type))
	{
		join_variable_names(&v);
		if (check_valid_var(v.var) || (v.var && v.var[0] == '-'))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n",
				2, v.var);
			g_exit_status = 1;
			if (v.vars == 1 && v.var[0] == '-')
			{
				free(v.var);
				return (1);
			}
		}
		else
			mooooore_steps(&v, env, length);
		free_some_variables(&v);
	}
	return (0);
}
