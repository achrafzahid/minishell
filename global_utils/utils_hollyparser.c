#include "../minishell.h"

char	*get_env_value(char *key, t_env *env)
{
	if (!key || !env)
		return ("");
	while (env)
	{
		if (env->key && ft_strcmp(env->key, key) == 0)
			return (env->value ? env->value : "");
		env = env->next;
	}
	return ("");
}

int	get_var_name(const char *wrd, char *var, int max_len, int *var_len)
{
	int	vi;
	int	i;

	i = 1;
	vi = 0;
	if (!wrd[i])
		return (0);
	if (wrd[i] == '?')
	{
		if (vi < max_len - 1)
		{
			var[vi++] = '?';
			var[vi] = '\0';
		}
		*var_len = vi;
		return (i + 1);
	}
	while (wrd[i] && (isalnum(wrd[i]) || wrd[i] == '_'))
	{
		if (vi < max_len - 1)
			var[vi++] = wrd[i];
		i++;
	}
	var[vi] = '\0';
	*var_len = vi;
	return (i);
}

int	expander_count(char *wrd, t_env *env)
{
	char	var[256];
	int		var_len;
	char	*val;
	int		len;
	char	status_str[12];

	var_len = 0;
	if (!wrd || wrd[0] != '$' || !env)
		return (0);
	get_var_name(wrd, var, 256, &var_len);
	if (var_len == 0)
		return (0);
	if (var[0] == '?' && var[1] == '\0')
	{
		len = snprintf(status_str, sizeof(status_str), "%d", env->exit_status);
		return (len);
	}
	val = get_env_value(var, env);
	len = 0;
	while (val[len])
		len++;
	return (len);
}

int	cw(char *wrd, t_env *env, int type)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!wrd)
		return (0);
	if (has_unbalanced_quotes(wrd))
	{
		if (env)
			env->exit_status = 2;
		printf("bash: syntax error: unbalanced quotes\n");
		return (-1);
	}
	while (wrd[i])
	{
		if (type == 2)
			return (ft_strlen(wrd));
		else if (wrd[i] == '\'')
			count += handle_squotes(wrd, &i);
		else if (wrd[i] == '"')
			count += handle_dquotes(wrd, &i, env);
		else if (wrd[i] == '$' && wrd[i + 1])
		{
			count += expander_count(wrd + i, env);
			i++;
			while (isalnum(wrd[i]) || wrd[i] == '_' || wrd[i] == '?')
				i++;
		}
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

char	*fill_word(char *dest, char *src, t_env *env, int type, int *flag)
{
	int	si;
	int	di;

	si = 0;
	di = 0;
	if (!src || !dest)
		return (NULL);
	while (src[si])
	{
		if (type == 2)
		{
			if (src[si] == '\'' || src[si] == '"')
				si++;
			while (src[si] && src[si] != '\'' && src[si] != '"')
				dest[di++] = src[si++];
			if (src[si])
				si++;
		}
		if (src[si] == '\'' || type == 2)
		{
			if (src[si] == '\'')
				si++;
			while (src[si] && src[si] != '\'')
				dest[di++] = src[si++];
			if (src[si])
				si++;
		}
		else if (src[si] == '"')
		{
			si++;
			while (src[si] && src[si] != '"')
			{
				if (src[si] == '$' && src[si + 1] && !ft_isspace(src[si + 1])
					&& src[si + 1] != '"' && type != 2)
				{
					di += expand_variable(src + si, env, dest + di, &si);
				}
				else
					dest[di++] = src[si++];
			}
			if (src[si])
				si++;
		}
		else if (src[si] == '$' && src[si + 1])
		{
			di += expand_variable(src + si, env, dest + di, &si);
			(*flag)++;
		}
		else
			dest[di++] = src[si++];
	}
	dest[di] = '\0';
	return (dest);
}
