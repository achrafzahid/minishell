/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azahid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:18:15 by azahid            #+#    #+#             */
/*   Updated: 2025/04/20 17:07:07 by azahid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/stat.h>

void exiter(char **exec){
  struct stat sf;
  if (!exec && !exec[0])
    perror("");
  stat(exec[0],&sf);
  char *res = ft_strjoin("minishell : ",exec[0]);
  char *str;
  if(S_ISDIR(sf.st_mode) && ft_strchr(exec[0], '/')){
    str = ft_strjoin(res, " : Is a directory\n");
    write(2,str,ft_strlen(str));
    exit(126);
  }
  else if (S_ISDIR(sf.st_mode))
  {
    str = ft_strjoin(res, " : command not found\n");
    write(2,str,ft_strlen(str));
    exit(127);
  }
/*   else if(S_ISREG(sf.st_mode) &&  !(sf.st_mode & S_IXUSR)){
    str = ft_strjoin(res, " : Permission denied\n");
    write(2,str,ft_strlen(str));
    exit(126);
  } */
  else{
    str = ft_strjoin(res, " : Permission denied\n");
    write(2,str,ft_strlen(str));
    exit(126);
  }
} 
