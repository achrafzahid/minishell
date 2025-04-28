#include "../minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char *str;
	int i;
	unsigned int l;

	l = ft_strlen(s);
	if (s == NULL)
		return (NULL);
	if (len > l - start)
		len = l - start;
	if (l <= start)
		return (ft_strdup(""));
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (len-- && *(s + start))
	{
		*(str + i) = *(s + start);
		i++;
		s++;
	}
	*(str + i) = 0;
	return (str);
}