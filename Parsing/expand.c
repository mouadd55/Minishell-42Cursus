#include "../minishell.h"

void index_list(t_list **list)
{
	t_list *tmp;
	int		count;

	tmp = *list;
	count = 0;
	while (tmp)
	{
		tmp->pos = count;
		count++;
		tmp = tmp->link;
	}
}

t_list *del_node(t_list **list, t_list *del_node)
{
	t_list *tmp;
	t_list *tmp1 = NULL;

	tmp = *list;
	if (del_node == tmp)
	{
		tmp = tmp->link;
		free(tmp->prev);
		tmp->prev = NULL;
		return(tmp);
	}
	else if (del_node == ft_lstlast(tmp))
	{
		tmp1 = ft_lstlast(tmp);
		tmp1 = tmp1->prev;
		free(tmp1->link);
		tmp1->link = NULL;
	}
	else
	{
		while (tmp)
		{
			if (tmp == del_node)
			{
				tmp1 = tmp->prev;
				tmp1->link = tmp->link;
				tmp->link->prev = tmp1;
				free(tmp);
				tmp = tmp1;
			}
			tmp = tmp->link;
		}
	}
	return (tmp1);
}
void remove_quotes(t_list **list)
{
	t_list *temp;

	temp = *list;
	while (temp)
	{
		if (!ft_strcmp("DOUBLE_Q", temp->type) || !ft_strcmp("SINGLE_Q", temp->type))
		{
			if (ft_strlen(temp->content) == 2)
				temp->content = ft_strdup("");
			else
				temp->content = ft_substr(temp->content, 1, ft_strlen(temp->content)-2);
		}
		temp = temp->link;
	}
}

void expand_in_quotes(t_list **list, t_env *envr)
{
	t_list *temp;
	t_vars v;
	char *save = NULL;
	t_env *env;

	v.i = 0;
	temp = *list;
	v.flag = 1;
	while (temp)
	{
		v.i = 0;
		if (check_char(temp->content, '$') && !ft_strcmp("DOUBLE_Q", temp->type))
		{
			while (temp->content[v.i])
			{
				v.flag = 1;
				if (temp->content[v.i] != '$')
				{
					v.j = v.i;
					while (temp->content[v.i] && temp->content[v.i] != '$')
						v.i++;
					save = ft_strjoin(save, ft_substr(temp->content, v.j, v.i-v.j));
					// printf("save == %s\n", save);
				}
				else if (temp->content[v.i] == '$')
				{
					v.i++;
					v.j = v.i;
					while (temp->content[v.i] && (ft_isalpha(temp->content[v.i])
						|| check_char("0123456789", temp->content[v.i])))
						v.i++;
					
					v.str = ft_substr(temp->content, v.j, v.i-v.j);
					// printf("v.str == |%s|\n", v.str);
					env = envr;
					while (env)
					{
						if (!ft_strcmp(env->key, v.str))
						{
							save = ft_strjoin(save, env->value);
							v.flag = 0;
							break;
						}
						env = env->link;
					}
					if (v.flag == 1)
					{
						save = ft_strjoin(save, "$");
						save = ft_strjoin(save, v.str);
					}
				}
			}
			temp->content = ft_strdup(save);
			save = NULL;
		}
		temp = temp->link;
	}
}

void expand_var(t_list **list, t_env *envr)
{
	t_list *tmp = *list;
	// t_list *save_node;
	t_env *tmp1 = envr;
	t_vars v;
	v.i = 0;
	while (tmp)
	{
		if (tmp && tmp->content[0] == '$' && tmp->link)
		{
			tmp = tmp->link;
			while (ft_isalpha(tmp->content[v.i]) ||
				check_char("0123456789", tmp->content[v.i]))
				v.i++;
			v.str = ft_substr(tmp->content, 0, v.i);
			tmp1 = envr;
			while (tmp1)
			{
				if (tmp1 && !ft_strcmp(tmp1->key, v.str))
				{
					tmp->prev->content = ft_strdup(tmp1->value);
					tmp->prev->content = ft_strjoin(tmp->prev->content ,ft_strdup(&tmp->content[v.i]));
					tmp =  del_node(list, tmp);
					break;
				}
				tmp1 = tmp1->link;
			}
		}
		tmp = tmp->link;
	}
	remove_quotes(list);
	expand_in_quotes(list, envr);
}