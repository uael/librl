/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read/notty.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 09:52:30 by alucas-           #+#    #+#             */
/*   Updated: 2018/01/06 11:11:59 by cmalfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/resource.h>

#include "../read.h"

#ifndef OPEN_MAX
# define OPEN_MAX RLIMIT_NOFILE
#endif

static t_ifs	g_in[OPEN_MAX + 1] = { { 0, 0, 0, 0, { 0 } } };
static t_sds	g_ln = { NULL, 0, 0 };
static ssize_t	g_rd[OPEN_MAX + 1] = { 0 };

inline void		rl_nottydtor(void)
{
	ft_sdsdtor(&g_ln);
}

inline void		rl_nottyfinalize(int fd)
{
	if (g_in[fd].ifd > 0)
		ft_ifsclose(g_in + fd);
}

inline char		*rl_readnotty(int fd)
{
	char		*ln;

	if (fd < 0 || fd > OPEN_MAX)
	{
		ENO_THROW(WUT, EINVAL);
		return (NULL);
	}
	g_in[fd].ifd = fd;
	if (g_rd[fd] > 0 && ft_ifsrd(g_in + fd, NULL, (size_t)g_rd[fd]) < 0)
		return (NULL);
	if ((g_rd[fd] = ft_ifschr(g_in + fd, 0, '\n', &ln)) > 0)
	{
		g_ln.len = 0;
		ft_sdsmpush(&g_ln, ln, (size_t)g_rd[fd]);
		return (g_ln.buf);
	}
	return (NULL);
}
