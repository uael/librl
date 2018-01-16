/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 09:52:30 by alucas-           #+#    #+#             */
/*   Updated: 2018/01/06 11:13:28 by cmalfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visual.h"
#include "read.h"

static t_sds	g_clipboard_stack = { NULL, 0, 0 };
static t_sds	*g_clipboard = &g_clipboard_stack;

inline void		rl_visualdtor(void)
{
	ft_sdsdtor(g_clipboard);
}

inline int		rl_visualtoggle(char const *prompt)
{
	if (g_mode == RL_VISUAL)
	{
		g_mode = RL_INSERT;
		rl_editprint(prompt);
	}
	else
	{
		g_mode = RL_VISUAL;
		g_eln->vidx = g_eln->idx;
	}
	return (YEP);
}

inline int		rl_visualyank(char const *prompt)
{
	size_t	i;

	if (g_mode == RL_INSERT && g_eln->idx < g_eln->str.len)
		ft_sdscpush(g_clipboard, *ft_sdsat(&g_eln->str, g_eln->idx));
	else if (g_mode == RL_VISUAL && g_eln->vidx > g_eln->idx)
	{
		i = g_eln->vidx == g_eln->str.len ? g_eln->vidx - 1 : g_eln->vidx;
		g_clipboard->len = 0;
		ft_sdsmpush(g_clipboard, ft_sdsat(&g_eln->str, g_eln->idx),
			i - g_eln->idx + 1);
		g_eln->idx = g_eln->vidx;
		rl_visualtoggle(prompt);
	}
	else if (g_mode == RL_VISUAL && g_eln->vidx < g_eln->idx)
	{
		i = g_eln->idx == g_eln->str.len ? g_eln->idx - 1 : g_eln->idx;
		g_clipboard->len = 0;
		ft_sdsmpush(g_clipboard, ft_sdsat(&g_eln->str, g_eln->vidx),
			i - g_eln->vidx + 1);
		g_eln->vidx = g_eln->idx;
		rl_visualtoggle(prompt);
	}
	return (YEP);
}

inline int		rl_visualdelete(char const *prompt)
{
	size_t	i;

	if (g_eln->vidx > g_eln->idx)
	{
		i = g_eln->vidx == g_eln->str.len ? g_eln->vidx - 1 : g_eln->vidx;
		g_clipboard->len = 0;
		ft_sdsmpush(g_clipboard, ft_sdsat(&g_eln->str, g_eln->idx),
			i - g_eln->idx + 1);
		ft_sdsnrem(&g_eln->str, g_eln->idx, i - g_eln->idx + 1, NULL);
		g_eln->vidx = g_eln->idx;
		rl_visualtoggle(prompt);
	}
	else if (g_eln->vidx < g_eln->idx)
	{
		i = g_eln->idx == g_eln->str.len ? g_eln->idx - 1 : g_eln->idx;
		g_clipboard->len = 0;
		ft_sdsmpush(g_clipboard, ft_sdsat(&g_eln->str, g_eln->vidx),
			i - g_eln->vidx + 1);
		ft_sdsnrem(&g_eln->str, g_eln->vidx, i - g_eln->vidx + 1, NULL);
		g_eln->idx = g_eln->vidx;
		rl_visualtoggle(prompt);
	}
	return (YEP);
}

inline int		rl_visualpaste(char const *prompt)
{
	if (g_clipboard->len)
	{
		ft_sdsmput(&g_eln->str, g_eln->idx, g_clipboard->buf,
			g_clipboard->len);
		g_eln->idx += g_clipboard->len;
		rl_editprint(prompt);
	}
	return (YEP);
}
