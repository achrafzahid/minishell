#include "../minishell.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void exit_prog(t_comm *com)
{
    int status = com && com->env ? com->env->exit_status : 0;
    int exit_code;

    if (com && com->p_com && com->p_com->next)
        validate_exit_args(com, com->p_com->next, &status);

    exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : status;
    if (com && com->env)
        com->env->exit_status = exit_code;
    exit(exit_code);
}

int exec_builtin(t_comm *com)
{
    if (!com || !com->p_com || !com->p_com->str || !com->env)
    {
        if (com && com->env)
            com->env->exit_status = 1;
        return 1;
    }

    char *cmd = com->p_com->str;
    if (!ft_strcmp(cmd, "pwd"))
        return handle_pwd(com);
    if (!ft_strcmp(cmd, "exit"))
    {
        exit_prog(com);
        return 0;
    }
    if (!ft_strcmp(cmd, "unset"))
        return handle_unset(com);
    if (!ft_strcmp(cmd, "cd"))
        return handle_cd(com);
    if (!ft_strcmp(cmd, "echo"))
    {
        int ret = echo(com);
        com->env->exit_status = ret;
        return ret;
    }
    if (!ft_strcmp(cmd, "env"))
    {
        env(com);
        com->env->exit_status = 0;
        return 0;
    }
    if (!ft_strcmp(cmd, "export"))
        return handle_export(com);

    com->env->exit_status = 1;
    return 1;
}

int check_builtin(t_comm *com)
{
    if (!com || !com->p_com || !com->p_com->str)
        return 1;

    char *cmd = com->p_com->str;
    return !(!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "exit") ||
             !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "cd") ||
             !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "env") ||
             !ft_strcmp(cmd, "export"));
}

static int execute_single_command(t_comm *com, char **envp)
{
    int sin = dup(STDIN_FILENO);
    int sout = dup(STDOUT_FILENO);
    int in = 0, out = 0;
    char *failed_file = NULL;
    int printed_error = 0;
    int redir_status = handle_redirections(com, 0, &in, &out, &failed_file, &printed_error);

    if (redir_status == -1)
    {
        if (!printed_error)
            fprintf(stderr, "minishell: %s: No such file or directory\n", failed_file ? failed_file : "unknown");
        if (com->env)
            com->env->exit_status = 1;
        free2d(envp);
        exit(1);
    }
    if (redir_status != 0)
    {
        if (com->env)
            com->env->exit_status = 1;
        free2d(envp);
        return 1;
    }

    int ret = exec_builtin(com);
    close(STDIN_FILENO);
    dup2(sin, STDIN_FILENO);
    dup2(sout, STDOUT_FILENO);
    close(sin);
    close(sout);
    free2d(envp);
    return com->env ? com->env->exit_status : ret;
}

static int execute_piped_commands(t_comm *coms, int size, char **envp)
{
    int pipes[2 * (size - 1)];
    int pids[size];

    if (setup_pipes(pipes, size, coms, envp))
        return 1;

    for (int i = 0; i < size; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("minishell: fork");
            for (int j = 0; j < 2 * (size - 1); j++)
                close(pipes[j]);
            if (coms && coms[0].env)
                coms[0].env->exit_status = 1;
            free2d(envp);
            return 1;
        }
        if (pids[i] == 0)
            handle_child_process(coms, i, size, pipes, envp);
    }

    for (int i = 0; i < 2 * (size - 1); i++)
        close(pipes[i]);
    int status = wait_for_children(pids, size, coms);
    free2d(envp);
    return coms && coms[0].env ? coms[0].env->exit_status : status;
}

int execute_all(t_comm *coms, char **envp, int size)
{
    if (!coms || size <= 0)
    {
        if (coms && coms[0].env)
            coms[0].env->exit_status = 1;
        free2d(envp);
        return 1;
    }

    if (size == 1 && coms[0].p_com && coms[0].p_com->str && !check_builtin(coms) && !coms[0].flag)
        return execute_single_command(coms, envp);

    return execute_piped_commands(coms, size, envp);
}