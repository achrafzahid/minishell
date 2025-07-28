/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 02:06:25 by amabbadi          #+#    #+#             */
/*   Updated: 2025/07/28 06:27:46 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*pwd(void)
{
	char	buff[PATH_MAX];
	char	*path;

	if (!getcwd(buff, sizeof(buff)))
	{
		perror("minishell: pwd");
		return (NULL);
	}
	path = ft_strdup(buff);
	if (!path)
		return (NULL);
	return (path);
}