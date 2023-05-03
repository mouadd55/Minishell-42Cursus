#include "../minishell.h"

void	syntax_error(char *s2, char e)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	if (s2)
		ft_putstr_fd(s2, 2);
	if (e)
		ft_putchar(e, 2);
	ft_putchar('\n', 2);
	return ;
}

int	check_double_quotes(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	if (ft_strchr(input, '\"') || input[0] == '\"')
	{
		i = ft_strchr(input, '\"');
		while (input[i++])
		{
			if (input[i] == '\"')
			{
				count++;
				break ;
			}
		}
		if (count % 2 != 0)
		{
			syntax_error(NULL, '\"');
			return (1);
		}
		else
			check_double_quotes(&input[i + 1]);
	}
	return (0);
}

int	check_single_quotes(char *input)
{
	t_vars v;

	v.i = 0;
	v.flag = 0;
	v.count = 0;
	v.d_quotes = 0;
	while (input[v.i])
	{
		if (input[v.i] == '\"' && (v.d_quotes % 2 == 0))
		{
			v.flag = 1;
			v.d_quotes++;
		}
		else if (input[v.i] == '\"' && (v.d_quotes % 2 != 0))
		{
			v.flag = 0;
			v.d_quotes++;
		}
		if (input[v.i] == '\\' && input[v.i + 1] && input[v.i + 1] == '\'')
			v.i += 2;
		else if (input[v.i] == '\'')
		{
			if (v.flag == 0)
				v.count++;
			v.i++;
		}
		else
			v.i++;
	}
	if (v.count % 2)
		return (syntax_error(NULL, '\''), 1);
	return (0);
}
