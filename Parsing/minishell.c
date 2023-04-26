/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/04/26 20:05:15 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_char(char *input, int c)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == (char) c)
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_count_char(char *input, char c)
{
	size_t	i;
	size_t	count;

	i = -1;
	count = 0;
	while (input[++i])
	{
		if (input[i] == c)
			count++;
	}
	return (count);
}

void	ft_first_middle_check(char *input)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(input))
	{
		if (((check_char("<>", input[i]) || check_char("<>", input[i + 1]))
				|| (input[i] == '|' && input[i + 1] == '|'))
			&& (!input[i + 2] || ft_count_char(&input[i + 2], ' ')
				== ft_strlen(&input[i + 2])))
			return (syntax_error(&input[i], 0));
		else if (input[i] == '|' && (input[i + 1] == '|'
				|| ft_count_char(&input[i + 1], ' ')
				== ft_strlen(&input[i + 1])))
			return (syntax_error(NULL, '|'));
		else if (input[i] == '(' || input[i] == ')')
			return (syntax_error(NULL, input[i]));
		i++;
	}
}

void	ft_first_last_check(char *input)
{
	size_t	i;

	i = -1;
	if (check_char("()|", input[0])
		|| (check_char("<>", input[0])
			&& (!input[1] || ft_count_char(&input[0], ' ')
				== ft_strlen(&input[1]))))
		return (syntax_error(NULL, input[0]));
	else if ((ft_count_char(input, '"') % 2))
		return (syntax_error(NULL, '"'));
	else if ((ft_count_char(input, '\'') % 2))
		return (syntax_error(NULL, '\''));
	else
		ft_first_middle_check(input);
}

char	*ft_create_updated_input(char *input)
{
	char *a = ft_strdup(""); // don't touch it
	ft_first_last_check(input);
	return(a);
}

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
		input = ft_create_updated_input(input);
		ft_fill_list(input, &list);
		while(list)
		{
			// printf("%s\n", list->content);
			list = list->link;
		}
		// free(input);
	}
	return (0);
}
