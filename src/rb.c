#include <stdlib.h>
#include <string.h>

#include "rb.h"

#define MAX(_x1, _x2) (((_x1) > (_x2)) ? (_x1) : (_x2))
#define MIN(_x1, _x2) (((_x1) < (_x2)) ? (_x1) : (_x2))

int rb_init(t_rb *rb)
{
    rb->off_r       = 0;
    rb->off_w       = 0;
    rb->size_filled = 0;

    return 0;
}

t_rb *rb_new(void)
{
    t_rb *rb;

    if ((rb = malloc(sizeof(t_rb))) == NULL) {
        return NULL;
    }

    rb_init(rb);

    return rb;
}

void rb_delete(t_rb **rb)
{
    free(*rb);
    *rb = NULL;
}

t_rb *rb_put(t_rb *rb, const void *src, size_t n)
{
    size_t size;

    /* loop until n bytes of src have been written */
    while (n != 0) {
        size = MIN(n, rb_get_size(rb) - rb->off_w);

        memcpy(&rb->buffer[rb->off_w], src, size);

        /* if we overrided the data to read, we need to move off_r */
        if (rb->off_w < rb->off_r && rb->off_w + size > rb->off_r) {
            rb->off_r = (rb->off_w + size) % rb_get_size(rb);
        } else if (rb->off_r == rb->off_w
                && rb->size_filled == rb_get_size(rb))
        {
            rb->off_r = (rb->off_w + size) % rb_get_size(rb);
        }

        /* this will put the offset back to 0 if we reached the end */
        rb->off_w = (rb->off_w + size) % rb_get_size(rb);

        n   -= size;
        src += size;

        rb->size_filled += size;
        rb->size_filled  = MIN(rb->size_filled, rb_get_size(rb));
    }

    return rb;
}

size_t rb_get(t_rb *rb, void *dest, size_t n)
{
    size_t size = rb_peek(rb, dest, n);

    rb->size_filled -= size;
    rb->off_r        = (rb->off_r + size) % rb_get_size(rb);

    if (rb->size_filled == 0) {
        /* optimize space */
        rb->off_w = rb->off_r = 0;
    }

    return size;
}

size_t rb_peek(const t_rb *rb, void *dest, size_t n)
{
    uint16_t offset;
    size_t size;
    size_t size2;

    if (rb->size_filled == 0 || n == 0) {
        return 0;
    }

    size = 0;
    if (rb->off_r >= rb->off_w) {
        /* do not try to retrieve too much data */
        size = MIN(n, rb_get_size(rb) - rb->off_r);

        memcpy(dest, &rb->buffer[rb->off_r], size);

        dest += size;
        n    -= size;
    }

    offset = (rb->off_r + size) % rb_get_size(rb);

    size2 = MIN(n, (size_t)(rb->off_w - offset));

    memcpy(dest, &rb->buffer[offset], size2);

    return size + size2;
}
