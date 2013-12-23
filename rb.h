#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <sys/types.h>

typedef struct t_ring_buffer {
    char buffer[8192];

    uint16_t off_r;
    uint16_t off_w;

    size_t size_filled;
} t_ring_buffer;

/* do not remove for backward compatibility */
typedef t_ring_buffer t_rb;

typedef t_ring_buffer ring_buffer_t;
typedef t_ring_buffer rb_t;

static inline size_t rb_get_size(const rb_t *rb)
{
    return sizeof(rb->buffer);
}

/*
 * initializes rb with default values.
 * returns 0 on success
 */
int rb_init(rb_t *rb);

/* returns an initialized newly allocated ring buffer */
__attribute((malloc)) rb_t *rb_new(void);

void rb_delete(rb_t **);

/* put n bytes of src into the ring buffer pointed by rb */
rb_t *rb_put(rb_t *rb, const void *src, size_t n);

/*
 * take at most n bytes from the ring buffer pointed by rb and
 * put them into dest
 */
size_t rb_get(rb_t *rb, void *dest, size_t n);

/*
 * like rb_get but does not modify the ring_buffer (the bytes are kept
 * in the ring_buffer)
 */
size_t rb_peek(const rb_t *ring_buffer, void *dest, size_t size);

/*
 * remove a maximum of n bytes from the ring buffer pointed to by rb
 * the functions returns the number of bytes actually removed
 */
static inline size_t rb_remove(rb_t *rb, size_t n)
{
    return rb_get(rb, NULL, n);
}

#endif /* RING_BUFFER_H */
