#include "../minishell.h"

static int	match_pattern(const char *pattern, const char *str)
{
	if (!*pattern && !*str)
		return (1);
	if (*pattern == '*')
	{
		pattern++;
		if (!*pattern)
			return (1);
		while (*str)
		{
			if (match_pattern(pattern, str))
				return (1);
			str++;
		}
		return (match_pattern(pattern, str));
	}
	if (!*str)
		return (0);
	if (*pattern == '?' || *pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (0); // Characters don't match
}

static t_chars	*wild_no_pattern(char *wildin)
{
	t_chars	*node;

	node = malloc(sizeof(t_chars));
	if (!node)
		return (NULL);
	node->str = ft_strdup(wildin);
	node->type = 0;
	node->next = NULL;
	if (!node->str)
	{
		free(node);
		return (NULL);
	}
	return (node);
}

static DIR	*wild_open_dir(char *wildin, t_chars **new_nodes)
{
	DIR	*dir;

	dir = opendir(".");
	if (!dir)
	{
		perror("opendir");
		*new_nodes = malloc(sizeof(t_chars));
		if (!*new_nodes)
			return (NULL);
		(*new_nodes)->str = ft_strdup(wildin);
		(*new_nodes)->type = 0;
		(*new_nodes)->next = NULL;
		if (!(*new_nodes)->str)
		{
			free(*new_nodes);
			*new_nodes = NULL;
		}
	}
	return (dir);
}

static int	wild_read_dir(DIR *dir, char *wildin, t_chars **new_nodes,
		t_chars **last_node)
{
	struct dirent	*entry;
	t_chars			*node;

	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name,
				"..") == 0)
			continue ;
		if (match_pattern(wildin, entry->d_name))
		{
			node = malloc(sizeof(t_chars));
			if (!node)
			{
				free_chars(*new_nodes);
				*new_nodes = NULL;
				return (1);
			}
			node->str = ft_strdup(entry->d_name);
			node->type = 0;
			node->next = NULL;
			if (!node->str)
			{
				free(node);
				free_chars(*new_nodes);
				*new_nodes = NULL;
				return (1);
			}
			if (!*new_nodes)
				*new_nodes = *last_node = node;
			else
			{
				(*last_node)->next = node;
				*last_node = node;
			}
		}
	}
	return (0);
}

static t_chars	*wild_fallback(char *wildin, DIR *dir)
{
	t_chars	*node;

	node = malloc(sizeof(t_chars));
	if (!node)
	{
		closedir(dir);
		return (NULL);
	}
	node->str = ft_strdup(wildin);
	node->type = 0;
	node->next = NULL;
	if (!node->str)
	{
		free(node);
		closedir(dir);
		return (NULL);
	}
	closedir(dir);
	return (node);
}

t_chars	*parse_wildcards(char *wildin, t_comm *com)
{
	t_chars *new_nodes = NULL;
	t_chars *last_node = NULL;

	if (!wildin || !com)
		return (NULL);
	if (!ft_strchr(wildin, '*') && !ft_strchr(wildin, '?'))
		return (wild_no_pattern(wildin));
	DIR *dir = wild_open_dir(wildin, &new_nodes);
	if (!dir)
		return (new_nodes);
	if (wild_read_dir(dir, wildin, &new_nodes, &last_node) != 0)
	{
		closedir(dir);
		return (NULL);
	}
	if (!new_nodes)
		return (wild_fallback(wildin, dir));
	closedir(dir);
	return (new_nodes);
}