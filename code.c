// clang-format off
#include "code.h"		// Code header file
#include "defines.h"	// Defines header file

#include <string.h>		// Used for memset
#include <stdint.h>		// Declares more integer types
#include <stdio.h>		// Used for input and output for our program
#include <stdlib.h>		// Used for macros and functions used in our program
// clang-format on

// Got how to set/clear/get a bit from:
// https://stackoverflow.com/questions/47981/how-do-i-set-clear-and-toggle-a-single-bit

// code_init: Function that creates a Code
Code code_init(void) {
  Code c;
  (&c)->top = 0; // Setting top to 0; top indicates the next avaliable space.
  memset((&c)->bits, 0, MAX_CODE_SIZE); // Zeroing out the array
  return c;
}

// code_size: Function that returns the size of the Code.
uint32_t code_size(Code *c) {
  return c->top; // Return the top number because top indicates the index of the
                 // stack where the top of the stack resides which is also the
                 // size of the stack
}

// code_empty: Functcion that returns TRUE/1 if the stack is empty. Else it
// returns FALSE/0
bool code_empty(Code *c) {
  if (code_size(c) == 0) {
    return 1; // If the top component is 0, this indicates that the stack empty
  }
  return 0;
}

// code_full: Function that returns TRUE/1 if the stack is full. Else it returns
// FALSE/0
bool code_full(Code *c) {
  if (code_size(c) == MAX_CODE_SIZE) {
    return 1; // If the top component is MAX_CODE_SIZE - 1, this indicates that
              // the stack full
  }
  return 0;
}

// code_set_bit: Function that will set the bit at index i. Returns TRUE/1 if
// able to set. Else it returns FALSE/0
bool code_set_bit(Code *c, uint32_t i) {
  if (0 < i < code_size(c)) { // If the index i is in range
    uint8_t byte = i / 8;     // Finds what 'block' the bit is in
    uint8_t bit =
        i % 8; // Finds the corresponding 'bit' the bit is in within the 'block'
    c->bits[byte] =
        c->bits[byte] | (1UL << bit); // Ensures the bit is now set to 1
    return 1;
  }
  return 0; // i was not in range
}

// code_clr_bit: Function that will set the bit at index i. Returns TRUE/1 if
// able to clear. Else it returns FALSE/0
bool code_clr_bit(Code *c, uint32_t i) {
  if (0 < i < code_size(c)) { // If the index i is in range
    uint8_t byte = i / 8;     // Finds what 'block' the bit is in
    uint8_t bit =
        i % 8; // Finds the corresponding 'bit' the bit is in within the 'block'
    c->bits[byte] =
        c->bits[byte] & ~(1UL << bit); // Ensures the bit is now set to 0
    return 1;
  }
  return 0; // i was not in range
}

// code_get_bit: Function that will get the bit at index i. Returns TRUE/1 if
// the bit at i is 1. Else it returns FALSE/0
bool code_get_bit(Code *c, uint32_t i) {
  if (0 < i < code_size(c)) { // If the index i is in range
    uint8_t byte = i / 8;     // Finds what 'block' the bit is in
    uint8_t bit =
        i % 8; // Finds the corresponding 'bit' the bit is in within the 'block'
    if (((c->bits[byte] >> bit) & 1) == 1) { // If the bit is 1, return 1
      return 1;
    }
  }
  return 0; // i was not in range or the bit at i == 0
}

// code_push_bit: Function that pushes a bit into the Code. Reuturns TRUE/1 if
// able to push bit into stack. Else it returns FALSE/0
bool code_push_bit(Code *c, uint8_t bit) {
  if (code_full(
          c)) { // If our stack is full, we can't push anymore bits, return 0.
    return 0;
  }
  uint32_t push = code_size(c); // Get the location where we will push the bit
  c->top += 1;                  // Increment our top component
  if (bit == 0) {
    code_clr_bit(c, push); // Clear the bit if we want to push a 0 bit
  } else {
    code_set_bit(c, push); // Set the bit if we want to push a 1 bit
  }
  return 1; // Return 1 because we pushed a bit
}

// code_pop_bit: Function that pops a bit from our Code. Returns TRUE/1 if able
// to pop bit out of stack. Else it returns FALSE/0
bool code_pop_bit(Code *c, uint8_t *bit) {
  if (code_empty(
          c)) { // If our stack is empty, there are no bits to pop, return 0.
    return 0;
  }
  uint32_t pop = code_size(c) - 1; // Get the location where we will pop the bit
  *bit = code_get_bit(c, pop);     // Pop the bit using our get function
  code_clr_bit(c, pop);            // clear the bit location
  c->top -= 1;                     // Decrease our top by 1
  return 1;                        // Return 1 because we popped a bit
}

// code_pop_bit: Function that will iterate through our code from 0 -> TOP
void code_print(Code *c) {
  for (uint32_t i = 0; i < code_size(c); i += 1) {
    printf("%d", code_get_bit(c, i));
    if ((i + 1) % 8 == 0) {
      printf(" "); // Spaces between each byte
    }
  }
  printf("\n"); // Terminating new line
  return;
}
