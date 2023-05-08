/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:22:34 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/03 15:24:30 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(int i, int count, t_list *env)
{
	if (i && i == count)
	{
		while (env)
		{
			printf("%s=%s\n", env->key, env->value);
			env = env->link;
		}
	}	
}

void	env_parsing(char *input, t_list *env)
{
	t_vars v;

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

t_list	*ft_lstnew_env(char *key, char *value)
{
	t_list	*head;

	head = (t_list *)malloc(sizeof(t_list));
	if (!head)
		return (NULL);
	head->key = key;
	head->value = value;
	head->link = NULL;
	head->prev = NULL;
	return (head);
}

t_list	*ft_split_environment(char **env)
{
	int		i;
	int		start;
	int		length;
	t_list	*envr;

	i = -1;
	envr = NULL;
	while (env[++i])
	{
		start = 0;
		length = ft_strlen(env[i]);
		while (env[i][start] != '=')
			start++;
		ft_lstadd_back(&envr, ft_lstnew_env(ft_substr(env[i], 0, start),
				ft_substr(env[i], start + 1, length)));
	}
	return (envr);
}
