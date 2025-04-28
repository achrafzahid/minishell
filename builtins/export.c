#include "../minishell.h"

static int	is_valid_identifier(const char *key)
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

	tmp = env;
	while (tmp)
	{
		if (tmp->key)
		{
			if (tmp->value)
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			else
				printf("declare -x %s\n", tmp->key);
		}
		tmp = tmp->next;
	}
	env->exit_status = 0;
}

static void	parse_export_arg(char *arg, char **key, char **value,
		int *is_append)
{
	char	*tosawi;
	char	*append_pos;

	tosawi = ft_strchr(arg, '=');
	append_pos = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (append_pos)
	{
		*is_append = 1;
		*key = ft_substr(arg, 0, append_pos - arg);
		*value = ft_strdup(append_pos + 2);
	}
	else if (tosawi)
	{
		*key = ft_substr(arg, 0, tosawi - arg);
		*value = ft_strdup(tosawi + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

static int	update_existing_var(t_env *tmp, char *key, char *value,
		int is_append)
{
	char	*new_value;

	if (tmp->key && ft_strcmp(tmp->key, key) == 0)
	{
		if (is_append && value && tmp->value)
		{
			new_value = ft_strjoin(tmp->value, value);
			if (new_value)
			{
				free(tmp->value);
				tmp->value = new_value;
			}
			free(value);
		}
		else if (value || tmp->value)
		{
			free(tmp->value);
			tmp->value = value ? value : ft_strdup("");
		}
		free(key);
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
		env->exit_status = 1;
		return ;
	}
	new_node->key = key;
	new_node->value = value ? value : (has_equal ? ft_strdup("") : NULL);
	new_node->next = NULL;
	tmp = env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	env->exit_status = 0;
}

void	export(char *arg, t_env *env)
{
	char	*key;
	int		is_append;
	t_env	*tmp;
	char 	*value;

	key = NULL;
	value = NULL;
	is_append = 0;
	if (!env)
	{
		fprintf(stderr, "minishell: export: environment not initialized\n");
		return ;
	}
	if (!arg || !*arg)
	{
		print_env_vars(env);
		return ;
	}
	parse_export_arg(arg, &key, &value, &is_append);
	if (!key || !is_valid_identifier(key))
	{
		fprintf(stderr, "minishell: export: `%s': not a valid identifier\n",
			arg);
		free(key);
		free(value);
		env->exit_status = 1;
		return ;
	}
	tmp = env;
	while (tmp)
	{
		if (update_existing_var(tmp, key, value, is_append))
		{
			env->exit_status = 0;
			return ;
		}
		tmp = tmp->next;
	}
	add_new_var(env, key, value, ft_strchr(arg, '=') || ft_strnstr(arg, "+=",
			ft_strlen(arg)));
}
