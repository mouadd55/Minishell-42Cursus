/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/07 20:19:21 by yonadry          ###   ########.fr       */
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
		if (check_char("|><", input[i]))
		{
			new_input[j++] = ' ';
			new_input[j++] = input[i];
			new_input[j++] = ' ';
		}
		else
			new_input[j++] = input[i];
	}
	new_input[j] = '\0';
	return (free(input), new_input);
}

void	ft_fill_list(char *input, t_list **list)
{
	int		i;
	char	**arr;

	i = -1;
	arr = ft_split(input, "<");
	while (arr[++i])
		ft_lstadd_back(list, ft_lstnew(ft_strdup(arr[i])));
	ft_free_arr(arr);
}

void	ft_builtins(char *input, char **env)
{
	env_parsing(input, ft_split_environment(env));
}
void minihell(char *input)
{
	t_list *lst;

	lst = ft_split_input(input);
	if (!check_syntax(lst))
	{

	while (lst)
	{
		printf("|%s|\n", lst->content);
		lst = lst->link;
	}
	}

}
int	main(int ac, char **av, char **env)
{
	char	*input;
	// t_list	*lst;
	(void) env;

	(void)av;
	if (ac != 1)
		return (0);
	// print_env(ft_split_environment(env));
	while (1)
	{
		input = readline("➜  Minishell ");
		if (!input)
			break ;
		if (ft_strlen(input))
			add_history(input);
		minihell(input);
		// ft_builtins(input, env);
	}
	system("leaks minishell");
	return (0);
}
