/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_access.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:27:54 by azahid            #+#    #+#             */
/*   Updated: 2025/04/20 17:03:39 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**parse_envp(t_env *e)
{
	char	**sp;

	while (e)
	{
		if (e->key && !ft_strcmp(e->key, "PATH"))
		{
			if (!e->value)
				return (NULL);
			sp = ft_split(e->value, ':');
			if (!sp)
				return (NULL);
			return (sp);
		}
		e = e->next;
	}
	return (NULL);
}

char	*joined(char *commande, char *sp)
{
	char	*com;
	char	*joinedpath;

	com = ft_strjoin("/", commande);
	if (!com)
		return (NULL);
	joinedpath = ft_strjoin(sp, com);
	free(com);
	if (!joinedpath)
		return (NULL);
	return (joinedpath);
}

char	*find_access(t_comm *com)
{
	char	**sp;
	char	*joinedpath;
	int		i;

	i = 0;
	if (com->p_com && ft_strchr(com->p_com->str, '/'))
	{
		if (!access(com->p_com->str, F_OK))
			return (ft_strdup(com->p_com->str));
		return (NULL);
	}
	sp = parse_envp(com->env);
	while (sp && sp[i] && com->p_com && com->p_com->str)
	{
		joinedpath = joined(com->p_com->str, sp[i]);
		if (!joinedpath)
		{
			free2d(sp);
			return (NULL);
		}
		if (!access(joinedpath, F_OK | X_OK))
		{
			free2d(sp);
			return (joinedpath);
		}
		free(joinedpath);
		i++;
	}
	if (com->p_com && !access(com->p_com->str, F_OK | X_OK))
	{
		free2d(sp);
		return (ft_strdup(com->p_com->str));
	}
	free2d(sp);
	return (NULL);
}

char	*createargs(t_comm *com)
{
	char	*first;

	first = find_access(com);
	if (!first)
		return (NULL);
	free(com->p_com->str);
	com->p_com->str = first;
	return (com->p_com->str);
}
