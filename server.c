/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghzal <aaghzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:40:59 by aaghzal           #+#    #+#             */
/*   Updated: 2025/02/13 10:10:54 by aaghzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	handle_signal(int signo, siginfo_t *info, void *more_info);
static void	set_bit(int signo, char *c, int bit);
static int	time_out(t_list **list, pid_t client_pid, int *bit, char *unicode);
static void	handle_char(char *c, int *curr_client, char *unicode);

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
	static t_list	*list;
	static char		unicode[8] = {0};

	(void)more_info;
	if (curr_client != 0 && !time_out(&list, info->si_pid, &bit, unicode)
		&& info->si_pid != curr_client)
		return ;
	lst_clr(&list);
	curr_client = info->si_pid;
	set_bit(signo, &c, bit++);
	if (bit == CHAR_BIT)
	{
		bit = 0;
		handle_char(&c, &curr_client, unicode);
	}
	if (curr_client != THIS_SERVER)
		kill(curr_client, SIGUSR1);
}

static void	set_bit(int signo, char *c, int bit)
{
	if (signo == SIGUSR1)
		(*c) &= ~(0b10000000) >> bit;
	else if (signo == SIGUSR2)
		(*c) |= 0b10000000 >> bit;
}

static int	time_out(t_list **list, pid_t client_pid, int *bit, char *unicode)
{
	t_list	*head;

	head = (*list);
	while (head && head->client_pid != client_pid)
		head = head->next;
	if (!head)
	{
		head = malloc(sizeof(t_list) * 1);
		if (!head)
			return (1);
		head->client_pid = client_pid;
		head->requests = 0;
		head->next = (*list);
		(*list) = head;
	}
	head->requests += 1;
	if (head->requests == TIME_OUT)
	{
		(*bit) = 0;
		unicode[0] = 0;
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	return (0);
}

static void	handle_char(char *c, int *curr_client, char *unicode)
{
	static int	pos;

	if ((*c) == '\0')
	{
		kill((*curr_client), SIGUSR2);
		(*curr_client) = 0;
		if (unicode[0] != 0)
			write(STDOUT_FILENO, unicode, pos + 1);
		unicode[0] = 0;
		pos = 0;
		write(STDOUT_FILENO, "\n", 1);
	}
	else if ((*c) & 0b10000000 && !((*c) & 0b01000000))
	{
		pos++;
		unicode[pos] = (*c);
	}
	else
	{
		if (unicode[0] != 0)
			write(STDOUT_FILENO, unicode, pos + 1);
		pos = 0;
		unicode[pos] = (*c);
	}
	(*c) = 0;
}
