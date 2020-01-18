CFLAGS = -Wall -g -Og

CC     = gcc $(CFLAGS)

PROGRAMS = \
	mult_benchmark \
	search_benchmark \


all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o vgcore.* 

# Matrix-vector optimization problem
matvec_util.o : matvec_util.c matvec.h
	$(CC) -c $<

baseline_matvec_mult.o : baseline_matvec_mult.c matvec.h
	$(CC) -c $<

optimized_matvec_mult.o : optimized_matvec_mult.c matvec.h
	$(CC) -c $<

mult_benchmark : mult_benchmark.o baseline_matvec_mult.o optimized_matvec_mult.o matvec_util.o
	$(CC) -o $@ $^

mult_benchmark.o : mult_benchmark.c matvec.h
	$(CC) -c $<

# Search Algorithm Problem
search_benchmark : search_benchmark.o search_funcs.o
	$(CC) -o $@ $^

search_benchmark.o : search_benchmark.c search.h
	$(CC) -c $<

search_funcs.o : search_funcs.c search.h
	$(CC) -c $<

# Testing Targets
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# run with valgrind to check for memory problems
p1-valgrind: 
	$(VALGRIND) ./mult_benchmark test

p2-valgrind:
	$(VALGRIND) ./search_benchmark 3 8 1 ablt
