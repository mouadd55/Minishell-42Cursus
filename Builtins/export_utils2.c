/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 12:02:34 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/13 11:34:51 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_if_variable_exist(t_env *env, char *var, t_env **tmp)
{
	while (env)
	{
		if (ft_strcmp(env->key, var) == 0)
		{
			*tmp = env;
			return (1);
		}
		env = env->link;
	}
	return (0);
}

int	check_type(char *type)
{
	if (ft_strcmp(type, "PIPE") && ft_strcmp(type, "APPEND")
		&& ft_strcmp(type, "INFILE") && ft_strcmp(type, "OUTFILE")
		&& ft_strcmp(type, "HEREDOC"))
		return (1);
	return (0);
}

int	check_valid_var(char *var)
{
	int	i;

	i = -1;
	if (!var)
		return (0);
	while (var[++i])
	{
		if (var[0] >= '0' && var[0] <= '9')
			return (1);
		else if (!ft_isalpha(var[i]) && !(var[i] >= '0' && var[i] <= '9')
			&& var[i] != '_')
			return (1);
	}
	return (0);
}

void	print_export(t_env *temp3, int fd_out)
{
	while (temp3)
	{
		if (!temp3->value)
			ft_printf("declare -x %s\n", fd_out, (temp3)->key);
		else
			ft_printf("declare -x %s=\"%s\"\n", fd_out,
				(temp3)->key, (temp3)->value);
		temp3 = (temp3)->link;
	}
}

void	sort_env(t_env *env, int fd_out)
{
	t_vars	v;

	v.temp1 = env;
	v.temp3 = env;
	while (v.temp1)
	{
		v.temp2 = env;
		while (v.temp2)
		{
			if (ft_strcmp(v.temp1->key, v.temp2->key) < 0)
			{
				v.tmp_key = ft_strdup(v.temp1->key);
				v.tmp_value = ft_strdup(v.temp1->value);
				free(v.temp1->key);
				free(v.temp1->value);
				v.temp1->key = v.temp2->key;
				v.temp1->value = v.temp2->value;
				v.temp2->key = v.tmp_key;
				v.temp2->value = v.tmp_value;
			}
			v.temp2 = v.temp2->link;
		}
		v.temp1 = v.temp1->link;
	}
	print_export(v.temp3, fd_out);
}
