/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghzal <aaghzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:36:14 by aaghzal           #+#    #+#             */
/*   Updated: 2025/02/11 14:49:54 by aaghzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void	send_char(char c, pid_t kingKai);
static void	ack_handler(int sig);
static void	end_handler(int sig);

int	main(int ac, char **av)
{
	pid_t	server_pid;
	char	*message;
	int		i;

	if (ac != 3)
	{
		write(STDERR_FILENO, "Usage: ./client <kingKai> \"message\"\n", 36);
		return (EXIT_FAILURE);
	}
	server_pid = ft_atoi(av[1]);
	message = av[2];
	signal(SIGUSR1, ack_handler);
	signal(SIGUSR2, end_handler);
	i = 0;
	while (message[i])
		send_char(message[i++], server_pid);
	send_char('\0', server_pid);
	return (EXIT_SUCCESS);
}

volatile sig_atomic_t	g_kingKai = BUSY;

static void	end_handler(int sig)
{
	(void)sig;
	exit(EXIT_SUCCESS);
}

static void	ack_handler(int sig)
{
	(void)sig;
	g_kingKai = READY;
}

static void	send_char(char c, pid_t kingKai)
{
	int	bit;

	bit = 0;
	while (bit < CHAR_BIT)
	{
		while (BUSY == g_kingKai)
		{
			if (c & (0x80 >> bit))
				kill(kingKai, SIGUSR1);
			else
				kill(kingKai, SIGUSR2);
			usleep(42);
		}
		bit++;
		g_kingKai = BUSY;
	}
}
