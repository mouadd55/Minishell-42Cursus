/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/14 20:30:52 by yonadry          ###   ########.fr       */
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

t_env	*ft_builtins(/*char *input, */char **env/*, t_list *list*/)
{
	t_env	*envr;

	envr = ft_split_environment(env);
	// env_parsing(input, envr);
	// export_parsing(input);

	return (envr);
}

t_list	*minihell(char *input)
{
	t_list	*lst;

	lst = ft_split_input(input);
	if (check_syntax(lst))
		return (0);
	lexer(&lst);
	// if (ft_strlen(input) && (export_parsing(input) || check_before_value(&lst)))
	// 	return (0);
	return (lst);
}
void echo(t_list *list)
{
	if (!ft_strcmp(list->content, "echo"))
	{
		list = list->link->link;
		while (list)
		{
			if (strstr("PIPE,HEREDOC,APPEND,OUTPUT,INPUT", list->type))
				break;
			printf("%s", list->content);
			list = list->link;
		}
		printf("\n");
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_env	*envr;
	t_list	*lst;

	(void)av;
	(void)env;
	if (ac != 1)
		return (0);
	while (1)
	{
		input = readline("➜  Minishell ");
		if (!input)
			break ;
		if (ft_strlen(input))
			add_history(input);
		lst = minihell(input);
		if (lst)
		{
			envr = ft_builtins(env);
			lexer(&lst);
			expand_var(&lst, envr);
			// ft(lst);
			echo(lst);
			// ft_destroy_list(&lst);
		}
		// ft_destroy_list_env(&envr);
		free(input);
	}
	return (0);
}
