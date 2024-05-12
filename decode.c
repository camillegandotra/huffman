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
void help(void) {
  fprintf(stderr, "SYNOPSIS\n"
                  "  A Huffman decoder.\n"
                  "  Decompresses a file using the Huffman coding algorithm.\n"
                  "\n"
                  "USAGE\n"
                  "  ./decode [-h] [-i infile] [-o outfile]\n"
                  "\n"
                  "OPTIONS\n"
                  "  -h             Program usage and help.\n"
                  "  -v             Print compression statistics.\n"
                  "  -i infile      Input file to decompress.\n"
                  "  -o outfile     Output of decompressed data.\n");
  return;
}

// main : main function for decode
int main(int argc, char **argv) {
  int opt = 0;                 // Used to store the current user input
  int infile = STDIN_FILENO;   // Used to store the input file to decode
  int outfile = STDOUT_FILENO; // Used to store the output file to decode
  bool stats = 0; // Used to indicate if the user wants to print out the
                  // decompression stats

  while ((opt = getopt(argc, argv, OPTIONS)) !=
         -1) {     // Go in a loop to handle users input(s)
    switch (opt) { // Use switch to handle users input
    case 'i':      // User wants to specify the input file to decode
      // Opening input file for decoding
      infile = open(optarg, O_RDONLY);

      // Ensures file exists before we use it
      if (infile < 0) {
        fprintf(stderr,
                "decode: Couldn't open %s : No such file or directory\n",
                optarg);
        help();             // Print the programs synopsis and usage
        exit(EXIT_FAILURE); // Exit with non-zero exit code
      }
      break; // Break; ensures we only go through this case

    case 'o': // User wants to specify the output file to decode
      // Opening output file for decoding; will create if does not exist
      outfile = open(optarg, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
      break; // Break; ensures we only go through this case

    case 'v': // User wants to print out the decompression stats after program
      stats = 1;
      break; // Break; ensures we only go through this case

    case 'h':             // User wants to displays program synopsis and usage
      help();             // Call our help() function
      exit(EXIT_SUCCESS); // Exits indicating a successful termination
      break;              // Break; ensures we only go through this case

    default: // User had invalid command
      help();
      exit(EXIT_FAILURE); // Exit with non-zero exit code
      break;              // Break; ensures we only go through this case
    }
  }

  // If input comes from stdin, we will put input into a temp file first
  uint8_t buff = '\0';
  // Read all bytes from stdin and write to temp file
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

  // Getting our header from infile
  Header h;
  read_bytes(infile, (uint8_t *)&h, sizeof(h));

  // Verifying magic number
  if (h.magic != MAGIC) {
    // In the case of a non-matching magic number
    fprintf(stderr, "decode: Header doesn't match magic number\n");
    help();             // Print the programs synopsis and usage
    exit(EXIT_FAILURE); // Exit with non-zero exit code
  }

  // Changing permissions of outfile based on header
  fchmod(outfile, h.permissions);

  // Rebuilding our huffman tree based on tree size from header
  uint8_t tree[MAX_TREE_SIZE];
  Node *huff_tree = rebuild_tree(read_bytes(infile, tree, h.tree_size), tree);

  // Decoding bits to symbols
  uint8_t cur_bit; // Will contain the current bit from our in file
  Node *cur_node =
      huff_tree; // Create a node that starts from the root of our huffman tree

  // Start decoding the bits into symbols
  while (bytes_written <
         h.file_size) { // Since we know that the number of bytes written has to
                        // equal the number of bytes of the original file
    if (!read_bit(infile, &cur_bit)) { // If read_bit returns 0; there are no
                                       // more bits to read; break
      break;
    }
    if (cur_node->left == NULL &&
        cur_node->right == NULL) { // If the node is a leaf node
      write_bytes(outfile, &(cur_node->symbol),
                  1); // Write the symbol of the current node
      cur_node =
          huff_tree; // Reset our cur node to the root of our huffman tree
    }
    if (cur_bit) {
      cur_node = cur_node->right; // If the current bit is 1; traverse through
                                  // the cur Nodes right link
    } else {
      cur_node = cur_node->left; // If the current bit is 0; traverse through
                                 // the cur Nodes left link
    }
  }

  if (stats) { // If our user enabled verbose to print out stats
    uint64_t comp_size = bytes_read;
    uint64_t decomp_size = bytes_written;
    float space_saving = (100 * (1 - ((float)comp_size / decomp_size)));
    fprintf(stderr,
            "Compressed file size: %lu bytes\n"
            "Decompressed file size: %lu bytes\n"
            "Space saving: %0.2f%s\n",
            comp_size, decomp_size, space_saving, "%");
  }

  // Deleteing our huff_tree
  delete_tree(&huff_tree);

  // Closing infile and outfile
  close(infile);
  close(outfile);

  exit(EXIT_SUCCESS); // Exits indicating a successful termination
}
