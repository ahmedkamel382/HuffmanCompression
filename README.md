# Huffman Coding File Compression and Decompression
This project implements Huffman coding, a lossless data compression algorithm that assigns variable-length codes to characters based on their frequencies. The system efficiently compresses and decompresses files by constructing a Huffman tree and using the generated Huffman codes.
## Features
- File Compression: Reads an input file, analyzes character frequencies, builds the Huffman tree, generates codes, and writes the compressed data to an output file.
- File Decompression: Reconstructs the original file using the Huffman tree, restoring the compressed data to its original form.
- Optimized Storage: Utilizes a priority queue for optimal code generation, ensuring efficient data compression.
## Components
- Priority Queue: Manages nodes based on character frequency for optimal Huffman tree construction.
- Frequency Analysis: Determines the frequency of characters in a file, preparing them for encoding.
- Huffman Tree Construction: Builds the Huffman tree and generates unique binary codes for each character.
- Compression & Decompression: Provides functions for encoding and decoding files using Huffman codes.
## Usage
- Compile the code using a C++ compiler.
- Run the program and input the file name when prompted.
- The program generates:
- compressed.txt – The compressed output file.
- decompressed.txt – The decompressed version of the original file.

