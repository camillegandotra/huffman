// clang-format off
#include "huffman.h"	// Huffman header file
#include "io.h"		    // IO header file
#include "node.h"		// Node header file
#include "pq.h"		    // PQ header file
#include "code.h"		// Code header file
#include "stack.h"	    // Code header file

#include <stdint.h>	    // Declares more integer types
#include <stdio.h>	    // Used for input and output for our program
#include <stdlib.h>	    // Used for macros and functions used in our program
// clang-format on

// build_tree : Function that builds a Huffman tree given a histogram
Node *build_tree(uint64_t hist[static ALPHABET]) {
  // Creating our PQ
  PriorityQueue *q = pq_create(ALPHABET);
  // Iterating our Histogram and adding symbols with frequencies > 0 to our PQ
  // as Nodes
  for (uint64_t i = 0; i < ALPHABET; i += 1) {
    if (hist[i] > 0) {
      Node *cur = node_create(i, hist[i]);
      enqueue(q, cur);
    }
  }
  // While there are two or more nodes in our PQ
  while (pq_size(q) >= 2) {
    Node *left;
    Node *right;
    dequeue(q, &left);                     // First dequeue is our left child
    dequeue(q, &right);                    // Second dequeue is our right child
    Node *joined = node_join(left, right); // Join the two
    enqueue(q, joined);                    // Enqueue the joined parent
  }
  // After our loop there is one node left in our PQ
  Node *ret;
  dequeue(q, &ret); // This will be the root of our tree
  pq_delete(&q);    // Delete our PQ
  return ret;       // Return the root of our tree
}

static Code c;       // Temp code to hold current path
static uint8_t temp; // Temp bit to hold the return of code_pop_bit
static uint8_t initialize_c =
    1; // Will act as a boolean variable to get our temp code initialized

// build_codes : Function that populates a code table
void build_codes(Node *root, Code table[static ALPHABET]) {
  if (initialize_c) { // Initializes our temp code once
    c = code_init();
    initialize_c = 0;
  }
  if (root != NULL) {
    if (root->left == NULL &&
        root->right == NULL) { // If we are at a leaf node, add the temp code to
                               // our code table
      table[root->symbol] = c;
    } else {
      code_push_bit(&c, 0);            // 0 indicates traversing left link
      build_codes(root->left, table);  // Recursive call to left link
      code_pop_bit(&c, &temp);         // Done visiting left link, pop the 0 bit
      code_push_bit(&c, 1);            // 1 indicates traversing right link
      build_codes(root->right, table); // Recursive call to right link
      code_pop_bit(&c, &temp); // Done visiting right link, pop the 1 bit
    }
  }
  return;
}

// dump_tree : Function that writes the bytes of the symbols of the Nodes from
// our Huffman tree to outfile
void dump_tree(int outfile, Node *root) {
  uint8_t l = 'L'; // Will be used to indicate that the bytes after are the
                   // symbol of a leaf node
  uint8_t i = 'I'; // WIll be used to indicate that we are at an interior node
  if (root != NULL) {                // Post traversal iteration
    dump_tree(outfile, root->left);  // Recursive call to left link
    dump_tree(outfile, root->right); // Recursive call to right link
    if (root->left == NULL && root->right == NULL) { // If we are at a leaf node
      write_bytes(outfile, &l, 1);
      write_bytes(outfile, &root->symbol, 1);
    } else { // If we are at an interior node
      write_bytes(outfile, &i, 1);
    }
  }
  return;
}

// rebuild_tree : Function that will rebuild our Huffman tree from our dumped
// tree
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
  // Create our stack
  Stack *s = stack_create(nbytes);

  // Node place holders
  Node *cur;
  Node *left;
  Node *right;

  // Iterate through our dumped tree
  for (uint16_t i = 0; i < nbytes; i += 1) {
    if (tree[i] == 'L') { // Indicates that the next element will be a symbol
                          // for a leaf node
      i += 1;             // Increment counter to go to next element
      cur = node_create(tree[i], 0); // Create a node with that symbol
      stack_push(s, cur);            // Push the node onto our stack
    } else if (tree[i] ==
               'I') { // Indicates that we have encountered an interior node.
      stack_pop(s, &right); // First pop off the stack is our right child
      stack_pop(s, &left);  // Second pop off the stack is our left child
      cur = node_join(left, right); // Join the two children
      stack_push(s, cur);           // Push the parent onto the stack
    }
  }
  // Last node on our stack is the root of our huffman tree
  stack_pop(s, &cur);
  stack_delete(&s);
  return cur;
}

// delete_tree : Function that acts as a destructor for a Huffman tree
void delete_tree(Node **root) {
  if (*root) {
    // Post-order traversal to free all nodes
    delete_tree(&(*root)->left);
    delete_tree(&(*root)->right);
    node_delete(root);
  }
  (*root) = NULL; // Setting our pointer to NULL
  return;
}
