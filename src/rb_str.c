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

#include <malloc.h>

#include "rb_str.h"

typedef uint8_t byte;

static char	*__rb_gets2(rb_t *rb, const char *const *delim,
        const byte *data, size_t size)
{
    char *ret = NULL;
    char *b_ptr = NULL;
    char *ptr;
    size_t b_len; /* length of the current best delimiter */
    size_t len;

    for (; *delim != NULL; ++delim) {
        len = strlen(*delim);
        ptr = memmem(data, size, *delim, len);

        if (ptr == NULL)
            continue;

        if ((b_ptr == NULL) || (ptr < b_ptr)
        || ((ptr == b_ptr) && len > b_len))
        {
            b_ptr = ptr;
            b_len = len;
        }
    }

    if (b_ptr != NULL) {
        ret = strndup((const char *)data, (size_t)b_ptr - (size_t)data);

        rb_remove(rb, (size_t)b_ptr - (size_t)data + b_len);
    }

    return ret;
}

char *rb_gets2(rb_t *rb, const char *const *delim)
{
    size_t size;
    char *ret;
    byte *data;

    data = malloc((rb->size_filled) * sizeof(byte));

    if (data == NULL) {
        return NULL;
    }

    size = rb_peek(rb, data, rb->size_filled);

    ret = __rb_gets2(rb, delim, data, size);

    free(data);

    return ret;
}

char *rb_gets(rb_t *rb, const char *delimit)
{
    char *data  = malloc((rb->size_filled) * sizeof(char));

    if (data == NULL) {
        return NULL;
    }

    size_t size = rb_peek(rb, data, rb->size_filled);
    void *ptr   = memmem(data, size, delimit, strlen(delimit));
    char *ret   = NULL;

    if (ptr != NULL) {
        ret = strndup(data, (size_t)ptr - (size_t)data);

        rb_remove(rb, (size_t)ptr - (size_t)data + strlen(delimit));
    }

    free(data);

    return ret;
}

rb_t *rb_vprintf(rb_t *rb, const char *fmt, va_list ap)
{
    char *buffer;

    if (vasprintf(&buffer, fmt, ap) < 0) {
        /*
         * TODO the manual says that the content of buffer is undefined,
         * we should reconsider the use of vasprintf()
         */

        return rb;
    }

    rb_puts(rb, buffer);

    free(buffer);

    return rb;
}

    __attribute__((format(printf, 2, 3)))
rb_t *rb_printf(rb_t *rb, const char *fmt, ...)
{
    va_list	ap;

    va_start(ap, fmt);

    rb_vprintf(rb, fmt, ap);

    va_end(ap);

    return rb;
}
