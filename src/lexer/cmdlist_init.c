/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdlist_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding-ha <wding-ha@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:32:31 by wding-ha          #+#    #+#             */
/*   Updated: 2022/07/04 14:59:44 by wding-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_checking(char **s)
{
	int	i;

	i = 0;
	if (parse_symbol(s[i]) == PIPE)
		return (error_msg(-1, 2, "syntax error near unexpected token", "`|'"));
	while (s[i])
	{
		if (parse_symbol(s[i]) > 1)
		{
			if (!s[i + 1])
				return (error_msg(-1, 2, "syntax error near unexpected token", \
				"newline"));
			if (parse_symbol(s[i + 1]))
				return (error_msg(-1, 2, "syntax error near unexpected token", \
				s[i + 1]));
		}
		if (parse_symbol(s[i]) == PIPE)
		{
			if (parse_symbol(s[i + 1]) == PIPE || !s[i + 1])
				return (error_msg(-1, 2, "syntax error near unexpected token", \
				"`|'"));
		}
		i++;
	}
	return (1);
}

char	**ft_divide(char **s, int len, int i)
{
	char	**ret;
	int		j;

	j = 0;
	ret = ft_calloc(sizeof(char *), len + 1);
	while (j < len)
	{
		ret[j] = ft_strdup(s[i]);
		i++;
		j++;
	}
	return (ret);
}

t_command	*struct_init(t_command	*cmd)
{
	cmd->token = NULL;
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->redir = NULL;
	return (cmd);
}

void	command_table_init(char **s, int *i, t_command *cmd)
{
	int			start;
	int			len;
	char		*temp;

	start = *i;
	while (parse_symbol(s[*i]) != PIPE && s[*i])
	{
		if (parse_symbol(s[*i]) == HEREDOC)
		{
			temp = add_quote_heredoc(s[*i + 1]);
			free(s[*i + 1]);
			s[*i + 1] = temp;
		}
		(*i)++;
	}
	len = *i - start;
	cmd->token = ft_divide(s, len, start);
}

t_cmdlist	*cmdlist_init(char **s)
{
	int			i;
	t_command	cmd;
	t_cmdlist	*list;

	i = 0;
	list = NULL;
	if (syntax_checking(s) < 0)
		return (token_error(s));
	while (s[i])
	{
		command_table_init(s, &i, struct_init(&cmd));
		ft_cmdadd_back(&list, ft_newcmd(cmd));
		if (parse_symbol(s[i]) == PIPE)
			i++;
	}
	free2d(s);
	return (list);
}
