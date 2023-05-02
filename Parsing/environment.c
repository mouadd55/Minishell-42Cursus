/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:22:34 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/02 20:44:51 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(t_list *env)
{
	printf("                 \e[1m\e[93mEnvironment        ");
	printf("\n--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("|              Key             |                                                         Value                                                         |\n");
	printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	while (env)
	{
		printf("|%29s |%118s |\n", env->key, env->value);
		env = env->link;
	}
	printf("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
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
