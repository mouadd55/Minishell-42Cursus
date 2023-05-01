# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/23 13:25:59 by moudrib           #+#    #+#              #
#    Updated: 2023/05/01 18:03:52 by moudrib          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CC		= cc
CFLAGS	= -Wall -Wextra -Werror  -g -O1 #-fsanitize=address
OBJDIR	= Object_files/
R		= \x1B[91m
G		= \x1B[32m
W		= \x1B[0m
B		= \033[1m
SRC		=	\
			Parsing/minishell.c \
			Parsing/list_utils.c \
			Parsing/libft_utils1.c \
			Parsing/libft_utils2.c \
			Parsing/libft_utils3.c \
			Parsing/parsing.c \
			Parsing/input.c \


OBJ		= $(addprefix $(OBJDIR),$(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline
#@clear
#@echo
#@echo "$(G)Minishell compilation done ✔️"
#@echo
#@echo "         $(R)███$(W)╗   $(R)███$(W)╗ $(R)██$(W)╗ $(R)███$(W)╗   $(R)██$(W)╗ $(R)██$(W)╗ $(R)███████$(W)╗ $(R)██$(W)╗  $(R)██$(W)╗ $(R)███████$(W)╗ $(R)██$(W)╗      $(R)██$(W)╗"
#@echo "         $(R)████$(W)╗ $(R)████$(W)║ $(R)██$(W)║ $(R)████$(W)╗  $(R)██$(W)║ $(R)██$(W)║ $(R)██$(W)╔════╝ $(R)██$(W)║  $(R)██$(W)║ $(R)██$(W)╔════╝ $(R)██$(W)║      $(R)██$(W)║"
#@echo "         $(R)██$(W)╔$(R)████$(W)╔$(R)██$(W)║ $(R)██$(W)║ $(R)██$(W)╔$(R)██$(W)╗ $(R)██$(W)║ $(R)██$(W)║ $(R)███████$(W)╗ $(R)███████$(W)║ $(R)█████$(W)╗   $(R)██$(W)║      $(R)██$(W)║"
#@echo "         $(W)██$(R)║╚$(W)██$(R)╔╝$(W)██$(R)║ $(W)██$(R)║ $(W)██$(R)║╚$(W)██$(R)╗$(W)██$(R)║ $(W)██$(R)║ ╚════$(W)██$(R)║ $(W)██$(R)╔══$(W)██$(R)║ $(W)██$(R)╔══╝   $(W)██$(R)║      $(W)██$(R)║"
#@echo "         $(W)██$(R)║ ╚═╝ $(W)██$(R)║ $(W)██$(R)║ $(W)██$(R)║ ╚$(W)████$(R)║ $(W)██$(R)║ $(W)███████$(R)║ $(W)██$(R)║  $(W)██$(R)║ $(W)███████$(R)╗ $(W)███████$(R)╗ $(W)███████$(R)╗"
#@echo "         $(R)╚═╝     ╚═╝ ╚═╝ ╚═╝  ╚═══╝ ╚═╝ ╚══════╝ ╚═╝  ╚═╝ ╚══════╝ ╚══════╝ ╚══════╝"
#@echo "                                                               $(B)$(R)Made by $(W)$(B)moudrib $(R)&& $(W)$(B)yonadry"
#@echo
#@echo

$(OBJDIR)%.o: %.c minishell.h
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(G)Compiling: $(W)$<"
	@clear

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

#< infile ls > outfile -a << here -l | wc -l >> outfile
# -fsanitize=address
