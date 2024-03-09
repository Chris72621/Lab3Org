#include <errno.h>   //errno
#include <stdint.h>  //uintptr_t
#include <stdio.h>   //printf() fprintf()
#include <stdlib.h>  //malloc() & free()
#include <string.h>  //strerror()

#include "my_string.h"
#include "process_file.h"
#include "tokenizer.h"

#define N_REGISTERS ((size_t)32)
#define MEM_SIZE ((size_t)1024)

// IMPORTANT 
uintptr_t r[N_REGISTERS];
uintptr_t pc;
unsigned char mem[MEM_SIZE];

// DOSENT SEEM IMPORTAJT
static void error_no_memory(void) {
  fprintf(stderr, "No more memory available. Error: %s\n", strerror(errno));
}

// JUST BACKGROUND STUFF BUT NEED TO UNDERSTAND
void init_memory_elements(void) {
  // Initialize registers
  for (size_t i = 0; i < N_REGISTERS; i++) r[i] = ((uintptr_t)0);

  // Initialize PC to 0
  pc = ((uintptr_t)0);

  // Initialize all memory to 0
  for (size_t i = 0; i < MEM_SIZE; ++i) mem[i] = ((unsigned char)0);

  // Set sp to be the top part of the memory 
  // THIS IS IMPORTANT I THINK THE OTHER REGISTERS ARE BASICALLY OPEN BUT THE SP
  r[2] = (uintptr_t)&mem[MEM_SIZE];
}

// WE CAN PUT OUR INSTRUCTIONS METHODS HERE
/* Methods for Instructions*/

  /*1) Loads CHRIS*/

  /*2)	Stores MARY */

  /*3)	Arithmetic SHARED */

  /*4)	Bitwise operators MARY*/

  /*5)	Pseudo-instructions CHRIS*/

  /*6)	Jump offset: CHRIS*/

  /*7)	Jump offset + Reg: MARY*/


/**
 * Fill out this function and use it to read interpret user input to execute
 * RV32 instructions. You may expect that a single, properly formatted RISC-V
 * instruction string will be passed as a parameter to this function.
 */ 
int interpret(char *instr) {
  char *instruction = instr;

  if (str_cmp(instruction, "LB") == 0) {
      // Process LB instruction 
  } else if (str_cmp(instruction, "LW") == 0) {
      // Process LW instruction 
  } else if (str_cmp(instruction, "SB") == 0) {
      // Process SB instruction 
  } else if (str_cmp(instruction, "SW") == 0) {
      // Process SW instruction 
  } else if (str_cmp(instruction, "ADD") == 0) {
      // Process ADD instruction 
  } else if (str_cmp(instruction, "ADDI") == 0) {
      // Process ADDI instruction 
  } else if (str_cmp(instruction, "SUB") == 0) {
      // Process SUB instruction 
  } else if (str_cmp(instruction, "XOR") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "XORI") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "SLLI") == 0) {
      // Process  instruction  
  } else if (str_cmp(instruction, "SRLI") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "MV") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "LI") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "NED") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "NOT") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "JAL") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "J") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "JALR") == 0) {
      // Process  instruction 
  } else if (str_cmp(instruction, "JR") == 0) {
      // Process  instruction 
  } else {
      printf("Invalid instruction\n");
      return 0;
  }

  return 1;
}

int main(int argc, char **argv) {
  FILE *file;
  char *buffer;

  if (argc != 2) {
    fprintf(stderr, "Only two parameters must be passed.\n");
    return 1;
  }

  if (process_file(argv[1])) return 1;
  if (open_file()) return 1;

  // Initialize PC, registers and memory
  init_memory_elements();

  buffer = (char *)malloc((LINE_SIZE + 1) * sizeof(char));

  if (buffer == NULL) {
    close_file();
    return 1;
  }

  /* --- Your code starts here. --- */

  // For each line in the file, interpret it.
  // NOTE: Use get_line() function in process_file.h

  /* --- Your code ends here. --- */

  close_file();
  free(buffer);

  return 0;
}
