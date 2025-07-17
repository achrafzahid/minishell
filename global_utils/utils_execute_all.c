#include "../minishell.h"

int	is_useless_variable(char *raw_input, t_env *env)
{
	char	*var_name;
	t_env	*tmp;

	if (!raw_input || raw_input[0] != '$')
		return (0);
	var_name = raw_input + 1;
	if (var_name[0] == '?' || var_name[0] == '\0')
		return (0);
	tmp = env;
	while (tmp)
	{
		if (tmp->key && ft_strcmp(tmp->key, var_name) == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void validate_exit_args(t_comm *com, t_chars *p, int *status)
{
	char	*arg;
	int		error;
	int		val;

	arg = p->str;
	if (p->next)
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		if (com && com->env)
			com->env->exit_status = 1;
		exit(com && com->env ? com->env->exit_status : 1);
	}
	error = 0;
	val = ft_atoi_safe(arg, &error);
	if (error)
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", arg);
		if (com && com->env)
			com->env->exit_status = 255;
		exit(com && com->env ? com->env->exit_status : 2);
	}
	if (status)
		*status = val;
}


char	*get_next_word(const char *str)
{
	int		i;
	int		start;
	char	*word;
	t_env	*env;

	i = 0;
	env = NULL;
	if (!str)
		return (NULL);
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (!str[i])
		return (NULL);
	while (str[i] && !ft_isspace(str[i]))
		i++;
	while (str[i] && ft_isspace(str[i]))
		i++;
	start = i;
	while (str[i] && !ft_isspace(str[i]))
		i++;
	if (start == i)
		return (NULL);
	word = ft_strdup(str + start);
	if (!word)
	{
		printf("minishell: cannot allocate memory\n");
		if (env)
			env->exit_status = 1;
		return (NULL);
	}
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] && !ft_isspace(str[i]))
	{
		free(word);
		printf("minishell: syntax error near unexpected token `%c'\n", str[i]);
		if (env)
			env->exit_status = 2;
		return (NULL);
	}
	return (word);
}

char	**envtodoublearr(t_env *e)
{
	int		count;
	t_env	*tmp;
	char	**envp;
	int		i;
	char	*first_part;

	count = 0;
	tmp = e;
	while (tmp)
	{
		if (tmp->key && tmp->value)
			count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
	{
		printf("minishell: cannot allocate memory\n");
		if (e)
			e->exit_status = 1;
		return (NULL);
	}
	tmp = e;
	i = 0;
	while (tmp)
	{
		if (tmp->key && tmp->value)
		{
			first_part = ft_strjoin(tmp->key, "=");
			if (!first_part)
			{
				free2d(envp);
				printf("minishell: cannot allocate memory\n");
				tmp->exit_status = 1;
				return (NULL);
			}
			envp[i] = ft_strjoin(first_part, tmp->value);
			free(first_part);
			if (!envp[i])
			{
				free2d(envp);
				printf("minishell: cannot allocate memory\n");
				tmp->exit_status = 1;
				return (NULL);
			}
			i++;
		}
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	**list_to_array(t_chars *list)
{
	int		size;
	t_chars	*tmp;
	char	**array;
	int		i;
	t_chars	*tt;

	size = 0;
	tmp = list;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	i = 0;
	tt = list;
	while (tt)
	{
		array[i] = ft_strdup(tt->str);
		i++;
		tt = tt->next;
	}
	array[i] = NULL;
	return (array);
}
