/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:18:15 by azahid            #+#    #+#             */
/*   Updated: 2025/07/28 10:07:52 by amabbadi         ###   ########.fr       */
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
		perror("minishell: command not found");
		exit(127);
	}
	
	// Check if file exists first
	if (stat(exec[0], &sf) == -1)
	{
		// File doesn't exist - exit code 127
		fprintf(stderr, "minishell: %s: No such file or directory\n", exec[0]);
		free2d(exec);
		exit(127);
	}
	
	res = ft_strjoin("minishell : ", exec[0]);
	if (S_ISDIR(sf.st_mode) && ft_strchr(exec[0], '/'))
	{
		str = ft_strjoin(res, " : is a directory\n");
		free(res);
		write(2, str, ft_strlen(str));
		free(str);
		free2d(exec);
		exit(126);
	}
	else if (S_ISDIR(sf.st_mode))
	{
		str = ft_strjoin(res, " : command not found\n");
		free(res);
		write(2, str, ft_strlen(str));
		free(str);
		free2d(exec);
		exit(127);
	}
	else
	{
		// File exists but check if it has execute permission
		if (access(exec[0], X_OK) == -1)
		{
			// File exists but no execute permission - exit code 126
			fprintf(stderr, "minishell: %s: Permission denied\n", exec[0]);
			free(res);
			free2d(exec);
			exit(126);
		}
		else
		{
			// This shouldn't happen if we reach here, but handle it
			str = ft_strjoin(res, " : Permission denied\n");
			free(res);
			write(2, str, ft_strlen(str));
			free(str);
			free2d(exec);
			exit(126);
		}
	}
}
