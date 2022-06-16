/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wding-ha <wding-ha@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 21:39:02 by wding-ha          #+#    #+#             */
/*   Updated: 2022/06/16 17:29:29 by wding-ha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_tolower(t_cmdlist *lst)
{
	char	*s;
	int		i;
	int		cap;

	i = 0;
	s = ft_strdup(*lst->cmd.cmd);
	cap = 0;
	while (s[i])
	{
		if (s[i] >= 65 && s[i] <= 90)
			cap = 1;
		s[i] = ft_tolower(s[i]);
		i++;
	}
	if (cap)
	{
		if (!ft_strcmp(s, "unset") || !ft_strcmp(s, "export") \
		|| !(!ft_strcmp(s, "exit")))
		{
			free(s);
			return (0);
		}
	}
	free(*lst->cmd.cmd);
	*lst->cmd.cmd = s;
	return (0);
}

void	cmdlist_expansion(t_cmdlist *list, int ex)
{
	t_cmdlist	*temp;

	temp = list;
	while (temp)
	{
		env_treatment(temp->cmd.token, ex);
		quote_treatment(temp->cmd.token, ex);
		temp = temp->next;
	}
}

t_cmdlist	*lexer_init(char *s, int *ex)
{
	int			i;
	char		**token;
	t_cmdlist	*lst;

	i = token_length(s);
	if (i <= 0)
	{
		*ex = i * i;
		return (NULL);
	}
	token = split_token(s, i);
	lst = cmdlist_init(token);
	if (!lst)
		return (NULL);
	set_cmd(lst);
	cmdlist_expansion(lst, *ex);
	*ex = cmd_tolower(lst);
	return (lst);
}
