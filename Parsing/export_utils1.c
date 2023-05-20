/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 18:22:13 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/20 22:19:56 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	join_variable_names_and_check_if_valid(t_vars *v)
{
	while (v->tmp1 && v->tmp1->type[0] == 's')
		v->tmp1 = v->tmp1->link;
	while (v->tmp1 && v->tmp1->type[0] == 'V' && check_type(v->tmp1->type))
	{
		v->vars++;
		if (!ft_strcmp(v->tmp1->type, "DOUBLE_Q"))
			v->var = ft_strjoin(v->var, ft_strtrim(v->tmp1->content, "\""));
		else if (!ft_strcmp(v->tmp1->type, "SINGLE_Q"))
			v->var = ft_strjoin(v->var, ft_strtrim(v->tmp1->content, "\'"));
		else
			v->var = ft_strjoin(v->var, v->tmp1->content);
		v->tmp1 = v->tmp1->link;
	}
	if (check_valid_var(v->var) || (v->var && v->var[0] == '-'))
	{
		printf("minishell: export: `%s': not a valid identifier\n", v->var);
		if (v->vars == 1 && v->var[0] == '-')
			return (1);
	}
	return (0);
}

t_list	*skip_whats_before_export(t_list *tmp, t_list *list)
{
	int	count;

	count = 0;
	while (tmp && ft_strcmp(tmp->content, "export"))
	{
		count++;
		tmp = tmp->link;
	}
	if (tmp && tmp->link && tmp->link->content[0] == '+')
		return (0);
	if (ft_lstsize(list) < count + 2)
		return (0);
	tmp = tmp->link->link;
	return (tmp);
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

int	check_valid_variable(char *input)
{
	int		i;
	char	*new_input;

	i = 0;
	new_input = strnstr(input, "export ", ft_strlen(input));
	if (new_input)
		new_input += 7;
	if (new_input)
	{
		while (new_input[i] && new_input[i] == ' ')
			i++;
	}
	return (0);
}

// void	delete_node(t_list **list, int position)
// {
// 	t_list	*tmp1;
// 	t_list	*tmp2;

// 	tmp1 = *list;
// 	if (position == 1)
// 	{
// 		*list = (*list)->link;
// 		free((*list)->prev);
// 		(*list)->prev = NULL;
// 	}
// 	else if (position > 1 && position < ft_lstsize(*list))
// 	{
// 		while (position > 1)
// 		{
// 			tmp1 = tmp1->link;
// 			position--;
// 		}
// 		tmp2 = tmp1->prev;
// 		tmp2->link = tmp1->link;
// 		tmp1->link->prev = tmp2;
// 		free(tmp1);
// 		tmp1 = NULL;
// 	}
// 	else if (position == ft_lstsize(*list))
// 	{
// 		tmp1 = ft_lstlast(*list);
// 		tmp2 = tmp1->prev;
// 		free(tmp1);
// 		tmp2->link = NULL;
// 	}
// }
