/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding-ha <wding-ha@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 15:32:09 by wding-ha          #+#    #+#             */
/*   Updated: 2022/06/22 16:04:28 by wding-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*tempfile_gen(int i)
{
	char	*s;
	char	*ret;

	s = ft_itoa(i);
	ret = ft_strjoin("temp", s);
	free(s);
	return (ret);
}

int	heredoc_routine(char *s, char *file)
{
	char	*str;
	int		fd;

	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0700);
	while (1)
	{
		str = readline("> ");
		if (!ft_strcmp(str, s))
			break ;
		ft_putstr_fd((ft_strjoinfree(str, ft_strdup("\n"))), fd);
	}
	close(fd);
	exit(0);
}

int	ft_heredoc(char *s, int i)
{
	int		fd;
	int		pid;
	int		status;
	char	*file;

	file = tempfile_gen(i);
	pid = fork();
	if (pid == 0)
		heredoc_routine(s, file);
	waitpid(pid, &status, 0);
	fd = open(file, O_RDONLY, 0700);
	unlink(file);
	return (fd);
}

void	parse_heredoc(t_cmdlist *cmd)
{
	t_cmdlist	*lst;
	t_list		*red;
	t_redir		*arr;
	int			i;

	i = 0;
	lst = cmd;
	while (lst)
	{
		red = lst->cmd.redir;
		while (red)
		{
			arr = red->content;
			if (arr->red == HEREDOC)
				lst->cmd.fd = ft_heredoc(*arr->file, i);
			red = red->next;
		}
		i++;
		lst = lst->next;
	}
}
