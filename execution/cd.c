/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amabbadi <amabbadi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 02:05:33 by amabbadi          #+#    #+#             */
/*   Updated: 2025/04/25 19:18:42 by amabbadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <string.h>

static int update_pwd_env(t_env *e, char *new_dir, int is_err, char *path)
{
    t_env *current = e;
    t_env *prev = NULL;
    char *temp;
    char *temp2;

    while (current)
    {
        if (current->key && ft_strcmp(current->key, "PWD") == 0)
        {
            if (is_err)
            {
                    if (current->value[strlen(current->value) - 1] != '/')
                    {
                        temp = ft_strjoin(current->value, "/");
                        temp2 = temp;
                        temp = ft_strjoin(temp2, path);
                    }
                    else
                        temp = ft_strjoin(current->value, path);
                free(current->value);
                current->value = temp;
                return (1);
            }
            free(current->value);
            current->value = ft_strdup(new_dir);
            if (!current->value)
            {
                perror("cd: error allocating memory for PWD");
                return (1);
            }
            return (0);
        }
        prev = current;
        current = current->next;
    }

    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
    {
        perror("cd: error allocating memory for new env node");
        return (1);
    }
    new_node->key = ft_strdup("PWD");
    new_node->value = ft_strdup(new_dir);
    new_node->next = NULL;
    new_node->exit_status = 0;

    if (!new_node->key || !new_node->value)
    {
        free(new_node->key);
        free(new_node->value);
        free(new_node);
        perror("cd: error allocating memory for PWD");
        return (1);
    }

    if (prev)
        prev->next = new_node;
    else
        e = new_node; 
    return (0);
}

static int cd_get_current_dir(char *new_dir, size_t size)
{
    if (getcwd(new_dir, size) == NULL)
    {
        perror("cd: error getting current directory");
        return (1);
    }
    return (0);
}

static int cd_change_to_home(t_env *e, char *prev_dir, char *new_dir, int is_err, char *path)
{
    (void)is_err;
    while (e)
    {
        if (e->key && (!ft_strcmp(e->key, "HOME") || !ft_strcmp(e->key, "~"))
            && e->value)
        {
            if (chdir(e->value) != 0)
                return (perror("cd"), 1);
            if (cd_get_current_dir(new_dir, PATH_MAX) != 0)
            {
                return (1);
            }
            if (update_pwd_env(e, new_dir, 0, path) != 0)
                return (1);
            if (prev_dir[0] != '\0')
                strncpy(prev_dir, new_dir, PATH_MAX - 1);
            return (0);
        }
        e = e->next;
    }
    fprintf(stderr, "cd: HOME not set\n");
    return (1);
}

static int cd_handle_special(char *path, t_env *e, char *prev_dir,
                            char *new_dir)
{
    if (!path || ft_strcmp(path, "~") == 0)
        return (cd_change_to_home(e, prev_dir, new_dir, 0 ,path));
    if (ft_strcmp(path, ".") == 0)
        return (0);
    if (ft_strcmp(path, "/") == 0)
    {
        if (chdir("/") != 0)
            return (perror("cd"), 1);
        if (cd_get_current_dir(new_dir, PATH_MAX) != 0)
        {
            new_dir = ft_strjoin(prev_dir, "/..");
            return (1);
        }
        if (update_pwd_env(e, new_dir, 0, path) != 0)
            return (1);
        if (prev_dir[0] != '\0')
            strncpy(prev_dir, new_dir, PATH_MAX - 1);
        return (0);
    }
    if (ft_strcmp(path, "-") == 0)
    {
        if (prev_dir[0] == '\0')
        {
            fprintf(stderr, "cd: no previous directory\n");
            return (1);
        }
        if (chdir(prev_dir) != 0)
            return (perror("cd"), 1);
        if (cd_get_current_dir(new_dir, PATH_MAX) != 0)
            return (1);
        if (update_pwd_env(e, new_dir, 0, path) != 0)
            return (1);
        printf("%s\n", prev_dir);
        strncpy(prev_dir, new_dir, PATH_MAX - 1);
        return (0);
    }
    return (-1);
}

static int cd_change_dir(char *path, char *prev_dir, char *new_dir, t_env *e)
{
    int is_err;
    is_err = 0;

    if (access(path, F_OK) != 0)
        return (perror("cd"), 1);
    if (chdir(path) != 0)
        return (perror("cd"), 1);
    if (cd_get_current_dir(new_dir, PATH_MAX) != 0)
        is_err = 1;
    if (update_pwd_env(e, new_dir, is_err, path) != 0)
        return (1);
    strncpy(prev_dir, new_dir, PATH_MAX - 1);
    return (0);
}

int cd(char *path, t_env *e)
{
    static char prev_dir[PATH_MAX];
    static char new_dir[PATH_MAX];
    int         ret;

    ret = cd_handle_special(path, e, prev_dir, new_dir);
    if (ret >= 0)
        return (ret);
    if (cd_change_dir(path, prev_dir, new_dir, e))
    {
        
        return (1);
    }
    else
        return (0);
}