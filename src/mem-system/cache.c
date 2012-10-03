/*
 *  Copyright (C) 2012  Rafael Ubal (ubal@ece.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <mem-system.h>


/*
 * Public Variables
 */

struct string_map_t cache_policy_map =
{
	3, {
		{ "LRU", cache_policy_lru },
		{ "FIFO", cache_policy_fifo },
		{ "Random", cache_policy_random }
	}
};

struct string_map_t cache_block_state_map =
{
	6, {
		{ "N", cache_block_noncoherent },
		{ "M", cache_block_modified },
		{ "O", cache_block_owned },
		{ "E", cache_block_exclusive },
		{ "S", cache_block_shared },
		{ "I", cache_block_invalid }
	}
};




/*
 * Private Functions
 */

enum cache_waylist_enum
{
	cache_waylist_head,
	cache_waylist_tail
};

static void cache_update_waylist(struct cache_set_t *set,
	struct cache_block_t *blk, enum cache_waylist_enum where)
{
	if (!blk->way_prev && !blk->way_next)
	{
		assert(set->way_head == blk && set->way_tail == blk);
		return;
		
	}
	else if (!blk->way_prev)
	{
		assert(set->way_head == blk && set->way_tail != blk);
		if (where == cache_waylist_head)
			return;
		set->way_head = blk->way_next;
		blk->way_next->way_prev = NULL;
		
	}
	else if (!blk->way_next)
	{
		assert(set->way_head != blk && set->way_tail == blk);
		if (where == cache_waylist_tail)
			return;
		set->way_tail = blk->way_prev;
		blk->way_prev->way_next = NULL;
		
	}
	else
	{
		assert(set->way_head != blk && set->way_tail != blk);
		blk->way_prev->way_next = blk->way_next;
		blk->way_next->way_prev = blk->way_prev;
	}

	if (where == cache_waylist_head)
	{
		blk->way_next = set->way_head;
		blk->way_prev = NULL;
		set->way_head->way_prev = blk;
		set->way_head = blk;
	}
	else
	{
		blk->way_prev = set->way_tail;
		blk->way_next = NULL;
		set->way_tail->way_next = blk;
		set->way_tail = blk;
	}
}

/*static void cache_update_streamlist(struct stream_buffers_t *sb,
	struct stream_block_t *blk, enum cache_waylist_enum where)
{
	if (!blk->way_prev && !blk->way_next)
	{
		assert(set->way_head == blk && set->way_tail == blk);
		return;
		
	}
	else if (!blk->way_prev)
	{
		assert(set->way_head == blk && set->way_tail != blk);
		if (where == cache_waylist_head)
			return;
		set->way_head = blk->way_next;
		blk->way_next->way_prev = NULL;
		
	}
	else if (!blk->way_next)
	{
		assert(set->way_head != blk && set->way_tail == blk);
		if (where == cache_waylist_tail)
			return;
		set->way_tail = blk->way_prev;
		blk->way_prev->way_next = NULL;
		
	}
	else
	{
		assert(set->way_head != blk && set->way_tail != blk);
		blk->way_prev->way_next = blk->way_next;
		blk->way_next->way_prev = blk->way_prev;
	}

	if (where == cache_waylist_head)
	{
		blk->way_next = set->way_head;
		blk->way_prev = NULL;
		set->way_head->way_prev = blk;
		set->way_head = blk;
	}
	else
	{
		blk->way_prev = set->way_tail;
		blk->way_next = NULL;
		set->way_tail->way_next = blk;
		set->way_tail = blk;
	}
}
*/



/*
 * Public Functions
 */


struct cache_t *cache_create(char *name, unsigned int num_sets, unsigned int block_size,
	unsigned int assoc, unsigned int num_streams, unsigned int pref_aggr, enum cache_policy_t policy)
{
	struct cache_t *cache;
	struct cache_block_t *block;
	struct stream_buffer_t *sb;
	unsigned int set, way, stream;

	/* Create cache */
	cache = calloc(1, sizeof(struct cache_t));
	if (!cache)
		fatal("%s: out of memory", __FUNCTION__);

	/* Name */
	cache->name = strdup(name);
	if (!cache->name)
		fatal("%s: out of memory", __FUNCTION__);

	/* Initialize */
	cache->num_sets = num_sets;
	cache->block_size = block_size;
	cache->assoc = assoc;
	cache->policy = policy;
	cache->prefetch.num_streams = num_streams;
	cache->prefetch.aggressivity = pref_aggr;
	cache->fifo = 0;

	/* Derived fields */
	assert(!(num_sets & (num_sets - 1)));
	assert(!(block_size & (block_size - 1)));
	assert(!(assoc & (assoc - 1)));
	cache->log_block_size = log_base2(block_size);
	cache->block_mask = block_size - 1;

	/* Create matrix of prefetched blocks */
	cache->prefetch.streams = calloc(num_streams, sizeof(struct stream_buffer_t));
	if (!cache->prefetch.streams)
		fatal("%s: out of memory", __FUNCTION__);
	for(stream=0; stream<num_streams; stream++){
		cache->prefetch.streams[stream].blocks =
			calloc(pref_aggr, sizeof(struct stream_block_t));
		if (!cache->prefetch.streams[stream].blocks)
			fatal("%s: out of memory", __FUNCTION__);
	}
	
	/* Initialize streams */
	cache->prefetch.stream_head = &cache->prefetch.streams[0];
	cache->prefetch.stream_tail = &cache->prefetch.streams[num_streams - 1];
	for (stream = 0; stream < num_streams; stream++){
		sb = &cache->prefetch.streams[stream];
		sb->stream = stream;
		sb->stream_prev = stream ? &cache->prefetch.streams[stream-1] : NULL;
		sb->stream_next = stream<num_streams-1 ? &cache->prefetch.streams[stream+1] : NULL;
	}

	/* Create array of sets */
	cache->sets = calloc(num_sets, sizeof(struct cache_set_t));
	if (!cache->sets)
		fatal("%s: out of memory", __FUNCTION__);

	/* Initialize array of sets */
	for (set = 0; set < num_sets; set++)
	{
		/* Create array of blocks */
		cache->sets[set].blocks = calloc(assoc, sizeof(struct cache_block_t));
		if (!cache->sets[set].blocks)
			fatal("%s: out of memory", __FUNCTION__);

		/* Initialize array of blocks */
		cache->sets[set].way_head = &cache->sets[set].blocks[0];
		cache->sets[set].way_tail = &cache->sets[set].blocks[assoc - 1];
		for (way = 0; way < assoc; way++)
		{
			block = &cache->sets[set].blocks[way];
			block->way = way;
			block->way_prev = way ? &cache->sets[set].blocks[way - 1] : NULL;
			block->way_next = way < assoc - 1 ? &cache->sets[set].blocks[way + 1] : NULL;
			block->prefetched = 0;
		}
	}
	
	/* Return it */
	return cache;
}


void cache_free(struct cache_t *cache)
{
	unsigned int set, stream;

	for (set = 0; set < cache->num_sets; set++)
		free(cache->sets[set].blocks);
	for(stream=0; stream<cache->prefetch.num_streams; stream++)
		free(cache->prefetch.streams[stream].blocks);
	free(cache->prefetch.streams);
	free(cache->sets);
	free(cache->name);
	free(cache);
}


/* Return {set, tag, offset} for a given address */
void cache_decode_address(struct cache_t *cache, unsigned int addr, int *set_ptr, int *tag_ptr, 
	unsigned int *offset_ptr)
{
	PTR_ASSIGN(set_ptr, (addr >> cache->log_block_size) % cache->num_sets);
	PTR_ASSIGN(tag_ptr, addr & ~cache->block_mask);
	PTR_ASSIGN(offset_ptr, addr & cache->block_mask);
}


/* Look for a block in the cache. If it is found and its state is other than 0,
 * the function returns 1 and the state and way of the block are also returned.
 * The set where the address would belong is returned anyways. */
int cache_find_block(struct cache_t *cache, unsigned int addr, int *set_ptr, int *way_ptr, 
	int *state_ptr)
{
	int set, tag, way;

	/* Locate block */
	tag = addr & ~cache->block_mask;
	set = (addr >> cache->log_block_size) % cache->num_sets;
	PTR_ASSIGN(set_ptr, set);
	PTR_ASSIGN(state_ptr, 0);  /* Invalid */
	for (way = 0; way < cache->assoc; way++)
		if (cache->sets[set].blocks[way].tag == tag && cache->sets[set].blocks[way].state)
			break;
	
	/* Block not found */
	if (way == cache->assoc)
		return 0;
	
	/* Block found */
	PTR_ASSIGN(way_ptr, way);
	PTR_ASSIGN(state_ptr, cache->sets[set].blocks[way].state);
	return 1;
}


/* Set the tag and state of a block.
 * If replacement policy is FIFO, update linked list in case a new
 * block is brought to cache, i.e., a new tag is set. */
void cache_set_block(struct cache_t *cache, int set, int way, int tag, int state, unsigned int prefetched)
{
	assert(set >= 0 && set < cache->num_sets);
	assert(way >= 0 && way < cache->assoc);

	mem_trace("mem.set_block cache=\"%s\" set=%d way=%d tag=0x%x state=\"%s\"\n",
			cache->name, set, way, tag,
			map_value(&cache_block_state_map, state));

	if (cache->policy == cache_policy_fifo
		&& cache->sets[set].blocks[way].tag != tag)
		cache_update_waylist(&cache->sets[set],
			&cache->sets[set].blocks[way],
			cache_waylist_head);
	cache->sets[set].blocks[way].tag = tag;
	cache->sets[set].blocks[way].state = state;
	cache->sets[set].blocks[way].prefetched = prefetched;
}


/* Set the tag and state of a prefetched block */
void cache_set_pref_block(struct cache_t *cache, int pref_stream, int pref_slot, int tag, int state)
{
	assert(pref_stream >= 0 && pref_stream < cache->prefetch.num_streams);
	assert(pref_slot>=0 && pref_slot < cache->prefetch.aggressivity);

	mem_trace("mem.set_block in prefetch buffer of \"%s\"\
			pref_stream=%d tag=0x%x state=\"%s\"\n",
			cache->name, pref_stream, tag,
			map_value(&cache_block_state_map, state));

	cache->prefetch.streams[pref_stream].blocks[pref_slot].tag = tag;
	cache->prefetch.streams[pref_stream].blocks[pref_slot].state = state;
}


void cache_get_block(struct cache_t *cache, int set, int way, int *tag_ptr, int *state_ptr)
{
	assert(set >= 0 && set < cache->num_sets);
	assert(way >= 0 && way < cache->assoc);
	PTR_ASSIGN(tag_ptr, cache->sets[set].blocks[way].tag);
	PTR_ASSIGN(state_ptr, cache->sets[set].blocks[way].state);
}

struct stream_block_t * cache_get_pref_block(struct cache_t *cache,
	int pref_stream, int pref_slot)
{
	assert(pref_stream>=0 && pref_stream < cache->prefetch.num_streams);
	assert(pref_slot>=0 && pref_slot < cache->prefetch.aggressivity);
	return &cache->prefetch.streams[pref_stream].blocks[pref_slot];
}

void cache_get_pref_block_data(struct cache_t *cache, int pref_stream,
	int pref_slot, int *tag_ptr, int *state_ptr)
{
	assert(pref_stream>=0 && pref_stream < cache->prefetch.num_streams);
	assert(pref_slot>=0 && pref_slot < cache->prefetch.aggressivity);

	PTR_ASSIGN(tag_ptr, cache->prefetch.streams[pref_stream].blocks[pref_slot].tag);
	PTR_ASSIGN(state_ptr, cache->prefetch.streams[pref_stream].blocks[pref_slot].state);
}


/* Update LRU counters, i.e., rearrange linked list in case
 * replacement policy is LRU. */
void cache_access_block(struct cache_t *cache, int set, int way)
{
	int move_to_head;
	
	assert(set >= 0 && set < cache->num_sets);
	assert(way >= 0 && way < cache->assoc);

	/* A block is moved to the head of the list for LRU policy.
	 * It will also be moved if it is its first access for FIFO policy, i.e., if the
	 * state of the block was invalid. */
	move_to_head = cache->policy == cache_policy_lru ||
		(cache->policy == cache_policy_fifo && !cache->sets[set].blocks[way].state);
	if (move_to_head && cache->sets[set].blocks[way].way_prev)
		cache_update_waylist(&cache->sets[set],
			&cache->sets[set].blocks[way],
			cache_waylist_head);
}

void cache_access_stream(struct cache_t *cache, int stream)
{
	struct stream_buffer_t * accessed;

	//Integrity tests
	assert(stream >= 0 && stream < cache->prefetch.num_streams);
#ifndef NDEBUG
	for(accessed = cache->prefetch.stream_head;
		accessed->stream_next;
		accessed = accessed->stream_next){};
	assert(accessed == cache->prefetch.stream_tail);
	for(accessed = cache->prefetch.stream_tail;
		accessed->stream_prev;
		accessed = accessed->stream_prev){};
#endif
	assert(accessed == cache->prefetch.stream_head);

	//Return if only one stream
	if(cache->prefetch.num_streams < 2) return;

	accessed = &cache->prefetch.streams[stream];
	//Is tail
	if(!accessed->stream_next && accessed->stream_prev){
		accessed->stream_prev->stream_next = NULL;
		cache->prefetch.stream_tail = accessed->stream_prev;
	//Is in the middle
	} else if(accessed->stream_next && accessed->stream_prev) {
		accessed->stream_prev->stream_next = accessed->stream_next;
		accessed->stream_next->stream_prev = accessed->stream_prev;
	//Is already in the head
	} else {
		return;
	}

	/* Put first */
	accessed->stream_prev = NULL;
	accessed->stream_next = cache->prefetch.stream_head;
	accessed->stream_next->stream_prev = accessed;
	cache->prefetch.stream_head = accessed;
}


/* Return LRU or empty stream buffer */
int cache_select_stream(struct cache_t *cache)
{
	/* Try to find an empty stream in the LRU order */
	struct stream_buffer_t *stream;
	for (stream = cache->prefetch.stream_tail; stream; stream = stream->stream_prev)
		if (!stream->blocks[0].state) //SLOT
				return stream->stream;
	/* LRU */
	return cache->prefetch.stream_tail->stream;
}


/* Return the way of the block to be replaced in a specific set,
 * depending on the replacement policy */
int cache_replace_block(struct cache_t *cache, int set)
{
	//struct cache_block_t *block;

	/* Try to find an invalid block. Do this in the LRU order, to avoid picking the
	 * MRU while its state has not changed to valid yet. */
	assert(set >= 0 && set < cache->num_sets);
	/*
	for (block = cache->sets[set].way_tail; block; block = block->way_prev)
		if (!block->state)
			return block->way;
	*/

	/* LRU and FIFO replacement: return block at the
	 * tail of the linked list */
	if (cache->policy == cache_policy_lru ||
		cache->policy == cache_policy_fifo)
	{
		int way = cache->sets[set].way_tail->way;
		cache_update_waylist(&cache->sets[set], cache->sets[set].way_tail, 
			cache_waylist_head);

		return way;
	}
	
	/* Random replacement */
	assert(cache->policy == cache_policy_random);
	return random() % cache->assoc;
}


void cache_set_transient_tag(struct cache_t *cache, int set, int way, int tag)
{
	struct cache_block_t *block;

	/* Set transient tag */
	block = &cache->sets[set].blocks[way];
	block->transient_tag = tag;
}
