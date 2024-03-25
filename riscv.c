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

uintptr_t r[N_REGISTERS];
int32_t pc;
unsigned char mem[MEM_SIZE];

static void error_no_memory(void) {
  fprintf(stderr, "No more memory available. Error: %s\n", strerror(errno));
}

static int print_registers (char *fd_name) {
    FILE *fptr;
    if ((fptr = fopen(fd_name, "w")) == NULL) return 1;
    // Print all registers
    fprintf(fptr, "Registers: \n");
    for (size_t i = 0; i < N_REGISTERS; i++)
        fprintf(fptr, "X[%zu] = %u \n", i, r[i]);
    return 0;
}

void init_memory_elements(void) {
  // Initialize registers
  for (size_t i = 0; i < N_REGISTERS; i++) r[i] = ((uintptr_t)0);

  // Initialize PC to 0
  pc = ((uintptr_t)0);

  // Initialize all memory to 0
  for (size_t i = 0; i < MEM_SIZE; ++i) mem[i] = ((unsigned char)0);

  // Set sp to be the top part of the memory 
  // THIS IS IMPORTANT I THINK THE OTHER REGISTERS ARE BASICALLY OPEN BUT THE SP
  r[2] = MEM_SIZE;
}

int is_digit(char character) {
    return character >= '0' && character <= '9';
}

// Custom function to parse immediate value string to integer because we can't use atoi()
int parseImmediate(const char *immediate) {
    int result = 0;
    int sign = 1;

    // Handle negative sign
    if (*immediate == '-') {
        sign = -1;
        immediate++; // Move to next character
    }

    printf("Immediate string after sign handling: %s\n", immediate);

    // Parse digits
    while (*immediate >= '0' && *immediate <= '9') {
        result = result * 10 + (*immediate - '0');
        printf("Current result: %d\n", result);
        immediate++;
    }

    printf("Final result before sign adjustment: %d\n", result);
    printf("Final result after sign adjustment: %d\n", result * sign);
    return (result * sign);
}

// COVERS ACTUAL REGISTER NAMES EX. X1
int get_r_index_extended(const char *reg_name) {
    int result = 0;
    int i = 0;

    // Finds position of X, 
    while (reg_name[i] != '\0' && !is_digit(reg_name[i])) {
        i++;
    }   

    // Finds the digit and converts to int 
    while (reg_name[i] != '\0' && is_digit(reg_name[i])) {
        result = result * 10 + (reg_name[i] - '0'); 
        i++;
    }

    // if i == 0 then only number will be found meaning that its an imm
    // Otherwise, will return the register number
    return result;
}

// COVERS NAME OF THE REGISTERS
int get_r_index(char *reg_name) {
    if (str_cmp(reg_name, "zero") == 0) return 0;
    else if (str_cmp(reg_name, "RA") == 0)  return 1; 
    else if (str_cmp(reg_name, "SP") == 0)  return 2;
    else if (str_cmp(reg_name, "GP") == 0)  return 3; 
    else if (str_cmp(reg_name, "TP") == 0)  return 4; 
    else if (str_cmp(reg_name, "T0") == 0)  return 5; 
    else if (str_cmp(reg_name, "T1") == 0)  return 6; 
    else if (str_cmp(reg_name, "T2") == 0)  return 7; 
    else if (str_cmp(reg_name, "S0") == 0)  return 8; 
    else if (str_cmp(reg_name, "S1") == 0)  return 9; 
    else if (str_cmp(reg_name, "A0") == 0)  return 10;
    else if (str_cmp(reg_name, "A1") == 0)  return 11; 
    else if (str_cmp(reg_name, "A2") == 0)  return 12; 
    else if (str_cmp(reg_name, "A3") == 0)  return 13; 
    else if (str_cmp(reg_name, "A4") == 0)  return 14; 
    else if (str_cmp(reg_name, "A5") == 0)  return 15; 
    else if (str_cmp(reg_name, "A6") == 0)  return 16; 
    else if (str_cmp(reg_name, "A7") == 0)  return 17; 
    else if (str_cmp(reg_name, "S2") == 0)  return 18; 
    else if (str_cmp(reg_name, "S3") == 0)  return 19; 
    else if (str_cmp(reg_name, "S4") == 0)  return 20; 
    else if (str_cmp(reg_name, "S5") == 0)  return 21; 
    else if (str_cmp(reg_name, "S6") == 0)  return 22; 
    else if (str_cmp(reg_name, "S7") == 0)  return 23; 
    else if (str_cmp(reg_name, "S8") == 0)  return 24; 
    else if (str_cmp(reg_name, "S9") == 0)  return 25; 
    else if (str_cmp(reg_name, "S10") == 0) return 26; 
    else if (str_cmp(reg_name, "S11") == 0) return 27;
    else if (str_cmp(reg_name, "T3") == 0)  return 28; 
    else if (str_cmp(reg_name, "T4") == 0)  return 29; 
    else if (str_cmp(reg_name, "T5") == 0)  return 30; 
    else if (str_cmp(reg_name, "T6") == 0)  return 31; 
    else { return get_r_index_extended(reg_name); }
    return -1;
} 

/*1) Loads CHRIS*/
// LB X7,1000(X5)
int LB (char **tokens) {
    int rd, rs1, imm;

    rd = get_r_index(tokens[1]);
    imm = parseImmediate(tokens[2]);
    rs1 = get_r_index(tokens[3]);

    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    printf("rs1: %u\n", rs1);
    printf("imm: %u\n", imm);
    
    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || imm == -1) {
        return 0;
    }
        
    uintptr_t address = r[rs1] + imm;
    
    //if (address < 0 || address >= MEM_SIZE) {
        //return 0;
    //}
    
    //unsigned char byte = mem[address]; // M[RS1+IMM][0:7]
    r[rd] = (int32_t)(int8_t)mem[address]; // RS <- byte
    
    printf("value in rd: %u \n", r[rd]);
    return 1;
}

// LW RD,RS1,IMM  LW RA,28(SP)  RS <- M[RS1+IMM][0:31]
int LW (char **tokens) {
    int rd, rs1, imm;

    rd = get_r_index(tokens[0]);
    imm = get_r_index(tokens[1]);
    rs1 = get_r_index(tokens[2]);

    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || imm == -1) {
        return 0;
    }

    uintptr_t address = (uintptr_t)r[rs1] + (uintptr_t)imm;
    if (address < 0 || address >= MEM_SIZE) {
        return 0;
    }

    uintptr_t byte = &mem[address];
    r[rd] = byte;
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    return 1;
}

/*2)	Stores MARY */
//SB A0,0(SP)
//M[RS1+IMM][0:15] <- RS2[0:7]
int SB (char **tokens){
    int rd, rs1, imm;
 
    rd = get_r_index(tokens[0]);
    imm = get_r_index(tokens[1]);
    rs1 = get_r_index(tokens[2]);
    
    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || imm == -1) {
        return 0;
    }
    
    uintptr_t address = (uintptr_t)r[rs1] + (uintptr_t)imm;
    
    if (address < 0 || address >= MEM_SIZE) {
        return 0;
    }
    
    mem[address] = r[rd] & 0xFF; // Store only the lower 8 bits
    return 1;
}

//M[RS1+IMM][0:31] <- RS2[0:31] 
//SW RD,RS1,IMM
//SW SP,RA,28
int SW (char **tokens){
    int rd, rs1, imm;

    rd = get_r_index(tokens[1]);
    rs1 = get_r_index(tokens[2]);
    imm = parseImmediate(tokens[3]);

    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || imm == -1) {
        return 0;
    }
    
    uint32_t address = r[rs1] + imm;
    
    if (address < 0 || address >= MEM_SIZE) {
        return 0;
    }
    
    *(u_int32_t*)&mem[address] = (unsigned char)r[rd];
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    return 1;
}

/*3)	Arithmetic SHARED */
int ADD (char **tokens) {
    int rd, rs1, rs2;

    rd = get_r_index(tokens[0]);
    rs1 = get_r_index(tokens[1]);
    rs2 = get_r_index(tokens[2]);

    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || rs2 == -1) {
        return 0;
    }

    r[rd] = r[rs1] + r[rs2];
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    return 1;
}

int ADDI (char **tokens) {
    int rd, rs1, imm;

    rd = get_r_index(tokens[1]);
    rs1 = get_r_index(tokens[2]);
    imm = parseImmediate(tokens[3]);

    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || imm == -1) {
        return 0;
    }
 
    r[rd] = r[rs1] + imm;
   
    printf("register: %d \n", rd);
    printf("value in rd: %d \n", r[rd]);
    return 1;
}

//SUB RD,RS1,RS2
//SUB A3,A3,A2
//RD <- RS1 - RS2
int SUB (char **tokens) {
    int rd, rs1, rs2;

    rd = get_r_index(tokens[0]);
    rs1 = get_r_index(tokens[1]);
    rs2 = get_r_index(tokens[2]);

    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || rs2 == -1) {
        return 0;
    }

    r[rd] = r[rs1] - r[rs2];
    
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    return 1;
}

/*4)	Bitwise operators MARY*/
//XOR RD,RS1,RS2 -- Exclusive OR
//XOR A4,A4,A5
//RD <- RS1 ^ RS1
int XOR (char **tokens) {
    int rd, rs1, rs2;

    rd = get_r_index(tokens[0]);
    rs1 = get_r_index(tokens[1]);
    rs2 = get_r_index(tokens[2]);

    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || rs2 == -1) {
        return 0;
    }

    r[rd] = r[rs1] ^ r[rs2];
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    return 1;
}

//XORI RD,RS1,IMM -- Exclusive OR imm
//XORI T0,T1,3
//RD <- RS1 ^ IMM 
int XORI (char **tokens) {
    int rd, rs1, imm;

    rd = get_r_index(tokens[0]);
    rs1 = get_r_index(tokens[1]);

    imm = parseImmediate(tokens[2]);


    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || imm == -1) {
        return 0;
    }

    r[rd] = (uintptr_t)(r[rs1] ^ (unsigned int)imm);
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    printf("value in rs1: %u \n", r[rs1]);
    return 1;
}

//SLLI RD,RS1,IMM -- Shift left imm
//SLLI A3,A3,2
//RD <- RS1 << imm[0:4]
int SLLI (char **tokens) {
    int rd, rs1, imm;

    rd = get_r_index(tokens[0]);
    rs1 = get_r_index(tokens[1]);
    imm = get_r_index(tokens[2]); // Mask to extract lower 5 bits


    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || imm == -1) {
        return 0;
    }

    r[rd] = r[rs1] << imm;
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    printf("value in rs1: %u \n", r[rs1]);
    return 1;
}

//SRLI RD,RS1,IMM -- Shift right imm
//SRLI A0,A0,1
//RD <- RS1 >> imm[0:4]
int SRLI (char **tokens) {
    int rd, rs1, imm;

    rd = get_r_index(tokens[0]);
    rs1 = get_r_index(tokens[1]);
    imm = parseImmediate(tokens[2]);// Mask to extract lower 5 bits


    if (rd == -1) {
        return 0;
    }

    if (rs1 == -1 || imm == -1) {
        return 0;
    }

    r[rd] = r[rs1] >> imm;
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    return 1;
}

/*5)	Pseudo-instructions CHRIS*/
// MV RD,RS   MV A2,X0   ADDI RD,RS,IMM
int MV (char **tokens) {
    int rd, rs;

    rd = get_r_index(tokens[1]);
    rs = get_r_index(tokens[2]);
        
    if (rd == -1 || rs == -1) {
        return 0;
    }
    
    printf("\n");
    printf("Register rd: %u, Register rs: %u\n", rd, rs);
    printf("Value in rd before assignment: %u\n", r[rd]);
    printf("Value in rs: %u\n", r[rs]);

    r[rd] = r[rs];

    printf("Value in rd after assignment: %u\n", r[rd]);
    
    return 1;
}

// LI RD,IMM   LI A1,6   ADDI RD,X0,IMM
int LI (char **tokens) {
    int rd, imm;

    rd = get_r_index(tokens[1]);

    imm = parseImmediate(tokens[2]);


    if (rd == 17 || rd == 1 || rd == -1 || imm == -1) {
        return 0;
    }
    
    printf("\n");
    printf("Register rd: %u\n", rd);
    printf("Value in rd before assignment: %u\n", r[rd]);
    
    r[rd] = (uintptr_t)imm;
    
    printf("Value in rd after assignment: %u\n", r[rd]);
    
    return 1;
}

int NED (char **tokens) {
    int rd, rs;

    rd = get_r_index(tokens[0]);
    rs = get_r_index(tokens[1]);

    if (rs == -1 || rd == -1) {
        return 0;
    }

    r[rd] = r[rs];
    return 1;
}

// NOT T0,T1
int NOT(char **tokens) { 
    int rd, rs;

    rd = get_r_index(tokens[0]);
    rs = get_r_index(tokens[1]);

    if (rd == -1 || rs == -1) {
        return 0;
    }

    r[rd] = ~r[rs];

    return 1;
}
  /*6)	Jump offset: CHRIS*/
int JAL (char **tokens) {
    int rd, imm;

    rd = get_r_index(tokens[0]);

    imm = get_r_index(tokens[1]);


    if (rd == -1 || imm == -1) {
        return 0;
    }

    int address = pc + imm;
    r[rd] = (uintptr_t)((int32_t)pc + 4);
    pc = address;
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    printf("pc value: %u \n", pc);

    return 1; 
}

int J(char **tokens) {
    int imm;
    imm = parseImmediate(tokens[1]);
    printf("Parsed immediate value: %d \n", imm);
    // Print initial program counter value
    printf("Initial pc value: %d \n", pc);

    pc += imm;

    // if(imm < 0){
    //   pc -= imm;
    // }else if(imm > 0){
    //   pc += imm;
    // }
    
    printf("Final PC value HERE: %d \n", pc);
    return 1;
}

/*7)	Jump offset + Reg: MARY*/
//JALR RA,RA,240
//JALR RD,RS1,IMM
//RD <- PC+4 then PC = RS1 + IMM
int JALR (char **tokens) {
    int rd, rs1, imm;

    rd = get_r_index(tokens[0]);
    rs1 = get_r_index(tokens[1]);
    imm = get_r_index(tokens[2]);

    if (rs1 == -1 || rd == -1) {
        return 0;
    }
  
    r[rd] = (uintptr_t)((int32_t)pc + 4);
    pc += (int32_t)r[rs1] + imm;
    
    printf("register: %u \n", rd);
    printf("value in rd: %u \n", r[rd]);
    printf("pc value: %u \n", pc);
    return 1;
}

//JR RS
//JR RA
//JALR X0, RS, 0
int JR (char **tokens) {
    int rs;

    rs = get_r_index(tokens[0]);

    if (rs == -1) {
        return 0;
    }

    // Set the program counter to the address stored in the source register
    pc = r[rs];

    printf("value in rs: %u \n", r[rs]);
    printf("pc value: %u \n", pc);
    return 1;
}

/**
 * Fill out this function and use it to read interpret user input to execute
 * RV32 instructions. You may expect that a single, properly formatted RISC-V
 * instruction string will be passed as a parameter to this function.
 */ 
int interpret(char *instr) {
    char **tokens = tokenize(instr, " ,()");
    char *instruction = tokens[0]; 
    printf("INSTRUCTION ENTERED: %s\n", instr);

    int pass = 0; 

    if (str_cmp(instruction, "LB") == 0) {
        pass = LB(tokens);
    } else if (str_cmp(instruction, "LW") == 0) {
        pass = LW(tokens);
    } else if (str_cmp(instruction, "SB") == 0) {
        pass = SB(tokens);
    } else if (str_cmp(instruction, "SW") == 0) {
        pass = SW(tokens);
    } else if (str_cmp(instruction, "ADD") == 0) {
        pass = ADD(tokens);
    } else if (str_cmp(instruction, "ADDI") == 0) {
        pass = ADDI(tokens);
    } else if (str_cmp(instruction, "SUB") == 0) {
        pass = SUB(tokens);
    } else if (str_cmp(instruction, "XOR") == 0) {
        pass = XOR(tokens);
    } else if (str_cmp(instruction, "XORI") == 0) {
        pass = XORI(tokens);
    } else if (str_cmp(instruction, "SLLI") == 0) {
        pass = SLLI(tokens);
    } else if (str_cmp(instruction, "SRLI") == 0) {
        pass = SRLI(tokens);
    } else if (str_cmp(instruction, "MV") == 0) {
        pass = MV(tokens);
    } else if (str_cmp(instruction, "LI") == 0) {
        pass = LI(tokens);
    } else if (str_cmp(instruction, "NED") == 0) {
        pass = NED(tokens);
    } else if (str_cmp(instruction, "NOT") == 0) {
        pass = NOT(tokens);
    } else if (str_cmp(instruction, "JAL") == 0) {
        pass = JAL(tokens);
    } else if (str_cmp(instruction, "J") == 0) {
        pass = J(tokens);
    } else if (str_cmp(instruction, "JALR") == 0) {
        pass = JALR(tokens);
    } else if (str_cmp(instruction, "JR") == 0) {
        pass = JR(tokens);
    } else {
        printf("Invalid instruction: %s\n", instruction);
        printf("Invalid, likely cause buffer is getting empty strings from files\n");
        return pass;
    }

    if (pass == 1) {
        printf("VALID instruction\n");
        return pass;
    }

    return 0;
}


int main(int argc, char **argv) {
  FILE *file;
  char *buffer;

  if (argc != 3) {
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

  while (pc != -1) {
    get_line(buffer, pc / 4);
    printf("BUFFER: %s \n", buffer);
    if (*buffer != '\0') {
      interpret(buffer); 
    }else if(*buffer == '\0'){
      break;
    }
    printf("\n");
    r[0] = 0;
    pc += 4;
    printf("IN WHILE LOOP PC: %d \n", pc);
  }

  /* --- Your code ends here. --- */
  close_file();
  free(buffer);

  return print_registers(argv[2]);
}
