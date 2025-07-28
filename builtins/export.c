#include "../minishell.h"

int	is_valid_identifier(const char *key)
{
	int	i;

	if (!key || !*key || (!isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_env_vars(t_env *env)
{
	t_env	*tmp;
	char	**keys;
	int		count = 0;
	int		i, j;

	// Count variables
	tmp = env;
	while (tmp)
	{
		if (tmp->key)
			count++;
		tmp = tmp->next;
	}

	if (count == 0)
		return;

	keys = malloc(sizeof(char *) * count);
	if (!keys)
		return;

	// Collect keys
	tmp = env;
	i = 0;
	while (tmp)
	{
		if (tmp->key)
			keys[i++] = tmp->key;
		tmp = tmp->next;
	}

	// Simple bubble sort
	for (i = 0; i < count - 1; i++)
	{
		for (j = 0; j < count - i - 1; j++)
		{
			if (ft_strcmp(keys[j], keys[j + 1]) > 0)
			{
				char *temp = keys[j];
				keys[j] = keys[j + 1];
				keys[j + 1] = temp;
			}
		}
	}

	// Print sorted
	for (i = 0; i < count; i++)
	{
		tmp = env;
		while (tmp)
		{
			if (tmp->key && ft_strcmp(tmp->key, keys[i]) == 0)
			{
				if (tmp->value)
					printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
				else
					printf("declare -x %s\n", tmp->key);
				break;
			}
			tmp = tmp->next;
		}
	}
	free(keys);
}

static void	parse_export_arg(char *arg, char **key, char **value, int *is_append)
{
	char	*tosawi;
	char	*append_pos;

	*key = NULL;
	*value = NULL;
	*is_append = 0;
	
	tosawi = ft_strchr(arg, '=');
	append_pos = ft_strnstr(arg, "+=", ft_strlen(arg));
	
	if (append_pos && (!tosawi || append_pos < tosawi))
	{
		*is_append = 1;
		*key = ft_substr(arg, 0, append_pos - arg);
		if (*key && is_valid_identifier(*key))
			*value = ft_strdup(append_pos + 2);
		else
		{
			free(*key);
			*key = NULL;
		}
	}
	else if (tosawi)
	{
		*key = ft_substr(arg, 0, tosawi - arg);
		if (*key && is_valid_identifier(*key))
			*value = ft_strdup(tosawi + 1);
		else
		{
			free(*key);
			*key = NULL;
		}
	}
	else
	{
		if (is_valid_identifier(arg))
			*key = ft_strdup(arg);
	}
}

static int	update_existing_var(t_env *tmp, char *key, char *value, int is_append)
{
	char	*new_value;

	if (tmp->key && ft_strcmp(tmp->key, key) == 0)
	{
		if (is_append && value)
		{
			if (tmp->value)
			{
				new_value = ft_strjoin(tmp->value, value);
				if (new_value)
				{
					free(tmp->value);
					tmp->value = new_value;
				}
				free(value);
			}
			else
			{
				tmp->value = value;
			}
		}
		else if (value)
		{
			free(tmp->value);
			tmp->value = value;
		}
		return (1);
	}
	return (0);
}

static void	add_new_var(t_env *env, char *key, char *value, int has_equal)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		fprintf(stderr, "minishell: export: cannot allocate memory\n");
		free(key);
		free(value);
		return ;
	}
	new_node->key = key;
	new_node->value = value ? value : (has_equal ? ft_strdup("") : NULL);
	new_node->next = NULL;
	new_node->exit_status = 0;
	
	tmp = env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

void	export(char *arg, t_env *env)
{
	char	*key;
	char	*value;
	int		is_append;
	t_env	*tmp;
	int		has_equal;
	int		found;

	if (!env)
		return ;
	if (!arg || !*arg)
	{
		print_env_vars(env);
		env->exit_status = 0;
		return ;
	}

	parse_export_arg(arg, &key, &value, &is_append);
	has_equal = ft_strchr(arg, '=') != NULL;
	
	if (!key)
	{
		fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
		env->exit_status = 1;
		return ;
	}

	found = 0;
	tmp = env;
	while (tmp)
	{
		if (update_existing_var(tmp, key, value, is_append))
		{
			found = 1;
			free(key);
			break;
		}
		tmp = tmp->next;
	}

	if (!found)
		add_new_var(env, key, value, has_equal);
	
	env->exit_status = 0;
}
