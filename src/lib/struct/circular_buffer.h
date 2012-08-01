#include <stdlib.h>
struct circular_buffer_t
{
    void **elem;     	// buffer
    size_t capacity;  	// maximum number of items in the buffer
    size_t count;     	// number of items in the buffer
    size_t head;       	// position of head
    size_t tail;       	// position of tail
};

struct circular_buffer_t* circular_buffer_create(size_t capacity);
void circular_buffer_free(struct circular_buffer_t *cb);
void circular_buffer_push(struct circular_buffer_t *cb, const void *item);
void* circular_buffer_pop(struct circular_buffer_t *cb);
void* circular_buffer_head(struct circular_buffer_t *cb);
