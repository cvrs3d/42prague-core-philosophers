# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/22 17:03:45 by yustinov          #+#    #+#              #
#    Updated: 2025/01/22 17:11:37 by yustinov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= gcc
CFLAGS	= -Werror -Wall -Wextra -pthread -g
NAME	= philo_bonus

SRC_PATH = sources/
OBJ_PATH = objects/

SRC		=	main.c \
			init_module.c \
			output_module.c \
			time_module.c \
			parsing_module.c \
			philosopher_module.c \
			arbitrator_module.c \
			ipc_module.c \
			utils_module.c \
			ft_grab_fork.c \
			string_module.c \
			memory_module.c \
			exit_module.c
SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

INC		= -I ./includes/

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean
