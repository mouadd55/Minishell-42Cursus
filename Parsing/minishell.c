/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/17 20:42:50 by moudrib          ###   ########.fr       */
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

t_env	*ft_builtins(char *input, t_env **env)
{
	env_parsing(input, *env);
	return (*env);
}

void echo(t_list *list)
{
	t_list *tmp;

	tmp = list;
	if (tmp && tmp->link && !ft_strcmp(tmp->content, "echo"))
	{
		tmp = tmp->link->link;
		while (tmp)
		{
			if (strstr("PIPE,HEREDOC,APPEND,OUTPUT,INPUT", tmp->type))
				break;
			printf("%s", tmp->content);
			tmp = tmp->link;
		}
	}
	if (!ft_strcmp(list->content, "echo"))
		printf("\n");
}

void	minihell(char *input, t_env **envr, t_list **lst)
{
	if (check_syntax(*lst))
		return ;
	lexer(lst);
	*envr = ft_builtins(input, envr);
	if (lst)
	{
		// lexer(&lst);
		expand_var(lst, *envr);
		echo(*lst);
		ft(*lst);
		// ft_destroy_list(&lst);
	}
	if (ft_strlen(input) && (export_parsing(input) || check_before_value(lst, envr)))
		return ;
}


int	main(int ac, char **av, char **env)
{
	char	*input;
	t_env	*envr;
	t_list	*lst;

	(void)av;
	if (ac != 1)
		return (0);
	envr = NULL;
	envr = ft_split_environment(env);
	while (1)
	{
		input = readline("➜  Minishell ");
		if (!input)
			break ;
		if (ft_strlen(input))
			add_history(input);
		lst = ft_split_input(input);
		minihell(input, &envr, &lst);
		// ft_destroy_list_env(&envr);
		free(input);
	}
	return (0);
}
