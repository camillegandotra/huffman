// clang-format off
#include "stack.h"	    // Stack header file
#include "node.h"		// Node header file

#include <stdint.h>	    // Declares more integer types
#include <stdio.h>	    // Used for input and output for our program
#include <stdlib.h>	    // Used for macros and functions used in our program
// clang-format on

// Stack Struct
struct Stack {
  uint32_t top;
  uint32_t capacity;
  Node **items;
};

// stack_create : Constructor for a stack. Returns the created stack or NULL if
// error.
Stack *stack_create(uint32_t capacity) {
  Stack *s = (Stack *)malloc(sizeof(Stack));
  if (s != NULL) { // If malloc worked for our Stack
    s->items = (Node **)malloc(capacity *
                               sizeof(Node)); // Allocate memory for our items
    if (s->items ==
        NULL) { // If we had a problem allocating memory for our items, free our
                // Stack component and return it as NULL.
      free(s);
      s = NULL;
      return s;
    }
    s->top = 0;
    // Set arguments passed
    s->capacity = capacity;
  }
  return s; // Returns Null if not created correctly, else returns the Stack
}

// stack_delete : Function that deletes our stack
void stack_delete(Stack **s) {
  free((*s)->items); // Frees our items
  (*s)->items = NULL;
  free(*s); // Frees our Stack
  (*s) = NULL;
  return;
}

// stack_empty : Function that returns TRUE/1 is our stack is empty. Else
// returns FALSE/0.
bool stack_empty(Stack *s) {
  if (stack_size(s) == 0) { // If the size of our stack == 0, return 1
    return 1;
  }
  return 0; // Else return 0
}

// stack_full : Function that returns TRUE/1 is our stack is full. Else returns
// FALSE/0.
bool stack_full(Stack *s) {
  if (stack_size(s) ==
      s->capacity) { // If the size of our stack == capacity, return 1
    return 1;
  }
  return 0; // Else return 0
}

// stack_full : Function that returns the size of our stack or how many nodes
// are in the stack.
uint32_t stack_size(Stack *s) {
  return s->top; // Since our top tracker == the number of Nodes on the stack
}

// stack_push : Function that pushes a node onto the stack. Returns TRUE/1 if
// able to. Else returns FALSE/0.
bool stack_push(Stack *s, Node *n) {
  if (stack_full(s) ==
      1) { // If our stack is full, we can't push anything onto it
    return 0;
  }
  s->items[s->top] = n; // Push a node onto the top of our stack
  s->top += 1;          // Increment the top
  return 1;
}

// stack_pop : Function that pops a node from the stack. Returns TRUE/1 if able
// to. Else returns FALSE/0.
bool stack_pop(Stack *s, Node **n) {
  if (stack_empty(s) ==
      1) { // If our stack is wmpty, we can't pop anything from our stack
    return 0;
  }
  *n = s->items[s->top - 1]; // Pop a node from our stack and store it into the
                             // passed node
  s->items[s->top - 1] = NULL;
  s->top -= 1; // Decrement the top
  return 1;
}

// stack_print : Function that prints the Nodes of the stack.
void stack_print(Stack *s) {
  for (uint32_t i = 0; i < stack_size(s); i += 1) { // Iterate through the stack
    node_print(s->items[i]);
  }
  printf("\n");
  return;
}
