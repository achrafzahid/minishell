/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hollyparser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 02:09:12 by azahid            #+#    #+#             */
/*   Updated: 2025/04/19 08:30:50 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	return (""); // Key not found
}

static int	get_var_name(const char *wrd, char *var, int max_len, int *var_len)
{
	int	vi;

	int i = 1; // Skip $
	vi = 0;
	// Special case: $ alone or $ at end
	if (!wrd[i])
		return (0);
	// Handle special variable $?
	if (wrd[i] == '?')
	{
		if (vi < max_len - 1)
		{
			var[vi++] = '?';
			var[vi] = '\0';
		}
		*var_len = vi;
		return (i + 1); // Skip $ and ?
	}
	// Extract alphanumeric or _ for variable name
	while (wrd[i] && (isalnum(wrd[i]) || wrd[i] == '_'))
	{
		if (vi < max_len - 1) // Prevent buffer overflow
			var[vi++] = wrd[i];
		i++;
	}
	var[vi] = '\0';
	*var_len = vi;
	return (i);
}

int	expander_count(char *wrd, t_env *env)
{
	char	var[256] = {0};
	int		var_len;
//	int		i;
	char	*val;
	int		len;
		char status_str[12];

	var_len = 0;
	if (!wrd || wrd[0] != '$' || !env)
		return (0);
	// Parse variable name
	get_var_name(wrd, var, 256, &var_len);
	if (var_len == 0)
		return (0);
	// Handle $? specially
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

int	is_squote(char c)
{
	return (c == '\'');
}

int	is_dquote(char c)
{
	return (c == '\"');
}
//"ls""
int	has_unbalanced_quotes(char *wrd)
{
	int	i = 0, sq = 0, dq;

	i = 0, sq = 0, dq = 0;
	if (!wrd)
		return (1);
	while (wrd[i])
	{
		if (wrd[i] == '\''){
      sq++; 
      i++;
      while(wrd[i] && wrd[i] != '\'')
        i++;
      if(wrd[i]) {sq++;}
    }
		else if (wrd[i] == '"'){
			dq++;
      i++;
      while(wrd[i] && wrd[i] != '"')
        i++;
      if(wrd[i]) {dq++;}
    }
    if (wrd[i]) i++;
	}
	return (sq % 2 || dq % 2);
}

int	handle_squotes(const char *wrd, int *i)
{
	int	count;

	count = 0;
	(*i)++; // Skip opening '
	while (wrd[*i] && wrd[*i] != '\'')
	{
		count++;
		(*i)++;
	}
	if (wrd[*i])
		(*i)++; // Skip closing '
	return (count);
}

int	handle_dquotes(const char *wrd, int *i, t_env *env)
{
	int	count;

	count = 0;
	(*i)++; // Skip opening "
	while (wrd[*i] && wrd[*i] != '"')
	{
		if (wrd[*i] == '$' && wrd[*i + 1] && !ft_isspace(wrd[*i + 1]) && !isquote(wrd[*i + 1]))
		{
			count += expander_count((char *)(wrd + *i), env);
			(*i)++; // Skip $
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

int	cw(char *wrd, t_env *env,int type)
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
      return ft_strlen(wrd);
    else if (wrd[i] == '\'')
			count += handle_squotes(wrd, &i);
		else if (wrd[i] == '"')
			count += handle_dquotes(wrd, &i, env);
		else if (wrd[i] == '$' && wrd[i + 1])
		{
			count += expander_count(wrd + i, env);
			i++; // Skip $
			while (isalnum(wrd[i]) || wrd[i] == '_' || wrd[i] == '?')
				i++; // Skip variable name or ?
		}
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

int	expand_variable(char *src, t_env *env, char *dest, int *si)
{
	char	var[256] = {0};
	int		var_len;
	//int		i;
	char	*val;
	int		len;

	var_len = 0;
	len = 0;
	if (!src || !dest || !env)
		return (0);
	// Parse variable name
	get_var_name(src, var, 256, &var_len);
	if (var_len == 0) // No valid variable
	{
		*si += 1; // Skip $
		return (0);
	}
	if (var[0] == '?' && var[1] == '\0')
	{
		char status_str[12]; // enough for intmax
		len = snprintf(status_str, sizeof(status_str), "%d", env->exit_status);
		for (int j = 0; j < len; j++)
			dest[j] = status_str[j];
		*si += 2;
		return (len);
	}
	val = get_env_value(var, env);
	if (val)
	{
		while (val[len])
		{
			dest[len] = val[len];
			len++;
		}
	}
	*si += var_len + 1;
	return (len);
}

char	*fill_word(char *dest, char *src, t_env *env,int type,int *flag)
{
	int	si;
	int	di;

	si = 0;
	di = 0;
	if (!src || !dest)
		return NULL;
	while (src[si])
	{
		if (type == 2)
		{
			if (src[si] == '\'' || src[si] == '"') si++; // Skip opening '
			while (src[si] && src[si] != '\'' && src[si] != '"')
				dest[di++] = src[si++];
			if (src[si])
				si++;
		}
		if (src[si] == '\'' || type == 2)
		{
			if (src[si] == '\'') si++; // Skip opening '
			while (src[si] && src[si] != '\'')
				dest[di++] = src[si++];
			if (src[si])
				si++;
		}
		else if (src[si] == '"')
		{
			si++; // Skip opening "
			while (src[si] && src[si] != '"')
			{
				if (src[si] == '$' && src[si + 1] && !ft_isspace(src[si + 1]) && src[si + 1] != '"' && type != 2)
        {
					di += expand_variable(src + si, env, dest + di, &si);
        }
        else
					dest[di++] = src[si++];
			}
			if (src[si])
				si++;
		}
		else if (src[si] == '$' && src[si + 1]){
			di += expand_variable(src + si, env, dest + di, &si);
      (*flag)++;}
		else
			dest[di++] = src[si++];
	}
	dest[di] = '\0';
  return dest;
}

char	**parser(char *str, t_env *env,int flag, int type)
{
	int countw;
	char *res;
  char **result;
  int f =  0;
  flag++;
	if (!str)
		return (NULL);
	if (str[0] == '\0')
	{
		res = ft_strdup("");
		if (!res && env)
			env->exit_status = 1;
		return (ft_split(res, 0));
	}
	countw = cw(str, env, type);
	if (countw == -1)
		return (NULL);
	res = malloc(sizeof(char) * (countw + 1));
	if (!res)
	{
		if (env)
			env->exit_status = 1;
		printf("bash: cannot allocate memory\n");
		return (NULL);
	}
	res = fill_word(res, str, env,type,&f);
  if (f)
    result = ft_split(res, ' ');
  else
    result = ft_split(res, 0);
	return (free(res), result);
}
