/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghzal <aaghzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:40:59 by aaghzal           #+#    #+#             */
/*   Updated: 2025/02/11 14:49:23 by aaghzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	handle_signal(int signo, siginfo_t *info, void *more_info);

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_putnbr_fd(getpid(), STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	while (1337)
		pause();
}

static void	handle_signal(int signo, siginfo_t *info, void *more_info)
{
	static pid_t	curr_client;
	static char		c;
	static int		bit;

	(void)more_info;
	if (curr_client != 0 && info->si_pid != curr_client)
		return ;
	curr_client = info->si_pid;
	if (signo == SIGUSR1)
		c &= ~(0b10000000) >> (bit++);
	else if (signo == SIGUSR2)
		c |= 0b10000000 >> (bit++);
	if (bit == CHAR_BIT)
	{
		bit = 0;
		if (c == '\0')
		{
			kill(curr_client, SIGUSR2);
			curr_client = 0;
			c = 0;
			return ((void)write(STDOUT_FILENO, "\n", 1));
		}
		write(STDOUT_FILENO, &c, 1);
		c = 0;
	}
	kill(curr_client, SIGUSR1);
}
