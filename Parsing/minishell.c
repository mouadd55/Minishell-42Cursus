/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/04/28 15:39:42 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_fill_list(char *input, t_list **list)
{
	int		i;
	char	**arr;

	i = -1;
	arr = ft_split(input, '.');
	while (arr[++i])
		ft_lstadd_back(list, ft_lstnew(ft_strdup(arr[i])));
	ft_free_arr(arr);
}

int	main(int ac, char **av)
{
	char	*input;
	t_list	*list;

	(void)av;
	if (ac != 1)
		return (0);
	while (1)
	{
		input = readline("➜  Minishell ");
		if (!input)
			break ;
		if (ft_strlen(input))
			add_history(input);
		if (ft_first_last_check(input))
			free(input);
		ft_fill_list(input, &list);
		// ft_destroy_list(&list);
		// free(input);
		// free(input);
		// while (list)
		// {
		// 	printf("%s\n", list->content);
		// 	list = list->link;
		// }
	}
	system("leaks minishell");
	return (0);
}
