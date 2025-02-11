/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghzal <aaghzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:06:32 by aaghzal           #+#    #+#             */
/*   Updated: 2024/06/27 17:33:22 by aaghzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	ft_putnbr_fd(int n, int fd)
{
	char			c;
	unsigned int	nb;

	nb = (unsigned int) n;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		nb = ~0u - nb + 1;
	}
	c = (nb % 10) + 48;
	if (nb / 10 != 0)
	{
		ft_putnbr_fd(nb / 10, fd);
	}
	ft_putchar_fd(c, fd);
}
