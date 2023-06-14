# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/23 13:25:59 by moudrib           #+#    #+#              #
#    Updated: 2023/06/14 13:41:01 by moudrib          ###   ########.fr        #
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
				Builtins/echo.c \
				Builtins/exit.c \
				Parsing/input.c \
				Parsing/lexer.c \
				Builtins/unset.c \
				Parsing/expand.c \
				Builtins/export.c \
				Signals/signals.c \
				Parsing/parsing1.c \
				Parsing/parsing2.c \
				Parsing/minishell.c \
				Parsing/final_list.c \
				Parsing/open_files.c \
				Builtins/environment.c \
				Parsing/ft_printf_fd.c \
				List_tools/list_utils.c \
				Builtins/export_utils1.c \
				Builtins/export_utils2.c \
				Libft_tools/libft_utils1.c \
				Libft_tools/libft_utils2.c \
				Libft_tools/libft_utils3.c \
				Libft_tools/libft_utils4.c \
				Builtins/builtins_lexers.c \
				List_tools/list_env_tools.c \
				List_tools/list_cmd_tools.c \
				Builtins/lexer_for_export.c \
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