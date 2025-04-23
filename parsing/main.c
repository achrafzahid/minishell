/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvrk <dvrk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:43:43 by azahid            #+#    #+#             */
/*   Updated: 2025/04/19 08:40:25 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

int		g_signal;

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

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
			"\001\x1b[1;36m\002[minishin]: \001\x1b[35m~%s\x1b[0m\002 ", last);
	}
	else
		snprintf(prompt, sizeof(prompt), "\x1b[1;32m[minihell]:~ \x1b[0m");
	return (strdup(prompt));
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGTERM, SIG_IGN);
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
}

int	main(int ac, char **av, char *envp[])
{
	char	*input;
	t_env	*env;
	char	*prompt;
	int		status;

	(void)ac;
	(void)av;
	env = initialize_shell(envp);
	while (1337)
	{
		prompt = get_prompt();
		input = read_shell_input(prompt);
		free(prompt);
		if (!input)
			break ;
		process_shell_input(input, envp, env);
		free(input);
	}
	if (env)
		status = env->exit_status;
	else
		status = 0;
	cleanup_shell(env);
	return (status);
}
