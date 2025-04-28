#include "../minishell.h"

size_t  ft_strlen(const char *str)
{
	int i;

	if (!str)
		return (0);
	i = 0;
	while (*str)
	{
		str++;
		i++;
	}
    return (i);
}
