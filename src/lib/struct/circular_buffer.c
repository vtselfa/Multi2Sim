#include "circular_buffer.h"

struct circular_buffer_t* circular_buffer_create(size_t capacity)
{
	struct circular_buffer_t *cb;
	cb = calloc(1, sizeof(struct circular_buffer_t));
    if(!buffer)
		return NULL;
	cb->elem = calloc(capacity, sizeof(void*));
	if (!cb->elem) {
		free(cb);
		return NULL;
	}
    cb->capacity = capacity;
	return cb;

}

void circular_buffer_free(struct circular_buffer_t *cb)
{
    free(cb->elem);
	free(cb);
}

int circular_buffer_empty(struct circular_buffer_t *cb)
{
	return !cb->count;
}

int circular_buffer_full(struct circular_buffer_t *cb)
{
	return cb->count == cb->capacity;
}

void circular_buffer_push(struct circular_buffer_t *cb, const void *item)
{
	if(circular_buffer_full(cb)) //The first inserted element is overwrited
			cb->head = (cb->head+1) % cb->capacity;
	cb->elem[cb->tail] = item;
	cb->tail = (cb->tail+1) % cb->capacity;
    cb->count = (cb->count+1) % cb->capacity;
}

void* circular_buffer_pop(struct circular_buffer_t *cb)
{
    if(circular_buffer_empty(cb))
        return NULL;

	void *item = cb->elem[cb->head];
	cb->head = (cb->head+1) % cb->capacity

    cb->count--;
}
