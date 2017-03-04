#ifndef LIB_NLIST_H
#define LIB_NLIST_H

#include <stdbool.h>

/*

   A linked list.

   TODO:

   - Implement remove() function.
   - Implement insert() function.
   - Maybe improve the iterator handling code.

*/

typedef struct nlist_node_t {
   struct nlist_node_t* next;
   void* data;
} nlist_node_t;

typedef struct {
   nlist_node_t* curr;
   nlist_node_t* last;
} nlist_iterator_t;

typedef struct {
   nlist_node_t* last;
   int size;
} nlist_t;

void nlist_init( nlist_t* );
void nlist_append( nlist_t*, void* data );
void nlist_prepend( nlist_t*, void* data );
int nlist_size( nlist_t* );
// Deletes all the nodes from the list.
void nlist_clear( nlist_t* );
nlist_iterator_t nlist_iterate( nlist_t* );
// Checks whether the iterator has gone past the list. Do NOT use the iterator
// when it has reached this point.
bool nlist_is_end( nlist_iterator_t* );
// Don't call this function if the iterator is at the end of the list.
void nlist_next( nlist_iterator_t* );
// Removes the node in the iterator from the list. The iterator will be updated
// to reflect the change. Other iterators in use from the same list may NOT be
// valid after this operation.
void nlist_remove( nlist_iterator_t* );
// Retrieves the data pointer in the current node of the iterator.
void* nlist_idata( nlist_iterator_t* );

#endif