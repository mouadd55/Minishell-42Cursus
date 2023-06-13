/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 18:22:13 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/13 19:28:02 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	join_variable_names(t_vars *v)
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

t_list	*skip_whats_before_the_first_var(t_list *tmp, t_list *list)
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

int	check_valid_variable(char *input)
{
	int		i;
	char	*new_input;

	i = 0;
	new_input = ft_strnstr(input, "export ", ft_strlen(input));
	if (new_input)
		new_input += 7;
	if (new_input)
	{
		while (new_input[i] && new_input[i] == ' ')
			i++;
	}
	return (0);
}
