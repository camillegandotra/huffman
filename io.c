// clang-format off
#include "io.h"			// IO header file
#include "defines.h"	// Defines header file

#include <string.h>     // Used for memset
#include <fcntl.h>		// Used for file functions
#include <unistd.h> 	// Used for functions
#include <stdint.h>		// Declares more integer types
#include <stdio.h>		// Used for input and output for our program
#include <stdlib.h>		// Used for macros and functions used in our program
// clang-format on

// Initializing our stats variables to 0
uint64_t bytes_read = 0;
uint64_t bytes_written = 0;

// read_bytes : Wrapper function that reads all nbytes from infile and stores
// the information in the passed buffer. Returns the number of bytes in our
// buffer.
int read_bytes(int infile, uint8_t *buf, int nbytes) {
  int b_read =
      0; // Temp variable to determine how many bytes read per function call
  while (b_read != nbytes) { // Read up to the number of bytes specified
    ssize_t ret = read(
        infile, buf + b_read,
        nbytes -
            b_read); // Read and places data into the next free spot in our
                     // passed buffer and only up to the number nbytes (total)
    if (ret == 0) { // If our call to read() returned 0, there are no more bytes
                    // to read from infile, break
      break;
    }
    b_read +=
        ret; // Increment our temp variable by the return value from read()
  }
  bytes_read +=
      b_read; // Once we are done reading, increment our stats variable by the
              // number of bytes read in this function call
  return b_read; // Return the number of bytes read in this function call
}

// write_bytes : Wrapper function that writes all nbytes from the buf to our
// file outfile. Returns the number of bytes written.
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
  int b_write =
      0; // Temp variable to determine how many bytes written per function call
  while (b_write != nbytes) { // Write up to the number of bytes specified
    ssize_t ret =
        write(outfile, buf + b_write,
              nbytes - b_write); // Write from the next data from our buffer and
                                 // only up to the number nbytes (total)
    if (ret == 0) { // If our call to write() returned 0, there are no more
                    // bytes to write to outfile, break
      break;
    }
    b_write +=
        ret; // Increment our temp variable by the return value from write()
  }
  bytes_written +=
      b_write; // Once we are done writing, increment our stats variable by the
               // number of bytes written in this function call
  return b_write; // Return the number of bytes written in this function call
}

static uint8_t r_buf =
    '\0'; // 1 Byte Buffer that gets contents from read_bytes and used in
          // read_bit to get individual bits from infile
static int read_bit_idx = 0; // Temp varbiable that indicates the current index
                             // of what bit to read from r_buf
static int read_byte_max =
    0; // Temp variable to hold the number of bytes in r_buf

// Got how to set/clear/get a bit from:
// https://stackoverflow.com/questions/47981/how-do-i-set-clear-and-toggle-a-single-bit

// read_bit : Function that reads out bits from a block of bytes which is
// collected from an infile
bool read_bit(int infile, uint8_t *bit) {
  if (read_bit_idx ==
      read_byte_max * 8) { // We are at the end of reading bits from r_buf
    read_byte_max = read_bytes(infile, &r_buf, 1); // Refill r_buf from infile
    read_bit_idx = 0;                              // Reset our bit index
    if (read_byte_max ==
        0) { // If the return value from read_bytes == 0; there are no more
             // bytes to read from infile, thus no more bits; return 0.
      return 0;
    }
  }
  uint8_t bit_num =
      read_bit_idx %
      8; // Finds the corresponding 'bit' the bit is in within r_buf
  *bit = (r_buf >> bit_num) & 1; // Gets the bit
  read_bit_idx += 1; // Increment out bit index counter for read_bit()
  return 1;
}

static uint8_t w_buf =
    '\0'; // 1 Byte Buffer that writes its contents to outfile and used in
          // write_code to set the individual bits of the buffer
static int write_bit_idx = 0; // Temp varbiable that indicates the current index
                              // of of what bit to write to in w_buf

// write_code : Function that writes out bits from a Code to outfile with the
// use of a buffer
void write_code(int outfile, Code *c) {
  for (uint32_t i = 0; i < code_size(c);
       i += 1) { // Iterate through the bits of the passed Code
    if (write_bit_idx == 8) {
      write_bytes(outfile, &w_buf,
                  1); // If our buffer is full, write the buffer to outfile
      memset(&w_buf, 0, 1);
      write_bit_idx = 0; // Reset out bit index
    }
    uint8_t bit_num =
        write_bit_idx %
        8; // Finds the corresponding 'bit' the bit is in within the 'block'
    if (code_get_bit(c, i)) {
      w_buf =
          w_buf | (1UL << bit_num); // If the bit at index i from our Code is 1,
                                    // set the buffer at our bit index to 1
    } // We dont have to worry about if the Code at index i is 0 because if it
      // is, the buffer at our bit index is already 0
    write_bit_idx += 1; // Increment our bit index counter for write_codes()
  }
  return;
}

// flush_codes : Function that writes out extra leftover bits in our buffer
void flush_codes(int outfile) {
  // Fill up the extra bits in our buffer with 0
  for (uint32_t i = write_bit_idx; i < 8; i += 1) {
    w_buf = w_buf & ~(1UL << i);
  }
  write_bytes(outfile, &w_buf, 1); // Write the current buffer to outfile
  return;
}
