#include <stdlib.h>

#include "nlist.h"

/*

   Implementation details:

   The next-node of the last-node refers to the root-node. This way we can
   refer both to the first and last nodes in the list. This makes the list
   circular, so special care needs to be taken when traversing this list.

*/

static nlist_node_t* new_node( void* );

void nlist_init( nlist_t* list ) {
   list->last = 0;
   list->size = 0;
}

void nlist_append( nlist_t* list, void* data ) {
   nlist_node_t* node = new_node( data );
   if ( list->last ) {
      // Transfer the root node to the new node.
      node->next = list->last->next;
      // Link the new node with the last node. Even if the last node is the
      // root node, the next-node field of the root node will be updated
      // correctly. The new node will become the second node in the list.
      list->last->next = node;
      // The new node is now the last node.
      list->last = node;
   }
   else {
      // The first node is both the root and the last node. 
      node->next = node;
      list->last = node;
   }
   ++list->size;
}

void nlist_prepend( nlist_t* list, void* data ) {
   nlist_node_t* node = new_node( data );
   if ( list->last ) {
      // The old root node becomes the second node in the list.
      node->next = list->last->next;
      // The new node becomes the root node.
      list->last->next = node;
   }
   else {
      node->next = node;
      list->last = node;
   }
   ++list->size;
}

int nlist_size( nlist_t* list ) {
   return list->size;
}

void nlist_clear( nlist_t* list ) {
   if ( ! list->last ) {
      return;
   }

   nlist_node_t* node = list->last;
   while ( list->size ) {
      nlist_node_t* next_node = node->next;
      free( node );
      node = next_node;
      --list->size;
   }

   list->last = 0;
}

nlist_iterator_t nlist_iterate( nlist_t* list ) {
   nlist_iterator_t i = { 0, 0 };
   if ( list->size ) {
      i.curr = list->last->next, 
      i.last = list->last;
   };
   return i;
}

bool nlist_is_end( nlist_iterator_t* i ) {
   return ( i->last == 0 ); 
}

void nlist_next( nlist_iterator_t* i ) {
   if ( i->curr == i->last ) {
      i->last = 0;
   }
   else {
      i->curr = i->curr->next;
   }
}

void* nlist_idata( nlist_iterator_t* i ) {
   return i->curr->data;
}

nlist_node_t* new_node( void* data ) {
   nlist_node_t* node = malloc( sizeof( nlist_node_t ) );
   node->data = data;
   return node;
}