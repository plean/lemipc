CC	= gcc

CFLAGS	+= -W -Wall -Wextra -Iinclude -g3
LDFLAGS += 

SRC	= $(addprefix src/, \
	  	main.c \
		ipc.c \
		utils.c \
		)

OBJ	= $(SRC:.c=.o)

NAME	= lemipc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re
