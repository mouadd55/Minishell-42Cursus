# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/23 13:25:59 by moudrib           #+#    #+#              #
#    Updated: 2023/06/12 22:44:51 by yonadry          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CC		= cc
CFLAGS	= -Wall -Wextra -g -O1 -fsanitize=address #-Werror
OBJDIR	= _Object_files/
R		= \x1B[91m
G		= \x1B[32m
W		= \x1B[0m
B		= \033[1m
LIB 	= $(shell brew --prefix readline)/lib 
INC 	= $(shell brew --prefix readline)/include
SRC		=	\
				Parsing/echo.c \
				Parsing/exit.c \
				Parsing/input.c \
				Parsing/unset.c \
				Parsing/lexer.c \
				Parsing/export.c \
				Parsing/expand.c \
				Parsing/signals.c \
				Parsing/parsing1.c \
				Parsing/parsing2.c \
				Parsing/minishell.c \
				Parsing/final_list.c \
				Parsing/list_utils.c \
				Parsing/open_files.c \
				Parsing/environment.c \
				Parsing/ft_printf_fd.c \
				Parsing/libft_utils1.c \
				Parsing/libft_utils2.c \
				Parsing/libft_utils3.c \
				Parsing/libft_utils4.c \
				Parsing/export_utils1.c \
				Parsing/export_utils2.c \
				Parsing/lexer_for_export.c \
				Execution/execution.c \
				Execution/simple_cmd.c \
				Execution/multiple_pipes.c \

OBJ		= $(addprefix $(OBJDIR),$(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline -L $(LIB)
# @clear
# @echo
# @echo "$(G)Minishell compilation done ✔️"
# @echo
# @echo "         $(R)███$(W)╗   $(R)███$(W)╗ $(R)██$(W)╗ $(R)███$(W)╗   $(R)██$(W)╗ $(R)██$(W)╗ $(R)███████$(W)╗ $(R)██$(W)╗  $(R)██$(W)╗ $(R)███████$(W)╗ $(R)██$(W)╗      $(R)██$(W)╗"
# @echo "         $(R)████$(W)╗ $(R)████$(W)║ $(R)██$(W)║ $(R)████$(W)╗  $(R)██$(W)║ $(R)██$(W)║ $(R)██$(W)╔════╝ $(R)██$(W)║  $(R)██$(W)║ $(R)██$(W)╔════╝ $(R)██$(W)║      $(R)██$(W)║"
# @echo "         $(R)██$(W)╔$(R)████$(W)╔$(R)██$(W)║ $(R)██$(W)║ $(R)██$(W)╔$(R)██$(W)╗ $(R)██$(W)║ $(R)██$(W)║ $(R)███████$(W)╗ $(R)███████$(W)║ $(R)█████$(W)╗   $(R)██$(W)║      $(R)██$(W)║"
# @echo "         $(W)██$(R)║╚$(W)██$(R)╔╝$(W)██$(R)║ $(W)██$(R)║ $(W)██$(R)║╚$(W)██$(R)╗$(W)██$(R)║ $(W)██$(R)║ ╚════$(W)██$(R)║ $(W)██$(R)╔══$(W)██$(R)║ $(W)██$(R)╔══╝   $(W)██$(R)║      $(W)██$(R)║"
# @echo "         $(W)██$(R)║ ╚═╝ $(W)██$(R)║ $(W)██$(R)║ $(W)██$(R)║ ╚$(W)████$(R)║ $(W)██$(R)║ $(W)███████$(R)║ $(W)██$(R)║  $(W)██$(R)║ $(W)███████$(R)╗ $(W)███████$(R)╗ $(W)███████$(R)╗"
# @echo "         $(R)╚═╝     ╚═╝ ╚═╝ ╚═╝  ╚═══╝ ╚═╝ ╚══════╝ ╚═╝  ╚═╝ ╚══════╝ ╚══════╝ ╚══════╝"
# @echo "                                                               $(B)$(R)Made by $(W)$(B)moudrib $(R)&& $(W)$(B)yonadry"
# @echo
# @echo

$(OBJDIR)%.o: %.c minishell.h
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -I $(INC) -c $< -o $@
	@echo "$(G)Compiling: $(W)$<"

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

#< infile ls > outfile -a << here -l | wc -l >> outfile