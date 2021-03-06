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
 *  You should have received stack copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <mem-system.h>
#include <x86-timing.h>

/* Events */

int EV_MOD_NMOESI_LOAD;
int EV_MOD_NMOESI_LOAD_LOCK;
int EV_MOD_NMOESI_LOAD_ACTION;
int EV_MOD_NMOESI_LOAD_MISS;
int EV_MOD_NMOESI_LOAD_UNLOCK;
int EV_MOD_NMOESI_LOAD_FINISH;

int EV_MOD_NMOESI_STORE;
int EV_MOD_NMOESI_STORE_LOCK;
int EV_MOD_NMOESI_STORE_ACTION;
int EV_MOD_NMOESI_STORE_UNLOCK;
int EV_MOD_NMOESI_STORE_FINISH;

int EV_MOD_NMOESI_NC_STORE;
int EV_MOD_NMOESI_NC_STORE_LOCK;
int EV_MOD_NMOESI_NC_STORE_WRITEBACK;
int EV_MOD_NMOESI_NC_STORE_ACTION;
int EV_MOD_NMOESI_NC_STORE_MISS;
int EV_MOD_NMOESI_NC_STORE_UNLOCK;
int EV_MOD_NMOESI_NC_STORE_FINISH;

int EV_MOD_NMOESI_FIND_AND_LOCK;
int EV_MOD_NMOESI_FIND_AND_LOCK_PORT;
int EV_MOD_NMOESI_FIND_AND_LOCK_ACTION;
int EV_MOD_NMOESI_FIND_AND_LOCK_FINISH;

int EV_MOD_NMOESI_PREF_FIND_AND_LOCK;
int EV_MOD_NMOESI_PREF_FIND_AND_LOCK_PORT;
int EV_MOD_NMOESI_PREF_FIND_AND_LOCK_ACTION;
int EV_MOD_NMOESI_PREF_FIND_AND_LOCK_FINISH;

int EV_MOD_NMOESI_INVALIDATE_SLOT;
int EV_MOD_NMOESI_INVALIDATE_SLOT_LOCK;
int EV_MOD_NMOESI_INVALIDATE_SLOT_ACTION;
int EV_MOD_NMOESI_INVALIDATE_SLOT_UNLOCK;
int EV_MOD_NMOESI_INVALIDATE_SLOT_FINISH;

int EV_MOD_NMOESI_EVICT;
int EV_MOD_NMOESI_EVICT_INVALID;
int EV_MOD_NMOESI_EVICT_ACTION;
int EV_MOD_NMOESI_EVICT_RECEIVE;
int EV_MOD_NMOESI_EVICT_WRITEBACK;
int EV_MOD_NMOESI_EVICT_WRITEBACK_EXCLUSIVE;
int EV_MOD_NMOESI_EVICT_WRITEBACK_FINISH;
int EV_MOD_NMOESI_EVICT_PROCESS;
int EV_MOD_NMOESI_EVICT_PROCESS_NONCOHERENT;
int EV_MOD_NMOESI_EVICT_WAIT_FOR_REQS;
int EV_MOD_NMOESI_EVICT_REPLY;
int EV_MOD_NMOESI_EVICT_REPLY_RECEIVE;
int EV_MOD_NMOESI_EVICT_FINISH;

int EV_MOD_NMOESI_PREF_EVICT;
int EV_MOD_NMOESI_PREF_EVICT_INVALID;
int EV_MOD_NMOESI_PREF_EVICT_ACTION;
int EV_MOD_NMOESI_PREF_EVICT_RECEIVE;
int EV_MOD_NMOESI_PREF_EVICT_PROCESS;
int EV_MOD_NMOESI_PREF_EVICT_REPLY;
int EV_MOD_NMOESI_PREF_EVICT_REPLY_RECEIVE;
int EV_MOD_NMOESI_PREF_EVICT_FINISH;

int EV_MOD_NMOESI_WRITE_REQUEST;
int EV_MOD_NMOESI_WRITE_REQUEST_RECEIVE;
int EV_MOD_NMOESI_WRITE_REQUEST_ACTION;
int EV_MOD_NMOESI_WRITE_REQUEST_EXCLUSIVE;
int EV_MOD_NMOESI_WRITE_REQUEST_UPDOWN;
int EV_MOD_NMOESI_WRITE_REQUEST_UPDOWN_FINISH;
int EV_MOD_NMOESI_WRITE_REQUEST_DOWNUP;
int EV_MOD_NMOESI_WRITE_REQUEST_DOWNUP_FINISH;
int EV_MOD_NMOESI_WRITE_REQUEST_REPLY;
int EV_MOD_NMOESI_WRITE_REQUEST_FINISH;

int EV_MOD_NMOESI_READ_REQUEST;
int EV_MOD_NMOESI_READ_REQUEST_RECEIVE;
int EV_MOD_NMOESI_READ_REQUEST_LOCK;
int EV_MOD_NMOESI_READ_REQUEST_ACTION;
int EV_MOD_NMOESI_READ_REQUEST_UPDOWN;
int EV_MOD_NMOESI_READ_REQUEST_UPDOWN_MISS;
int EV_MOD_NMOESI_READ_REQUEST_UPDOWN_FINISH;
int EV_MOD_NMOESI_READ_REQUEST_DOWNUP;
int EV_MOD_NMOESI_READ_REQUEST_DOWNUP_WAIT_FOR_REQS;
int EV_MOD_NMOESI_READ_REQUEST_DOWNUP_FINISH;
int EV_MOD_NMOESI_READ_REQUEST_REPLY;
int EV_MOD_NMOESI_READ_REQUEST_FINISH;

int EV_MOD_NMOESI_INVALIDATE;
int EV_MOD_NMOESI_INVALIDATE_FINISH;

int EV_MOD_NMOESI_PEER_SEND;
int EV_MOD_NMOESI_PEER_RECEIVE;
int EV_MOD_NMOESI_PEER_REPLY_ACK;
int EV_MOD_NMOESI_PEER_FINISH;

int EV_MOD_PREF;
int EV_MOD_PREF_LOCK;
int EV_MOD_PREF_ACTION;
int EV_MOD_PREF_MISS;
int EV_MOD_PREF_UNLOCK;
int EV_MOD_PREF_FINISH;

/* AUXILIARY FUNCTIONS*/
int must_enqueue_prefetch(struct mod_stack_t *stack, int level)
{
	struct mod_t *target_mod;
	
	/* L1 prefetch */
	if(level == 1)
		return stack->mod->prefetch_enabled;

	/* Deeper than L1 prefetch */
	target_mod = stack->target_mod;
	if( target_mod->prefetch_enabled && //El prefetch està habilitat
		!stack->prefetch && //No estem en una cadena de prefetch
		target_mod->kind == mod_kind_cache && //És una cache
		stack->request_dir == mod_request_up_down) //Petició up down
		
		return 1;
	else
		return 0;
}

void enqueue_prefetch_on_miss(struct mod_stack_t *stack, int stride, int level)
{
	int i, stream, num_prefetches;
	struct mod_t *mod;
	struct cache_t *cache;
	struct x86_uop_t * uop; 
	struct stream_buffer_t *sb;

	/* Useful aliases */
	if(level == 1)
		mod = stack->mod;
	else
		mod = stack->target_mod;
	cache = mod->cache;
	num_prefetches = cache->prefetch.aggressivity;
	
	/* Select stream
	 * If there is already a stream with the same tag, replace it
	 * else, replace the last recently used one.
	 * */
	stream = cache_find_stream(cache, (stack->addr + mod->block_size) & ~cache->prefetch.stream_mask);
	if(stream == -1) /* stream_tag not found */
		stream = cache_select_stream(cache);
	sb = &cache->prefetch.streams[stream];

	/* Not enqueue prefetch if there are pending prefetches */
	if(sb->pending_prefetches){
		mem_debug("    Canceled prefetch group at addr=0x%x to stream=%d with stride=0x%x(%d)\n", stack->addr+stride, stream, stride, stride);
		return;
	}

	/* Set stream's transcient tag to indicate the block is being brought */
	sb->stream_transcient_tag = (stack->addr + mod->block_size) & ~cache->prefetch.stream_mask;
	
	/* Set stream's new stride */
	sb->stride = stride;

	/* Debug */
	mem_debug("    Enqueued prefetch group at addr=0x%x to stream=%d with stride=0x%x(%d)\n", stack->addr+stride, stream, stride, stride);

	/* Mark the number of pending prefetches for this stream */
	sb->pending_prefetches += num_prefetches;
	
	/* Insert prefetches */
	for(i=0; i<num_prefetches; i++){
		uop = x86_uop_create();
		uop->phy_addr = stack->addr + (i+1) * stride;
		/* If we reach the end of the stream, AKA the stream_tag changes, insert invalidations */
		if(sb->stream_transcient_tag != (uop->phy_addr & ~cache->prefetch.stream_mask)){
			uop->phy_addr = sb->stream_transcient_tag;
			uop->pref.invalidating = 1;
		}
		uop->core = stack->core;
		uop->thread = stack->thread;
		uop->flags = X86_UINST_MEM;
		uop->pref.kind = GROUP;
		uop->pref.mod = mod;
		uop->pref.dest_stream = stream;
		uop->pref.dest_slot = (sb->head + i) % sb->num_slots; //HEAD
		
		if(level == 1){
			x86_pq_insert(uop);
		} else {
			linked_list_out(stack->target_mod->pq);
			linked_list_insert(stack->target_mod->pq, uop);
		}
	}

	/* Reset tail */
	sb->tail = sb->head; //TAIL
		
	/* Statistics */
	mod->group_prefetches++;

	/* Update next address to be fetched */
	sb->next_address = stack->addr + (i+1) * stride;
}

void enqueue_prefetch_on_hit(struct mod_stack_t *stack, int level)
{
	struct mod_t *mod;
	struct cache_t *cache;
	struct x86_uop_t *uop; 
	struct stream_buffer_t *sb; 
	
	/* Aliases */
	if(level == 1)
		mod = stack->mod;
	else
		mod = stack->target_mod;
	cache = mod->cache;
	sb = &mod->cache->prefetch.streams[stack->pref_stream];

	/* Don't prefetch if next_address is not in the stream anymore */
	if(sb->stream_transcient_tag != (sb->next_address & ~cache->prefetch.stream_mask) && sb->stream_tag != (sb->next_address & ~cache->prefetch.stream_mask)){
		mod->canceled_prefetches++;
		return;
	}

	uop = x86_uop_create();
	uop->phy_addr = sb->next_address;
	sb->next_address += sb->stride; /* Next address to fetch */
	uop->core = stack->core;
	uop->thread = stack->thread;
	uop->flags = X86_UINST_MEM;
	uop->pref.kind = SINGLE;
	uop->pref.mod = mod;
	uop->pref.dest_stream = stack->pref_stream; //Destination stream
	uop->pref.dest_slot = sb->tail; //TAIL
	
	/* Update tail */
	sb->tail = (stack->pref_slot + 1) % sb->num_slots; //TAIL

	if(level == 1){
		x86_pq_insert(uop);
	} else {
		linked_list_out(stack->target_mod->pq);
		linked_list_insert(stack->target_mod->pq, uop);
	}
	
	/* Statistics */
	mod->single_prefetches++;
}



/* NMOESI Protocol */

void mod_handler_pref(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *new_stack;
	struct mod_t *mod = stack->mod;
	struct cache_t *cache = mod->cache;
	struct stream_buffer_t *sb;

	stack->prefetch = mod->level;

	if (event == EV_MOD_PREF)
	{
	    struct mod_stack_t *master_stack;
		if(stack->pref.kind == SINGLE)
			mem_debug("%lld %lld 0x%x %s single pref\n", esim_cycle, stack->id, stack->addr, mod->name);
		else
			mem_debug("%lld %lld 0x%x %s dest_stream=%d dest_slot=%d group pref\n", esim_cycle, stack->id, stack->addr, mod->name, stack->pref.dest_stream, stack->pref.dest_slot);
			
		mem_trace("mem.new_access name=\"A-%lld\" type=\"pref\" state=\"%s:pref\" addr=0x%x\n", stack->id, mod->name, stack->addr);

		/* Record access */
		mod_access_start(mod, stack, mod_access_prefetch);
		
		/* Invalidate slot if required */
		if(stack->pref.invalidating){
			new_stack = mod_stack_create(stack->id, mod, stack->addr, EV_MOD_PREF_FINISH, stack, stack->core, stack->thread, stack->prefetch);
			new_stack->retry = stack->retry;
			new_stack->pref = stack->pref;
			esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT, new_stack, 0);
			return;
		}

		/* Coalesce access -> finish */
		master_stack = mod_can_coalesce(mod, mod_access_prefetch, stack->addr, stack);
		if (master_stack){
			mem_debug("    %lld will finish due to %lld\n",stack->id, master_stack->id);
			if(stack->pref.kind == GROUP){
				new_stack = mod_stack_create(stack->id, mod, stack->addr, EV_MOD_PREF_FINISH, stack, stack->core, stack->thread, stack->prefetch);
				new_stack->retry = stack->retry;
				new_stack->pref = stack->pref;
				esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT, new_stack, 0);
			} else {
				esim_schedule_event(EV_MOD_PREF_FINISH, stack, 0);
			}
			return;
		}
		
		/* Next event */
		esim_schedule_event(EV_MOD_PREF_LOCK, stack, 0);
		return;
	}

	if (event == EV_MOD_PREF_LOCK)
	{
	    struct mod_stack_t *older_stack;

		mem_debug("  %lld %lld 0x%x %s pref lock\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_lock\"\n", stack->id, mod->name);

		/* If there is any older write, finish */
		older_stack = mod_in_flight_write(mod, stack);
		if (older_stack){
			mem_debug("    %lld will finish due to write %lld\n",stack->id, older_stack->id);
			if(stack->pref.kind == GROUP){
				new_stack = mod_stack_create(stack->id, mod, stack->addr, EV_MOD_PREF_FINISH, stack, stack->core, stack->thread, stack->prefetch);
				new_stack->retry = stack->retry;
				new_stack->pref = stack->pref;
				esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT, new_stack, 0);
			} else {
				esim_schedule_event(EV_MOD_PREF_FINISH, stack, 0);
			}
			return;
		}

		/* If there is any older access to the same address that this access could not
		 * be coalesced with, finish. */
		older_stack = mod_in_flight_address(mod, stack->addr, stack);
		if (older_stack)
		{
			mem_debug("    %lld will finish due to access %lld\n", stack->id, older_stack->id);
			new_stack = mod_stack_create(stack->id, mod, 0, EV_MOD_PREF_FINISH, stack, stack->core, stack->thread, stack->prefetch);
			if(stack->pref.kind == GROUP){
				new_stack = mod_stack_create(stack->id, mod, stack->addr, EV_MOD_PREF_FINISH, stack, stack->core, stack->thread, stack->prefetch);
				new_stack->retry = stack->retry;
				new_stack->pref = stack->pref;
				esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT, new_stack, 0);
			} else {
				esim_schedule_event(EV_MOD_PREF_FINISH, stack, 0);
			}
			return;
		}

		/* Call find and lock */
		new_stack = mod_stack_create(stack->id, mod, stack->addr, EV_MOD_PREF_ACTION, stack,stack->core, stack->thread, stack->prefetch);
		new_stack->blocking = 1; /* Blocking */
		new_stack->read = 1;
		new_stack->retry = stack->retry;
		new_stack->access_kind = mod_access_prefetch;
		new_stack->request_dir = mod_request_up_down;
		new_stack->pref = stack->pref;
		esim_schedule_event(EV_MOD_NMOESI_PREF_FIND_AND_LOCK, new_stack, 0);
		return;
	
	}

	if (event == EV_MOD_PREF_ACTION)
	{
	    //int retry_lat;

		mem_debug("  %lld %lld 0x%x %s pref action\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_action\"\n",
			stack->id, mod->name);

		/* Error locking */
		if (stack->err)
		{
			/*mod->read_retries++;
			retry_lat = mod_get_retry_latency(mod);
			*/
			mem_debug("    no slots avaible, dying\n");
			/*stack->retry = 1;*/
			assert(1==2); //Açò no vull que passe, així que avissam
			esim_schedule_event(EV_MOD_PREF_FINISH, stack, 0);
			return;
		}

		/* Hit */
		if (stack->hit){
			mem_debug("    will finish due to block already in cache\n");
			if(stack->pref.kind == GROUP){
				new_stack = mod_stack_create(stack->id, mod, stack->addr, EV_MOD_PREF_FINISH, stack, stack->core, stack->thread, stack->prefetch);
				new_stack->retry = stack->retry;
				new_stack->pref = stack->pref;
				esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT, new_stack, 0);
			} else {
				esim_schedule_event(EV_MOD_PREF_FINISH, stack, 0);
			}
			return;
		}

		/* Prefetch hit -- Block already in the stream */
		if(stack->prefetch_hit){
			mem_debug("     block is already in the stream ");
			if(stack->pref_slot == stack->pref_slot_origin){
				mem_debug("and in the correct slot\n");
				esim_schedule_event(EV_MOD_PREF_FINISH, stack, 0);
			} else {
				mem_debug("but not in the correct slot\n");
				esim_schedule_event(EV_MOD_PREF_MISS, stack, 0);
			}
			return;
		}

		/* Miss */
		new_stack = mod_stack_create(stack->id, mod, stack->tag,
			EV_MOD_PREF_MISS, stack,stack->core, stack->thread, stack->prefetch);
		new_stack->peer = mod;
		new_stack->target_mod = mod_get_low_mod(mod, stack->tag);
		new_stack->request_dir = mod_request_up_down;
		new_stack->access_kind = mod_access_prefetch;
		esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST, new_stack, 0);
		return;
	
	}

	if (event == EV_MOD_PREF_MISS)
	{
	    int retry_lat;

		mem_debug("  %lld %lld 0x%x %s pref miss\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_miss\"\n",
			stack->id, mod->name);

		/* Error on read request. Unlock block and retry prefetch */
		if (stack->err)
		{
			mod->read_retries++;
			retry_lat = mod_get_retry_latency(mod);
			dir_pref_entry_unlock(mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
			mod->cache->prefetch.streams[stack->pref_stream].count--; //COUNT
			mem_debug("    pref lock error (while eviction?), retrying in %d cycles\n", retry_lat);
			stack->retry = 1;
			esim_schedule_event(EV_MOD_PREF_LOCK, stack, retry_lat);
			return;
		}

		cache_get_pref_block_data(cache, stack->pref_stream, stack->pref_slot, NULL, &stack->state); //SLOT
		assert(!stack->state);

		/* Store block */
		cache_set_pref_block(cache, stack->pref_stream, stack->pref_slot, stack->tag, stack->shared ? cache_block_shared : cache_block_exclusive); //SLOT
		
		/* If block comes from another slot in the same stream invalidate the original one */
		if(stack->prefetch_hit)
			cache_set_pref_block(cache, stack->pref_stream_origin, stack->pref_slot_origin, 0, cache_block_invalid); //SLOT
		
		/* Continue */
		esim_schedule_event(EV_MOD_PREF_UNLOCK, stack, 0);
		return;
	}

	if (event == EV_MOD_PREF_UNLOCK)
	{
	    mem_debug("  %lld %lld 0x%x %s pref unlock\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_unlock\"\n",
			stack->id, mod->name);

		/* Unlock directory entry */
		dir_pref_entry_unlock(mod->dir, stack->pref_stream, stack->pref_slot); //SLOT

		/* Block was in the stream */
		if(stack->prefetch_hit)
			dir_pref_entry_unlock(mod->dir, stack->pref_stream_origin, stack->pref_slot_origin); //SLOT
		
		/* Statitistics */
		mod->completed_prefetches++;
		
		/* The last prefetch (or invalidation) of a prefetch group sets the stream tag */
		if(stack->pref.kind == GROUP){
			sb = &cache->prefetch.streams[stack->pref_stream];
			assert(stack->pref_stream >= 0 && stack->pref_stream < cache->prefetch.num_streams);
			assert(stack->pref_slot >= 0 && stack->pref_slot < cache->prefetch.aggressivity);
			assert(sb->pending_prefetches > 0);
			if(sb->pending_prefetches == 1){
				sb->stream_tag = stack->addr & ~cache->prefetch.stream_mask;
				assert(sb->stream_tag == sb->stream_transcient_tag);
			}
			sb->pending_prefetches--;
		}

		/* Continue */
		esim_schedule_event(EV_MOD_PREF_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_PREF_FINISH)
	{
	    mem_debug("%lld %lld 0x%x %s pref finish\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_finish\"\n",
			stack->id, mod->name);
		mem_trace("mem.end_access name=\"A-%lld\"\n",
			stack->id);

		/* Increment witness variable */
        if (stack->witness_ptr) {
            (*stack->witness_ptr)++;
		}

		/* Return event queue element into event queue */
		if (stack->event_queue && stack->event_queue_item)
			linked_list_add(stack->event_queue, stack->event_queue_item);

		/* Finish access */
		mod_access_finish(mod, stack);

		/* Return */
		mod_stack_return(stack);
		return;
	}

	abort();
}

/* NMOESI Protocol */

void mod_handler_nmoesi_load(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;
	struct cache_t *cache = mod->cache;

	if (event == EV_MOD_NMOESI_LOAD)
	{
		struct mod_stack_t *master_stack;

		mem_debug("%lld %lld 0x%x %s load\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.new_access name=\"A-%lld\" type=\"load\" state=\"%s:load\" addr=0x%x\n", stack->id, mod->name, stack->addr);

		/* Record access */
		mod_access_start(mod, stack, mod_access_load);
		
		/* Add access to stride detector and record if there is a stride */
		if(mod->prefetch_enabled)
			stack->stride = cache_detect_stride(mod->cache, stack->addr);

		/* Coalesce access with LOADS accessing same block */
		master_stack = mod_can_coalesce(mod, mod_access_load, stack->addr, stack);
		if (master_stack)
		{
			mod->reads++;
			mod_coalesce(mod, master_stack, stack);
			mod_stack_wait_in_stack(stack, master_stack, EV_MOD_NMOESI_LOAD_FINISH);
			return;
		}

		/* Next event */
		esim_schedule_event(EV_MOD_NMOESI_LOAD_LOCK, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_LOAD_LOCK)
	{
		struct mod_stack_t *older_stack;

		mem_debug("  %lld %lld 0x%x %s load lock\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:load_lock\"\n", stack->id, mod->name);

		/* If there is any older write, wait for it */
		older_stack = mod_in_flight_write(mod, stack);
		if (older_stack)
		{
			mem_debug("    %lld wait for write %lld\n", stack->id, older_stack->id);
			mod_stack_wait_in_stack(stack, older_stack, EV_MOD_NMOESI_LOAD_LOCK);
			return;
		}

		/* If there is any older access to the same address that this access could not
		 * be coalesced with, wait for it. */
		older_stack = mod_in_flight_address(mod, stack->addr, stack);
		if (older_stack && !older_stack->prefetch)
		{
			mem_debug("    %lld wait for access %lld\n", stack->id, older_stack->id);
			mod_stack_wait_in_stack(stack, older_stack, EV_MOD_NMOESI_LOAD_LOCK);
			return;
		}

		/* Call find and lock */
		new_stack = mod_stack_create(stack->id, mod, stack->addr, EV_MOD_NMOESI_LOAD_ACTION, stack,stack->core, stack->thread, stack->prefetch);
		new_stack->blocking = 1;
		new_stack->read = 1;
		new_stack->retry = stack->retry;
		new_stack->access_kind = mod_access_load;
		new_stack->request_dir = mod_request_up_down;
		esim_schedule_event(EV_MOD_NMOESI_FIND_AND_LOCK, new_stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_LOAD_ACTION)
	{
		int retry_lat;

		mem_debug("  %lld %lld 0x%x %s load action\n", esim_cycle, stack->id,stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:load_action\"\n", stack->id, mod->name);

		/* Error locking */
		if (stack->err)
		{
			mod->read_retries++;
			retry_lat = mod_get_retry_latency(mod);
			mem_debug("    lock error, retrying in %d cycles\n", retry_lat);
			stack->retry = 1;
			esim_schedule_event(EV_MOD_NMOESI_LOAD_LOCK, stack, retry_lat);
			return;
		}

		/* Hit */
		if (stack->state){	
			esim_schedule_event(EV_MOD_NMOESI_LOAD_UNLOCK, stack, 0);
			return;
		}

		/* Enqueue prefetch(es) */
		if(!stack->state && must_enqueue_prefetch(stack, mod->level)){
			if(stack->prefetch_hit){
				if(stack->sequential_hit){
					/* Prefetch only one block */
					assert(stack->pref_stream>=0 && stack->pref_stream<cache->prefetch.num_streams);
					assert(stack->pref_slot>=0 && stack->pref_slot<cache->prefetch.aggressivity);
					enqueue_prefetch_on_hit(stack, mod->level);
				}
			} else { 
				/* Fill all the stream buffer if a stride is detected */
				assert(stack->stride != -1);
				if(stack->stride)
					enqueue_prefetch_on_miss(stack, stack->stride, mod->level);
			}
		}

		/* Prefetch hit */
		if (stack->prefetch_hit)
		{	
			esim_schedule_event(EV_MOD_NMOESI_LOAD_MISS, stack, 0);
			return;
		}

		/* Miss */
		new_stack = mod_stack_create(stack->id, mod, stack->tag, EV_MOD_NMOESI_LOAD_MISS, stack,stack->core, stack->thread, stack->prefetch);
		new_stack->peer = mod;
		new_stack->target_mod = mod_get_low_mod(mod, stack->tag);
		new_stack->request_dir = mod_request_up_down;
		new_stack->access_kind = mod_access_load;
		esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST, new_stack, 0);
		
		return;
	}

	if (event == EV_MOD_NMOESI_LOAD_MISS)
	{
		int retry_lat;
		struct stream_buffer_t *sb; 

		mem_debug("  %lld %lld 0x%x %s load miss\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:load_miss\"\n", stack->id, mod->name);
        
		/* Error on read request. Unlock block and retry load. */
		if (stack->err)
		{
			mod->read_retries++;
			retry_lat = mod_get_retry_latency(mod);
			dir_entry_unlock(mod->dir, stack->set, stack->way);
			mem_debug("    lock error, retrying in %d cycles\n", retry_lat);
			stack->retry = 1;
			esim_schedule_event(EV_MOD_NMOESI_LOAD_LOCK, stack, retry_lat);
			return;
		}
		
		if(stack->prefetch_hit){
			int tag;
			cache_get_pref_block_data(cache, stack->pref_stream, stack->pref_slot, &tag, &stack->state);//SLOT*
			assert(stack->tag == tag);
			assert(stack->state);

			/* Portem el bloc del buffer a la cache */
			cache_set_block(cache, stack->set, stack->way, tag, stack->state, 0);

			/* Free buffer entry */
			cache_set_pref_block(cache, stack->pref_stream, stack->pref_slot, -1, cache_block_invalid); //SLOT*
			sb = &cache->prefetch.streams[stack->pref_stream];
			sb->count--; //COUNT*
		}else{
			/* Set block state to excl/shared depending on return var 'shared'.
		 	* Also set the tag of the block. */
			cache_set_block(cache, stack->set, stack->way, stack->tag, stack->shared ? cache_block_shared : cache_block_exclusive, 0);
		}

		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_LOAD_UNLOCK, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_LOAD_UNLOCK)
	{
		mem_debug("  %lld %lld 0x%x %s load unlock\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:load_unlock\"\n",
			stack->id, mod->name);	

		/* Unlock directory entry */
		dir_entry_unlock(mod->dir, stack->set, stack->way);
		if(stack->prefetch_hit)
			dir_pref_entry_unlock(mod->dir, stack->pref_stream, stack->pref_slot); //SLOT
		
		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_LOAD_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_LOAD_FINISH)
	{
		mem_debug("%lld %lld 0x%x %s load finish\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:load_finish\"\n",
			stack->id, mod->name);
		mem_trace("mem.end_access name=\"A-%lld\"\n",
			stack->id);

		/* Increment witness variable */
		if (stack->witness_ptr) {
			(*stack->witness_ptr)++;
		}

		/* Return event queue element into event queue */
		if (stack->event_queue && stack->event_queue_item)
			linked_list_add(stack->event_queue, stack->event_queue_item);

		/* Finish access */
		mod_access_finish(mod, stack);

		/* Return */
		mod_stack_return(stack);
		return;
	}

	abort();
}


void mod_handler_nmoesi_store(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;


	if (event == EV_MOD_NMOESI_STORE)
	{
		struct mod_stack_t *master_stack;

		mem_debug("%lld %lld 0x%x %s store\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.new_access name=\"A-%lld\" type=\"store\" "
			"state=\"%s:store\" addr=0x%x\n",
			stack->id, mod->name, stack->addr);

		/* Record access */
		mod_access_start(mod, stack, mod_access_store);
		
		/* Add access to stride detector and record if there is a stride */
		if(mod->prefetch_enabled)
			stack->stride = cache_detect_stride(mod->cache, stack->addr);

		/* Coalesce access */
		master_stack = mod_can_coalesce(mod, mod_access_store, stack->addr, stack);
		if (master_stack)
		{
			mod->writes++;
			mod_coalesce(mod, master_stack, stack);
			mod_stack_wait_in_stack(stack, master_stack, EV_MOD_NMOESI_STORE_FINISH);

			/* Increment witness variable */
			if (stack->witness_ptr)
				(*stack->witness_ptr)++;

			return;
		}

		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_STORE_LOCK, stack, 0);
		return;
	}


	if (event == EV_MOD_NMOESI_STORE_LOCK)
	{
		struct mod_stack_t *older_stack;

		mem_debug("  %lld %lld 0x%x %s store lock\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:store_lock\"\n",
			stack->id, mod->name);

		/* If there is any older access, wait for it */
		older_stack = stack->access_list_prev;
		if (older_stack)
		{
			mem_debug("    %lld wait for access %lld\n",
				stack->id, older_stack->id);
			mod_stack_wait_in_stack(stack, older_stack, EV_MOD_NMOESI_STORE_LOCK);
			return;
		}

		/* Call find and lock */
		new_stack = mod_stack_create(stack->id, mod, stack->addr,
			EV_MOD_NMOESI_STORE_ACTION, stack,stack->core, stack->thread, stack->prefetch);
		new_stack->blocking = 1;
		new_stack->read = 0;
		new_stack->retry = stack->retry;
		new_stack->witness_ptr = stack->witness_ptr;
		new_stack->access_kind = mod_access_store;
		new_stack->request_dir = mod_request_up_down;
		esim_schedule_event(EV_MOD_NMOESI_FIND_AND_LOCK, new_stack, 0);

		/* Set witness variable to NULL so that retries from the same
		 * stack do not increment it multiple times */
		stack->witness_ptr = NULL;

		return;
	}

	if (event == EV_MOD_NMOESI_STORE_ACTION)
	{
		int retry_lat;

		mem_debug("  %lld %lld 0x%x %s store action\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:store_action\"\n",
			stack->id, mod->name);

		/* Error locking */
		if (stack->err)
		{
			mod->write_retries++;
			retry_lat = mod_get_retry_latency(mod);
			mem_debug("    lock error, retrying in %d cycles\n", retry_lat);
			stack->retry = 1;
			esim_schedule_event(EV_MOD_NMOESI_STORE_LOCK, stack, retry_lat);
			return;
		}
		
		/* Set state if prefetch hit */
		if(stack->prefetch_hit)
		{
			cache_get_pref_block_data(mod->cache, stack->pref_stream, stack->pref_slot, NULL, &stack->state); //SLOT*
		}

		/* Hit - state=M/E */
		if (stack->state == cache_block_modified ||
			stack->state == cache_block_exclusive)
		{
			esim_schedule_event(EV_MOD_NMOESI_STORE_UNLOCK, stack, 0);
			return;
		}
		
		/* Miss - state=O/S/I/N */
		new_stack = mod_stack_create(stack->id, mod, stack->tag,
			EV_MOD_NMOESI_STORE_UNLOCK, stack,stack->core, stack->thread, stack->prefetch);
		new_stack->peer = mod;
		new_stack->target_mod = mod_get_low_mod(mod, stack->tag);
		new_stack->request_dir = mod_request_up_down;
		new_stack->access_kind = mod_access_store;
		esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST, new_stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_STORE_UNLOCK)
	{
		int retry_lat;

		mem_debug("  %lld %lld 0x%x %s store unlock\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:store_unlock\"\n",
			stack->id, mod->name);

		/* Error in write request, unlock block and retry store. */
		if (stack->err)
		{
			mod->write_retries++;
			retry_lat = mod_get_retry_latency(mod);
			dir_entry_unlock(mod->dir, stack->set, stack->way);
			mem_debug("    lock error, retrying in %d cycles\n", retry_lat);
			stack->retry = 1;
			esim_schedule_event(EV_MOD_NMOESI_STORE_LOCK, stack, retry_lat);
			return;
		}

		/* Update tag/state and unlock */
		cache_set_block(mod->cache, stack->set, stack->way,
			stack->tag, cache_block_modified, 0);
		dir_entry_unlock(mod->dir, stack->set, stack->way);
		
		/* If prefetch hit, unlock dir entry and free buffer slot */
		if(stack->prefetch_hit){
			dir_pref_entry_unlock(mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
			cache_set_pref_block(mod->cache, stack->pref_stream, stack->pref_slot, -1, cache_block_invalid); //SLOT*
			struct stream_buffer_t *sb = &mod->cache->prefetch.streams[stack->pref_stream];
			sb->count--; //COUNT*
			if(stack->sequential_hit) /* Hit in head */
				sb->head = (sb->head + 1) % sb->num_slots; //HEAD
		}

		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_STORE_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_STORE_FINISH)
	{
		mem_debug("%lld %lld 0x%x %s store finish\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:store_finish\"\n",
			stack->id, mod->name);
		mem_trace("mem.end_access name=\"A-%lld\"\n",
			stack->id);

		/* Return event queue element into event queue */
		if (stack->event_queue && stack->event_queue_item)
			linked_list_add(stack->event_queue, stack->event_queue_item);

		/* Finish access */
		mod_access_finish(mod, stack);

		/* Return */
		mod_stack_return(stack);
		return;
	}

	abort();
}

void mod_handler_nmoesi_nc_store(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;


	if (event == EV_MOD_NMOESI_NC_STORE)
	{
		struct mod_stack_t *master_stack;

		mem_debug("%lld %lld 0x%x %s nc store\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.new_access name=\"A-%lld\" type=\"nc_store\" "
			"state=\"%s:nc store\" addr=0x%x\n", stack->id, mod->name, stack->addr);

		/* Record access */
		mod_access_start(mod, stack, mod_access_nc_store);

		/* Coalesce access */
		master_stack = mod_can_coalesce(mod, mod_access_nc_store, stack->addr, stack);
		if (master_stack)
		{
			/* FIXME change to NC writes */
			mod->reads++;
			mod_coalesce(mod, master_stack, stack);
			mod_stack_wait_in_stack(stack, master_stack, EV_MOD_NMOESI_NC_STORE_FINISH);
			return;
		}

		/* Next event */
		esim_schedule_event(EV_MOD_NMOESI_NC_STORE_LOCK, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_NC_STORE_LOCK)
	{
		struct mod_stack_t *older_stack;

		mem_debug("  %lld %lld 0x%x %s nc store lock\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:nc_store_lock\"\n",
			stack->id, mod->name);

		/* If there is any older write, wait for it */
		older_stack = mod_in_flight_write(mod, stack);
		if (older_stack)
		{
			mem_debug("    %lld wait for write %lld\n",
				stack->id, older_stack->id);
			mod_stack_wait_in_stack(stack, older_stack, EV_MOD_NMOESI_NC_STORE_LOCK);
			return;
		}

		/* If there is any older access to the same address that this access could not
		 * be coalesced with, wait for it. */
		older_stack = mod_in_flight_address(mod, stack->addr, stack);
		if (older_stack)
		{
			mem_debug("    %lld wait for access %lld\n",
				stack->id, older_stack->id);
			mod_stack_wait_in_stack(stack, older_stack, EV_MOD_NMOESI_NC_STORE_LOCK);
			return;
		}

		/* Call find and lock */
		new_stack = mod_stack_create(stack->id, mod, stack->addr,
			EV_MOD_NMOESI_NC_STORE_WRITEBACK, stack, stack->core, stack->thread, stack->prefetch);
		new_stack->blocking = 1;
		/* FIXME */
		new_stack->read = 1;
		new_stack->retry = stack->retry;
		new_stack->access_kind = mod_access_store;
		esim_schedule_event(EV_MOD_NMOESI_FIND_AND_LOCK, new_stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_NC_STORE_WRITEBACK)
	{
		int retry_lat;

		mem_debug("  %lld %lld 0x%x %s nc store writeback\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:nc_store_writeback\"\n",
			stack->id, mod->name);

		/* Error locking */
		if (stack->err)
		{
			/* FIXME */
			mod->read_retries++;
			retry_lat = mod_get_retry_latency(mod);
			mem_debug("    lock error, retrying in %d cycles\n", retry_lat);
			stack->retry = 1;
			esim_schedule_event(EV_MOD_NMOESI_NC_STORE_LOCK, stack, retry_lat);
			return;
		}

		/* If the block has modified data, evict it so that the lower-level cache will
		 * have the latest copy */
		if (stack->state == cache_block_modified || stack->state == cache_block_owned)
		{
			stack->eviction = 1;
			new_stack = mod_stack_create(stack->id, mod, 0,
				EV_MOD_NMOESI_NC_STORE_ACTION, stack, stack->core, stack->thread, stack->prefetch);
			new_stack->set = stack->set;
			new_stack->way = stack->way;
			esim_schedule_event(EV_MOD_NMOESI_EVICT, new_stack, 0);
			return;
		}

		esim_schedule_event(EV_MOD_NMOESI_NC_STORE_ACTION, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_NC_STORE_ACTION)
	{
		int retry_lat;

		mem_debug("  %lld %lld 0x%x %s nc store action\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:nc_store_action\"\n",
			stack->id, mod->name);

		/* Error locking */
		if (stack->err)
		{
			/* FIXME */
			mod->read_retries++;
			retry_lat = mod_get_retry_latency(mod);
			mem_debug("    lock error, retrying in %d cycles\n", retry_lat);
			stack->retry = 1;
			esim_schedule_event(EV_MOD_NMOESI_NC_STORE_LOCK, stack, retry_lat);
			return;
		}

		/* N/E/S are hit */
		if (stack->state == cache_block_shared || 
			stack->state == cache_block_noncoherent ||
			stack->state == cache_block_exclusive)
		{
			esim_schedule_event(EV_MOD_NMOESI_NC_STORE_UNLOCK, stack, 0);
			return;
		}

		/* All other states need to call read request */
		new_stack = mod_stack_create(stack->id, mod, stack->tag,
			EV_MOD_NMOESI_NC_STORE_MISS, stack, stack->core, stack->thread, stack->prefetch);
		new_stack->peer = mod;
		new_stack->nc_store = 1;
		new_stack->target_mod = mod_get_low_mod(mod, stack->tag);
		new_stack->request_dir = mod_request_up_down;
		esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST, new_stack, 0);

		return;
	}

	if (event == EV_MOD_NMOESI_NC_STORE_MISS)
	{
		int retry_lat;

		mem_debug("  %lld %lld 0x%x %s nc store miss\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:nc_store_miss\"\n",
			stack->id, mod->name);

		/* Error on read request. Unlock block and retry nc store. */
		if (stack->err)
		{
			/* FIXME */
			mod->read_retries++;
			retry_lat = mod_get_retry_latency(mod);
			dir_entry_unlock(mod->dir, stack->set, stack->way);
			mem_debug("    lock error, retrying in %d cycles\n", retry_lat);
			stack->retry = 1;
			esim_schedule_event(EV_MOD_NMOESI_NC_STORE_LOCK, stack, retry_lat);
			return;
		}

		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_NC_STORE_UNLOCK, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_NC_STORE_UNLOCK)
	{
		mem_debug("  %lld %lld 0x%x %s nc store unlock\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:nc_store_unlock\"\n",
			stack->id, mod->name);

		/* Set block state to excl/shared depending on return var 'shared'.
		 * Also set the tag of the block. */
		cache_set_block(mod->cache, stack->set, stack->way, stack->tag,
			cache_block_noncoherent, 0);

		/* Unlock directory entry */
		dir_entry_unlock(mod->dir, stack->set, stack->way);

		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_NC_STORE_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_NC_STORE_FINISH)
	{
		mem_debug("%lld %lld 0x%x %s nc store finish\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:nc_store_finish\"\n",
			stack->id, mod->name);
		mem_trace("mem.end_access name=\"A-%lld\"\n",
			stack->id);

		/* Increment witness variable */
                if (stack->witness_ptr) {
                        (*stack->witness_ptr)++;
		}

		/* Return event queue element into event queue */
		if (stack->event_queue && stack->event_queue_item)
			linked_list_add(stack->event_queue, stack->event_queue_item);

		/* Finish access */
		mod_access_finish(mod, stack);

		/* Return */
		mod_stack_return(stack);
		return;
	}

	abort();
}

void mod_handler_nmoesi_pref_find_and_lock(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *ret = stack->ret_stack;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;
	struct cache_t *cache = mod->cache;
	struct stream_buffer_t *sb;
	
	assert(stack->request_dir);
	assert(stack->access_kind);

	if (event == EV_MOD_NMOESI_PREF_FIND_AND_LOCK)
	{
		mem_debug("  %lld %lld 0x%x %s pref find and lock (blocking=%d)\n", esim_cycle, stack->id, stack->addr, mod->name, stack->blocking);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_find_and_lock\"\n", stack->id, mod->name);

		/* Default return values */
		ret->err = 0;
		
		/* Get a port */
		mod_lock_port(mod, stack, EV_MOD_NMOESI_PREF_FIND_AND_LOCK_PORT);
		return;
	}

	if (event == EV_MOD_NMOESI_PREF_FIND_AND_LOCK_PORT)
	{
		struct mod_port_t *port = stack->port;
		struct dir_lock_t *dir_lock;
		struct stream_block_t *sblock;
		int slot;

		assert(stack->port);
		mem_debug("  %lld %lld 0x%x %s pref find and lock port\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_find_and_lock_port\"\n", stack->id, mod->name);

		/* Select prefetch stream and slot */
		stack->pref_stream = stack->pref.dest_stream;
		stack->pref_slot = stack->pref.dest_slot;
		
		/* Set parent stack flag expressing that port has already been locked.
		 * This flag is checked by new writes to find out if it is already too
		 * late to coalesce. */
		ret->port_locked = 1;

		/* Search block in cache and stream */
		if(stack->access_kind != mod_access_invalidate){
			sb = &cache->prefetch.streams[stack->pref.dest_stream];
			/* Search in cache */
			stack->hit = mod_find_block(mod, stack->addr, &stack->set, &stack->way, &stack->tag, &stack->state);
			if (stack->hit){
				/* Block in cache */
				mem_debug("    %lld 0x%x %s hit: set=%d, way=%d, state=%s\n", stack->id, stack->tag, mod->name, stack->set, stack->way, map_value(&cache_block_state_map, stack->state));
			} else {
				/* Block is not in cache */
				if(sb->stream_tag == sb->stream_transcient_tag){
					/* Search block in the stream */
					slot = mod_find_block_in_stream(mod, stack->addr, stack->pref_stream);
					if(slot > -1){
						/* Block found */
						if(slot == stack->pref_slot){
							/* Block already in the correct slot. Do nothing. */
							ret->prefetch_hit = 1;
							ret->pref_slot = stack->pref_slot;
							ret->pref_stream = stack->pref_stream;
							ret->pref_slot_origin = slot;
							ret->pref_stream_origin = stack->pref_stream;
							mod_unlock_port(mod, port, stack);
							mod_stack_return(stack);
							return;
						} else {
							/* Block is not in the correct slot */
							dir_lock = dir_pref_lock_get(mod->dir, sb->stream, slot); //SLOT
							if (!dir_lock->lock){
								/* Lock it if is unlocked */
								dir_pref_entry_lock(mod->dir, sb->stream, slot, EV_MOD_NMOESI_PREF_FIND_AND_LOCK, stack); //SLOT
								/* Set transcient tag -- The block will be removed from this slot and reallocated */
								sblock = cache_get_pref_block(cache, sb->stream, slot); 
								sblock->transient_tag = 0;
								/* Mark this circumstance with a prefetch hit */
								ret->prefetch_hit = 1;
								ret->pref_slot_origin = slot;
								ret->pref_stream_origin = stack->pref_stream;
							}
						}
					}
				}
			}
		}

		/* Statistics */
		mod->accesses++;
		if (stack->hit)	
			mod->hits++;
		if (stack->read){
			mod->reads++;
			mod->effective_reads++;
			stack->blocking ? mod->blocking_reads++ : mod->non_blocking_reads++;
			if (stack->hit)
				mod->read_hits++;
		}
		if (!stack->retry){
			mod->no_retry_accesses++;
			if (stack->hit)
				mod->no_retry_hits++;
			if (stack->read){
				mod->no_retry_reads++;
				if (stack->hit)
					mod->no_retry_read_hits++;
			}
		}
		
		/* Cache hit */
		if(stack->hit){
			ret->hit = stack->hit;
			mod_unlock_port(mod, port, stack);
			mod_stack_return(stack);
			return;
		}

		/* Assertions */
		sb = &cache->prefetch.streams[stack->pref_stream];
		assert(stack->pref_slot>=0 && stack->pref_slot<sb->num_slots);
		assert(stack->pref_stream>=0 && stack->pref_stream<cache->prefetch.num_streams);

		mem_debug("    %lld 0x%x %s stream=%d, slot=%d, state=%s\n", stack->id, stack->tag, mod->name, stack->pref_stream, stack->pref_slot, map_value(&cache_block_state_map, stack->state));
		
		/* Lock prefetch entry */
		dir_lock = dir_pref_lock_get(mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
		if (dir_lock->lock && !stack->blocking){
			struct stream_block_t * block = cache_get_pref_block(cache, stack->pref_stream, stack->pref_slot); //SLOT*
			mem_debug("    %lld 0x%x %s pref_stream=%d pref_slot=%d containing 0x%x (0x%x) already locked by stack %lld, retrying...\n", stack->id, stack->tag, mod->name, stack->pref_stream, stack->pref_slot, block->tag, block->transient_tag, dir_lock->stack_id);
			ret->err = 1;
			mod_unlock_port(mod, port, stack);
			mod_stack_return(stack);
			return;
		}
		if (!dir_pref_entry_lock(mod->dir, stack->pref_stream, stack->pref_slot, EV_MOD_NMOESI_PREF_FIND_AND_LOCK, stack)){ //SLOT*
			mem_debug("    %lld 0x%x %s pref_stream=%d pref_slot=%d already locked by stack %lld, waiting...\n", stack->id, stack->tag, mod->name, stack->pref_stream, stack->pref_slot, dir_lock->stack_id);
			mod_unlock_port(mod, port, stack);
			return;
		}

		/* Entry is locked. Record the transient tag so that a subsequent lookup
		 * detects that the block is being brought. */
		struct stream_block_t *block = cache_get_pref_block(cache, stack->pref_stream, stack->pref_slot); //SLOT*
		block->transient_tag = stack->tag;
		
		/* Update count */
		if(stack->access_kind != mod_access_invalidate){
			assert(sb->stream == stack->pref_stream);
			if(!block->state)
				sb->count++; //COUNT
		}
		
		/* Update LRU stream */
		cache_access_stream(mod->cache, stack->pref_stream);

		/* Access latency */
		esim_schedule_event(EV_MOD_NMOESI_PREF_FIND_AND_LOCK_ACTION, stack, mod->latency);
		return;
	}

	if (event == EV_MOD_NMOESI_PREF_FIND_AND_LOCK_ACTION)
	{
		struct mod_port_t *port = stack->port;

		assert(port);
		mem_debug("  %lld %lld 0x%x %s pref find and lock action\n", esim_cycle, stack->id, stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_find_and_lock_action\"\n", stack->id, mod->name);

		/* Release port */
		mod_unlock_port(mod, port, stack);

		/* Prefetch buffer slot eviction */
		struct stream_block_t *block = cache_get_pref_block(cache, stack->pref_stream, stack->pref_slot); //SLOT*
		if(block->state){
			assert(stack->request_dir == mod_request_up_down);
			assert(!stack->hit && !stack->prefetch_hit);

			stack->pref_eviction = 1;
			new_stack = mod_stack_create(stack->id, mod, 0, EV_MOD_NMOESI_PREF_FIND_AND_LOCK_FINISH, stack,stack->core, stack->thread, stack->prefetch);
			new_stack->pref_stream = stack->pref_stream;
			new_stack->pref_slot = stack->pref_slot;
			new_stack->access_kind = stack->access_kind;
			esim_schedule_event(EV_MOD_NMOESI_PREF_EVICT, new_stack, 0);
			return;
		}

		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_PREF_FIND_AND_LOCK_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_PREF_FIND_AND_LOCK_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s pref find and lock finish (err=%d)\n", esim_cycle, stack->id, stack->tag, mod->name, stack->err);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_find_and_lock_finish\"\n", stack->id, mod->name);

		/* If evict produced err, return err */
		if (stack->err && stack->pref_eviction){
			cache_get_pref_block_data(mod->cache, stack->pref_stream, stack->pref_slot, NULL, &stack->state); //SLOT*
			assert(stack->state);
			ret->err = 1;
			dir_pref_entry_unlock(mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
			mod_stack_return(stack);
			return;
		}
		assert(!stack->err);

		/* Eviction */
		if(stack->pref_eviction){
			//mod->evictions++;
			cache_get_pref_block_data(mod->cache, stack->pref_stream, stack->pref_slot, NULL, &stack->state); //SLOT
			assert(!stack->state);
		}

		/* Return */
		ret->err = 0;
		ret->state = stack->state;
		ret->tag = stack->tag;
		ret->pref_stream = stack->pref_stream;
		ret->pref_slot = stack->pref_slot;
		mod_stack_return(stack);
		return;
	}

	abort();
}

void mod_handler_nmoesi_find_and_lock(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *ret = stack->ret_stack;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;
	struct cache_t *cache = mod->cache;

	assert(stack->request_dir);
	
	if (event == EV_MOD_NMOESI_FIND_AND_LOCK)
	{
		mem_debug("  %lld %lld 0x%x %s find and lock (blocking=%d)\n", esim_cycle, stack->id, stack->addr, mod->name, stack->blocking);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:find_and_lock\"\n", stack->id, mod->name);
		
		/* Una load que ha fet wait en un slot no pot tenir en la llista d'espera una altra load */
		assert(stack->pref_stream == -1 || stack->pref_slot == -1 || !stack->dir_lock_next || stack->dir_lock_next->access_kind == mod_access_prefetch);

		/* Default return values */
		ret->err = 0;

		/* If this stack has already been assigned a way, keep using it */
		stack->way = ret->way;
		
		/* Get a port */
		mod_lock_port(mod, stack, EV_MOD_NMOESI_FIND_AND_LOCK_PORT);
		return;
	}

	if (event == EV_MOD_NMOESI_FIND_AND_LOCK_PORT)
	{
		struct mod_port_t *port = stack->port;
		struct dir_lock_t *dir_lock;

		assert(stack->port);
		mem_debug("  %lld %lld 0x%x %s find and lock port\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:find_and_lock_port\"\n", stack->id, mod->name);

		/* Set parent stack flag expressing that port has already been locked.
		 * This flag is checked by new writes to find out if it is already too
		 * late to coalesce. */
		ret->port_locked = 1;

		/* Look for block */
		stack->hit = mod_find_block(mod, stack->addr, &stack->set, &stack->way, &stack->tag, &stack->state);
		if (stack->hit)
			mem_debug("    %lld 0x%x %s hit: set=%d, way=%d, state=%s\n", stack->id, stack->tag, mod->name, stack->set, stack->way, map_value(&cache_block_state_map, stack->state));
		
		/* Look for block in prefetch stream buffers */
		stack->prefetch_hit = 0;
		if(!stack->hit && mod->prefetch_enabled){
			int result = mod_find_pref_block(mod, stack->addr, &stack->pref_stream, &stack->pref_slot);
			if(result){
				struct stream_buffer_t *sb = &cache->prefetch.streams[stack->pref_stream];
				stack->prefetch_hit = 1;
				if(result == 1) /* {0 miss, 1 head hit, 2 middle stream hit} */
					stack->sequential_hit = 1;

				mem_debug("    %lld 0x%x %s prefetch_hit=%d stream=%d slot=%d in head=%d up_down=%d\n", stack->id, stack->tag, mod->name, stack->prefetch_hit, stack->pref_stream, stack->pref_slot, stack->sequential_hit, stack->request_dir == mod_request_up_down);
				
				/* Print debug info */
				int i, slot, count;
				struct stream_block_t *block;
				struct dir_lock_t *dir_lock;
				count = sb->head + sb->num_slots;
				for(i = sb->head; i < count; i++){
					slot = i % sb->num_slots;
					block = cache_get_pref_block(cache, sb->stream, slot);
					dir_lock = dir_pref_lock_get(mod->dir, sb->stream, slot);
					mem_debug("\t\t{slot=%d, tag=0x%x, transient_tag=0x%x, state=%s, locked=%d}\n", slot, block->tag, block->transient_tag, map_value(&cache_block_state_map, block->state), dir_lock->lock);
				}

				if(stack->request_dir == mod_request_up_down)
					sb->head = (stack->pref_slot + 1) % sb->num_slots; //HEAD
				
				/* Statistics */
				if(stack->request_dir == mod_request_up_down){
					if(stack->sequential_hit)
						mod->up_down_head_hits++;
					mod->up_down_hits++;
				}else{
					if(stack->read)
						mod->down_up_read_hits++;
					else
						mod->down_up_write_hits++;
				}
			}
		}

		/* Any down up request must be a hit or a prefetch hit */
		assert(stack->hit || stack->prefetch_hit || stack->request_dir == mod_request_up_down);
		/* No request can't be a hit and a prefetch hit at same time */
		assert(!stack->hit || !stack->prefetch_hit);
		
		/* Statistics */
		mod->accesses++;
		if (stack->hit)	
			mod->hits++;
		if (stack->read){
			mod->reads++;
			mod->effective_reads++;
			stack->blocking ? mod->blocking_reads++ : mod->non_blocking_reads++;
			if (stack->hit)
				mod->read_hits++;
		}else{
			mod->writes++;
			mod->effective_writes++;
			stack->blocking ? mod->blocking_writes++ : mod->non_blocking_writes++;

			/* Increment witness variable when port is locked */
			if (stack->witness_ptr)	{
				(*stack->witness_ptr)++;
				stack->witness_ptr = NULL;
			}

			if (stack->hit)
				mod->write_hits++;
		}
		if (!stack->retry){
			mod->no_retry_accesses++;
			if (stack->hit)
				mod->no_retry_hits++;
			if (stack->read){
				mod->no_retry_reads++;
				if (stack->hit)
					mod->no_retry_read_hits++;
			}else{
				mod->no_retry_writes++;
				if (stack->hit)
					mod->no_retry_write_hits++;
			}
		}
		

		/* On miss */
		if(!stack->hit){
			/* Select victim way */
			if (stack->way < 0)
				stack->way = cache_replace_block(mod->cache, stack->set);	
		}
		assert(stack->way >= 0);
		
		/* If prefetch hit lock prefetch entry */
		if(stack->prefetch_hit)
		{
			dir_lock = dir_pref_lock_get(mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
			if (dir_lock->lock && !stack->blocking)
			{
				struct stream_block_t * block = cache_get_pref_block(cache,stack->pref_stream, stack->pref_slot); //SLOT*
				mem_debug("    %lld 0x%x %s pref_stream %d pref_slot %d containing 0x%x (0x%x) already locked by stack %lld, retrying...",stack->id, stack->tag, mod->name, stack->pref_stream, stack->pref_slot, block->tag, block->transient_tag, dir_lock->stack_id);

				ret->err = 1;
				mod_unlock_port(mod, port, stack);
				mod_stack_return(stack);
				return;
			}
			if (!dir_pref_entry_lock(mod->dir, stack->pref_stream, stack->pref_slot, //SLOT*
				EV_MOD_NMOESI_FIND_AND_LOCK, stack))
			{
				mem_debug("    %lld 0x%x %s pref_stream %d pref_slot %d already locked by stack %lld, waiting...\n", stack->id, stack->tag, mod->name, stack->pref_stream, stack->pref_slot, dir_lock->stack_id);
				mod_unlock_port(mod, port, stack);
				return;
			}
		}

		/* If cache hit or is a up_down request lock cache entry */
		if(stack->hit || stack->request_dir == mod_request_up_down ) 
		{
			dir_lock = dir_lock_get(mod->dir, stack->set, stack->way);
			if (dir_lock->lock && !stack->blocking)
			{
				mem_debug("    %lld 0x%x %s block already locked: set=%d, way=%d by stack %lld\n", stack->id, stack->tag, mod->name, stack->set, stack->way, dir_lock->stack_id);
				ret->err = 1;
				mod_unlock_port(mod, port, stack);
				mod_stack_return(stack);
				return;
			}
			if (!dir_entry_lock(mod->dir, stack->set, stack->way,
				EV_MOD_NMOESI_FIND_AND_LOCK, stack))
			{
				mem_debug("    %lld 0x%x %s block locked at set=%d, way=%d by stack %lld\n", stack->id, stack->tag, mod->name, stack->set, stack->way, dir_lock->stack_id);
				mod_unlock_port(mod, port, stack);
				return;
			}
		}


		/* Miss */
		if (!stack->hit) //TODO: down up
		{
			/* Get victim */
			cache_get_block(mod->cache, stack->set, stack->way, NULL, &stack->state);
			assert(stack->state || !dir_entry_group_shared_or_owned(mod->dir, stack->set, stack->way));
			mem_debug("    %lld 0x%x %s miss -> lru: set=%d, way=%d, state=%s\n",stack->id, stack->tag, mod->name, stack->set, stack->way, map_value(&cache_block_state_map, stack->state));
		}


		/* Entry is locked. Record the transient tag so that a subsequent lookup
		 * detects that the block is being brought.
		 * Also, update LRU counters here. */
		if(stack->request_dir == mod_request_up_down) //VVV
		{
			cache_set_transient_tag(mod->cache, stack->set, stack->way, stack->tag);
			cache_access_block(mod->cache, stack->set, stack->way);
			
			//Si hi ha un prefetch hit en up down el block el marquem com a que l'anem a llevar del buffer i actulitzem LRU
			if(stack->prefetch_hit){
				struct stream_block_t *block = cache_get_pref_block(cache, stack->pref_stream, stack->pref_slot); //SLOT*
				block->transient_tag = cache_block_invalid;
				if(stack->sequential_hit)
					cache_access_stream(cache, stack->pref_stream);
			}
		}
		
		/* Access latency */
		esim_schedule_event(EV_MOD_NMOESI_FIND_AND_LOCK_ACTION, stack, mod->latency);
		return;
	}

	if (event == EV_MOD_NMOESI_FIND_AND_LOCK_ACTION)
	{
		struct mod_port_t *port = stack->port;

		assert(port);
		mem_debug("  %lld %lld 0x%x %s find and lock action\n", esim_cycle, stack->id,
			stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:find_and_lock_action\"\n",
			stack->id, mod->name);

		/* Release port */
		mod_unlock_port(mod, port, stack);

		/* Eviction */
		if (!stack->hit && stack->state && //Valid block
			stack->request_dir == mod_request_up_down) //Up down request
		{
			assert(stack->request_dir == mod_request_up_down);
			stack->eviction = 1;
			new_stack = mod_stack_create(stack->id, mod, 0,
				EV_MOD_NMOESI_FIND_AND_LOCK_FINISH, stack,stack->core,
				stack->thread, stack->prefetch);
			new_stack->set = stack->set;
			new_stack->way = stack->way;
			new_stack->access_kind = stack->access_kind;
			esim_schedule_event(EV_MOD_NMOESI_EVICT, new_stack, 0);
			return;
		}

		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_FIND_AND_LOCK_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_FIND_AND_LOCK_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s find and lock finish (err=%d)\n",
			esim_cycle, stack->id, stack->tag, mod->name, stack->err);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:find_and_lock_finish\"\n",
			stack->id, mod->name);

		/* If evict produced err, return err */
		if (stack->err && stack->eviction)
		{
			cache_get_block(mod->cache, stack->set, stack->way, NULL, &stack->state);
			assert(stack->state);
			ret->err = 1;
			dir_entry_unlock(mod->dir, stack->set, stack->way);
			mod_stack_return(stack);
			return;
		}
		assert(!stack->err);

		/* Eviction */
		if (stack->eviction)
		{
			mod->evictions++;
			cache_get_block(mod->cache, stack->set, stack->way, NULL, &stack->state);
			assert(!stack->state);
		}
		
		/* If this is a main memory, the block is here. A previous miss was just a miss
		 * in the directory. */
		if (mod->kind == mod_kind_main_memory && !stack->state)
		{
			stack->state = cache_block_exclusive;
			cache_set_block(mod->cache, stack->set, stack->way, stack->tag, stack->state, 0);
		}

		/* Return */
		ret->err = 0;
		ret->set = stack->set;
		ret->way = stack->way;
		ret->state = stack->state;
		ret->tag = stack->tag;
		ret->prefetch_hit = stack->prefetch_hit;
		ret->sequential_hit = stack->sequential_hit;
		ret->pref_stream = stack->pref_stream;
		ret->pref_slot = stack->pref_slot;
		mod_stack_return(stack);
		return;
	}

	abort();
}

void mod_handler_nmoesi_evict(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *ret = stack->ret_stack;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;
	struct mod_t *target_mod = stack->target_mod;

	struct dir_t *dir;
	struct dir_entry_t *dir_entry;

	uint32_t dir_entry_tag, z;


	if (event == EV_MOD_NMOESI_EVICT)
	{
		/* Default return value */
		ret->err = 0;

		/* Get block info */
		cache_get_block(mod->cache, stack->set, stack->way, &stack->tag, &stack->state);
		
		assert(stack->state || !dir_entry_group_shared_or_owned(mod->dir,
			stack->set, stack->way));
		
		mem_debug("  %lld %lld 0x%x %s evict (set=%d, way=%d, state=%s)\n",
			esim_cycle, stack->id, stack->tag, mod->name, stack->set, stack->way,
			map_value(&cache_block_state_map, stack->state));
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict\"\n",
			stack->id, mod->name);

		/* Save some data */
		stack->src_set = stack->set;
		stack->src_way = stack->way;
		stack->src_tag = stack->tag;
		stack->target_mod = mod_get_low_mod(mod, stack->tag);

		/* Send write request to all sharers */
		new_stack = mod_stack_create(stack->id, mod, 0, EV_MOD_NMOESI_EVICT_INVALID, stack, stack->core, stack->thread, stack->prefetch);
		new_stack->except_mod = NULL;
		new_stack->set = stack->set;
		new_stack->way = stack->way;
		esim_schedule_event(EV_MOD_NMOESI_INVALIDATE, new_stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_EVICT_INVALID)
	{
		mem_debug("  %lld %lld 0x%x %s evict invalid\n", esim_cycle, stack->id,
			stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict_invalid\"\n",
			stack->id, mod->name);

		/* If module is main memory, we just need to set the block as invalid, 
		 * and finish. */
		if (mod->kind == mod_kind_main_memory)
		{
			cache_set_block(mod->cache, stack->src_set, stack->src_way,
				0, cache_block_invalid, 0);
			esim_schedule_event(EV_MOD_NMOESI_EVICT_FINISH, stack, 0);
			return;
		}

		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_EVICT_ACTION, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_EVICT_ACTION)
	{
		struct mod_t *low_mod;
		struct net_node_t *low_node;
		int msg_size;

		mem_debug("  %lld %lld 0x%x %s evict action\n", esim_cycle, stack->id,
			stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict_action\"\n",
			stack->id, mod->name);

		/* Get low node */
		low_mod = stack->target_mod;
		low_node = low_mod->high_net_node;
		assert(low_mod != mod);
		assert(low_mod == mod_get_low_mod(mod, stack->tag));
		assert(low_node && low_node->user_data == low_mod);
		
		/* State = I */
		if (stack->state == cache_block_invalid)
		{
			esim_schedule_event(EV_MOD_NMOESI_EVICT_FINISH, stack, 0);
			return;
		}

		/* If state is M/O/N, data must be sent to lower level mod */
		if (stack->state == cache_block_modified || stack->state == cache_block_owned ||
			stack->state == cache_block_noncoherent)
		{
			/* Need to transmit data to low module */
			msg_size = 8 + mod->block_size;
			stack->reply = reply_ACK_DATA;
		}
		/* If state is E/S, just an ACK needs to be sent */
		else 
		{
			msg_size = 8;
			stack->reply = reply_ACK;
		}

		/* Send message */
		stack->msg = net_try_send_ev(mod->low_net, mod->low_net_node,
			low_node, msg_size, EV_MOD_NMOESI_EVICT_RECEIVE, stack, event, stack);
		return;
	}

	if (event == EV_MOD_NMOESI_EVICT_RECEIVE)
	{
		mem_debug("  %lld %lld 0x%x %s evict receive\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict_receive\"\n",
			stack->id, target_mod->name);

		/* Receive message */
		net_receive(target_mod->high_net, target_mod->high_net_node, stack->msg);

		/* Find and lock */
		if (stack->state == cache_block_noncoherent)
		{
			new_stack = mod_stack_create(stack->id, target_mod, stack->src_tag,
				EV_MOD_NMOESI_EVICT_PROCESS_NONCOHERENT, stack, stack->core, stack->thread, stack->prefetch);
		}
		else 
		{
			new_stack = mod_stack_create(stack->id, target_mod, stack->src_tag,
				EV_MOD_NMOESI_EVICT_PROCESS, stack, stack->core, stack->thread, stack->prefetch);
		}
		new_stack->blocking = 0;
		new_stack->read = 0;
		new_stack->retry = 0;
		new_stack->request_dir = mod_request_up_down;
		esim_schedule_event(EV_MOD_NMOESI_FIND_AND_LOCK, new_stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_EVICT_PROCESS)
	{
		mem_debug("  %lld %lld 0x%x %s evict process\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict_process\"\n",
			stack->id, target_mod->name);
		
		assert(!stack->prefetch_hit);

		/* Error locking block */
		if (stack->err)
		{
			ret->err = 1;
			esim_schedule_event(EV_MOD_NMOESI_EVICT_REPLY, stack, 0);
			return;
		}

		/* If data was received, set the block to modified */
		if (stack->reply == reply_ACK_DATA)
		{
			cache_set_block(target_mod->cache, stack->set, stack->way, stack->tag,
				cache_block_modified, stack->prefetch);
		}

		/* Remove sharer and owner */
		dir = target_mod->dir;
		for (z = 0; z < dir->zsize; z++)
		{
			/* Skip other subblocks */
			dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
			assert(dir_entry_tag < stack->tag + target_mod->block_size);
			if (dir_entry_tag < stack->src_tag || 
				dir_entry_tag >= stack->src_tag + mod->block_size)
			{
				continue;
			}

			dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
			dir_entry_clear_sharer(dir, stack->set, stack->way, z, 
				mod->low_net_node->index);
			if (dir_entry->owner == mod->low_net_node->index)
			{
				dir_entry_set_owner(dir, stack->set, stack->way, z, 
					DIR_ENTRY_OWNER_NONE);
			}
		}

		/* Unlock the directory entry */
		dir_entry_unlock(dir, stack->set, stack->way);

		esim_schedule_event(EV_MOD_NMOESI_EVICT_REPLY, stack, 0);
		return;

	}

	if (event == EV_MOD_NMOESI_EVICT_PROCESS_NONCOHERENT)
	{
		struct mod_t *owner;

		mem_debug("  %lld %lld 0x%x %s evict process noncoherent\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict_process_noncoherent\"\n",
			stack->id, target_mod->name);

		/* Error locking block */
		if (stack->err)
		{
			ret->err = 1;
			esim_schedule_event(EV_MOD_NMOESI_EVICT_REPLY, stack, 0);
			return;
		}

		/* Remove sharer and owner */
		dir = target_mod->dir;
		for (z = 0; z < dir->zsize; z++)
		{
			/* Skip other subblocks */
			dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
			assert(dir_entry_tag < stack->tag + target_mod->block_size);
			if (dir_entry_tag < stack->src_tag || 
				dir_entry_tag >= stack->src_tag + mod->block_size)
			{
				continue;
			}

			dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
			dir_entry_clear_sharer(dir, stack->set, stack->way, z, 
				mod->low_net_node->index);
			if (dir_entry->owner == mod->low_net_node->index)
			{
				dir_entry_set_owner(dir, stack->set, stack->way, z, 
					DIR_ENTRY_OWNER_NONE);
			}
		}

		/* XXX WHY THE READ REQUEST?! */
		/* Send a read request to any subblock with an owner */
		stack->pending = 1;
		dir = target_mod->dir;
		for (z = 0; z < dir->zsize; z++)
		{
			struct net_node_t *node;

			dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
			assert(dir_entry_tag < stack->tag + target_mod->block_size);
			dir_entry = dir_entry_get(dir, stack->set, stack->way, z);

			/* No owner, so skip */
			if (!DIR_ENTRY_VALID_OWNER(dir_entry))
				continue;

			/* Get owner mod */
			node = list_get(target_mod->high_net->node_list, 
				dir_entry->owner);
			assert(node && node->kind == net_node_end);
			owner = node->user_data;

			/* Not the first sub-block */
			if (dir_entry_tag % owner->block_size)
				continue;

			stack->pending++;
			new_stack = mod_stack_create(stack->id, target_mod, 
				dir_entry_tag, EV_MOD_NMOESI_EVICT_WAIT_FOR_REQS, stack, stack->core, stack->thread, stack->prefetch);
			new_stack->target_mod = owner;
			new_stack->request_dir = mod_request_down_up;
			esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST, new_stack, 0);
		}

		esim_schedule_event(EV_MOD_NMOESI_EVICT_WAIT_FOR_REQS, stack, 0);
		return;

	}

	if (event == EV_MOD_NMOESI_EVICT_WAIT_FOR_REQS)
	{
		/* Ignore while pending requests */
		assert(stack->pending > 0);
		stack->pending--;
		if (stack->pending)
			return;

		mem_debug("  %lld %lld 0x%x %s evict wait for reqs\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict_wait_for_reqs\"\n",
			stack->id, target_mod->name);

		/* FIXME We need to consider stack->err here */

		dir = target_mod->dir;

		/* Set owner to NONE for all directory entries */
		for (z = 0; z < dir->zsize; z++)
		{
			dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
			dir_entry_set_owner(dir, stack->set, stack->way, z, DIR_ENTRY_OWNER_NONE);
		}

		/* If any data is received, set state to M.  Otherwise,
		 * set state to N */
		if (stack->reply == reply_ACK_DATA)
		{
			cache_set_block(target_mod->cache, stack->set, stack->way, 
				stack->tag, cache_block_modified, stack->prefetch);
		}
		else 
		{
			cache_set_block(target_mod->cache, stack->set, stack->way, 
				stack->tag, cache_block_noncoherent, stack->prefetch);
		}

		/* Unlock the directory entry */
		dir_entry_unlock(dir, stack->set, stack->way);

		esim_schedule_event(EV_MOD_NMOESI_EVICT_REPLY, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_EVICT_REPLY)
	{
		mem_debug("  %lld %lld 0x%x %s evict reply\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict_reply\"\n",
			stack->id, target_mod->name);

		/* Send message */
		stack->msg = net_try_send_ev(target_mod->high_net, target_mod->high_net_node,
			mod->low_net_node, 8, EV_MOD_NMOESI_EVICT_REPLY_RECEIVE, stack,
			event, stack);
		return;

	}

	if (event == EV_MOD_NMOESI_EVICT_REPLY_RECEIVE)
	{
		mem_debug("  %lld %lld 0x%x %s evict reply receive\n", esim_cycle, stack->id,
			stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict_reply_receive\"\n",
			stack->id, mod->name);

		/* Receive message */
		net_receive(mod->low_net, mod->low_net_node, stack->msg);

		/* Invalidate block if there was no error. */
		if (!stack->err)
			cache_set_block(mod->cache, stack->src_set, stack->src_way,
				0, cache_block_invalid, 0);

		assert(!dir_entry_group_shared_or_owned(mod->dir, stack->src_set, stack->src_way));
		esim_schedule_event(EV_MOD_NMOESI_EVICT_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_EVICT_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s evict finish\n", esim_cycle, stack->id,
			stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:evict_finish\"\n",
			stack->id, mod->name);

		mod_stack_return(stack);
		return;
	}

	abort();
}



void mod_handler_nmoesi_invalidate_slot(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *new_stack;
	struct mod_t *mod = stack->mod;
	struct cache_t *cache = mod->cache;

	if (event == EV_MOD_NMOESI_INVALIDATE_SLOT)
	{
		//struct mod_stack_t *master_stack;

		mem_debug("%lld %lld 0x%x %s invalidate slot\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.new_access name=\"A-%lld\" type=\"invalidate_slot\" state=\"%s:invalidate_slot\" addr=0x%x\n", stack->id, mod->name, stack->addr);
		
		/* Record access */
		mod_access_start(mod, stack, mod_access_invalidate);
		
		/* No coalesce
		master_stack = mod_can_coalesce(mod, mod_access_invalidate, stack->addr, stack);
		if (master_stack){
			assert(!master_stack->prefetch);
			assert(!master_stack->read);
			assert(master_stack->request_dir == mod_request_up_down);
			esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT_FINISH, stack, 0);
			return;
		}*/

		/* Next event */
		esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT_LOCK, stack, 0);
		return;
	}


	if (event == EV_MOD_NMOESI_INVALIDATE_SLOT_LOCK)
	{
		//struct mod_stack_t *older_stack;

		mem_debug("  %lld %lld 0x%x %s invalidate slot lock\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:invalidate_slot_lock\"\n", stack->id, mod->name);

		/* If there is any older access to the same address that this access could not
		 * be coalesced with, wait for it. 
		older_stack = mod_in_flight_address(mod, stack->addr, stack);
		if (older_stack){
			mem_debug("    %lld wait for access %lld\n", stack->id, older_stack->id);
			mod_stack_wait_in_stack(stack, older_stack, EV_MOD_NMOESI_INVALIDATE_SLOT_LOCK);
			return;
		}*/

		/* Call pref find and lock */
		new_stack = mod_stack_create(stack->id, mod, stack->addr, EV_MOD_NMOESI_INVALIDATE_SLOT_ACTION, stack, stack->core, stack->thread, stack->prefetch);
		new_stack->blocking = 1;
		new_stack->retry = stack->retry;
		new_stack->pref = stack->pref;
		new_stack->access_kind = mod_access_invalidate;
		new_stack->request_dir = mod_request_up_down;
		esim_schedule_event(EV_MOD_NMOESI_PREF_FIND_AND_LOCK, new_stack, 0);
		return;
	}


	if (event == EV_MOD_NMOESI_INVALIDATE_SLOT_ACTION)
	{
		int retry_lat;

		mem_debug("  %lld %lld 0x%x %s invalidate slot action\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:invalidate_slot_action\"\n", stack->id, mod->name);

		/* Error locking */
		if (stack->err){
			mod->read_retries++;
			retry_lat = mod_get_retry_latency(mod);
			mem_debug("    lock error, retrying in %d cycles\n", retry_lat);
			stack->retry = 1;
			esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT_LOCK, stack, retry_lat);
			return;
		}

		esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT_UNLOCK, stack, 0);
		return;
	}
		

	if (event == EV_MOD_NMOESI_INVALIDATE_SLOT_UNLOCK){
		mem_debug("  %lld %lld 0x%x %s invalidate slot unlock\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:invalidate_slot_unlock\"\n", stack->id, mod->name);	

		/* Unlock directory entry */
		dir_pref_entry_unlock(mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
		
		/* Continue */
		esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_SLOT_FINISH, stack, 0);
		return;
	}


	if (event == EV_MOD_NMOESI_INVALIDATE_SLOT_FINISH)
	{
		mem_debug("%lld %lld 0x%x %s invalidate slot finish\n", esim_cycle, stack->id, stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:load_finish\"\n", stack->id, mod->name);
		mem_trace("mem.end_access name=\"A-%lld\"\n", stack->id);

		/* Increment witness variable */
		if (stack->witness_ptr) {
			(*stack->witness_ptr)++;
		}

		/* Return event queue element into event queue */
		if (stack->event_queue && stack->event_queue_item)
			linked_list_add(stack->event_queue, stack->event_queue_item);

		/* Finish access */
		mod_access_finish(mod, stack);
		
		/* The last prefetch (or invalidation) of a prefetch group sets the stream tag */
		struct stream_buffer_t *sb = &cache->prefetch.streams[stack->pref_stream];
		assert(stack->pref_stream >= 0 && stack->pref_stream < cache->prefetch.num_streams);
		assert(stack->pref_slot >= 0 && stack->pref_slot < cache->prefetch.aggressivity);
		assert(sb->pending_prefetches > 0);
		if(sb->pending_prefetches == 1){
			sb->stream_tag = stack->addr & ~cache->prefetch.stream_mask;
			/* Debug */
			if(sb->stream_tag != sb->stream_transcient_tag){
				int i,slot,count; 
				struct stream_block_t *block;
				struct dir_lock_t *dir_lock;

				mem_debug("stream_tag=0x%x stream_transcient_tag=0x%x\n", sb->stream_tag, sb->stream_transcient_tag);
				
				count = sb->head + sb->num_slots;
				for(i = sb->head; i < count; i++){
					slot = i % sb->num_slots;
					block = cache_get_pref_block(cache, sb->stream, slot);
					dir_lock = dir_pref_lock_get(mod->dir, sb->stream, slot);
					mem_debug("\t\t{slot=%d, tag=0x%x, transient_tag=0x%x, state=%s, locked=%d}\n", slot, block->tag, block->transient_tag, map_value(&cache_block_state_map, block->state), dir_lock->lock);
				}

			}
			assert(sb->stream_tag == sb->stream_transcient_tag);
		}
		sb->pending_prefetches--;
		
		/* Statistics */
		mod->canceled_prefetches++;

		/* Return */
		if(stack->ret_stack){
			stack->ret_stack->pref_stream = stack->pref_stream;
			stack->ret_stack->pref_slot = stack->pref_slot;
		}
		mod_stack_return(stack);
		return;
	}

	abort();
}



void mod_handler_nmoesi_pref_evict(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *ret = stack->ret_stack;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;
	struct mod_t *target_mod = stack->target_mod;

	struct dir_t *dir;
	struct dir_entry_t *dir_entry;

	uint32_t dir_entry_tag, z;


	if (event == EV_MOD_NMOESI_PREF_EVICT)
	{
		/* Default return value */
		ret->err = 0;
		
		/* Get block info */
		cache_get_pref_block_data(mod->cache, stack->pref_stream, stack->pref_slot, &stack->tag, &stack->state); //SLOT
		
		/* Save some data */
		stack->src_pref_stream = stack->pref_stream;
		stack->src_pref_slot = stack->pref_slot;
		stack->src_tag = stack->tag;

		mem_debug("  %lld %lld 0x%x %s pref_evict (pref_stream=%d, pref_slot=%d, state=%s)\n", esim_cycle, stack->id, stack->tag, mod->name, stack->pref_stream,stack->pref_slot, map_value(&cache_block_state_map, stack->state));
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_evict\"\n", stack->id, mod->name);

		assert(stack->state);
		assert(stack->access_kind);

		stack->target_mod = mod_get_low_mod(mod, stack->tag);

		esim_schedule_event(EV_MOD_NMOESI_PREF_EVICT_ACTION, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_PREF_EVICT_ACTION)
	{
		struct mod_t *low_mod;
		struct net_node_t *low_node;
		int msg_size;

		mem_debug("  %lld %lld 0x%x %s pref evict action\n", esim_cycle, stack->id,stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_evict_action\"\n", stack->id, mod->name);

		/* Get low node */
		low_mod = stack->target_mod;
		low_node = low_mod->high_net_node;
		assert(low_mod != mod);
		assert(low_mod == mod_get_low_mod(mod, stack->tag));
		assert(low_node && low_node->user_data == low_mod);
		
		/* State = I */
		if (stack->state == cache_block_invalid){
			esim_schedule_event(EV_MOD_NMOESI_PREF_EVICT_FINISH, stack, 0);
			return;
		}
		/* State E or S */
		else{
			assert(stack->state == cache_block_exclusive || stack->state == cache_block_shared);
			msg_size = 8;
			stack->reply = reply_ACK;
		}

		/* Send message */
		stack->msg = net_try_send_ev(mod->low_net, mod->low_net_node, low_node, msg_size, EV_MOD_NMOESI_PREF_EVICT_RECEIVE, stack, event, stack);
		return;
	}

	if (event == EV_MOD_NMOESI_PREF_EVICT_RECEIVE)
	{
		mem_debug("  %lld %lld 0x%x %s pref evict receive\n", esim_cycle, stack->id,stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_evict_receive\"\n", stack->id, target_mod->name);

		/* Receive message */
		net_receive(target_mod->high_net, target_mod->high_net_node, stack->msg);

		/* Find and lock */
		new_stack = mod_stack_create(stack->id, target_mod, stack->src_tag, EV_MOD_NMOESI_PREF_EVICT_PROCESS, stack, stack->core, stack->thread,stack->prefetch);
		new_stack->blocking = 0;
		new_stack->read = 0;
		new_stack->retry = 0;
		new_stack->request_dir = mod_request_up_down;
		esim_schedule_event(EV_MOD_NMOESI_FIND_AND_LOCK, new_stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_PREF_EVICT_PROCESS)
	{
		mem_debug("  %lld %lld 0x%x %s pref evict process\n", esim_cycle, stack->id,stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_evict_process\"\n", stack->id, target_mod->name);
		
		assert(!stack->prefetch_hit);

		/* Error locking block */
		if (stack->err){
			ret->err = 1;
			esim_schedule_event(EV_MOD_NMOESI_PREF_EVICT_REPLY, stack, 0);
			return;
		}

		/* If data was received, there is an error */
		assert(stack->reply != reply_ACK_DATA);

		/* Remove sharer and owner */
		dir = target_mod->dir;
		for (z = 0; z < dir->zsize; z++){
			/* Skip other subblocks */
			dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
			assert(dir_entry_tag < stack->tag + target_mod->block_size);
			if (dir_entry_tag < stack->src_tag || dir_entry_tag >= stack->src_tag + mod->block_size)
				continue;

			dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
			dir_entry_clear_sharer(dir, stack->set, stack->way, z, 
				mod->low_net_node->index);
			if (dir_entry->owner == mod->low_net_node->index){
				dir_entry_set_owner(dir, stack->set, stack->way, z,	DIR_ENTRY_OWNER_NONE);
			}
		}

		/* Unlock the directory entry */
		dir_entry_unlock(dir, stack->set, stack->way);

		esim_schedule_event(EV_MOD_NMOESI_PREF_EVICT_REPLY, stack, 0);
		return;
	}

	
	if (event == EV_MOD_NMOESI_PREF_EVICT_REPLY){
		mem_debug("  %lld %lld 0x%x %s pref evict reply\n", esim_cycle, stack->id,stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_evict_reply\"\n",
			stack->id, target_mod->name);

		/* Send message */
		stack->msg = net_try_send_ev(target_mod->high_net, target_mod->high_net_node,mod->low_net_node, 8, EV_MOD_NMOESI_PREF_EVICT_REPLY_RECEIVE, stack, event, stack);
		return;
	}

	if (event == EV_MOD_NMOESI_PREF_EVICT_REPLY_RECEIVE)
	{
		mem_debug("  %lld %lld 0x%x %s pref evict reply receive\n", esim_cycle, stack->id, stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_evict_reply_receive\"\n",stack->id, mod->name);

		/* Receive message */
		net_receive(mod->low_net, mod->low_net_node, stack->msg);

		/* Invalidate block if there was no error. */
		if (!stack->err){
			cache_set_pref_block(mod->cache, stack->src_pref_stream, stack->src_pref_slot, 0, cache_block_invalid); //SLOT
			
			/* Decrement count if invalidating slot */
			if(stack->access_kind == mod_access_invalidate)
				mod->cache->prefetch.streams[stack->src_pref_stream].count--; //COUNT
		}

		esim_schedule_event(EV_MOD_NMOESI_PREF_EVICT_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_PREF_EVICT_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s pref evict finish\n", esim_cycle, stack->id,stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:pref_evict_finish\"\n", stack->id, mod->name);
		
		mod_stack_return(stack);
		return;
	}

	abort();
}


void mod_handler_nmoesi_read_request(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *ret = stack->ret_stack;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;
	struct mod_t *target_mod = stack->target_mod;

	struct dir_t *dir;
	struct dir_entry_t *dir_entry;

	uint32_t dir_entry_tag, z;
	
	if (event == EV_MOD_NMOESI_READ_REQUEST)
	{
		struct net_t *net;
		struct net_node_t *src_node;
		struct net_node_t *dst_node;

		mem_debug("  %lld %lld 0x%x %s read request\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request\"\n",
			stack->id, mod->name);

		/* Default return values*/
		ret->shared = 0;
		ret->err = 0;

		/* Checks */
		assert(stack->request_dir);
		assert(mod_get_low_mod(mod, stack->addr) == target_mod ||
			stack->request_dir == mod_request_down_up);
		assert(mod_get_low_mod(target_mod, stack->addr) == mod ||
			stack->request_dir == mod_request_up_down);

		/* Get source and destination nodes */
		if (stack->request_dir == mod_request_up_down)
		{
			net = mod->low_net;
			src_node = mod->low_net_node;
			dst_node = target_mod->high_net_node;
		}
		else
		{
			net = mod->high_net;
			src_node = mod->high_net_node;
			dst_node = target_mod->low_net_node;
		}

		/* Send message */
		stack->msg = net_try_send_ev(net, src_node, dst_node, 8,
			EV_MOD_NMOESI_READ_REQUEST_RECEIVE, stack, event, stack);
		return;
	}

	if (event == EV_MOD_NMOESI_READ_REQUEST_RECEIVE)
	{
		struct mod_stack_t * master_stack;

		mem_debug("  %lld %lld 0x%x %s read request receive\n", esim_cycle, stack->id,
			stack->addr, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_receive\"\n",
			stack->id, target_mod->name);

		/* Receive message */
		if (stack->request_dir == mod_request_up_down)
			net_receive(target_mod->high_net, target_mod->high_net_node, stack->msg);
		else
			net_receive(target_mod->low_net, target_mod->low_net_node, stack->msg);
		
		/* Record access */
		if(stack->request_dir == mod_request_up_down)
		{
			/* Record access -- We can threat all read_requests as loads */
			mod_access_start(target_mod, stack, mod_access_load);
				
			/* Coalesce (loads with L2 prefetches) */
			master_stack = mod_can_coalesce(target_mod, mod_access_load, stack->addr, stack);
			if (master_stack){
				assert(master_stack->prefetch==2);
				mem_debug("    %lld waiting %lld\n",stack->id, master_stack->id);
				mod_stack_wait_in_stack(stack, master_stack, EV_MOD_NMOESI_READ_REQUEST_LOCK);
				return;
			}
		}

		esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_LOCK, stack, 0);
		return;
	}
	
	
	if (event == EV_MOD_NMOESI_READ_REQUEST_LOCK)
	{
		struct mod_stack_t * older_stack;
		
		mem_debug("  %lld %lld 0x%x %s read request lock\n", esim_cycle, stack->id,
			stack->addr, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_lock\"\n",
			stack->id, mod->name);

		/* If there is any older write, wait for it */
		older_stack = mod_in_flight_write(target_mod, stack);
		if (older_stack)
		{
			mem_debug("    %lld wait for write %lld\n", stack->id, older_stack->id);
			mod_stack_wait_in_stack(stack, older_stack, EV_MOD_NMOESI_READ_REQUEST_LOCK);
			return;
		}

		/* If there is any older access to the same address that this access could not
		 * be coalesced with, wait for it. */
		older_stack = mod_in_flight_address(target_mod, stack->addr, stack);
		if (older_stack)
		{
			mem_debug("    %lld wait for access %lld\n",
				stack->id, older_stack->id);
			mod_stack_wait_in_stack(stack, older_stack, EV_MOD_NMOESI_READ_REQUEST_LOCK);
			return;
		}
		
		/* Call find and lock */
		new_stack = mod_stack_create(stack->id, target_mod, stack->addr,
			EV_MOD_NMOESI_READ_REQUEST_ACTION, stack,stack->core, stack->thread, stack->prefetch);
		new_stack->blocking = stack->request_dir == mod_request_down_up;
		new_stack->read = 1;
		new_stack->retry = 0;
		new_stack->request_dir = stack->request_dir; //VVV
		new_stack->access_kind = stack->access_kind;
		esim_schedule_event(EV_MOD_NMOESI_FIND_AND_LOCK, new_stack, 0);
		return;
	}


	if (event == EV_MOD_NMOESI_READ_REQUEST_ACTION)
	{
		mem_debug("  %lld %lld 0x%x %s read request action\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_action\"\n",
			stack->id, target_mod->name);

		/* Check block locking error. If read request is down-up, there should not
		 * have been any error while locking. */
		if (stack->err)
		{
			assert(stack->request_dir == mod_request_up_down);
			ret->err = 1;
			mod_stack_set_reply(ret, reply_ACK_ERROR);
			stack->reply_size = 8;
			esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_REPLY, stack, 0);
			return;
		}

		/* Enqueue prefetch(es) on cache miss */
		if(!stack->state && must_enqueue_prefetch(stack, target_mod->level)){
			if(stack->prefetch_hit){
				/* We only have to prefetch one block and put it on the tail of the buffer */
				enqueue_prefetch_on_hit(stack, target_mod->level);
			} else { 
				/* We have to fill all the stream buffer */
				enqueue_prefetch_on_miss(stack, 0/* TODO: stride*/, target_mod->level);
			}
		}

		esim_schedule_event(stack->request_dir == mod_request_up_down ?
			EV_MOD_NMOESI_READ_REQUEST_UPDOWN : EV_MOD_NMOESI_READ_REQUEST_DOWNUP, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_READ_REQUEST_UPDOWN)
	{
		struct mod_t *owner;

		mem_debug("  %lld %lld 0x%x %s read request updown\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_updown\"\n",
			stack->id, target_mod->name);

		stack->pending = 1;

		/* Set the initial reply message and size.  This will be adjusted later if
		 * a transfer occur between peers. */
		stack->reply_size = mod->block_size + 8;
		mod_stack_set_reply(stack, reply_ACK_DATA);
		
		/* If prefetch_hit, block can't be in any upper cache */
		if(stack->prefetch_hit)
		{
			assert(stack->addr % mod->block_size == 0);
			dir = target_mod->dir;
			for (z = 0; z < dir->zsize; z++)
			{
				dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
				assert(dir_entry_tag < stack->tag + target_mod->block_size);
				if (dir_entry_tag < stack->addr || dir_entry_tag >= stack->addr + mod->block_size)
					continue;
				dir_entry = dir_pref_entry_get(dir, stack->pref_stream, stack->pref_slot, z); //SLOT* //Z
				assert(dir_entry->owner == DIR_ENTRY_OWNER_NONE);
			}

			esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_UPDOWN_MISS, stack, 0);
			return;
		}
		
		if (stack->state)
		{
			/* Status = M/O/E/S/N
			 * Check: address is a multiple of requester's block_size
			 * Check: no sub-block requested by mod is already owned by mod */

			assert(stack->addr % mod->block_size == 0);
			dir = target_mod->dir;
			for (z = 0; z < dir->zsize; z++)
			{
				dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
				assert(dir_entry_tag < stack->tag + target_mod->block_size);
				if (dir_entry_tag < stack->addr || dir_entry_tag >= stack->addr + mod->block_size)
					continue;
				dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
				
				/* Error. Crash report.*/
				if(dir_entry->owner == mod->low_net_node->index){
					int stream, slot, count, i;
					struct stream_block_t *block;
					struct dir_lock_t *dir_lock;
					struct stream_buffer_t *sb;
					for(stream = 0; stream<mod->cache->prefetch.num_streams; stream++){
						sb = &mod->cache->prefetch.streams[stream];
						count = sb->head + sb->num_slots;
						mem_debug("\n\t\tStream %d\n", stream);
						for(i = sb->head; i < count; i++){
							slot = i % sb->num_slots;
							block = cache_get_pref_block(mod->cache, sb->stream, slot);
							dir_lock = dir_pref_lock_get(mod->dir, sb->stream, slot);
							mem_debug("\t\t{slot=%d, tag=0x%x, transient_tag=0x%x, state=%s, locked=%d}\n", slot, block->tag, block->transient_tag, map_value(&cache_block_state_map, block->state), dir_lock->lock);
						}
					}
				}
				assert(dir_entry->owner != mod->low_net_node->index);
			}

			/* Send read request to owners other than mod for all sub-blocks. */
			for (z = 0; z < dir->zsize; z++)
			{
				struct net_node_t *node;

				dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
				dir_entry_tag = stack->tag + z * target_mod->sub_block_size;

				/* No owner */
				if (!DIR_ENTRY_VALID_OWNER(dir_entry))
					continue;

				/* Owner is mod */
				if (dir_entry->owner == mod->low_net_node->index)
					continue;

				/* Get owner mod */
				node = list_get(target_mod->high_net->node_list, dir_entry->owner);
				assert(node->kind == net_node_end);
				owner = node->user_data;
				assert(owner);

				/* Not the first sub-block */
				if (dir_entry_tag % owner->block_size)
					continue;

				/* Send read request */
				stack->pending++;
				new_stack = mod_stack_create(stack->id, target_mod, dir_entry_tag,
					EV_MOD_NMOESI_READ_REQUEST_UPDOWN_FINISH, stack,stack->core, stack->thread, stack->prefetch);
				/* Only set peer if its a subblock that was requested */
				if (dir_entry_tag >= stack->addr && 
					dir_entry_tag < stack->addr + mod->block_size)
				{
					new_stack->peer = stack->mod;
				}
				new_stack->target_mod = owner;
				new_stack->request_dir = mod_request_down_up;
				new_stack->access_kind = stack->access_kind;
				esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST, new_stack, 0);
			}
			esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_UPDOWN_FINISH, stack, 0);
		}
		else
		{
			/* State = I */
			assert(!dir_entry_group_shared_or_owned(target_mod->dir,
				stack->set, stack->way));
			new_stack = mod_stack_create(stack->id, target_mod, stack->tag,
				EV_MOD_NMOESI_READ_REQUEST_UPDOWN_MISS, stack,stack->core, stack->thread, stack->prefetch);
			/* Peer is NULL since we keep going up-down */
			new_stack->target_mod = mod_get_low_mod(target_mod, stack->tag);
			new_stack->request_dir = mod_request_up_down;
			new_stack->access_kind = stack->access_kind;
			esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST, new_stack, 0);
		}
		return;
	}

	if (event == EV_MOD_NMOESI_READ_REQUEST_UPDOWN_MISS)
	{
		mem_debug("  %lld %lld 0x%x %s read request updown miss\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_updown_miss\"\n",
			stack->id, target_mod->name);

		/* Check error */
		if (stack->err)
		{
			dir_entry_unlock(target_mod->dir, stack->set, stack->way);
			if(stack->prefetch_hit)
				dir_pref_entry_unlock(target_mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
			ret->err = 1;
			mod_stack_set_reply(ret, reply_ACK_ERROR);
			stack->reply_size = 8;
			esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_REPLY, stack, 0);
			return;
		}

		if(stack->prefetch_hit) //VVV
		{
			/* Portem el bloc del buffer a la cache */
			struct stream_block_t *block = cache_get_pref_block(target_mod->cache, stack->pref_stream, stack->pref_slot); //SLOT*
			assert(stack->tag == block->tag);
			cache_set_block(target_mod->cache, stack->set, stack->way, block->tag, block->state, 0);
			block->state = cache_block_invalid;
			block->tag = -1;
			struct stream_buffer_t *sb = &target_mod->cache->prefetch.streams[stack->pref_stream];
			sb->count--; //COUNT
			if(stack->sequential_hit)
				sb->head = (sb->head + 1) % sb->num_slots; //HEAD
		}
		/* Set block state to excl/shared depending on the return value 'shared'
		 * that comes from a read request into the next cache level.
		 * Also set the tag of the block. */
		else
		{
			cache_set_block(target_mod->cache, stack->set, stack->way, stack->tag,
				stack->shared ? cache_block_shared : cache_block_exclusive, 0);
		}
		esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_UPDOWN_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_READ_REQUEST_UPDOWN_FINISH)
	{
		int shared;

		/* Ensure that a reply was received */
		assert(stack->reply);

		/* Ignore while pending requests */
		assert(stack->pending > 0);
		stack->pending--;
		if (stack->pending)
			return;

		/* Trace */
		mem_debug("  %lld %lld 0x%x %s read request updown finish\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_updown_finish\"\n",
			stack->id, target_mod->name);

		/* If blocks were sent directly to the peer, the reply size would
		 * have been decreased.  Based on the final size, we can tell whether
		 * to send more data or simply ACK */
		if (stack->reply_size == 8) 
		{
			mod_stack_set_reply(ret, reply_ACK);
		}
		else if (stack->reply_size > 8)
		{
			mod_stack_set_reply(ret, reply_ACK_DATA);
		}
		else 
		{
			fatal("Invalid reply size: %d", stack->reply_size);
		}

		dir = target_mod->dir;

		shared = 0;

		/* With the Owned state, the directory entry may remain owned by the sender */
		if (!stack->retain_owner)
		{
			/* Set owner to 0 for all directory entries not owned by mod. */
			for (z = 0; z < dir->zsize; z++)
			{
				dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
				if (dir_entry->owner != mod->low_net_node->index)
					dir_entry_set_owner(dir, stack->set, stack->way, z, 
						DIR_ENTRY_OWNER_NONE);
			}
		}

		/* For each sub-block requested by mod, set mod as sharer, and
		 * check whether there is other cache sharing it. */
		for (z = 0; z < dir->zsize; z++)
		{
			dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
			if (dir_entry_tag < stack->addr || dir_entry_tag >= stack->addr + mod->block_size)
				continue;
			dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
			dir_entry_set_sharer(dir, stack->set, stack->way, z, mod->low_net_node->index);
			if (dir_entry->num_sharers > 1 || stack->nc_store || stack->shared)
				shared = 1;
		}

		/* If no sub-block requested by mod is shared by other cache, set mod
		 * as owner of all of them. Otherwise, notify requester that the block is
		 * shared by setting the 'shared' return value to true. */
		ret->shared = shared;
		if (!shared)
		{
			for (z = 0; z < dir->zsize; z++)
			{
				dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
				if (dir_entry_tag < stack->addr || dir_entry_tag >= stack->addr + mod->block_size)
					continue;
				dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
				dir_entry_set_owner(dir, stack->set, stack->way, z, mod->low_net_node->index);
			}
		}

		dir_entry_unlock(dir, stack->set, stack->way);
		if(stack->prefetch_hit)
			dir_pref_entry_unlock(target_mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*

		esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_REPLY, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_READ_REQUEST_DOWNUP)
	{
		struct mod_t *owner;

		mem_debug("  %lld %lld 0x%x %s read request downup\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_downup\"\n",
			stack->id, target_mod->name);
		
		//VVV
		if(stack->prefetch_hit)
			cache_get_pref_block_data(target_mod->cache, stack->pref_stream, stack->pref_slot, NULL, &stack->state); //SLOT*
		
		/* Check: state must not be invalid or shared.
		 * By default, only one pending request.
		 * Response depends on state */
		assert(stack->state != cache_block_invalid);
		assert(stack->state != cache_block_shared);
		assert(stack->state != cache_block_noncoherent);
		stack->pending = 1;
		
		/* VVV Si ha hagut prefetch_hit, el bloc esta en el buffer de prefetch, per tant,
		 * ninguna caché en un nivell superior pot tenir el bloc.*/
		if(stack->prefetch_hit)
		{
			esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_DOWNUP_WAIT_FOR_REQS, stack, 0);
			return;
		}

		/* Send a read request to the owner of each subblock. */
		dir = target_mod->dir;
		for (z = 0; z < dir->zsize; z++)
		{
			struct net_node_t *node;

			dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
			assert(dir_entry_tag < stack->tag + target_mod->block_size);
			dir_entry = dir_entry_get(dir, stack->set, stack->way, z);

			/* No owner */
			if (!DIR_ENTRY_VALID_OWNER(dir_entry))
				continue;

			/* Get owner mod */
			node = list_get(target_mod->high_net->node_list, dir_entry->owner);
			assert(node && node->kind == net_node_end);
			owner = node->user_data;

			/* Not the first sub-block */
			if (dir_entry_tag % owner->block_size)
				continue;

			stack->pending++;
			new_stack = mod_stack_create(stack->id, target_mod, dir_entry_tag,
				EV_MOD_NMOESI_READ_REQUEST_DOWNUP_WAIT_FOR_REQS, stack, stack->core, stack->thread, stack->prefetch);
			new_stack->target_mod = owner;
			new_stack->request_dir = mod_request_down_up;
			new_stack->access_kind = stack->access_kind;
			esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST, new_stack, 0);
		}

		esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_DOWNUP_WAIT_FOR_REQS, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_READ_REQUEST_DOWNUP_WAIT_FOR_REQS)
	{
		/* Ignore while pending requests */
		assert(stack->pending > 0);
		stack->pending--;
		if (stack->pending)
			return;

		mem_debug("  %lld %lld 0x%x %s read request downup wait for reqs\n", 
			esim_cycle, stack->id, stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_downup_wait_for_reqs\"\n",
			stack->id, target_mod->name);

		if (stack->peer)
		{
			/* Send this block (or subblock) to the peer */
			new_stack = mod_stack_create(stack->id, target_mod, stack->tag,
				EV_MOD_NMOESI_READ_REQUEST_DOWNUP_FINISH, stack,stack->core, stack->thread, stack->prefetch);
			new_stack->peer = stack->peer;
			new_stack->target_mod = stack->target_mod;
			esim_schedule_event(EV_MOD_NMOESI_PEER_SEND, new_stack, 0);
		}
		else 
		{
			/* No data to send to peer, so finish */
			esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_DOWNUP_FINISH, stack, 0);
		}

		return;
	}

	if (event == EV_MOD_NMOESI_READ_REQUEST_DOWNUP_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s read request downup finish\n", 
			esim_cycle, stack->id, stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_downup_finish\"\n",
			stack->id, target_mod->name);

		if (stack->reply == reply_ACK_DATA)
		{
			/* If data was received, it was owned or modified by a higher level cache.
			 * We need to continue to propagate it up until a peer is found */

			if (stack->peer) 
			{
				/* Peer was found, so this directory entry should be changed 
			 	* to owned */
				cache_set_block(target_mod->cache, stack->set, stack->way,
					stack->tag, cache_block_owned, stack->prefetch);

				/* Higher-level cache changed to shared, set owner of 
			 	* sub-blocks to NONE. */
				dir = target_mod->dir;
				for (z = 0; z < dir->zsize; z++)
				{
					dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
					assert(dir_entry_tag < stack->tag + target_mod->block_size);
					dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
					dir_entry_set_owner(dir, stack->set, stack->way, z, DIR_ENTRY_OWNER_NONE);
				}
				stack->reply_size = 8;
				mod_stack_set_reply(ret, reply_ACK_DATA_SENT_TO_PEER);

				/* Decrease the amount of data that mod will have to send back
				 * to its higher level cache */
				ret->reply_size -= target_mod->block_size;
				assert(ret->reply_size >= 8);

				/* Let the lower-level cache know not to delete the owner */
				ret->retain_owner = 1;
			}
			else
			{
				/* Set state to shared */
				cache_set_block(target_mod->cache, stack->set, stack->way, 
					stack->tag, cache_block_shared, stack->prefetch);

				/* State is changed to shared, set owner of sub-blocks to 0. */
				dir = target_mod->dir;
				for (z = 0; z < dir->zsize; z++)
				{
					dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
					assert(dir_entry_tag < stack->tag + target_mod->block_size);
					dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
					dir_entry_set_owner(dir, stack->set, stack->way, z, DIR_ENTRY_OWNER_NONE);
				}
				stack->reply_size = target_mod->block_size + 8;
				mod_stack_set_reply(ret, reply_ACK_DATA);
			}
		}
		else if (stack->reply == reply_ACK)
		{
			/* Higher-level cache was exclusive with no modifications above it */
			stack->reply_size = 8;

			cache_set_block(target_mod->cache, stack->set, stack->way, 
				stack->tag, cache_block_shared, stack->prefetch);

			/* State is changed to shared, set owner of sub-blocks to 0. */
			dir = target_mod->dir;
			for (z = 0; z < dir->zsize; z++)
			{
				dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
				assert(dir_entry_tag < stack->tag + target_mod->block_size);
				dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
				dir_entry_set_owner(dir, stack->set, stack->way, z, DIR_ENTRY_OWNER_NONE);
			}

			if (stack->peer)
			{
				stack->reply_size = 8;
				mod_stack_set_reply(ret, reply_ACK_DATA_SENT_TO_PEER);

				/* Decrease the amount of data that mod will have to send back
				 * to its higher level cache */
				ret->reply_size -= target_mod->block_size;
				assert(ret->reply_size >= 8);
			}
			else
			{
				mod_stack_set_reply(ret, reply_ACK);
				stack->reply_size = 8;
			}
		}
		else if (stack->reply == reply_NO_REPLY)
		{
			/* This block is not present in any higher level caches */
			if (stack->peer) 
			{
				stack->reply_size = 8;
				mod_stack_set_reply(ret, reply_ACK_DATA_SENT_TO_PEER);

				/* Decrease the amount of data that mod will have to send back
				 * to its higher level cache */
				ret->reply_size -= target_mod->sub_block_size;
				assert(ret->reply_size >= 8);
				
				if(stack->prefetch_hit){
					/* Set prefetched block to shared */
					struct stream_block_t *block = cache_get_pref_block(
						target_mod->cache, stack->pref_stream, stack->pref_slot); //SLOT
					block->state = cache_block_shared;
				} else if (stack->state == cache_block_modified || stack->state == cache_block_owned){
					/* Let the lower-level cache know not to delete the owner */
					ret->retain_owner = 1;
						
					/* Set block to owned */
					cache_set_block(target_mod->cache, stack->set, stack->way, 
						stack->tag, cache_block_owned, stack->prefetch);
				}
				else 
				{/* Set block to shared */
					cache_set_block(target_mod->cache, stack->set, stack->way, 
						stack->tag, cache_block_shared, stack->prefetch);
				}
			}
			else 
			{
				if (stack->state == cache_block_exclusive || 
					stack->state == cache_block_shared)
				{
					stack->reply_size = 8;
					mod_stack_set_reply(ret, reply_ACK);

				}
				else if (stack->state == cache_block_owned ||
					stack->state == cache_block_modified || 
					stack->state == cache_block_noncoherent)
				{
					/* No peer exists, so data is returned to mod */
					stack->reply_size = target_mod->sub_block_size + 8;
					mod_stack_set_reply(ret, reply_ACK_DATA);
				}
				else 
				{
					fatal("Invalid cache block state: %d\n", stack->state);
				}

				/* Set block to shared */
				if(stack->prefetch_hit)
				{
					struct stream_block_t * block = cache_get_pref_block(
						target_mod->cache, stack->pref_stream, stack->pref_slot); //SLOT*
					block->state = cache_block_shared;
				}
				else
				{
					cache_set_block(target_mod->cache, stack->set, stack->way, 
						stack->tag, cache_block_shared, stack->prefetch);
				}
			}
		}

		if(stack->prefetch_hit)
			dir_pref_entry_unlock(target_mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
		else	
			dir_entry_unlock(target_mod->dir, stack->set, stack->way);
		
		esim_schedule_event(EV_MOD_NMOESI_READ_REQUEST_REPLY, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_READ_REQUEST_REPLY)
	{
		struct net_t *net;
		struct net_node_t *src_node;
		struct net_node_t *dst_node;

		mem_debug("  %lld %lld 0x%x %s read request reply\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_reply\"\n",
			stack->id, target_mod->name);
		
		/* Checks */
		assert(stack->reply_size);
		assert(stack->request_dir);
		assert(mod_get_low_mod(mod, stack->addr) == target_mod ||
			mod_get_low_mod(target_mod, stack->addr) == mod);

		/* Get network and nodes */
		if (stack->request_dir == mod_request_up_down)
		{
			net = mod->low_net;
			src_node = target_mod->high_net_node;
			dst_node = mod->low_net_node;
		}
		else
		{
			net = mod->high_net;
			src_node = target_mod->low_net_node;
			dst_node = mod->high_net_node;
		}
		
		/* Send message */
		stack->msg = net_try_send_ev(net, src_node, dst_node, stack->reply_size,
			EV_MOD_NMOESI_READ_REQUEST_FINISH, stack, event, stack);
		return;
	}

	if (event == EV_MOD_NMOESI_READ_REQUEST_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s read request finish\n", esim_cycle, stack->id,
			stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:read_request_finish\"\n",
			stack->id, mod->name);

		/* Receive message */
		if (stack->request_dir == mod_request_up_down)
			net_receive(mod->low_net, mod->low_net_node, stack->msg);
		else
			net_receive(mod->high_net, mod->high_net_node, stack->msg);
		
		/* Delete access */
		if(stack->request_dir == mod_request_up_down)
			mod_access_finish(target_mod, stack);

		/* Return */
		mod_stack_return(stack);
		return;
	}

	abort();
}


void mod_handler_nmoesi_write_request(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *ret = stack->ret_stack;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;
	struct mod_t *target_mod = stack->target_mod;

	struct dir_t *dir;
	struct dir_entry_t *dir_entry;

	uint32_t dir_entry_tag, z;

	if (event == EV_MOD_NMOESI_WRITE_REQUEST)
	{
		struct net_t *net;
		struct net_node_t *src_node;
		struct net_node_t *dst_node;

		mem_debug("  %lld %lld 0x%x %s write request\n", esim_cycle, stack->id,
			stack->addr, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request\"\n",
			stack->id, mod->name);

		/* Default return values */
		ret->err = 0;

		/* For write requests, we need to set the initial reply size because
		 * in updown, peer transfers must be allowed to decrease this value
		 * (during invalidate). If the request turns out to be downup, then 
		 * these values will get overwritten. */
		stack->reply_size = mod->block_size + 8;
		mod_stack_set_reply(stack, reply_ACK_DATA);

		/* Checks */
		assert(stack->request_dir);
		assert(mod_get_low_mod(mod, stack->addr) == target_mod ||
			stack->request_dir == mod_request_down_up);
		assert(mod_get_low_mod(target_mod, stack->addr) == mod ||
			stack->request_dir == mod_request_up_down);

		/* Get source and destination nodes */
		if (stack->request_dir == mod_request_up_down)
		{
			net = mod->low_net;
			src_node = mod->low_net_node;
			dst_node = target_mod->high_net_node;
		}
		else
		{
			net = mod->high_net;
			src_node = mod->high_net_node;
			dst_node = target_mod->low_net_node;
		}

		/* Send message */
		stack->msg = net_try_send_ev(net, src_node, dst_node, 8,
			EV_MOD_NMOESI_WRITE_REQUEST_RECEIVE, stack, event, stack);
		return;
	}

	if (event == EV_MOD_NMOESI_WRITE_REQUEST_RECEIVE)
	{
		mem_debug("  %lld %lld 0x%x %s write request receive\n", esim_cycle, stack->id,
			stack->addr, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request_receive\"\n",
			stack->id, target_mod->name);

		/* Receive message */
		if (stack->request_dir == mod_request_up_down)
			net_receive(target_mod->high_net, target_mod->high_net_node, stack->msg);
		else
			net_receive(target_mod->low_net, target_mod->low_net_node, stack->msg);
		
		/* Record access */
		if(target_mod->prefetch_enabled)
		{
			mod_access_start(target_mod, stack, mod_access_store);
		}

		/* Find and lock */
		new_stack = mod_stack_create(stack->id, target_mod, stack->addr,
			EV_MOD_NMOESI_WRITE_REQUEST_ACTION, stack,stack->core, stack->thread, stack->prefetch);
		new_stack->blocking = stack->request_dir == mod_request_down_up;
		new_stack->read = 0;
		new_stack->retry = 0;
		new_stack->access_kind = stack->access_kind;
		new_stack->request_dir = stack->request_dir;
		esim_schedule_event(EV_MOD_NMOESI_FIND_AND_LOCK, new_stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_WRITE_REQUEST_ACTION)
	{
		mem_debug("  %lld %lld 0x%x %s write request action\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request_action\"\n",
			stack->id, target_mod->name);

		/* Check lock error. If write request is down-up, there should
		 * have been no error. */
		if (stack->err)
		{
			assert(stack->request_dir == mod_request_up_down);
			ret->err = 1;
			stack->reply_size = 8;
			esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST_REPLY, stack, 0);
			return;
		}
		
		/* If prefetch_hit, there aren't any upper level sharers of the block */
		if(stack->prefetch_hit)
		{
			mem_debug("  %lld %lld 0x%x %s write prefetch hit direction %d\n",
				esim_cycle, stack->id, stack->tag, target_mod->name,
				stack->request_dir==mod_request_up_down);
			
			/* Li posem l'estat del bloc prebuscat */
			struct stream_block_t * block = cache_get_pref_block(
				target_mod->cache, stack->pref_stream, stack->pref_slot); //SLOT*
			assert(stack->tag == block->tag);
			stack->state = block->state;
			
			esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST_EXCLUSIVE, stack, 0);
			return;

		}

		/* Invalidate the rest of upper level sharers */
		new_stack = mod_stack_create(stack->id, target_mod, 0,
			EV_MOD_NMOESI_WRITE_REQUEST_EXCLUSIVE, stack,stack->core, stack->thread, stack->prefetch);
		new_stack->except_mod = mod;
		new_stack->set = stack->set;
		new_stack->way = stack->way;
		new_stack->peer = stack->peer;
		new_stack->access_kind = stack->access_kind;
		esim_schedule_event(EV_MOD_NMOESI_INVALIDATE, new_stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_WRITE_REQUEST_EXCLUSIVE)
	{
		mem_debug("  %lld %lld 0x%x %s write request exclusive\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request_exclusive\"\n",
			stack->id, target_mod->name);

		if (stack->request_dir == mod_request_up_down)
			esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST_UPDOWN, stack, 0);
		else
			esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST_DOWNUP, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_WRITE_REQUEST_UPDOWN)
	{
		mem_debug("  %lld %lld 0x%x %s write request updown\n", esim_cycle, stack->id,
			stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request_updown\"\n",
			stack->id, target_mod->name);
		

		/* state = M/E */
		if (stack->state == cache_block_modified ||
			stack->state == cache_block_exclusive)
		{
			esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST_UPDOWN_FINISH, stack, 0);
		}
		/* state = O/S/I/N */
		else if (stack->state == cache_block_owned || stack->state == cache_block_shared ||
			stack->state == cache_block_invalid || stack->state == cache_block_noncoherent)
		{
			new_stack = mod_stack_create(stack->id, target_mod, stack->tag,
				EV_MOD_NMOESI_WRITE_REQUEST_UPDOWN_FINISH, stack,stack->core,
				stack->thread, stack->prefetch);
			new_stack->peer = mod;
			new_stack->target_mod = mod_get_low_mod(target_mod, stack->tag);
			new_stack->request_dir = mod_request_up_down;
			new_stack->access_kind = stack->access_kind;
			esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST, new_stack, 0);
		}
		else 
		{
			fatal("Invalid cache block state: %d\n", stack->state);
		}

		return;
	}

	if (event == EV_MOD_NMOESI_WRITE_REQUEST_UPDOWN_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s write request updown finish\n",
			esim_cycle, stack->id, stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request_updown_finish\"\n",
			stack->id, target_mod->name);

		/* Ensure that a reply was received */
		assert(stack->reply);

		/* Error in write request to next cache level */
		if (stack->err)
		{
			ret->err = 1;
			mod_stack_set_reply(ret, reply_ACK_ERROR);
			stack->reply_size = 8;
			dir_entry_unlock(target_mod->dir, stack->set, stack->way);
			if(stack->prefetch_hit)
				dir_pref_entry_unlock(target_mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
			esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST_REPLY, stack, 0);
			return;
		}

		/* Check that addr is a multiple of mod.block_size.
		 * Set mod as sharer and owner. */
		dir = target_mod->dir;
		for (z = 0; z < dir->zsize; z++)
		{
			assert(stack->addr % mod->block_size == 0);
			dir_entry_tag = stack->tag + z * target_mod->sub_block_size;
			assert(dir_entry_tag < stack->tag + target_mod->block_size);
			if (dir_entry_tag < stack->addr || dir_entry_tag >= stack->addr + mod->block_size)
				continue;
			dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
			dir_entry_set_sharer(dir, stack->set, stack->way, z, mod->low_net_node->index);
			dir_entry_set_owner(dir, stack->set, stack->way, z, mod->low_net_node->index);
			assert(dir_entry->num_sharers == 1);
		}
		
		if(stack->prefetch_hit)
		{
			cache_set_pref_block(target_mod->cache, stack->pref_stream, stack->pref_slot, -1, cache_block_invalid); //SLOT*
			struct stream_buffer_t *sb = &target_mod->cache->prefetch.streams[stack->pref_stream];
			sb->count--; //COUNT
			if(stack->sequential_hit)
				sb->head = (sb->head + 1) % sb->num_slots; //HEAD
			dir_pref_entry_unlock(target_mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
		}
		dir_entry_unlock(target_mod->dir, stack->set, stack->way);
		
		/* Set states O/E/S/I->E */
		cache_set_block(target_mod->cache, stack->set, stack->way,
			stack->tag, cache_block_exclusive, stack->prefetch);

		/* If blocks were sent directly to the peer, the reply size would
		* have been decreased.  Based on the final size, we can tell whether
		* to send more data up or simply ACK */
		if (stack->reply_size == 8) 
		{
			mod_stack_set_reply(ret, reply_ACK);
		}
		else if (stack->reply_size > 8)
		{
			mod_stack_set_reply(ret, reply_ACK_DATA);
		}
		else 
		{
			fatal("Invalid reply size: %d", stack->reply_size);
		}

		esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST_REPLY, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_WRITE_REQUEST_DOWNUP)
	{
		mem_debug("  %lld %lld 0x%x %s write request downup\n", esim_cycle, stack->id,stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request_downup\"\n",stack->id, target_mod->name);

		assert(stack->state != cache_block_invalid);
		assert(stack->prefetch_hit || //VVV
			!dir_entry_group_shared_or_owned(target_mod->dir, stack->set, stack->way));

		/* Compute reply size */	
		if (stack->state == cache_block_exclusive || 
			stack->state == cache_block_shared) 
		{
			/* Exclusive and shared states send an ACK */
			stack->reply_size = 8;
			mod_stack_set_reply(stack, reply_ACK);
		}
		else if (stack->state == cache_block_noncoherent)
		{
			/* Non-coherent state sends data */
			stack->reply_size = target_mod->block_size + 8;
			mod_stack_set_reply(stack, reply_ACK_DATA);
		}
		else if (stack->state == cache_block_modified || 
			stack->state == cache_block_owned)
		{
			if (stack->peer) 
			{
				/* Modified or owned entries send data directly to peer 
				 * if it exists */
				mod_stack_set_reply(stack, reply_ACK_DATA_SENT_TO_PEER);
				stack->reply_size = 8;

				/* This control path uses an intermediate stack that disappears, so 
				 * we have to update the return stack of the return stack */
				ret->ret_stack->reply_size -= target_mod->block_size;
				assert(ret->ret_stack->reply_size >= 8);

				/* Send data to the peer */
				new_stack = mod_stack_create(stack->id, target_mod, stack->tag,EV_MOD_NMOESI_WRITE_REQUEST_DOWNUP_FINISH, stack,stack->core, stack->thread, stack->prefetch);
				new_stack->peer = stack->peer;
				new_stack->target_mod = stack->target_mod;
				new_stack->access_kind = stack->access_kind;
				esim_schedule_event(EV_MOD_NMOESI_PEER_SEND, new_stack, 0);
				return;
			}	
			else 
			{
				/* If peer does not exist, data is returned to mod */
				mod_stack_set_reply(stack, reply_ACK_DATA);
				stack->reply_size = target_mod->block_size + 8;
			}
		}
		else 
		{
			fatal("Invalid cache block state: %d\n", stack->state);
		}

		esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST_DOWNUP_FINISH, stack, 0);

		return;
	}

	if (event == EV_MOD_NMOESI_WRITE_REQUEST_DOWNUP_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s write request downup finish\n", esim_cycle, stack->id, stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request_downup_finish\"\n",stack->id, target_mod->name);

		/* Set state to I, unlock */
		if(stack->prefetch_hit){
			cache_set_pref_block(target_mod->cache, stack->pref_stream, stack->pref_slot, -1, cache_block_invalid); //SLOT*
			struct stream_buffer_t *sb = &target_mod->cache->prefetch.streams[stack->pref_stream];
			sb->count--; //COUNT
			dir_pref_entry_unlock(target_mod->dir, stack->pref_stream, stack->pref_slot); //SLOT*
			/* If we remove head, we must increment it */
			if(stack->pref_slot == sb->head)
				sb->head = (stack->pref_slot + 1) % sb->num_slots; //HEAD
		}else{
			cache_set_block(target_mod->cache, stack->set, stack->way, 0,cache_block_invalid, 0);
			dir_entry_unlock(target_mod->dir, stack->set, stack->way);
		}

		esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST_REPLY, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_WRITE_REQUEST_REPLY)
	{
		struct net_t *net;
		struct net_node_t *src_node;
		struct net_node_t *dst_node;

		mem_debug("  %lld %lld 0x%x %s write request reply\n", esim_cycle, stack->id,stack->tag, target_mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request_reply\"\n", stack->id, target_mod->name);

		/* Checks */
		assert(stack->reply_size);
		assert(mod_get_low_mod(mod, stack->addr) == target_mod || mod_get_low_mod(target_mod, stack->addr) == mod);

		/* Get network and nodes */
		if (stack->request_dir == mod_request_up_down)
		{
			net = mod->low_net;
			src_node = target_mod->high_net_node;
			dst_node = mod->low_net_node;
		}
		else
		{
			net = mod->high_net;
			src_node = target_mod->low_net_node;
			dst_node = mod->high_net_node;
		}

		stack->msg = net_try_send_ev(net, src_node, dst_node, stack->reply_size,EV_MOD_NMOESI_WRITE_REQUEST_FINISH, stack, event, stack);
		return;
	}

	if (event == EV_MOD_NMOESI_WRITE_REQUEST_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s write request finish\n", esim_cycle, stack->id,stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:write_request_finish\"\n",stack->id, mod->name);

		/* Receive message */
		if (stack->request_dir == mod_request_up_down)
			net_receive(mod->low_net, mod->low_net_node, stack->msg);
		else
			net_receive(mod->high_net, mod->high_net_node, stack->msg);
	
		/* Delete access */
		if(target_mod->prefetch_enabled) mod_access_finish(target_mod, stack); //VVV

		/* Return */
		mod_stack_return(stack);
		return;
	}

	abort();
}


void mod_handler_nmoesi_peer(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_t *src = stack->target_mod;
	struct mod_t *peer = stack->peer;

	if (event == EV_MOD_NMOESI_PEER_SEND) 
	{
		mem_debug("  %lld %lld 0x%x %s %s peer send\n", esim_cycle, stack->id,
			stack->tag, src->name, peer->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:peer\"\n",
			stack->id, src->name);

		/* Send message from src to peer */
		stack->msg = net_try_send_ev(src->low_net, src->low_net_node, peer->low_net_node, 
			src->block_size + 8, EV_MOD_NMOESI_PEER_RECEIVE, stack, event, stack);

		return;
	}

	if (event == EV_MOD_NMOESI_PEER_RECEIVE) 
	{
		mem_debug("  %lld %lld 0x%x %s %s peer receive\n", esim_cycle, stack->id,
			stack->tag, src->name, peer->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:peer_receive\"\n",
			stack->id, peer->name);

		/* Receive message from src */
		net_receive(peer->low_net, peer->low_net_node, stack->msg);

		esim_schedule_event(EV_MOD_NMOESI_PEER_REPLY_ACK, stack, 0);

		return;
	}

	if (event == EV_MOD_NMOESI_PEER_REPLY_ACK) 
	{
		mem_debug("  %lld %lld 0x%x %s %s peer reply ack\n", esim_cycle, stack->id,
			stack->tag, src->name, peer->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:peer_reply_ack\"\n",
			stack->id, peer->name);

		/* Send ack from peer to src */
		stack->msg = net_try_send_ev(peer->low_net, peer->low_net_node, src->low_net_node, 
				8, EV_MOD_NMOESI_PEER_FINISH, stack, event, stack); 

		return;
	}

	if (event == EV_MOD_NMOESI_PEER_FINISH) 
	{
		mem_debug("  %lld %lld 0x%x %s %s peer finish\n", esim_cycle, stack->id,
			stack->tag, src->name, peer->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:peer_finish\"\n",
			stack->id, src->name);

		/* Receive message from src */
		net_receive(src->low_net, src->low_net_node, stack->msg);

		mod_stack_return(stack);
		return;
	}

	abort();
}


void mod_handler_nmoesi_invalidate(int event, void *data)
{
	struct mod_stack_t *stack = data;
	struct mod_stack_t *new_stack;

	struct mod_t *mod = stack->mod;

	struct dir_t *dir;
	struct dir_entry_t *dir_entry;

	uint32_t dir_entry_tag;
	uint32_t z;

	if (event == EV_MOD_NMOESI_INVALIDATE)
	{
		struct mod_t *sharer;
		int i;

		/* Get block info */
		cache_get_block(mod->cache, stack->set, stack->way, &stack->tag, &stack->state);
		mem_debug("  %lld %lld 0x%x %s invalidate (set=%d, way=%d, state=%s)\n", esim_cycle, stack->id,
			stack->tag, mod->name, stack->set, stack->way,
			map_value(&cache_block_state_map, stack->state));
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:invalidate\"\n",
			stack->id, mod->name);

		/* At least one pending reply */
		stack->pending = 1;
		
		/* Send write request to all upper level sharers except 'except_mod' */
		dir = mod->dir;
		for (z = 0; z < dir->zsize; z++)
		{
			dir_entry_tag = stack->tag + z * mod->sub_block_size;
			assert(dir_entry_tag < stack->tag + mod->block_size);
			dir_entry = dir_entry_get(dir, stack->set, stack->way, z);
			for (i = 0; i < dir->num_nodes; i++)
			{
				struct net_node_t *node;
				
				/* Skip non-sharers and 'except_mod' */
				if (!dir_entry_is_sharer(dir, stack->set, stack->way, z, i))
					continue;

				node = list_get(mod->high_net->node_list, i);
				sharer = node->user_data;
				if (sharer == stack->except_mod)
					continue;

				/* Clear sharer and owner */
				dir_entry_clear_sharer(dir, stack->set, stack->way, z, i);
				if (dir_entry->owner == i)
					dir_entry_set_owner(dir, stack->set, stack->way, z, DIR_ENTRY_OWNER_NONE);

				/* Send write request upwards if beginning of block */
				if (dir_entry_tag % sharer->block_size)
					continue;
				new_stack = mod_stack_create(stack->id, mod, dir_entry_tag,
					EV_MOD_NMOESI_INVALIDATE_FINISH, stack,stack->core, stack->thread, stack->prefetch);
				new_stack->target_mod = sharer;
				new_stack->request_dir = mod_request_down_up;
				new_stack->access_kind = stack->access_kind;
				/* If there is a peer and this is a block that he is interested in,
				 * and this sharer is the first sharer that is capable of 
				 * sending data, send the data directly to the peer. */
				/*
				// FIXME Peer is getting propagated incorrectly during invalidation
				if (stack->peer && first_sharer)
                                {
					new_stack->peer = stack->peer;
					first_sharer = 0;
				}
				*/
				esim_schedule_event(EV_MOD_NMOESI_WRITE_REQUEST, new_stack, 0);
				stack->pending++;
			}
		}
		esim_schedule_event(EV_MOD_NMOESI_INVALIDATE_FINISH, stack, 0);
		return;
	}

	if (event == EV_MOD_NMOESI_INVALIDATE_FINISH)
	{
		mem_debug("  %lld %lld 0x%x %s invalidate finish\n", esim_cycle, stack->id,
			stack->tag, mod->name);
		mem_trace("mem.access name=\"A-%lld\" state=\"%s:invalidate_finish\"\n",
			stack->id, mod->name);

		/* Ignore while pending */
		assert(stack->pending > 0);
		stack->pending--;
		if (stack->pending)
			return;
		mod_stack_return(stack);
		return;
	}

	abort();
}

