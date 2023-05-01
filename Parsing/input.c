#include "../minishell.h"

int is_space(int c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	ft_count_arguments(char *input)
{
	t_vars	var;

	var.i = 0;
	var.count = 0;
	while (input[var.i])
	{
		while (input[var.i] && is_space(input[var.i]))
			var.i++;
		if (input[var.i] == '\"')
		{
			var.i++;
			var.count++;
			while (input[var.i] && input[var.i] != '\"')
				var.i++;
			var.i++;
		}
		else if (input[var.i])
		{
			while (input[var.i] && input[var.i] != ' ' && input[var.i] != '\"')
				var.i++;
			var.count++;
		}
	}
	return (var.count);
}

void	ft_split_input(char *input)
{
	int		i;
	char	**arr;

	i = 0;
	arr = ft_split(input, ' ');
	if (!arr)
		return ;

}

// char	*ft_strjoin_c(char *s1, char c)
// {
// 	char	*str;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	if (!s1)
// 		s1 = ft_strdup("");
// 	if (!c)
// 		return (NULL);
// 	str = malloc((ft_strlen(s1) + 2) * sizeof(char));
// 	if (!str)
// 		return (NULL);
// 	while (s1[i])
// 	{
// 		str[i] = s1[i];
// 		i++;
// 	}
// 	str[i] = c;
// 	str[++i] = '\0';
// 	// free(s1);
// 	return (str);
// }
