/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:25:13 by gabastos          #+#    #+#             */
/*   Updated: 2025/03/11 10:46:58 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void set_dir(t_data *ms, char *dir, char ***envp)
{
    char *temp;
    char **var;
    
    // Early return if dir is NULL
    if (!dir) {
        ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
        ms->error = 1;
        return;
    }
    
    // Allocate buffer for getcwd
    temp = malloc(BUFFER_SIZE);
    if (!temp) {
        ms->error = 1;
        return;
    }
    
    var = ft_calloc(3, sizeof(char *));
    if (!var) {
        free(temp);
        ms->error = 1;
        return;
    }
    
    if (getcwd(temp, BUFFER_SIZE) == NULL) {
        free(temp);
        free(var);
        ms->error = 1;
        return;
    }
    
    var[0] = ft_strdup("export");
    if (!var[0]) {
        free(temp);
        free(var);
        ms->error = 1;
        return;
    }
    
    if (chdir(dir)) {
        perror("minishell: cd");
        ms->error = 1;
    } else {
        var[1] = ft_strjoin("OLDPWD=", temp);
        ft_export(ms, var, envp);
        free(var[1]);
        
        if (getcwd(temp, BUFFER_SIZE) == NULL) {
            ms->error = 1;
        } else {
            var[1] = ft_strjoin("PWD=", temp);
            ft_export(ms, var, envp);
            free(var[1]);
            ms->error = 0;
        }
    }
    
    free(temp);
    free(var[0]);
    free(var);
}

static char *get_home(char **envp)
{
    int i;
    char *home = NULL;
    
    i = 0;
    if (!envp)
        return NULL;
        
    while (envp[i]) {
        if (!ft_strncmp(envp[i], "HOME=", 5)) {
            home = ft_strdup(envp[i] + 5);
            break;
        }
        i++;
    }
    
    return home;
}

void ft_cd(t_data *ms, char **cmd, char ***envp)
{
    char *home_dir = NULL;
    
    if (!cmd) {
        ms->error = 1;
        return;
    }
    
    if (cmd[1] && !cmd[2])
        set_dir(ms, cmd[1], envp);
    else if (!cmd[1]) {
        home_dir = get_home(*envp);
        if (!home_dir) {
            ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
            ms->error = 1;
            return;
        }
        set_dir(ms, home_dir, envp);
        free(home_dir);
    }
    else {
        ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
        ms->error = 1;
    }
}
