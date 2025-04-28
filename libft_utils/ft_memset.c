#include "../minishell.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char *ptr;
	unsigned char value;
	size_t i;

	ptr = (unsigned char *)s;
	value = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		ptr[i] = value;
		i++;
	}
	return (s);
}