# Copyright (C) 2013 Olivier Gayot
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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
