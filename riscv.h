#ifndef __RISCV_H__
#define __RISCV_H__

static void error_no_memory(void);

static int print_registers (char *fd_name);

void init_memory_elements(void);

int is_digit(char character);

int parseImmediate(const char *immediate);

int get_r_index_extended(const char *reg_name);

int get_r_index(const char *reg_name);

int LB(char *instr);

int LW(char *instr);

int SB(char *instr);

int SW(char *instr);

int ADD(char *instr);

int ADDI(char *instr);

int SUB(char *instr);

int XOR(char *instr);

int XORI(char *instr);

int SLLI(char *instr);

int SRLI(char *instr);

int MV(char *instr);

int LI(char *instr);

int NED(char *instr);

int NOT(char *instr);

int JAL(char *instr);

int J(char *instr);

int JALR(char *instr);

int JR(char *instr);

int interpret(char *instr);

#endif
