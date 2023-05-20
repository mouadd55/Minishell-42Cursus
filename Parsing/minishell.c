/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:31:57 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/20 15:31:00 by moudrib          ###   ########.fr       */
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

void	echo(t_list *list)
{
	t_list	*tmp;
	int		flag;

	flag = 0;
	tmp = list;
	if (tmp && tmp->link && !ft_strcmp(tmp->content, "echo"))
	{
		tmp = tmp->link->link;
		if (tmp && !ft_strcmp(tmp->content, "-n"))
		{
			flag = 1;
			if (tmp->link && tmp->link->link)
				tmp = tmp->link->link;
			else
				return ;
		}
		while (tmp)
		{
			if (strstr("PIPE,HEREDOC,APPEND,OUTPUT,INPUT", tmp->type))
				break ;
			if (tmp)
				printf("%s", tmp->content);
			tmp = tmp->link;
		}
	}
	if (!flag && list && !ft_strcmp(list->content, "echo"))
		printf("\n");
}

char	*strlower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
	return (str);
}

void	pwd(t_list *list)
{
	char	*pwd;

	if (list && !strcmp("pwd", strlower(list->content)))
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
			printf("%s\n", pwd);
	}
}

void	change_dir(t_list *list, t_env **envr)
{
	char	*pwd;
	t_env	*env;

	env = *envr;
	if (list && !ft_strcmp(list->content, "cd"))
	{
		if (list && (!list->link
				|| (list->link->link
					&& (!ft_strcmp(list->link->link->content, "~")))))
			list->content = ft_strdup(getenv("HOME"));
		else if (list && list->link && list->link->link)
			list = list->link->link;
		pwd = getcwd(NULL, 0);
		if (list && !chdir(list->content))
		{
			while (env)
			{
				if (!ft_strcmp(env->key, "PWD"))
					env->value = ft_strdup(getcwd(NULL, 0));
				if (!ft_strcmp(env->key, "OLDPWD"))
					env->value = ft_strdup(pwd);
				env = env->link;
			}
		}
	}
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

/*if (ft_strlen(input) && (export_parsing(input)
	|| check_before_value(lst, envr)))*/
void	minihell(char *input, t_env **envr, t_list **lst)
{
	t_env	*env_copy;

	if (check_syntax(*lst))
		return ;
	lexer(lst);
	*envr = ft_builtins(input, envr);
	if (lst)
	{
		expand_var(lst, *envr);
		echo(*lst);
		change_dir(*lst, envr);
		pwd(*lst);
		// ft(*lst);
	}
	if (ft_lstsize(*lst) == 1 && !ft_strcmp((*lst)->content, "export"))
	{
		env_copy = ft_copy_env_list(*envr);
		sort_env(env_copy);
		ft_destroy_list_env(&env_copy);
	}
	if (ft_strlen(input) && (check_before_value(lst, envr)))
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
		ft(lst);
		free(input);
	}
	return (0);
}
