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


// ADD X# WHEN EVER NEEDED SP 
int get_r_index(const char *reg_name) {
    if (str_cmp(reg_name, "X0") == 0) return 0;
    else if (str_cmp(reg_name, "X1") == 0) return 1;
    else if (str_cmp(reg_name, "X7") == 0) return 7;
    else if (str_cmp(reg_name, "RA") == 0) return 31; 
    else if (str_cmp(reg_name, "SP") == 0) return 2;
    else {
        // IF NOT A REG THEN ITS A IMM
        int imm = atoi(reg_name);
        if(imm == 0) {
            return -1;
        }  else {
            return imm;
        }
    }
}
// WE CAN PUT OUR INSTRUCTIONS METHODS HERE
/* Methods for Instructions*/

/*1) Loads CHRIS*/
// LB X7,1000(X5)
int LB (char *instr) {
    int rd, rs1, imm;

    if (sscanf(instr, "LB X%d,%d(X%d)", &rd, &imm, &rs1) != 3) {
        return 0;
    };
    
    int address = r[rs1] + imm;

    if (address < 0 || address >= MEM_SIZE) {
        return 0;
    }
    
    unsigned char byte = mem[address]; // M[RS1+IMM][0:7]
    r[rd] = byte; // RS <- byte
    return 1;
}
// LW RD,RS1,IMM  LW RA,28(SP)  RS <- M[RS1+IMM][0:31]
int LW (char *instr) {
    int rd, rs1, imm;
    char *token;
    token = tokenize(instr," ,()");

    token = tokenize(NULL," ,()");
    rd = get_r_index(token);

    token = tokenize(NULL," ,()");
    imm = get_r_index(token);

    if (imm == -1) {
        return 0;
    };

    token = tokenize(NULL," ,()");
    rs1 = get_r_index(token);

    int address = r[rs1] + imm;

    if (address < 0 || address >= MEM_SIZE) {
        return 0;
    }

    char byte = mem[address];
    r[rd] = byte;
    return 1;
}

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
  int pass = 0; // TO CHECK IN INSTRUCTION WORKED AT THE END

  if (str_cmp(instruction, "LB") == 0) {
      pass = LB(instr);
  } else if (str_cmp(instruction, "LW") == 0) {
      pass = LW(instr);
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

  if (pass == 1) {
    return 1;
  };

  return 0;
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
