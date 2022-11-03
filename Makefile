# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fkernbac <fkernbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/22 15:43:54 by fkernbac          #+#    #+#              #
#    Updated: 2022/03/30 14:36:04 by fkernbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRC = checks.c io.c main.c philosopher.c table.c terminate.c time.c
FLAGS = -Wall -Wextra -Werror -Ofast



all: $(NAME)

$(NAME): $(SRC) philo.h
	$(CC) $(SRC) $(FLAGS) -pthread -o $@

clean:
	/bin/rm -f *.o

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY:  all clean fclean re debug
