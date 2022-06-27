/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding-ha <wding-ha@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 14:27:02 by wding-ha          #+#    #+#             */
/*   Updated: 2022/06/27 16:20:41 by wding-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_cmdchild(t_cmdlist *lst, t_shell *sh)
{
	int		ret;

	ret = 0;
	if (redir_dup(lst->cmd))
		return (1);
	if (lst->cmd.cmd)
	{
		ret = check_builtin(lst->cmd, sh->ex);
		if (ret < 0)
			ret = executor(lst->cmd, sh);
	}
	return (ret);
}

void	killchild(int signo)
{
	(void)signo;
	kill(0, SIGINT);
}

int	child_create(t_cmdlist *lst, t_shell *sh)
{
	signal(SIGINT, killchild);
	signal(SIGQUIT, SIG_DFL);
	if (sh->i != -1)
		dup2(sh->fd[1], 1);
	if (sh->i != 0)
	{
		dup2(sh->pipe, 0);
		close(sh->pipe);
	}
	close(sh->fd[0]);
	close(sh->fd[1]);
	exit (parse_cmdchild(lst, sh));
}

// void	fork_it(t_shell *sh)
// {
// 	sh->i++;
// 	if (sh->i > 0)
// 	{
// 		close(sh->pipe);
// 		sh->pipe = dup(sh->fd[0]);
// 	}
// 	close(sh->fd[0]);
// 	close(sh->fd[1]);
// 	exit (parse_cmdchild(lst, sh));
// }

void	fork_it(t_shell *sh)
{
	sh->i++;
	if (sh->i > 0)
	{
		close(sh->pipe);
		sh->pipe = dup(sh->fd[0]);
	}
	close(sh->fd[0]);
	close(sh->fd[1]);
}

int	parse_cmdline(t_cmdlist *lst, t_shell *sh)
{
	int	pid;
	int	status;

	parse_heredoc(lst);
	if (lst && !lst->next)
		return (parse_cmd(lst, sh));
	else
	{
		sh->i = 0;
		while (lst)
		{
			pipe(sh->fd);
			if (!lst->next)
				sh->i = -1;
			pid = fork();
			if (pid == 0)
				exit(child_create(lst, sh));
			fork_it(sh);
			free_cmdlist(&lst);
		}
		close(sh->pipe);
		while (waitpid(-1, &status, 0) > 0)
			;
	}
	return (0);
}
