/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 13:25:55 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/29 12:05:03 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "string.h"
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_list
{
	char			*content;
	int				pos;
	char			*type;
	struct s_list	*link;
	struct s_list	*prev;
}	t_list;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*link;
	struct s_env	*prev;
}	t_env;

typedef struct s_command
{
	char			**cmd;   // {"ls", "-la", NULL}
	int				fd_in;  //0
	int				fd_out; //1
	struct s_command	*link;
	struct s_command	*prev;
}	t_command;

typedef struct t_vars
{
	int		i;
	int		j;
	int		end;
	int		flag;
	int		vars;
	int		start;
	int		count;
	int		d_quotes;
	char	*tmp;
	char	*str;
	char	*var;
	char	*val;
	char	**arr;
	char	*tmp_key;
	char	*tmp_value;
	t_list	*tmp1;
	t_list	*tmp2;
	t_list	*tmp3;
	t_env	*temp1;
	t_env	*temp2;
	t_env	*temp3;
}	t_vars;

/********************************* Libft utils ********************************/

int			is_space(int c);
int			ft_isalpha(int ch);
char		**ft_free_arr(char **str);
size_t		ft_strlen(const char *str);
char		*ft_strdup(const char *s1);
void		ft_putchar(char ch, int fd);
int			ft_strrchr(char *str, int c);
void		ft_putstr_fd(char *s, int fd);
int			check_char(char *input, int c);
int			ft_strchr( char *str, int find);
char		*ft_strjoin(char *s1, char *s2);
char		*f_strchr(const char *s, int c);
char		*f_strrchr(const char *s, int c);
char		*ft_strtrim(char *s1, char *set);
char		*ft_strtrim(char *s1, char *set);
char		**ft_split(char const *s, char *c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strnstr(const char *str, const char *to_find, size_t len);
char		is_special(char c);
/********************************* List utils *********************************/

int			ft_lstsize(t_list *lst);
t_list		*ft_lstnew(char *content);
t_list		*ft_lstlast(t_list *head);
int			ft_lstsize_env(t_env *env);
t_env		*ft_lstlast_env(t_env *head);
void		*ft_destroy_list(t_list **head);
void		*ft_destroy_list_env(t_env **head);
t_env		*ft_lstnew_env(char *key, char *value);
void		ft_lstadd_back(t_list **head, t_list *new);
void		ft_lstadd_back_env(t_env **head, t_env *new);

/***************************** Parsing functions ******************************/

void		lexer(t_list **list);
int			check_syntax(t_list *lst);
t_list		*ft_split_input(char *input);
void		syntax_error(char *s2, char e);
size_t		ft_count_char(char *input, char c);
void		expand_var(t_list **list, t_env *envr);

/**************************** Builtins functions *****************************/

void		sort_env(t_env *env);
void		ft_exit(t_list **list);
int			check_type(char *type);
int			check_valid_var(char *var);
void		print_export(t_env *temp3);
t_env		*ft_copy_env_list(t_env *env);
void		free_some_variables(t_vars *v);
void		join_variable_names(t_vars *v);
t_list		*lexer_for_unset(t_list **list);
void		lexer_for_export(t_list **list);
int			check_valid_variable(char *input);
void		unset(t_list **list, t_env **env);
t_env		*ft_split_environment(char **env);
void		env_parsing(char *input, t_env *env);
void		mooooore_steps(t_vars *v, t_env **env);
void		delete_node(t_env **env, int position);
int			export_parsing(t_list **list, t_env **env);
void		check_cmd(t_list **list, t_env **envr, char *input, int fd);
int			check_if_variable_exist(t_env *env, char *var, t_env **tmp);
t_list		*skip_whats_before_the_first_var(t_list *tmp, t_list *list);
int			open_files(t_list *list, t_command **final_list);
int			ft_printf_fd(const char *first, int fd, ...);
int			count_commands(t_list *list);
void		create_final_list(t_list *list, t_command **final_list);
t_command	*lstnew_final(char **command, int fd_in, int fd_out);
t_command	*lstlast_final(t_command *head);
void		lstadd_back_final(t_command **head, t_command *new);
void		switch_space(char *input, int x);

#endif