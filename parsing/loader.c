/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 23:09:58 by azahid            #+#    #+#             */
/*   Updated: 2025/04/17 08:32:52 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	skip_quoted_section(char *str, int i)
{
	char	quote;

	quote = str[i++];
	while (str[i] && str[i] != quote)
		i++;
	if (str[i])
		i++;
	return (i);
}

static int	get_token_end(char *str, int start, int fquotes)
{
	int		j;
	char	quote;

	j = start;
	if (isquote(str[j]))
	{
		if (str[j] == '\"')
			fquotes++;
		quote = str[j++];
		while (str[j] && str[j] != quote)
			j++;
		if (str[j])
			j++; // Include closing quote
	}
	else
	{
		while (str[j] && !ft_isspace(str[j]) && !is_redirection(str[j]))
			j++;
	}
	while (str[j] && !ft_isspace(str[j]) && !is_redirection(str[j]))
		j++;
	return (j);
}

static int	check_syntax_error(char *str, int i)
{
	if (is_redirection(str[i]) || str[i] == '|')
	{
		perror("Syntax error");
		return (-1);
	}
	return (0);
}

static int	process_redirection(char *str, int *i, t_comm *comm, t_env *env,
		int redir_type)
{
	int		j;
	char	*sub;
	char	**parsed;
	int		fquotes;
	int		k;

	fquotes = 0;
	k = 0;
	if (check_syntax_error(str, *i))
		return (-1);
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
	j = get_token_end(str, *i, fquotes);
	if (j > *i)
	{
		sub = ft_substr(str, *i, j - *i);
		if (!sub)
			return (-1);
		parsed = parser(sub, env, fquotes, redir_type); // expand normally
		if (!parsed)
		{
			free(sub);
			return (-1);
		}
		if (redir_type == 2 && fquotes > 0)
		{
			push_to_list(&comm->redirections, sub, redir_type);
		}
		else
		{
			while (parsed[k])
				push_to_list(&comm->redirections, parsed[k++], redir_type);
			free(sub);
		}
		free(parsed);
	}
	*i = j;
	return (0);
}

int	loader(char *str, t_comm *comm, t_env *env)
{
	int	i;
	int	len;

	(void)(env);
	i = 0;
	len = ft_strlen(str);
	while (str && str[i])
	{
		while (str[i] && !is_redirection(str[i]) && !isquote(str[i]))
			i++;
		if (isquote(str[i]))
			i = skip_quoted_section(str, i);
		else if (str[i] && str[i] == str[i + 1] && str[i] == '<')
		{
			i += 2;
			if (process_redirection(str, &i, comm, env, 2) < 0)
				return (-1);
		}
		else if (str[i] && str[i] == str[i + 1] && str[i] == '>')
		{
			i += 2;
			if (process_redirection(str, &i, comm, env, 3) < 0)
				return (-1);
		}
		else if (str[i] == '<')
		{
			i++;
			if (process_redirection(str, &i, comm, env, 0) < 0)
				return (-1);
		}
		else if (str[i] == '>')
		{
			i++;
			if (process_redirection(str, &i, comm, env, 1) < 0)
				return (-1);
		}
		else if (i < len - 1)
			i++;
	}
	return (0);
}
