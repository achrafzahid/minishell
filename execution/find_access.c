#include "../minishell.h"

static char **parse_env_paths(t_env *env)
{
    while (env)
    {
        if (env->key && !ft_strcmp(env->key, "PATH") && env->value)
            return ft_split(env->value, ':');
        env = env->next;
    }
    return NULL;
}

static char *join_path(const char *dir, const char *cmd)
{
    char *slash_cmd = ft_strjoin("/", cmd);
    if (!slash_cmd)
        return NULL;
    char *full_path = ft_strjoin(dir, slash_cmd);
    free(slash_cmd);
    return full_path;
}

char *find_access(t_comm *com)
{
    if (!com || !com->p_com || !com->p_com->str)
        return NULL;

    char *cmd = com->p_com->str;
    if (ft_strchr(cmd, '/'))
        return access(cmd, F_OK) == 0 ? ft_strdup(cmd) : NULL;

    char **paths = parse_env_paths(com->env);
    if (!paths && access(cmd, F_OK | X_OK) == 0)
        return ft_strdup(cmd);

    for (int i = 0; paths && paths[i]; i++)
    {
        char *full_path = join_path(paths[i], cmd);
        if (!full_path)
        {
            free2d(paths);
            return NULL;
        }
        if (access(full_path, F_OK | X_OK) == 0)
        {
            free2d(paths);
            return full_path;
        }
        free(full_path);
    }

    free2d(paths);
    return NULL;
}

char *createargs(t_comm *com)
{
    char *path = find_access(com);
    if (!path)
        return NULL;
    free(com->p_com->str);
    com->p_com->str = path;
    return path;
}