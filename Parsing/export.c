/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 15:47:40 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/12 20:59:27 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if ((to_find == 0 || str == 0) && len == 0)
		return (NULL);
	if (*to_find == 0)
		return ((char *)str);
	while (str[i] && i < len)
	{
		while (to_find[j] == str[i + j] && str[i + j] && (i + j) < len)
			j++;
		if (!to_find[j])
			return ((char *)str + i);
		j = 0;
		i++;
	}
	return (NULL);
}

// int	ft_count_arguments(char *input)
// {
// 	t_vars	var;

// 	var.i = 0;
// 	var.count = 0;
// 	while (input[var.i])
// 	{
// 		while (input[var.i] && is_space(input[var.i]))
// 			var.i++;
// 		if (input[var.i] == '\"')
// 		{
// 			var.i++;
// 			var.count++;
// 			while (input[var.i] && input[var.i] != '\"')
// 				var.i++;
// 			var.i++;
// 		}
// 		else if (input[var.i])
// 		{
// 			while (input[var.i] && input[var.i] != ' ' && input[var.i] != '\"')
// 				var.i++;
// 			var.count++;
// 		}
// 	}
// 	return (var.count);
// }

void	delete_node(t_list **list, int position)
{
	t_list	*tmp1;
	t_list	*tmp2;

	tmp1 = *list;
	if (position == 1)
	{
		*list = (*list)->link;
		free((*list)->prev);
		(*list)->prev = NULL;
	}
	else if (position > 1 && position < ft_lstsize(*list))
	{
		while (position > 1)
		{
			tmp1 = tmp1->link;
			position--;
		}
		tmp2 = tmp1->prev;
		tmp2->link = tmp1->link;
		tmp1->link->prev = tmp2;
		free(tmp1);
		tmp1 = NULL;
	}
	else if (position == ft_lstsize(*list))
	{
		tmp1 = ft_lstlast(*list);
		tmp2 = tmp1->prev;
		free(tmp1);
		tmp2->link = NULL;
	}
}

int	check_valid_variable(char *input)
{
	int		i;
	char	*new_input;

	i = 0;
	new_input = strnstr(input, "export ", ft_strlen(input));
	if (new_input)
		new_input += 7;
	if (new_input)
	{
		while (new_input[i] && new_input[i] == ' ')
			i++;
	}
	return (0);
}

int	remove_empty_quotes(t_list **list)
{
	int		position;
	t_list	*tmp;

	tmp = *list;
	position = 1;
	while (tmp)
	{
		if ((tmp->content[0] == '\"' || tmp->content[0] == '\'')
			&& ft_strlen(tmp->content) == 2)
			delete_node(list, position);
		else if (tmp->content[0] == '=')
			break ;
		else
			position++;
		tmp = tmp->link;
	}
	return (1);
}

char	*f_strrchr(const char *s, int c)
{
	int		len;
	char	*str;

	str = (char *)s;
	len = ft_strlen(str);
	while (len >= 0)
	{
		if (str[len] == (char)c)
			return (str + len);
		len--;
	}
	return (0);
}

char	*f_strchr(const char *s, int c)
{
	int		i;
	char	*str;

	str = (char *)s;
	i = 0;
	while (str[i])
	{
		if (str[i] == (char)c)
			return (str + i);
		i++;
	}
	if (c == '\0')
		return (str + i);
	return (0);
}

char	*ft_strtrim(char *s1, char *set)
{
	int		i;
	int		j;
	int		end;
	char	*str;

	j = 0;
	i = 0;
	if (!s1 || !set)
		return (NULL);
	end = ft_strlen(s1);
	while (f_strchr(set, s1[i]) && s1[i])
		i++;
	while (f_strrchr(set, s1[end - 1]) && end > i)
		end--;
	str = malloc((end - i + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (i < end)
	{
		str[j] = s1[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (str);
}

// int	check_before_value(t_list **list)
// {
// 	t_list	*tmp;

// 	tmp = *list;
// 	while (tmp)
// 	{
// 		if (!ft_strcmp(tmp->content, "export"))
// 		{
// 			tmp = tmp->link;
// 			while (tmp && !ft_strcmp(tmp->type, "SPACE"))
// 				tmp = tmp->link;
// 			remove_empty_quotes(list);
// 			tmp = ft_lstlast(*list);
// 			if (tmp->content[0] == '=' && !ft_strcmp(tmp->prev->type, "SPACE")
// 				&& !ft_strcmp(tmp->prev->prev->content, "export"))
// 				printf("minishell: export: `%s': not a valid identifier\n",
// 						tmp->content);
// 			break ;
// 		}
// 		tmp = tmp->link;
// 	}
// 	return (0);
// }

int	check_type(char *type)
{
	if (ft_strcmp(type, "PIPE") && ft_strcmp(type, "APPEND") && ft_strcmp(type,
			"INPUT") && ft_strcmp(type, "OUTPUT") && ft_strcmp(type, "HEREDOC"))
		return (1);
	return (0);
}

void	lexer_for_export(t_list **list)
{
	t_list	*tmp;

	tmp = *list;
	while (tmp && check_type(tmp->type))
	{
		while (tmp && ft_strcmp(tmp->content, "=") && check_type(tmp->type))
		{
			if (ft_strcmp(tmp->type, "SPACE"))
			{
				free(tmp->type);
				tmp->type = ft_strdup("VAR");
			}
			tmp = tmp->link;
		}
		if (tmp && !ft_strcmp(tmp->content, "="))
		{
			free(tmp->type);
			tmp->type = ft_strdup("EQUAL");
			tmp = tmp->link;
		}
		while (tmp && ft_strcmp(tmp->type, "SPACE") && check_type(tmp->type))
		{
			free(tmp->type);
			tmp->type = ft_strdup("VALUE");
			tmp = tmp->link;
		}
		if (tmp && !ft_strcmp(tmp->type, "SPACE"))
			tmp = tmp->link;
	}
}

int	check_valid_var(char *var)
{
	int	i;

	i = -1;
	if (!var)
		return (0);
	while (var[++i])
	{
		if (var[0] >= '0' && var[0] <= '9')
			return (1);
		else if (!ft_isalpha(var[i]) && !(var[i] >= '0' && var[i] <= '9')
			&& var[i] != '_')
			return (1);
	}
	return (0);
}

int	check_before_value(t_list **list)
{
	char	*var;
	t_list	*tmp1;

	var = NULL;
	tmp1 = (*list)->link->link;
	lexer_for_export(&tmp1);
	while (tmp1)
	{
		while (tmp1 && !ft_strcmp(tmp1->type, "SPACE"))
			tmp1 = tmp1->link;
		while (tmp1 && !ft_strcmp(tmp1->type, "VAR"))
		{
			var = ft_strjoin(var, ft_strtrim(tmp1->content, "\"\'"));
			tmp1 = tmp1->link;
		}
		if (check_valid_var(var))
			printf("minishell: export: `%s': not a valid identifier\n", var);
		free(var);
		var = NULL;
		while (tmp1 && ft_strcmp(tmp1->type, "SPACE"))
			tmp1 = tmp1->link;
		if (tmp1)
			tmp1 = tmp1->link;
	}
	return (0);
}

int	export_parsing(char *input)
{
	int		i;
	char	*new_input;

	i = 0;
	new_input = strnstr(input, "export ", ft_strlen(input));
	if (new_input)
		new_input += 7;
	if (new_input)
	{
		while (new_input[i] && new_input[i] == ' ')
			i++;
		if (new_input[i] && new_input[i] == '=')
		{
			printf("minishell: export: `");
			while (new_input[i] && new_input[i] != ' ')
				printf("%c", new_input[i++]);
			printf("': not a valid identifier\n");
			return (1);
		}
		// else if ()
	}
	return (0);
}

// void	export_parsing(t_list *list, t_env *env)
// {
// 	int		i;
// 	int		j;
// 	char	*tmp_key;
// 	char	*tmp_value;
// 	t_env	*tmp1;
// 	t_env	*tmp2;
// 	t_env	*tmp3;

// 	i = 0;
// 	j = 0;
// 	tmp1 = env;
// 	tmp3 = env;
// 	if (!ft_strcmp(list->content, "export"))
// 	{
// 		while (tmp1)
// 		{
// 			tmp2 = env;
// 			while (tmp2)
// 			{
// 				if (ft_strcmp(tmp1->key, tmp2->key) < 0)
// 				{
// 					tmp_key = ft_strdup(tmp1->key);
// 					tmp_value = ft_strdup(tmp1->value);
// 					free(tmp1->key);
// 					free(tmp1->value);
// 					tmp1->key = tmp2->key;
// 					tmp1->value = tmp2->value;
// 					tmp2->key = tmp_key;
// 					tmp2->value = tmp_value;
// 				}
// 				tmp2 = tmp2->link;
// 			}
// 			tmp1 = tmp1->link;
// 		}
// 		while (tmp3)
// 		{
// 			printf("declare -x %s=\"%s\"\n", (tmp3)->key, (tmp3)->value);
// 			tmp3 = (tmp3)->link;
// 		}
// 	}
// }

// void	list_join(t_list *list)
// {
// 	printf("`");
// 	while ((list && list->content[0] == 32) || list)
// 	{
// 		printf("%s", list->content);
// 		list = list->link;
// 	}
// 	printf("': not a valid identifier\n");
// }

// int	check_export(t_list *list)
// {
// 	t_vars	v;
// 	t_list	*tmp;

// 	tmp = list;
// 	v.count = 0;
// 	while (list)
// 	{
// 		if (check_char("=+-", list->content[0]))
// 			v.count++;
// 		printf("|%s|\n", list->content);
// 		list = list->link;
// 	}
// 	if (v.count == ft_lstsize(tmp))
// 		return (printf("export: `%s': not a valid identifier\n",
// 				ft_lstlast(tmp)->content), 1);
// 	return (0);
// }

// void	export_parsing(t_list *list)
// {
// 	if (list->prev == NULL)
// 	{
// 		if (!ft_strcmp(list->content, "export"))
// 		{
// 			if (list->link->link && list->link->link->content[0] == '=')
// 			{
// 				printf("minishell: export: ");
// 				list_join(list->link->link);
// 				return ;
// 			}
// 			else if (list->link->link && check_export(list->link->link))
// 				return ;
// 			else
// 				return ;
// 		}
// 		return ;
// 	}
// }
