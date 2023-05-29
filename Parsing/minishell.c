/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/29 17:41:28 by yonadry          ###   ########.fr       */
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

void	final(t_command *list)
{
	int			i;
	t_command	*tmp;

	tmp = list;
	printf("\n\e[1m\e[93m-----------------------------------------------------------------------\n");
	printf("|                            Final List                               |\n");
	printf("-----------------------------------------------------------------------\n");
	while (tmp)
	{
		i = 0;
		printf("|    ---------------------------------------                          |\n");
		printf("|    |               Command               |                          |\n");
		printf("|    ---------------------------------------                          |\n");
		while (tmp->cmd[i])
		{
			printf("|    |%37s|                          |\n", tmp->cmd[i]);
			i++;
		}
		printf("|    ---------------------------------------                          |\n");
		printf("|%29s %d                                      |\n", "fd_in:", tmp->fd_in);
		printf("|%30s %d                                     |\n", "fd_out:", tmp->fd_out);
		tmp = tmp->link;
	}
	printf("-----------------------------------------------------------------------\x1B[0m\n\n");
}

t_env	*ft_builtins(char *input, t_env **env)
{
	env_parsing(input, *env);
	return (*env);
}

t_env	*ft_copy_env_list(t_env *env)
{
	t_env	*copy;

	copy = NULL;
	while (env)
	{
		if (env->value)
			ft_lstadd_back_env(&copy,
				ft_lstnew_env(ft_strdup(env->key), ft_strdup(env->value)));
		else
			ft_lstadd_back_env(&copy, ft_lstnew_env(ft_strdup(env->key), NULL));
		env = env->link;
	}
	return (copy);
}

void	recreate_list(t_command *final_list, char *input, t_env **envr)
{
	t_vars	v;

	v.str = NULL;
	while (final_list)
	{
		v.i = 0;
		while (final_list->cmd[v.i])
		{
			v.str = ft_strjoin(v.str, final_list->cmd[v.i]);
			v.str = ft_strjoin(v.str, " ");
			v.i++;
		}
		v.tmp1 = ft_split_input(v.str);
		lexer(&v.tmp1);
		check_cmd(&v.tmp1, envr, input, 0);
		free(v.str);
		v.str = NULL;
		final_list = final_list->link;
	}
}

void	minihell(char *input, t_env **envr, t_list **lst)
{
	t_command	*final_list;

	if (check_syntax(*lst))
		return ;
	lexer(lst);
	final_list = NULL;
	*envr = ft_builtins(input, envr);
	if (lst)
	{
		expand_var(lst, *envr);
		ft(*lst);
		create_final_list(*lst, &final_list);
		recreate_list(final_list, input, envr);
		final(final_list);
		open_files(*lst, &final_list);
		// check_cmd(lst, envr, input, fd);
		// open_files(*lst);
	}
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
		{
			add_history(input);
			lst = ft_split_input(input);
			if (lst)
				minihell(input, &envr, &lst);
		}
		free(input);
	}
	return (0);
}
