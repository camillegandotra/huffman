// clang-format off
#include "pq.h"			// PQ header file
#include "node.h"		// Node header file

#include <stdint.h>		// Declares more integer types
#include <stdio.h>		// Used for input and output for our program
#include <stdlib.h>		// Used for macros and functions used in our program
// clang-format on

// Using a Insertion Sort for our Priority Queue
// Used the following link to understand how to use Insertion Sort:
// https://www.youtube.com/watch?v=JU767SDMDvA

void insertion_sort(Node **arr, uint32_t size) {
  uint32_t i, j;
  for (i = 1; i < size;
       i +=
       1) { // Iterate through all the elemtents in array (starting at index 1)
    j = i;  // Start j from i
    while (j > 0 &&
           node_cmp(arr[j - 1],
                    arr[j])) { // While node at j - 1 > j , we swap them
      Node *temp = arr[j];
      arr[j] = arr[j - 1];
      arr[j - 1] = temp;
      j -= 1; // Go through all j from [i->0]
    }
  }
  return;
}

typedef struct PriorityQueue PriorityQueue;

// Priority Queue Struct, using Insertion Sort
struct PriorityQueue {
  Node **arr;        // Array implementation
  uint32_t capacity; // Max capacity of our PQ
  uint32_t size;     // Current size of our PQ
};

// pq_create: Function that creates our PQ which utilizes insertion sort
PriorityQueue *pq_create(uint32_t capacity) {
  PriorityQueue *q = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  if (q != NULL) { // If malloc worked for our PQ
    q->arr = (Node **)malloc(capacity *
                             sizeof(Node)); // malloc space for our min array
    if (q->arr == NULL) {
      free(q);
      q = NULL;
      return q;
    }
    q->size = 0;            // Initiallize size to be 0
    q->capacity = capacity; // Set the capacity to what is passed
  }
  return q; // Returns Null if not created correctly, else returns the PQ
}

// pq_delete: Function that deletes the passed PQ
void pq_delete(PriorityQueue **q) {
  free((*q)->arr); // Free Array of Nodes for PQ
  (*q)->arr = NULL;
  free(*q); // Free our PQ
  (*q) = NULL;
  return;
}

// pq_size: Function that returns the current size of our PQ
uint32_t pq_size(PriorityQueue *q) { return q->size; }

// pq_empty: Function that returns TRUE/1 if our priority queue is empty. Else,
// returns FALSE/0
bool pq_empty(PriorityQueue *q) {
  if (pq_size(q) == 0) {
    return 1;
  }
  return 0;
}

// pq_full: Function thats returns TRUE/1 is our priority queue is full. Else
// returns FALSE/0
bool pq_full(PriorityQueue *q) {
  if (pq_size(q) == q->capacity) {
    return 1;
  }
  return 0;
}

// enqueue: Function that enqueues a Node n to our PQ. Returns FALSE/0 if any
// error, Else returns TRUE/1
bool enqueue(PriorityQueue *q, Node *n) {
  if (pq_full(q)) { // If full, cant enqueue anything
    return 0;
  }
  q->arr[q->size] = n;             // Place our node
  q->size += 1;                    // Increment size
  insertion_sort(q->arr, q->size); // Sort our array
  return 1;
}

// dequeue: Function that dequeues a Node n to our PQ. Returns FALSE/0 if any
// error, Else returns TRUE/1
bool dequeue(PriorityQueue *q, Node **n) {
  if (pq_empty(q)) { // If empty, cant dequeue anything
    return 0;
  }
  *n = q->arr[0];                  // Get our node
  q->arr[0] = q->arr[q->size - 1]; // Replace node at arr[0]
  q->size -= 1;                    // Decrease size
  insertion_sort(q->arr, q->size); // Sort our array
  return 1;
}

// pq_print: Function that prints our array
void pq_print(PriorityQueue *q) {
  for (uint32_t i = 0; i < q->size; i += 1) { // Traverse array, print each node
    printf("Index: %d, Node: ", i);
    node_print(q->arr[i]);
  }
  return;
}
