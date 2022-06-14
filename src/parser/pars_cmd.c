/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding-ha <wding-ha@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 14:27:02 by wding-ha          #+#    #+#             */
/*   Updated: 2022/06/14 19:05:52 by wding-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_cmd(t_command *cmd)
{
	int		i;

	i = 0;
	while (cmd->token[i])
	{
		if (parse_symbol(cmd->token[i]) > 1)
			i = i + 2;
		else
		{
			if (!cmd->cmd && cmd->token[i])
				cmd->cmd = &(cmd->token[i]);
			else
				ft_lstadd_back(&(cmd->args), ft_lstnew(&(cmd->token[i])));
			i++;
		}
	}
}

void	set_cmd(t_cmdlist *cmd)
{
	t_cmdlist	*lst;

	lst = cmd;
	while (lst)
	{
		check_cmd(&(lst->cmd));
		lst = lst->next;
	}
}

int	check_builtin(t_command cmd, int ex)
{
	char	*s;

	s = *cmd.cmd;
	if (!ft_strcmp(s, "echo"))
		return (bin_echo(cmd));
	if (!ft_strcmp(s, "cd"))
		return (bin_cd(cmd));
	if (!ft_strcmp(s, "pwd"))
		return (bin_pwd(cmd));
	if (!ft_strcmp(s, "export"))
		return (bin_export(cmd));
	if (!ft_strcmp(s, "unset"))
		return (bin_unset(cmd));
	if (!ft_strcmp(s, "env"))
		return (bin_env(cmd));
	if (!ft_strcmp(s, "exit"))
		return (bin_exit(cmd, ex));
	return (-1);
}

int	parse_cmd(t_cmdlist *lst, int ex)
{
	int		ret;

	ret = check_builtin(lst->cmd, ex);
	return (ret);
}
