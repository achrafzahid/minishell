/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 05:52:06 by azahid            #+#    #+#             */
/*   Updated: 2025/04/25 14:30:26 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quoted(const char *str)
{
	int	len;

	len = strlen(str);
	if (len < 2)
		return (0);
	if ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"' && str[len \
				- 1] == '"'))
		return (1);
	return (0);
}

// Function to process heredoc
int	process_heredoc(t_comm *com, const char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	com->infile = pipefd[0];
	return (0);
}

int	handle_redirections(t_comm *com, int i, int *redir_in, int *redir_out,
		char **failed_file, int *printed_error)
{
	t_chars	*redir;
	int		fd;

	*redir_in = 0;
	*redir_out = 0;
	*failed_file = NULL;
	*printed_error = 0;
	redir = com[i].redirections;
	while (redir)
	{
		if (redir->type == 2)
		{
			if (process_heredoc(&com[i], redir->str) != 0)
			{
				*failed_file = redir->str;
				*printed_error = 1;
				return (perror("minishell"), 1);
			}
		}
		redir = redir->next;
	}
	redir = com[i].redirections;
	while (redir)
	{
		if (redir->type == 0)
		{
			fd = open(redir->str, O_RDONLY);
			if (fd == -1)
			{
				*failed_file = redir->str;
				*printed_error = 0;
				return (-1);
			}
			dup2(fd, 0);
			close(fd);
			*redir_in = 1;
		}
		else if (redir->type == 1)
		{
			fd = open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				*failed_file = redir->str;
				*printed_error = 1;
				return (perror("minishell"), 1);
			}
			dup2(fd, 1);
			close(fd);
			*redir_out = 1;
		}
		else if (redir->type == 3)
		{
			fd = open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				*failed_file = redir->str;
				*printed_error = 1;
				fprintf(stderr, "minishell: %s: Permission denied\n",
					redir->str);
				return (1);
			}
			dup2(fd, 1);
			close(fd);
			*redir_out = 1;
		}
		else if (redir->type == 2)
		{
			dup2(com[i].infile, 0);
			*redir_in = 1;
		}
		redir = redir->next;
	}
	return (0);
}
