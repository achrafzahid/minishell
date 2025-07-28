#include "../minishell.h"

int	process_heredoc(t_comm *com, const char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (!com || !delimiter)
		return (1);
	if (pipe(pipefd) == -1)
		return (1);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, (char *)delimiter))
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	com->infile = pipefd[0];
	return (0);
}
int handle_redirections(t_comm *com, int i, int *redir_in, int *redir_out,
                        char **failed_file, int *printed_error)
{
    t_chars *redir = com[i].redirections;
    int fd;

    *redir_in = 0;
    *redir_out = 0;
    *failed_file = NULL;
    *printed_error = 0;

    while (redir)
    {
        if (redir->type == 2) // Heredoc
        {
            if (process_heredoc(&com[i], redir->str) != 0)
            {
                *failed_file = redir->str;
                *printed_error = 1;
                return 1;
            }
            dup2(com[i].infile, STDIN_FILENO);
            *redir_in = 1;
        }
        else if (redir->type == 0) // Input redirection
        {
            if (ft_strchr(redir->str, '*') || ft_strchr(redir->str, '?'))
            {
                t_chars *matches = parse_wildcards(redir->str, &com[i]);
                if (matches && matches->str)
                {
                    fd = open(matches->str, O_RDONLY);
                    if (fd == -1)
                    {
                        *failed_file = matches->str;
                        *printed_error = 0;
                        return -1;
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                    *redir_in = 1;
                }
                free_chars(matches);
            }
            else
            {
                fd = open(redir->str, O_RDONLY);
                if (fd == -1)
                {
                    *failed_file = redir->str;
                    *printed_error = 0;
                    return -1;
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                *redir_in = 1;
            }
        }
        else if (redir->type == 1 || redir->type == 3) // Output redirection
        {
            int flags = (redir->type == 1) ? O_WRONLY | O_CREAT | O_TRUNC : O_WRONLY | O_CREAT | O_APPEND;
            fd = open(redir->str, flags, 0644);
            if (fd == -1)
            {
                *failed_file = redir->str;
                *printed_error = 1;
                fprintf(stderr, "minishell: %s: Permission denied\n", redir->str);
                return 1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            *redir_out = 1;
        }
        redir = redir->next;
    }
    return 0;
}

int	setup_redirections(t_comm *com)
{
	t_chars	*redir;
	int		fd;

	if (!com)
		return (1);
	redir = com->redirections;
	while (redir)
	{
		if (redir->type == 2)
		{
			if (process_heredoc(com, redir->str) != 0)
				return (1);
		}
		else if (redir->type == 0)
		{
			fd = open(redir->str, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->str);
				return (1);
			}
			if (com->infile != -1)
				close(com->infile);
			com->infile = fd;
		}
		else if (redir->type == 1 || redir->type == 3)
		{
			int	flags = O_WRONLY | O_CREAT;
			flags |= (redir->type == 1) ? O_TRUNC : O_APPEND;
			fd = open(redir->str, flags, 0644);
			if (fd == -1)
			{
				perror(redir->str);
				return (1);
			}
			if (com->outfile != -1)
				close(com->outfile);
			com->outfile = fd;
		}
		redir = redir->next;
	}
	return (0);
}