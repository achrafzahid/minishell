/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 02:05:33 by amabbadi          #+#    #+#             */
/*   Updated: 2025/04/19 14:07:54 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	cd_get_current_dir(char *new_dir, size_t size)
{
	if (getcwd(new_dir, size) == NULL)
	{
		perror("cd: error getting current directory");
		return (1);
	}
	return (0);
}

static int	cd_change_to_home(t_env *e, char *prev_dir, char *new_dir)
{
	while (e)
	{
		if (e->key && (!ft_strcmp(e->key, "HOME") || !ft_strcmp(e->key, "~"))
			&& e->value)
		{
			if (chdir(e->value) != 0)
				return (perror("cd"), 1);
			if (prev_dir[0] != '\0')
				strncpy(prev_dir, new_dir, PATH_MAX - 1);
			return (0);
		}
		e = e->next;
	}
	fprintf(stderr, "cd: HOME not set\n");
	return (1);
}

static int	cd_handle_special(char *path, t_env *e, char *prev_dir,
		char *new_dir)
{
	if (!path || ft_strcmp(path, "~") == 0)
		return (cd_change_to_home(e, prev_dir, new_dir));
	if (ft_strcmp(path, ".") == 0)
		return (0);
	if (ft_strcmp(path, "/") == 0)
	{
		if (chdir("/") != 0)
			return (perror("cd"), 1);
		if (prev_dir[0] != '\0')
			strncpy(prev_dir, new_dir, PATH_MAX - 1);
		return (0);
	}
	if (ft_strcmp(path, "-") == 0)
	{
		if (prev_dir[0] == '\0')
		{
			fprintf(stderr, "cd: no previous directory\n");
			return (1);
		}
		if (chdir(prev_dir) != 0)
			return (perror("cd"), 1);
		printf("%s\n", prev_dir);
		strncpy(prev_dir, new_dir, PATH_MAX - 1);
		return (0);
	}
	return (-1);
}

static int	cd_change_dir(char *path, char *prev_dir, char *new_dir)
{
	if (access(path, F_OK) != 0)
		return (perror("cd"), 1);
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	strncpy(prev_dir, new_dir, PATH_MAX - 1);
	return (0);
}

int	cd(char *path, t_env *e)
{
	static char	prev_dir[PATH_MAX];
	static char	new_dir[PATH_MAX];
	int			ret;

	if (cd_get_current_dir(new_dir, sizeof(new_dir)) != 0)
		return (1);
	ret = cd_handle_special(path, e, prev_dir, new_dir);
	if (ret >= 0)
		return (ret);
	return (cd_change_dir(path, prev_dir, new_dir));
}
