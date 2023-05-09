/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/09 16:18:05 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft(t_list *stack)
{
	t_list	*tmp;

	tmp = stack;
	printf("\n                 \e[1m\e[93mList");
	printf("\n---------------------------------------\n");
	printf("|   Token          |           Type   |\n");
	printf("---------------------------------------\n");
	while (tmp)
	{
		printf("|%18s|%18s|\n", tmp->content, tmp->type);
		tmp = tmp->link;
	}
	printf("---------------------------------------\x1B[0m\n\n");
}

void	ft_builtins(char *input, char **env, t_list *list)
{
	env_parsing(input, ft_split_environment(env));
	export_parsing(list);
}

t_list	*minihell(char *input, char **env)
{
	t_list	*lst;

	lst = ft_split_input(input);
	ft_builtins(input, env, lst);
	if (check_syntax(lst))
		return (0);
	return (lst);
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_list	*lst;

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
		lst = minihell(input, env);
		if (lst)
		{
			lexer(&lst);
			ft(lst);
		}
	}
	return (0);
}
