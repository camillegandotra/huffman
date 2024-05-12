## Huffman

### Description

*encode* and *decode* programs effectively compress and decompress the size of files passed. This program will utilize Huffman encoding and decoding to do so. Throughout this assignment not only do we learn the importance of data sizes and the problems we encounter with it; but we come up with our huffman implementation that will make our file sizes significantly smaller which we have learned, goes a long way in the industry.

Note that this project was a programming assignment for Computer Systems and C Programming.


### Author

*Created By:* Camille Gandotra

### Usage

When downloaded; *preferably* with Linux OP and clang compiler; we can use our *enode* and *decode* program like so (in terminal; make sure you are in the right directory):

For *encode.c*:
```
./encode [-h] [-i infile] [-o outfile]

OPTIONS
  -h             Program usage and help.
  -v             Print compression statistics.
  -i infile      Input file to compress.
  -o outfile     Output of compressed data.

```

For *decode.c*:
```
./decode [-h] [-i infile] [-o outfile]

OPTIONS
  -h             Program usage and help.
  -v             Print compression statistics.
  -i infile      Input file to decompress.
  -o outfile     Output of decompressed data.
```

If you are having trouble running the program, refer to the commands below.

For *Makefile*:

The following command builds *encode* and *decode* (same as the command *make all*):
```
make
```

More Options:
```
make [options]

OPTIONS:
    encode : Builds the encode program.
    decode : Builds the decode program.
    clean : Removes all files that are compiler generated except the executable.
    spotless :  Removes all files that are compiler generated and the executable
    format : Formats all source code.
    all : Builds decode and encode.
```

This passes scan-build cleanly.

### Files

- ```encode.c``` - C program that contains the main() function for the encode program.
- ```decode.c``` - C program that contains the main() function for the decode program.
- ```defines.h``` - Header file that defines the macro definitions used throughout the assignment.
- ```header.h``` - Header file that contains the struct definition for a file header..
- ```node.c``` - C program that contains the implementation of my Node ADT.
- ```node.h``` - Header file that defines the interface for the Node ADT.
- ```pq.c``` - C program that contains the implementation of my Priority Queue ADT.
- ```pq.h``` - Header file that defines the interface for the Priority Queue ADT.
- ```code.c``` - C program that contains the implementation of the Code ADT.
- ```code.h``` - Header file that defines the interface for the Code ADT.
- ```io.c``` - C program that contains the implementation of the IO ADT.
- ```io.h``` - Header file that defines the interface for the IO ADT.
- ```stack.c``` - C program that contains the implementation of the Stack ADT.
- ```stack.h``` - Header file that defines the interface for the Stack ADT.
- ```huffman.c``` - C program that contains the implementation of the Huffman coding module.
- ```huffman.h``` - Header file that defines the interface for the Huffman coding module.
- ```Makefile``` - Directs the compilation process. Able to build decode and/or encode. Able to clean or remove all files that are compiler generated (with or without the executable). Also able to format all source code.
- ```README.md``` - Description of the assignment and files provided. Demonstrates how input files and what to expect for the output. Also shows how to change settings using *encode* and *decode*.


### Sources

GetOpt Manual: https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html  

