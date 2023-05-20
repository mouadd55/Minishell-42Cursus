/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:22:34 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/19 20:02:25 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(int i, int count, t_env *env)
{
	if (i && i == count)
	{
		while (env)
		{
			if (env->value)
				printf("%s=%s\n", env->key, env->value);
			env = env->link;
		}
	}	
}

void	env_parsing(char *input, t_env *env)
{
	t_vars	v;

	v.i = -1;
	v.count = 0;
	v.arr = ft_split(input, " \t\n\v\f\r");
	while (v.arr[++v.i])
	{
		if (ft_strcmp(v.arr[v.i], "env") == 0)
			v.count++;
		else if (v.count && v.arr[v.i][0] == '-' && ft_strlen(v.arr[v.i]) > 1)
		{
			printf("env: illegal option -- %c\n", v.arr[v.i][1]);
			return ;
		}
		else if (v.count && v.arr[v.i][0] != '-')
		{
			printf("env: %s: No such file or directory\n", v.arr[v.i]);
			return ;
		}
	}
	print_env(v.i, v.count, env);
	ft_free_arr(v.arr);
}

void	ft_lstadd_back_env(t_env **head, t_env *new)
{
	t_env	*tmp;

	if (!*head || !head)
		*head = new;
	else
	{
		tmp = ft_lstlast_env(*head);
		tmp->link = new;
		new->prev = tmp;
	}
}

void	*ft_destroy_list_env(t_env **head)
{
	t_env	*tmp;

	if (!head || !*head)
		return (0);
	tmp = *head;
	while (tmp)
	{
		tmp = (*head)->link;
		free((*head)->key);
		free((*head)->value);
		free(*head);
		(*head) = tmp;
	}
	return (0);
}

t_env	*ft_split_environment(char **env)
{
	int		i;
	int		start;
	int		length;
	t_env	*envr;

	i = -1;
	envr = NULL;
	while (env[++i])
	{
		start = 0;
		length = ft_strlen(env[i]);
		while (env[i][start] != '=')
			start++;
		ft_lstadd_back_env(&envr, ft_lstnew_env(ft_substr(env[i], 0, start),
				ft_substr(env[i], start + 1, length)));
	}
	return (envr);
}
