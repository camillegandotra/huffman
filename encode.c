// clang-format off
#include "io.h"		      // IO Header File
#include "huffman.h"	  // Huffman header file
#include "code.h"	      // Code header file
#include "defines.h"	  // Defines Header File
#include "header.h"	    // Headers Header File

#include <fcntl.h>	    // Used for file functions
#include <sys/stat.h>	  // Used for getting permission bits
#include <stdint.h>	    // Declares more integer types
#include <stdio.h>	    // Used for input and output for our program
#include <stdlib.h>	    // Used for macros and functions used in our program
#include <unistd.h>	    // Used for types and functions for our program
// clang-format on

#define OPTIONS "hi:o:v" // Valid User commands

// help : Help message that displayes program synopsis and usage; prints to
// stderr
void help(void) { // Help message that displayes program synopsis and usage
  fprintf(stderr, "SYNOPSIS\n"
                  "  A Huffman encoder.\n"
                  "  Compresses a file using the Huffman coding algorithm.\n"
                  "\n"
                  "USAGE\n"
                  "  ./encode [-h] [-i infile] [-o outfile]\n"
                  "\n"
                  "OPTIONS\n"
                  "  -h             Program usage and help.\n"
                  "  -v             Print compression statistics.\n"
                  "  -i infile      Input file to compress.\n"
                  "  -o outfile     Output of compressed data.\n");
  return;
}

// main : main function for encode
int main(int argc, char **argv) {
  int opt = 0;                 // Used to store the current user input
  int infile = STDIN_FILENO;   // Used to store the input file to encode
  int outfile = STDOUT_FILENO; // Used to store the output file to encode
  bool stats = 0; // Used to indicate if the user wants to print out the
                  // compression stats

  while ((opt = getopt(argc, argv, OPTIONS)) !=
         -1) {     // Go in a loop to handle users input(s)
    switch (opt) { // Use switch to handle users input
    case 'i':      // User wants to specify the input file to encode
      // Opening input file for encoding
      infile = open(optarg, O_RDONLY);

      // Ensures file exists before we use it
      if (infile < 0) {
        fprintf(stderr,
                "encode: Couldn't open %s : No such file or directory\n",
                optarg);
        help();             // Print the programs synopsis and usage
        exit(EXIT_FAILURE); // Exit with non-zero exit code
      }

      break; // Break; ensures we only go through this case

    case 'o': // User wants to specify the output file to encode
      // Opening output file for encoding, will create if does not exist
      outfile = open(optarg, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
      break; // Break; ensures we only go through this case

    case 'v': // User wants to print out the compression stats after program
      stats = 1;
      break; // Break; ensures we only go through this case

    case 'h':             // Displays program synopsis and usage
      help();             // Call our help() function
      exit(EXIT_SUCCESS); // Exits indicating a successful termination
      break;              // Break; ensures we only go through this case

    default: // User had invalid command
      help();
      exit(EXIT_FAILURE); // Exit with non-zero exit code
      break;              // Break; ensures we only go through this case
    }
  }

  // Creating our histogram
  uint64_t hist[ALPHABET];

  // If input comes from stdin, we will put input into a temp file first
  uint8_t buff = '\0';
  if (infile == STDIN_FILENO) {
    FILE *temp_file =
        tmpfile(); // Referenced :
                   // https://www.tutorialspoint.com/c_standard_library/c_function_tmpfile.htm
    int temp = fileno(
        temp_file); // Referenced :
                    // https://man7.org/linux/man-pages/man3/fileno.3.html
    while (read_bytes(infile, &buff, 1) > 0) {
      write_bytes(temp, &buff, 1);
    }
    free(temp_file);
    infile = temp;
  }
  // Reset stats
  bytes_read = 0;
  bytes_written = 0;

  // Starting at the beginning of infile
  lseek(infile, 0, SEEK_SET);

  int uniq_sym = 0; // Unique symbol counter

  // Reading our infile to fill our histogram
  while (read_bytes(infile, &buff, 1) > 0) {
    if (hist[buff] == 0) { // Increment unique symbol counter
      uniq_sym += 1;
    }
    hist[buff] += 1; // Increment histogram
  }

  // Setting the first two symbols (if not set)
  if (hist[0] == 0) {
    uniq_sym += 1; // Increment unique symbol counter
    hist[0] = 1;   // Increment histogram
  }
  if (hist[1] == 0) {
    uniq_sym += 1; // Increment unique symbol counter
    hist[1] = 1;   // Increment histogram
  }

  // Build our Huffman Tree
  Node *huff_tree = build_tree(hist);

  // Creating our Code Table
  Code code_table[ALPHABET];
  for (int i = 0; i < ALPHABET; i += 1) {
    code_table[i] =
        code_init(); // Initialize our table with codes at every index
  }

  // Filling up our Code Table
  build_codes(huff_tree, code_table);

  // Building our Header
  Header h;
  // Setting magic number field
  h.magic = MAGIC;
  // Getting permission bits of infile
  struct stat s_buff;
  fstat(infile, &s_buff);
  // Setting permissions
  h.permissions = s_buff.st_mode;
  // Set same permissions to outfile
  fchmod(outfile, h.permissions);
  // Setting tree_size
  h.tree_size = ((3 * uniq_sym) - 1);
  // Setting file_size
  h.file_size = s_buff.st_size;

  // Writing our header to outfile
  write_bytes(outfile, (uint8_t *)&h, sizeof(h));

  // Writing our our huffman tree to outfile
  dump_tree(outfile, huff_tree);

  // Starting at the beginning of infile
  lseek(infile, 0, SEEK_SET);

  // Reset our buffer
  buff = '\0';

  // Writing each code for each symbol to outfile from infile
  while (read_bytes(infile, &buff, 1) > 0) {
    write_code(outfile, &code_table[buff]);
  }

  // Flush remaining codes
  flush_codes(outfile);

  if (stats) { // If our user enabled verbose to print out stats
    uint64_t comp_size = bytes_written;
    uint64_t uncomp_size =
        bytes_read / 2; // Take half because we read through the file twice
    float space_saving = (100 * (1 - ((float)comp_size / uncomp_size)));
    fprintf(stderr,
            "Uncompressed file size: %lu bytes\n"
            "Compressed file size: %lu bytes\n"
            "Space saving: %0.2f%s\n",
            uncomp_size, comp_size, space_saving, "%");
  }

  // Deleteing our huff_tree
  delete_tree(&huff_tree);

  // Closing infile and outfile
  close(infile);
  close(outfile);

  exit(EXIT_SUCCESS); // Exits indicating a successful termination
}
