#include "../minishell.h"

static int is_valid_unset_identifier(const char *key)
{
	int i;

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

int	unset(t_env **env, char *var)
{
	t_env	*tmp;
	t_env	*prev;

	if (!env || !*env || !var)
		return (0);
		
	if (!is_valid_unset_identifier(var))
	{
		fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n", var);
		return (1);
	}

	tmp = *env;
	prev = NULL;
	
	while (tmp)
	{
		if (tmp->key && ft_strcmp(tmp->key, var) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (0);
}