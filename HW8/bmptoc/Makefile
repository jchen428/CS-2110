# Makefile for CS 2110 bmptoc Homework

PROGNAME = bmptoc
SRC = bmptoc.c
OBJ = $(SRC:%.c=%.o)
CFLAGS = -Wall -Werror -std=c99 -pedantic
LFLAGS =

all : $(OBJ)
	@echo "[LINK] Linking $(PROGNAME)"
	@gcc -o $(PROGNAME) $^ $(LFLAGS)

debug : CFLAGS += -DDEBUG -g
debug : all
	@echo "!! Built using debug flags !!"

%.o : %.c
	@echo "[COMPILE] Compiling $@"
	@gcc $(CFLAGS) -c $< -o $@

clean :
	@echo "[CLEAN] Removing all compiled files"
	@rm -f *.o $(PROGNAME)
