/*
 * Copyright (C) 2013 Olivier Gayot
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef RB_STR_H
#define RB_STR_H

#include <string.h>
#include <stdarg.h>

#include "rb.h"

/*
 * locate in the ring buffer pointed to by rb the first string ending by the
 * string pointed to by string and return it. the string returned is
 * allocated using malloc and should be freed by the developer.
 * if no such delimiter can be found, NULL is returned and the ring buffer
 * is left unmodified. Otherwise, the string is removed from it.
 * nb: the delimiter is discarded in the returned string
 *
 * common use case:
 *      request = rb_gets(rb, "\r\n")
 */
char *rb_gets(rb_t *rb, const char *delim);

/*
 * same as the above function but takes a null terminated array of delimiters
 * the function will return the shortest string matching the criteria.
 * if two or more delimiters are found at the same place in the ring buffer,
 * the longest will be discarded
 *
 * common use case:
 *      request = rb_gets(rb, (const char *[]){"\n", "\r", "\r\n", NULL})
 *  which will return a request disregarding whether netcat or telnet is used
 */
char *rb_gets2(rb_t *rb, const char *const *delim);

/*
 * this functions writes str to the ring buffer pointed by rb. If the ring
 * buffer is not large enough to contain all the data, the beginning will be
 * overriden. However, a buffer overflow is not susceptible to happen.
 */
static inline rb_t *rb_puts(rb_t *rb, const char *str)
{
    return rb_put(rb, str, strlen(str));
}

rb_t *rb_vprintf(rb_t *rb, const char *fmt, va_list list);

/*
 * function which behaves like sprintf but which writes into a ring buffer
 * if the ring buffer is not large enough to contain all the data,
 * the beginning will be overriden. However, a buffer overflow will not occur
 */
    __attribute__((format(printf, 2, 3)))
rb_t *rb_printf(rb_t *rb, const char *fmt, ...);

#endif /* RB_STR_H */
