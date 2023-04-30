#include "../minishell.h"

int    ft_count_arguments(char *input)
{
    int    i;
    int    flag;
    int    count;

    i = 0;
    flag = 0;
    count = 0;
    while (input[i])
    {
        while (input[i] && input[i] == ' ')
            i++;
        if (input[i] == '\"')
        {
            if (flag == 0)
            {
                flag = 1;
                count++;
                i++;
            }
            while (input[i] && input[i] != '\"')
                i++;
            if (flag == 1)
            {
                flag = 0;
                i++;
            }
        }
        else if (input[i])
        {
            while (input[i] && input[i] != ' ')
            {
                if (input[i] == '\"')
                    break;
                else
                    i++;
            }
            count++;
        }
    }
    return (count);
}

// int	ft_count_arguments(char *input)
// {
// 	int	i;
// 	int	flag;
// 	int	count;

// 	i = 0;
// 	flag = 0;
// 	count = 0;
// 	while (input[i])
// 	{
// 		if (flag == 0)
// 		{
// 			while (input[i] && input[i] == ' ')
// 				i++;
// 			if (input[i] && input[i] != '\"')
// 				count++;
// 			while (input[i] && input[i] != '\"' && input[i] != ' ')
// 				i++;
// 			if (input[i] == '\"')
// 			{
// 				flag = 1;
// 				i++;
// 			}
// 		}
// 		else
// 		{
// 			while (input[i] && input[i] != '\"')
// 				i++;
// 			if (input[i] == '\"')
// 			{
// 				flag = 0;
// 				count++;
// 			}
// 		}
// 	}
// 	return (count);
// }

void	ft_split_input(char *input)
{
	int		i;
	int		j;
	int		k;
	int		start;
	int		flag;
	char	**arr;

	i = 0;
	j = 0;
	k = 0;
	start = 0;
	flag = 0;
	arr = malloc (ft_count_arguments(input) * sizeof(char *) + 1);
	if (!arr)
		return ;
	while (input[i])
	{
		while (input[i] && input[i] == ' ')
			i++;
		if (input[i] == '\"')
		{
			start = i;
			while (input[++i] && input[i] != '\"')
				i++;
			arr[k] = ft_substr(input, start, i);
			k++;
			i++;
		}
		else{
			start = i;
			while (input[i] && input[i] != ' ')
				i++;
			arr[k] = ft_substr(input, start, i);
			k++;
		}
	}
	arr[k] = NULL;
	i = 0;
	while (arr[i])
		printf("|%s|\n", arr[i++]);
	
}