// clang-format off
#include "node.h"		// Node header file

#include <ctype.h>		// Used to for functions for printing a Node
#include <stdint.h>		// Declares more integer types
#include <stdio.h>		// Used for input and output for our program
#include <stdlib.h>		// Used for macros and functions used in our program
// clang-format on

// node_create: Function that creates a Node (or null if error)
Node *node_create(uint8_t symbol, uint64_t frequency) {
  Node *n = (Node *)malloc(sizeof(Node));
  if (n != NULL) { // If malloc worked for our Node
    // Set left and right to Null
    n->left = NULL;
    n->right = NULL;
    // Set arguments passed
    n->symbol = symbol;
    n->frequency = frequency;
  }
  return n; // Returns Null if not created correctly, else returns the Node
}

// node_delete: Function that deletes and frees up mememory of the Node passed
void node_delete(Node **n) {
  (*n)->left = NULL;
  (*n)->right = NULL;
  free((*n)); // Frees the Node itself
  (*n) = NULL;
  return;
}

// node_join: Function that joins the left and right node via a created parent
// node which is then returned
Node *node_join(Node *left, Node *right) {
  uint64_t tot_freq =
      (left->frequency) +
      (right->frequency); // Total frequency = Frequency for parent
  Node *n = node_create('$', tot_freq); // Create parent node with symbol as '$'
  n->left = left;                       // Set the left node
  n->right = right;                     // Set the right node
  return n;
}

// node_print: Function Prints the Node (symbol and frequency)
void node_print(Node *n) {
  if (n != NULL) {
    if (!iscntrl(n->symbol) &&
        isprint(n->symbol)) { // Checks if the symbol is not a control character
                              // and that the symbol is printable
      printf("%c: %lu\n", n->symbol,
             n->frequency); // Print the cur node (symbol then frequency)
    } else { // Else if the symbol is not printable, print a statement
             // indicating so.
      printf("(Not Printable): %lu\n",
             n->frequency); // Print the cur node (symbol then frequency)
    }
  }
  return;
}

// node_cmp: Function that compares two nodes via their frequency. Returns 1 if
// n's frequency is more than m's frequency, else returns 0.
bool node_cmp(Node *n, Node *m) {
  if (n->frequency > m->frequency) {
    return 1;
  }
  return 0;
}

// node_print_sym: Function prints the Node (symbol)
void node_print_sym(Node *n) {
  if (n != NULL) {
    if (!iscntrl(n->symbol) &&
        isprint(n->symbol)) { // Checks if the symbol is not a control character
                              // and that the symbol is printable
      printf("%c\n", n->symbol); // Print the cur node (symbol)
    } else { // Else if the symbol is not printable, print a statement
             // indicating so.
      printf("(Not Printable)\n"); // Print the cur node (symbol)
    }
  }
  return;
}
