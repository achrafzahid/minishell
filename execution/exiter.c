/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:18:15 by azahid            #+#    #+#             */
/*   Updated: 2025/04/25 15:42:34 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exiter(char **exec)
{
	struct stat	sf;
	char		*res;
	char		*str;

	if (!exec && !exec[0])
		perror("");
	stat(exec[0], &sf);
	res = ft_strjoin("minishell : ", exec[0]);
	if (S_ISDIR(sf.st_mode) && ft_strchr(exec[0], '/'))
	{
		str = ft_strjoin(res, " : is a directory\n");
		(free(res), write(2, str, ft_strlen(str)), free(str), exit(126));
	}
	else if (S_ISDIR(sf.st_mode))
	{
		str = ft_strjoin(res, " : command not found\n");
		(free(res), write(2, str, ft_strlen(str)), free(str), exit(127));
	}
	else
	{
		str = ft_strjoin(res, " : Permission denied\n");
		(free(res), write(2, str, ft_strlen(str)), free(str), exit(126));
	}
}
