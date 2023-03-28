/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/03/28 06:03:41 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_count_bytes(char *input)
{
	int	i;
	int	bytes;

	i = -1;
	bytes = 1;
	while (input[++i])
	{
		if (input[i] == '|')
			bytes += 2;
		bytes++;
	}
	return (bytes);
}

char	*ft_create_updated_input(char *input)
{
	int		i;
	int		j;
	char	*new_input;

	j = 0;
	i = -1;
	new_input = malloc(sizeof(char) * ft_count_bytes(input));
	if (!new_input)
		return (NULL);
	while (input[++i])
	{
		if (input[i] == ' ')
			new_input[j++] = '.';
		else if (input[i] == '|')
		{
			new_input[j++] = '.';
			new_input[j++] = input[i];
			new_input[j++] = '.';
		}
		else
			new_input[j++] = input[i];
	}
	new_input[j] = '\0';
	return (free(input), new_input);
}

void	ft_first_check(char *input)
{
	int	length;

	length = ft_strlen(input);
	if (ft_strlen(input) == 0)
		return ;
	else if ((input[0] == '|' && input[1] == '|') || !ft_strcmp(input + length
			- 4, "||||"))
		ft_putstr_fd("minishell: syntax error near unexpected token `||'\n", 2);
	else if (!ft_strcmp(input + length - 3, "|||") || input[0] == '|')
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	else if (!ft_strcmp(input + length - 6, "<<<<<<"))
		ft_putstr_fd("minishell: syntax error near unexpected token `<<<'\n",
			2);
	else if (!ft_strcmp(input + length - 5, "<<<<<"))
		ft_putstr_fd("minishell: syntax error near unexpected token `<<'\n", 2);
	else if (!ft_strcmp(input + length - 4, "<<<<"))
		ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
	else if (!ft_strcmp(input + length - 4, ">>>>"))
		ft_putstr_fd("minishell: syntax error near unexpected token `>>'\n", 2);
	else if (!ft_strcmp(input + length - 3, ">>>"))
		ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", 2);
	else if (input[length - 1] == '<' || input[length - 1] == '>')
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
			2);
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
		ft_first_check(input);
		input = ft_create_updated_input(input);
		ft_fill_list(input, &list);
		free(input);
	}
	return (0);
}
