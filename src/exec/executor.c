/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding-ha <wding-ha@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 20:46:04 by wding-ha          #+#    #+#             */
/*   Updated: 2022/07/06 14:10:14 by wding-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**listto2d(void)
{
	t_list	*lst;
	char	**ret;
	int		i;

	lst = g_env;
	ret = ft_calloc(sizeof(char *), ft_lstsize(lst) + 1);
	i = 0;
	while (lst)
	{
		if (ft_envcmp(lst->content, "?"))
			ret[i] = ft_strdup(lst->content);
		lst = lst->next;
	}
	return (ret);
}

char	*pathextract(char *s, int *i)
{
	char	*ev;
	char	*ret;
	int		start;
	int		len;

	start = *i;
	ev = ft_getenv("PATH");
	if (!ev)
	{
		error_msg(127, 2, s, ": No such file or directory");
		return (NULL);
	}
	while (ev[*i] && ev[*i] != ':')
		(*i)++;
	len = *i - start;
	ret = ft_combine_key(ft_substr(ev, start, len), ft_strdup(s), '/');
	if (ev[*i] == ':')
		(*i)++;
	if (!ev[*i])
	{
		error_msg(127, 2, s, ": command not found");
		return (NULL);
	}
	return (ret);
}

char	**argumentextract(char *s, t_command cmd)
{
	char	**ret;
	int		i;

	i = 1;
	ret = ft_calloc(sizeof(char *), ft_lstsize(cmd.args) + 2);
	ret[0] = s;
	while (cmd.args)
	{
		ret[i] = *(char **)cmd.args->content;
		cmd.args = cmd.args->next;
		i++;
	}
	return (ret);
}

int	executor(t_command cmd, t_shell *sh)
{
	char	**env;
	char	*exe;
	char	**arg;
	int		i;

	i = 0;
	(void)sh;
	env = listto2d();
	exe = *cmd.cmd;
	arg = argumentextract(*cmd.cmd, cmd);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	while (execve(exe, arg, env) < 0)
	{
		exe = pathextract(*cmd.cmd, &i);
		if (!exe)
			exit(127);
	}
	exit(0);
}

int	execute(t_command cmd, t_shell *sh)
{
	int	pid;

	pid = fork();
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (pid == 0)
		executor(cmd, sh);
	return (waitforchild(0));
}
