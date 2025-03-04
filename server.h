/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaghzal <aaghzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:48:17 by aaghzal           #+#    #+#             */
/*   Updated: 2025/02/13 10:08:20 by aaghzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <unistd.h>
# include <signal.h>
# include <limits.h>
# include <stdlib.h>

# define TIME_OUT 13
# define THIS_SERVER 0

typedef struct s_list
{
	pid_t			client_pid;
	int				requests;
	struct s_list	*next;
}	t_list;

void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
void	lst_clr(t_list **list);

#endif
