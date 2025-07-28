#include "../minishell.h"

int	is_redirection(char c)
{
	return (c == '<' || c == '>');
}

int	isquote(char c)
{
	return (c == '\'' || c == '\"');
}

char	*alloc_word(char *des, int len, char *src)
{
	int	i;

	i = 0;
	while (src && src[i] && i < len)
	{
		des[i] = src[i];
		i++;
	}
	des[i] = 0;
	return (des);
}

char *parse_redir_target(const char *str, int *i)
{
	char buf[PATH_MAX * 2];
	int j = 0;
	while (str[*i] && !ft_isspace(str[*i]) && !is_redirection(str[*i]))
	{
		if (isquote(str[*i]))
		{
			char quote = str[(*i)++];
			while (str[*i] && str[*i] != quote)
				buf[j++] = str[(*i)++];
			if (str[*i] == quote)
				(*i)++;
		}
		else
		{
			buf[j++] = str[(*i)++];
		}
	}
	while (str[*i] && !ft_isspace(str[*i]) && !is_redirection(str[*i]))
	{
		if (isquote(str[*i]))
		{
			char quote = str[(*i)++];
			while (str[*i] && str[*i] != quote)
				buf[j++] = str[(*i)++];
			if (str[*i] == quote)
				(*i)++;
		}
		else
		{
			buf[j++] = str[(*i)++];
		}
	}
	buf[j] = 0;
	return ft_strdup(buf);
}

char	**allocate(char **arr, char *str, int cc)
{
	int		i;
	int		wrdlen;
	int		j;

	j = 0;
	cc += 1 - 1;
	i = 0;
	while (str[i])
	{
		while (ft_isspace(str[i]))
			i++;
		if (is_redirection(str[i]))
		{
			i++;
			if (str[i] && is_redirection(str[i])) // Handle >> or <<
				i++;
			while (ft_isspace(str[i]))
				i++;
			// Skip the target of redirection
			while (str[i] && !ft_isspace(str[i]) && !is_redirection(str[i]))
			{
				if (isquote(str[i]))
				{
					char quote = str[i++];
					while (str[i] && str[i] != quote)
						i++;
					if (str[i] == quote)
						i++;
				}
				else
					i++;
			}
		}
		else if (str[i])
		{
			wrdlen = ft_wdlen(str + i);
			if (wrdlen == -1)
				return (NULL);
			arr[j] = (char *)malloc(wrdlen + 1);
			if (!arr[j])
				return (NULL);
			arr[j] = alloc_word(arr[j], wrdlen, str + i);
			i += wrdlen;
			j++;
		}
	}
	arr[j] = NULL;
	return (arr);
}

int	ft_wdlen(char *str)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i] && !ft_isspace(str[i]) && !is_redirection(str[i]))
	{
		if (isquote(str[i]))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i])
				i++;
		}
		else
			i++;
	}
	return (i);
}
