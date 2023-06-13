/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_env_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:21:16 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/13 10:29:12 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*ft_lstnew_env(char *key, char *value)
{
	t_env	*head;

	head = (t_env *)malloc(sizeof(t_env));
	if (!head)
		return (NULL);
	head->key = key;
	head->value = value;
	head->link = NULL;
	head->prev = NULL;
	return (head);
}

t_env	*ft_lstlast_env(t_env *head)
{
	if (!head)
		return (NULL);
	while (head)
	{
		if (head->link == NULL)
			return (head);
		head = head->link;
	}
	return (NULL);
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

int	ft_lstsize_env(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->link;
	}
	return (count);
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
