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

volatile sig_atomic_t	g_kingKai = 0;

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
	if (server_pid < 1)
		return (write(STDERR_FILENO, "Invalid pid\n", 12), EXIT_FAILURE);
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
	g_kingKai = 1;
}

static void	send_char(char c, pid_t server_pid)
{
	int	bit;

	bit = 0;
	while (bit < CHAR_BIT)
	{
		while (g_kingKai == 0)
		{
			if (c & (0b10000000 >> bit))
				kill(server_pid, SIGUSR2);
			else
				kill(server_pid, SIGUSR1);
			usleep(1337);
		}
		bit++;
		g_kingKai = 0;
	}
}
