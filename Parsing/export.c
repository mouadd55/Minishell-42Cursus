/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 15:47:40 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/09 16:20:08 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	list_join(t_list *list)
{
	printf("`");
	while ((list && list->content[0] == 32) || list)
	{
		printf("%s", list->content);
		list = list->link;
	}
	printf("': not a valid identifier\n");
}

int	check_export(t_list *list)
{
	t_vars	v;
	t_list	*tmp;

	tmp = list;
	v.count = 0;
	while (list)
	{
		if (check_char("=+-", list->content[0]))
			v.count++;
		printf("|%s|\n", list->content);
		list = list->link;
	}
	if (v.count == ft_lstsize(tmp))
		return (printf("export: `%s': not a valid identifier\n",
				ft_lstlast(tmp)->content), 1);
	return (0);
}

void	export_parsing(t_list *list)
{
	if (list->prev == NULL)
	{
		if (!ft_strcmp(list->content, "export"))
		{
			if (list->link->link && list->link->link->content[0] == '=')
			{
				printf("minishell: export: ");
				list_join(list->link->link);
				return ;
			}
			else if (list->link->link && check_export(list->link->link))
				return ;
			else
				return ;
		}
		return ;
	}
}
