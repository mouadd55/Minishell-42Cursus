/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/20 19:44:59 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_exit_status;

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

void	final(t_cmd *list)
{
	int		i;
	t_cmd	*tmp;

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
		printf("| %25s %2d                                        |\n", "fd_in:", tmp->fd_in);
		printf("| %25s %2d                                        |\n", "fd_out:", tmp->fd_out);
		printf("| %25s %2s                                        |\n", "file_name:", tmp->file_name);
		tmp = tmp->link;
	}
	printf("-----------------------------------------------------------------------\x1B[0m\n\n");
}

void	ft_builtins(t_list *list, t_env *envr, t_cmd *f_list, int length)
{
	t_list	*tmp;
	t_env	*env_copy;

	tmp = list;
	if (f_list && !ft_strcmp(f_list->cmd[0], "cd"))
		change_dir(&envr, f_list);
	else if (list && !(list)->prev && (list)->link
		&& (list)->link->type[0] == 's' && !strcmp("unset", (list)->content))
		unset(&list, &envr);
	if (!ft_strcmp(f_list->cmd[0], "export") && f_list->cmd[1] == 0)
	{
		env_copy = ft_copy_env_list(envr);
		sort_env(env_copy, f_list->fd_out);
		ft_destroy_list_env(&env_copy);
	}
	if (export_parsing(&list, &envr, length))
		return ;
	if (f_list->cmd && f_list->cmd[0] && !ft_strcmp(f_list->cmd[0], "exit"))
		ft_exit(f_list->cmd, f_list);
	else if (f_list->cmd && f_list->cmd[0] && !ft_strcmp(f_list->cmd[0], "env"))
		env_parsing(f_list->cmd, envr, f_list->fd_out);
	echo(f_list);
	pwd(f_list, envr);
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

void	split_string(t_vars *v, t_cmd *final_list, t_env **envr, int size)
{
	v->tmp1 = ft_split_input(v->str);
	lexer(&v->tmp1);
	if (size == 1)
		check_cmd(&v->tmp1, envr, final_list);
	ft_destroy_list(&v->tmp1);
	free(v->str);
	v->str = NULL;
}

void	recreate_list(t_cmd *final_list, t_env **envr)
{
	t_vars	v;
	int		size;

	size = lstsize_cmd(final_list);
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
			split_string(&v, final_list, envr, size);
		final_list = final_list->link;
	}
	spaces_in_quotes(&final_list);
}

void	minihell(t_env **envr, t_list **lst)
{
	t_cmd	*final_list;

	if (check_syntax(*lst))
	{
		g_exit_status = 258;
		return ;
	}
	lexer(lst);
	final_list = NULL;
	if (lst)
	{
		expand_var(lst, *envr, 1);
		create_final_list(*lst, &final_list);
		open_files(*lst, final_list, envr);
		if (g_exit_status != -1)
		{
			recreate_list(final_list, envr);
			// ft(*lst);
			execution(final_list, envr, lst);
			// final(final_list);
		}
		else
			g_exit_status = 1;
	}
	ft_destroy_final(&final_list);
}

void	shell_level(t_env **env)
{
	t_vars	v;
	int		shlvl;

	v.temp1 = *env;
	while (v.temp1)
	{
		if (!ft_strcmp(v.temp1->key, "SHLVL"))
		{
			shlvl = (int)ft_atoi(v.temp1->value);
			free(v.temp1->value);
			shlvl++;
			v.temp1->value = ft_itoa(shlvl);
		}
		v.temp1 = v.temp1->link;
	}
}

void	everything_starts_here(t_env *envr)
{
	char	*input;
	t_list	*lst;

	lst = NULL;
	while (1)
	{
		input = readline("➜  Minishell ");
		if (!input)
			return ;
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
}

int	main(int ac, char **av, char **env)
{
	t_env	*envr;

	(void)av;
	if (ac != 1)
		return (0);
	envr = NULL;
	envr = ft_split_environment(env);
	shell_level(&envr);
	signal(SIGINT, &catching_signals);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
	everything_starts_here(envr);
	ft_destroy_list_env(&envr);
	printf("exit\n");
	return (0);
}
