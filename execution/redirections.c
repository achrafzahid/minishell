/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 05:52:06 by azahid            #+#    #+#             */
/*   Updated: 2025/04/17 10:17:45 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_quoted(const char *str) 
{
    int len = strlen(str);
    if (len < 2) return 0;
    if ((str[0] == '\'' && str[len-1] == '\'') || (str[0] == '"' && str[len-1] == '"'))
        return 1;
    return 0;
}

// Function to process heredoc
int process_heredoc(t_comm *com, const char *delimiter)
{
    int pipefd[2];
    char *line;
    if (pipe(pipefd) == -1)
        return (perror("pipe"), 1);
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (strcmp(line, delimiter) == 0)
        {
            free(line);break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]); // Close write-end
    com->infile = pipefd[0]; // Use read-end in dup2(stdin)
    return 0;
}

int handle_redirections(t_comm *com, int i, int *redir_in, int *redir_out)
{
    t_chars *redir;

    *redir_in = 0;
    *redir_out = 0;

    // FIRST PASS: handle heredocs only
    redir = com[i].redirections;
    while (redir)
    {
        if (redir->type == 2) // heredoc '<<'
        {
            if (process_heredoc(&com[i], redir->str) != 0)
                return (perror("minishell"), 1);
        }
        redir = redir->next;
    }

    // SECOND PASS: handle input/output redirections
    redir = com[i].redirections;
    while (redir)
    {
        if (redir->type == 0) // input '<'
        {
            int fd = open(redir->str, O_RDONLY);
            if (fd == -1)
                return (perror("minishell"), 1);
            dup2(fd, 0);
            close(fd);
            *redir_in = 1;
        }
        else if (redir->type == 1) // output '>'
        {
            int fd = open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
                return (perror("minishell"), 1);
            dup2(fd, 1);
            close(fd);
            *redir_out = 1;
        }
        else if (redir->type == 3) // append '>>'
        {
            int fd = open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
                return (perror("minishell"), 1);
            dup2(fd, 1);
            close(fd);
            *redir_out = 1;
        }
        else if (redir->type == 2) // heredoc again
        {
            // Use the read-end from heredoc pipe
            dup2(com[i].infile, 0);
            *redir_in = 1;
        }
        redir = redir->next;
    }

    return 0;
}

