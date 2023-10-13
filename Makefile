# Variabili del compilatore e delle opzioni
CC = gcc
CFLAGS = -Wall -Ilibs

# Lista di dipendenze: file .o creati dai file .c nella cartella libs
LIB_OBJS = $(patsubst src/libs/%.c, src/libs/%.o, $(wildcard src/libs/*.c))

# Regola di default
all: orango

# Regola per l'eseguibile orango
orango: src/orango.o $(LIB_OBJS)
	$(CC) src/orango.o $(LIB_OBJS) -o orango


# Regola per orango.o
orango.o: src/orango.c $(LIB_OBJS)
	$(CC) $(CFLAGS) -c src/orango.c -o src/orango.o

# Regola generica per compilare file .o da file .c nella cartella libs
%.o: src/libs/%.c src/libs/%.h
	$(CC) $(CFLAGS) -c $< -o src/libs/$@

# Regola per pulizia
clean:
	rm -f orango
	rm -f src/*.o
	rm -f src/libs/*.o

.PHONY: all clean
