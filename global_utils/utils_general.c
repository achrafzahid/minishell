#include "../minishell.h"

// gives me how much element i should allocate in the array of structs
int	double_array_size(char **str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (-1);
	while (str[i])
		i++;
	return (i);
}

// skip spaces in a string from an existing
// index and return where the index next should be
void	skip_spaces(int *i, char *str)
{
	if (!str)
		return ;
	while (str[(*i)] == ' ')
		(*i)++;
}

/* this checks for spaces in a character , pretty basic*/
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

int	is_void(char *s)
{
	while (*s)
	{
		if (!isspace(*s))
			return (0);
		s++;
	}
	return (1);
}
