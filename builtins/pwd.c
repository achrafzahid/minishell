/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 02:06:25 by amabbadi          #+#    #+#             */
/*   Updated: 2025/04/25 11:14:16 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*pwd(void)
{
	char	buff[PATH_MAX];
	char	*path;

	if (!getcwd(buff, sizeof(buff)))
		return (NULL);
	path = ft_strdup(buff);
	if (!path)
		return (NULL);
	return (path);
}
