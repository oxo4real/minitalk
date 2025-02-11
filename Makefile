# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaghzal <aaghzal@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 14:32:10 by aaghzal           #+#    #+#              #
#    Updated: 2025/02/11 14:47:56 by aaghzal          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
BUILD_DIR = build/
SRC_SERV = server.c ft_putchar_fd.c ft_putnbr_fd.c
SRC_USR = client.c ft_atoi.c ft_isdigit.c
OBJ_SERV = $(addprefix $(BUILD_DIR),$(SRC_SERV:.c=.o))
OBJ_USR = $(addprefix $(BUILD_DIR),$(SRC_USR:.c=.o))

all: $(BUILD_DIR) server client

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_SERV): server.h

$(OBJ_USR): client.h

server: $(OBJ_SERV)
	$(CC) $(CFLAGS) $^ -o $@

client: $(OBJ_USR)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm -rf $(BUILD_DIR)

fclean: clean
	@rm -f server client

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus
