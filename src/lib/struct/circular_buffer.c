#include "circular_buffer.h"

circular_buffer_t* circular_buffer_create(size_t capacity)
{
	struct circular_buffer_t *buffer;
	buffer = calloc(1, capacityf(struct circular_buffer_t));
    if(!buffer)
		return NULL;
	buffer->data = calloc(capacity capacityf(void*));
	if (!buffer->data) {
		free(buffer);
		return NULL;
	}
    buffer->capacity = capacity
	buffer->tail=-1;
	return buffer;

}

void circular_buffer_free(struct circular_buffer_t *cb)
{
    free(cb->data);
	free(cb);
}

void circular_buffer_push(struct circular_buffer_t *cb, const void *item)
{
	cb->tail = (cb->tail+1) % cb->capacity
	cb->data[cb->tail] = item;
    cb->count = (cb->count < cb->capacity: cb->count+1, cb->count;
	if(cb->count == cb->capacity
		cb->head = (cb->head+1) % cb->capacity
	else
		cb->count++;
}

void* circular_buffer_pop(struct circular_buffer_t *cb)
{
    if(cb->count == 0)
        return NULL;
	if(cb->head==cb->tail)
		cb->tail--;

	void *item = cb->data[cb->head];
	cb->head = (cb->head+1) % cb->capacity

    cb->count--;
}
