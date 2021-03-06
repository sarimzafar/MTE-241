#ifndef __HALF_FIT_H__
#define __HALF_FIT_H__

#include "util.h"

#define NUM_BUCKETS 11
#define MAX_MEMORY 32768

// Header is 31 useful bits + 1 padding bit = 4 bytes
#define HEADER_SIZE 4
#define BLOCK_SIZE_MULTIPLE 32

/*  
    A struct representing an allocated memory block.
    Implemented with bitfields.
*/

typedef struct memmap {
	U16 prev_block:10;
	U16 next_block:10;
	U16 block_size:10;
	BOOL alloc:1;
	BOOL padding:1;
}__attribute__ ((packed, aligned(4)))  memmap_t;

/* 
    A struct representing an unallocated (free) memory block.
    The first member of this struct is a U32-casted struct of type `memmap`
    (of size 32).
*/
typedef struct memmap_free {
	U32 memmap:32;
	U16 prev_free:10;
	U16 next_free:10;
} __attribute__ ((packed, aligned(4))) memmap_free_t;

// The buckets and the initial memory array
extern memmap_free_t* mprgmmap[];
extern unsigned char base_ptr[];

// Internal Helpers
__inline void* get_prev_block( memmap_t const* );
__inline void* get_next_block( memmap_t const* );
__inline U32 get_block_size( memmap_t const* );
__inline BOOL get_allocated( memmap_t const* );
__inline void* get_prev_free( memmap_free_t const* );
__inline void* get_next_free( memmap_free_t const* );

__inline void set_block_size( memmap_t*, U32 );
__inline void set_prev_block( memmap_t*, void* );
__inline void set_next_block( memmap_t*, void* );
__inline void set_allocated( memmap_t*, BOOL );
__inline void set_prev_free( memmap_free_t*, void* );
__inline void set_next_free( memmap_free_t*, void* );

__inline void remove_free_block( memmap_free_t* );
__inline void insert_free_block( memmap_free_t* );

__inline bool is_first_in_bucket(memmap_free_t*);
__inline bool is_last_in_bucket(memmap_free_t*);

__inline bool is_first_in_memory(memmap_t*);
__inline bool is_last_in_memory(memmap_t*);

__inline void memmap_free_init(memmap_free_t* const, U32 );
__inline void memmap_init(memmap_t* const, U32 );

memmap_free_t* split_block(memmap_free_t* const, U32);
memmap_free_t* coalesce_block(memmap_free_t*);
memmap_free_t* merge_block(memmap_free_t*, memmap_free_t*);

// Public interface
void half_init();
void *half_alloc( U32 n );
void half_free( void* );

#ifdef DEBUG_MEMORY
static U32 free_memory = 0;
#endif

#endif