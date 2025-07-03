#include "../minishell.h"

char	*get_prompt(void)
{
	char	cwd[PATH_MAX];
	char	*last;
	char	prompt[128];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		last = strrchr(cwd, '/');
		if (last && *(last + 1))
			last++;
		else
			last = cwd;
		snprintf(prompt, sizeof(prompt),
			"minishell > ~ %s : ", last);
	}
	else
		snprintf(prompt, sizeof(prompt), "minishell :: ");
	return (strdup(prompt));
}

t_env	*initialize_shell(char *envp[])
{
	t_env	*env;

	env = env_list_from_array(envp);
	increment_shlvl(env);
	setup_signals();
	read_history("history.txt");
	return (env);
}

char	*read_shell_input(char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (!input)
		return (NULL);
	if (*input)
		add_history(input);
	return (input);
}

void	process_shell_input(char *input, char *envp[], t_env *env)
{
	if (*input)
		parserlexer(input, envp, env);
}

void	cleanup_shell(t_env *env)
{
	free_env(env);
	write(1, "exit\n", 5);
	rl_clear_history();
	// Close any open file descriptors if needed
}
