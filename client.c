/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghzal <aaghzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:36:14 by aaghzal           #+#    #+#             */
/*   Updated: 2025/02/12 20:08:44 by aaghzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void	send_char(char c, pid_t server_pid);
static void	ack_handler(int sig);
static void	end_handler(int sig);

volatile sig_atomic_t	g_server_state = BUSY;

int	main(int ac, char **av)
{
	pid_t	server_pid;
	char	*message;
	int		i;

	if (ac != 3)
	{
		write(STDERR_FILENO, "Usage: ./client <server_pid> \"message\"\n", 36);
		return (EXIT_FAILURE);
	}
	server_pid = ft_atoi(av[1]);
	if (server_pid < 1)
		return (write(STDERR_FILENO, "Invalid server_pid\n", 19), EXIT_FAILURE);
	message = av[2];
	signal(SIGUSR1, ack_handler);
	signal(SIGUSR2, end_handler);
	i = 0;
	while (message[i])
		send_char(message[i++], server_pid);
	send_char('\0', server_pid);
	return (EXIT_SUCCESS);
}

static void	end_handler(int sig)
{
	(void)sig;
	exit(EXIT_SUCCESS);
}

static void	ack_handler(int sig)
{
	(void)sig;
	g_server_state = READY;
}

static void	send_char(char c, pid_t server_pid)
{
	int	bit;

	bit = 0;
	while (bit < CHAR_BIT)
	{
		while (g_server_state == BUSY)
		{
			if (c & (0b10000000 >> bit))
				kill(server_pid, SIGUSR2);
			else
				kill(server_pid, SIGUSR1);
			usleep(1000);
		}
		bit++;
		g_server_state = BUSY;
	}
}
