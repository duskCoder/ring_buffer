CC       ?= gcc
CFLAGS   += -W -Wall -std=c99 -Wextra
CFLAGS   += -I./
CFLAGS   += -D _GNU_SOURCE
NAME      = librb.a
SRC       = src/rb.c src/rb_str.c

AR        = ar rc

all: depend $(NAME)

depend: .depend

.depend: $(SRC)
	@$(RM) .depend
	@$(CC) $(CFLAGS) -MM $^ > .depend

include .depend

OBJ     = $(SRC:.c=.o)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)
	ranlib $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all depend clean fclean all re
