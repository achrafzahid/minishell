/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:18:15 by azahid            #+#    #+#             */
/*   Updated: 2025/07/28 09:58:42 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exiter(char **exec)
{
	struct stat	sf;
	char		*res;
	char		*str;

	if (!exec || !exec[0])
	{
		fprintf(stderr, "minishell: command not found\n");
		exit(127);
	}
	if (stat(exec[0], &sf) == -1)
	{
		if (strchr(exec[0], '/'))
			fprintf(stderr, "minishell: %s: No such file or directory\n", exec[0]);
		else
			fprintf(stderr, "minishell: %s: command not found\n", exec[0]);
		free2d(exec);
		exit(127);
	}
	res = ft_strjoin("minishell: ", exec[0]);
	if (S_ISDIR(sf.st_mode) && ft_strchr(exec[0], '/'))
	{
		str = ft_strjoin(res, ": is a directory\n");
		free(res);
		write(2, str, ft_strlen(str));
		free(str);
		free2d(exec);
		exit(126);
	}
	else if (S_ISDIR(sf.st_mode))
	{
		str = ft_strjoin(res, ": command not found\n");
		free(res);
		write(2, str, ft_strlen(str));
		free(str);
		free2d(exec);
		exit(127);
	}
	else if (access(exec[0], X_OK) != 0)
	{
		str = ft_strjoin(res, ": Permission denied\n");
		free(res);
		write(2, str, ft_strlen(str));
		free(str);
		free2d(exec);
		exit(126);
	}
	else
	{
		str = ft_strjoin(res, ": No such file or directory\n");
		free(res);
		write(2, str, ft_strlen(str));
		free(str);
		free2d(exec);
		exit(127);
	}
}
