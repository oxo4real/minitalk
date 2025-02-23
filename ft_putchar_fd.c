/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghzal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 09:03:34 by aaghzal           #+#    #+#             */
/*   Updated: 2024/06/27 09:13:50 by aaghzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	ft_putchar_fd(char c, int fd)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	if (fd >= 0)
		write(fd, &uc, 1);
}
