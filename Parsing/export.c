/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 15:47:40 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/23 16:21:27 by moudrib          ###   ########.fr       */
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
		v->val = ft_strjoin(v->val, ft_strtrim(v->tmp1->content, "\"\'"));
		v->tmp1 = v->tmp1->link;
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

void	mooooore_steps(t_vars *v, t_env **env)
{
	v->tmp3 = v->tmp1;
	join_variable_values(v);
	if (ft_strlen(v->var) == 0 && v->val)
		printf("minishell: export: `=%s': not a valid identifier\n",
			v->val);
	else if (ft_strlen(v->var) == 0)
		printf("minishell: export: `': not a valid identifier\n");
	else if (check_if_variable_exist(*env, v->var, &v->temp2))
		existed_variable(v);
	else
		add_variable_to_env(env, v);
	v->flag = 0;
}

int	export_parsing(t_list **list, t_env **env)
{
	t_vars	v;

	v.vars = 0;
	v.flag = 0;
	v.count = 0;
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
			printf("minishell: export: `%s': not a valid identifier\n", v.var);
			if (v.vars == 1 && v.var[0] == '-')
				return (1);
		}
		else
			mooooore_steps(&v, env);
		free_some_variables(&v);
	}
	return (0);
}
