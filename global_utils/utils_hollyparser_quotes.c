#include "../minishell.h"

int	is_squote(char c)
{
	return (c == '\'');
}

int	is_dquote(char c)
{
	return (c == '\"');
}

int	has_unbalanced_quotes(char *wrd)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	if (!wrd)
		return (1);
	while (wrd[i])
	{
		if (wrd[i] == '\'')
		{
			sq++;
			i++;
			while (wrd[i] && wrd[i] != '\'')
				i++;
			if (wrd[i])
			{
				sq++;
			}
		}
		else if (wrd[i] == '"')
		{
			dq++;
			i++;
			while (wrd[i] && wrd[i] != '"')
				i++;
			if (wrd[i])
			{
				dq++;
			}
		}
		if (wrd[i])
			i++;
	}
	return (sq % 2 || dq % 2);
}

int	handle_squotes(const char *wrd, int *i)
{
	int	count;

	count = 0;
	(*i)++;
	while (wrd[*i] && wrd[*i] != '\'')
	{
		count++;
		(*i)++;
	}
	if (wrd[*i])
		(*i)++;
	return (count);
}

int	handle_dquotes(const char *wrd, int *i, t_env *env)
{
	int	count;

	count = 0;
	(*i)++;
	while (wrd[*i] && wrd[*i] != '"')
	{
		if (wrd[*i] == '$' && wrd[*i + 1] && !ft_isspace(wrd[*i + 1])
			&& !isquote(wrd[*i + 1]))
		{
			count += expander_count((char *)(wrd + *i), env);
			(*i)++;
			while (isalnum(wrd[*i]) || wrd[*i] == '_' || wrd[*i] == '?')
				(*i)++;
		}
		else
		{
			count++;
			(*i)++;
		}
	}
	if (wrd[*i])
		(*i)++;
	return (count);
}
