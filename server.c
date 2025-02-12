/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghzal <aaghzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:40:59 by aaghzal           #+#    #+#             */
/*   Updated: 2025/02/12 20:49:19 by aaghzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	handle_signal(int signo, siginfo_t *info, void *more_info);
static void	set_bit(int signo, char *c, int bit);
static int	time_out(t_list **list, pid_t client_pid, int *bit);
static void	lst_clr(t_list **list);

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

	(void)more_info;
	if (curr_client != 0 && !time_out(&list, info->si_pid, &bit) && info->si_pid != curr_client)
		return ;
	lst_clr(&list);
	curr_client = info->si_pid;
	set_bit(signo, &c, bit++);
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

static void	set_bit(int signo, char *c, int bit)
{
	if (signo == SIGUSR1)
		(*c) &= ~(0b10000000) >> bit;
	else if (signo == SIGUSR2)
		(*c) |= 0b10000000 >> bit;
}

static int	time_out(t_list **list, pid_t client_pid, int *bit)
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
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	return (0);
}

static void	lst_clr(t_list **list)
{
	t_list	*tmp;

	if (!list)
		return ;
	while (*list)
	{
		tmp = (*list);
		(*list) = (*list)->next;
		free(tmp);
	}
	(*list) = NULL;
}
