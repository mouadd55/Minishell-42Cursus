/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/09 21:46:08 by moudrib          ###   ########.fr       */
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

	if (!list)
		return ;
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
		while (tmp->cmd && tmp->cmd[i])
		{
			printf("|    |%37s|                          |\n", tmp->cmd[i]);
			i++;
		}
		printf("|    ---------------------------------------                          |\n");
		printf("|%29s %2d                                     |\n", "fd_in:", tmp->fd_in);
		printf("| %29s %2d                                    |\n", "fd_out:", tmp->fd_out);
		printf("| %29s %2s                                    |\n", "file_name:", tmp->file_name);
		tmp = tmp->link;
	}
	printf("-----------------------------------------------------------------------\x1B[0m\n\n");
}

void	ft_builtins(char **cmd, t_env **env, int fd_out)
{
	env_parsing(cmd, *env, fd_out);
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

void	recreate_list(t_command *final_list, t_env **envr)
{
	t_vars	v;

	while (final_list)
	{
		v.i = 0;
		v.str = NULL;
		while (final_list->cmd && final_list->cmd[v.i])
		{
			v.tmp = spaces_in_quotes_utils(final_list->cmd[v.i], 1);
			v.str = ft_strjoin(v.str, v.tmp);
			free(v.tmp);
			v.str = ft_strjoin(v.str, " ");
			v.i++;
		}
		if (v.str)
		{
			v.tmp1 = ft_split_input(v.str);
			lexer(&v.tmp1);
			check_cmd(&v.tmp1, envr, final_list);
			ft_destroy_list(&v.tmp1);
			free(v.str);
			v.str = NULL;
		}
		if (final_list->cmd && final_list->cmd[0] && !ft_strcmp(final_list->cmd[0], "exit"))
			ft_exit(final_list->cmd, final_list);
		else if (final_list->cmd && final_list->cmd[0] && !ft_strcmp(final_list->cmd[0], "env"))
			ft_builtins(final_list->cmd, envr, final_list->fd_out);
		final_list = final_list->link;
	}
	spaces_in_quotes(&final_list);
}

void	minihell(t_env **envr, t_list **lst)
{
	t_command	*final_list;

	if (check_syntax(*lst))
		return ;
	lexer(lst);
	final_list = NULL;
	if (lst)
	{
		expand_var(lst, *envr, 1);
		create_final_list(*lst, &final_list);
		open_files(*lst, final_list, envr);
		recreate_list(final_list, envr);
		execution(final_list, *envr);
		// ft(*lst);
		// final(final_list);
	}
	ft_destroy_final(&final_list);
}

// void	l()
// {
// 	system ("leaks minishell");
// }

int	main(int ac, char **av, char **env)
{
	// atexit(l);
	char	*input;
	t_env	*envr;
	t_list	*lst;

	(void)av;
	if (ac != 1)
		return (0);
	lst = NULL;
	envr = NULL;
	envr = ft_split_environment(env);
	signal(SIGINT, &catching_signals);
	signal(SIGQUIT,SIG_IGN);
	// signal(SIGQUIT,SIG_DFL);
	rl_catch_signals = 0;
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
				minihell(&envr, &lst);
		}
		ft_destroy_list(&lst);
		free(input);

	}
	ft_destroy_list_env(&envr);
	printf("exit\n");
	return (0);
}
