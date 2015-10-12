/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */

#include "mm_alloc.h"

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
/* Your final implementation should comment out this macro. */
void *and = NULL;
struct zandile {
  size_t size;
  bool free;
  struct zandile* next;
  struct zandile* prev;
};
#define BAR_SIZE sizeof(struct zandile)
struct zandile* zandile2 (struct zandile *prev, size_t size){
	
	struct zandile *newzandile;
	newzandile = sbrk(0); 
	sbrk(size+BAR_SIZE); 
	if (prev){	
		prev->next=newzandile;
		newzandile->prev=prev;
	}
	newzandile->size = size;
	newzandile->next = NULL;
	newzandile->free = false;

	return newzandile;	
}
struct block* splitBlock(struct zandile* zandile1, size_t size){
	
	if (zandile1->size>=1+size+BAR_SIZE){  
		struct zandile * le = (struct zandile *)((char*)zandile1+BAR_SIZE+size); 
		le->size = zandile1->size-(size+BAR_SIZE); 
		le->free=true;
		le->next=zandile1->next;	
		le->prev=zandile1;
		if (zandile1->next){
			zandile1->next->prev=le;
		}
		zandile1->size=size;
		zandile1->next = le;
	}
	return zandile1;	
}

struct zandile* Free(struct zandile *prev, size_t size){	
	struct zandile *thiszandile = and;	

	while (true){	
		if (!thiszandile){	
			return zandile2(prev ,size);
		}
		if (thiszandile&&thiszandile->free && (thiszandile->size)>=size){	
			thiszandile=splitBlock(thiszandile,size);
			thiszandile->free=false;
			return thiszandile;	
		}
		prev = thiszandile; 
		thiszandile = thiszandile->next;	
	}
	return 0;	
}
void merge(struct zandile *thiszandile){	
	if (thiszandile->next){	
		if (thiszandile->next->free){	
			thiszandile->size+=thiszandile->next->size+BAR_SIZE;	
			if (thiszandile->next->next){	
				thiszandile->next->next->prev=thiszandile;
				thiszandile->next=thiszandile->next->next;
			}
		}
	}
	if (thiszandile->prev){
		if (thiszandile->prev->free){	
			merge(thiszandile->prev);
		}
	}
}


void* mm_malloc(size_t size)	
{
	struct ncza* newzandile;
	if (and){	
		struct zandile* finalzandile=and;
		newzandile=Free(finalzandile, size);
	}else{	
                newzandile=zandile2(NULL,size);
		and=newzandile;
	}
  return (void*) ((long) newzandile+BAR_SIZE);

}



void* mm_realloc(void* ptr, size_t size)
{
void *newzandilePtr = mm_malloc(size);	
	struct zandile* prevzandile = (struct zandile*) ptr - 1;	
  memcpy(newzandilePtr, ptr, prevzandile->size);	 
  mm_free(ptr);	
  return newzandilePtr;	


}

void mm_free(void* ptr)	
{
	struct zandile* thiszandile = (struct zandile*) ptr - 1;	
	thiszandile->free=true;	
	merge(thiszandile);	
}

