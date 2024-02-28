CC   = cc
OBJS = my_string.o tokenizer.o process_file.o

CFLAGS = -O3 -g3 -Wall -Wextra -Werror=format-security -Werror=implicit-function-declaration \
         -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Wconversion

all: riscv

riscv: $(OBJS) riscv.o
	$(CC) -o $@ $^

run: riscv
	./riscv assembly.txt

clean:
	rm -f *.o riscv seekable_file.txt

my_string.o: my_string.c my_string.h
tokenizer.o: tokenizer.c tokenizer.h my_string.h
process_file.o: process_file.c process_file.h
riscv.o: riscv.c my_string.h tokenizer.h process_file.h

