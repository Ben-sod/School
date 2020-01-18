CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS = \
	thermo_main \


all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o vgcore.* hybrid_main

# thermometer problem
thermo_main : thermo_main.o thermo_sim.o thermo_update_asm.o 
	$(CC) -o $@ $^

thermo_main.o : thermo_main.c thermo.h
	$(CC) -c $<

thermo_sim.o : thermo_sim.c thermo.h
	$(CC) -c $<

# required assembly implementation
thermo_update_asm.o : thermo_update_asm.s thermo.h
	$(CC) -c $<


# C version of functions
thermo_update.o : thermo_update.c thermo.h
	$(CC) -c $<


# main which uses both assmebly and C update functions for incremental
# testing
hybrid_main : thermo_main.o thermo_sim.o thermo_update_asm.o thermo_update.o
	$(CC) -o $@ $^

################################################################################
# Testing Targets
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

test: test-p1 

test-p1 : test_thermo_update thermo_main
	@printf "===TESTS for P1===\n"
	@printf "Running functions tests for thermo_update_asm.s\n"
	./test_thermo_update
	@printf "\n"
	@printf "Running tests in Valgrind\n"
	$(VALGRIND) ./test_thermo_update
	@printf "\n"
	@printf "Running shell tests for thermo_main\n"
	./test_thermo_main.sh thermo_main

test_thermo_update : test_thermo_update.o thermo_sim.o thermo_update_asm.o
	$(CC) -o $@ $^

test_thermo_update.o : test_thermo_update.c
	$(CC) -c $<


clean-tests : clean
	rm -f test-data/*.tmp test_thermo_update test_hybrid


# test hybrid for incremental work
test-hybrid : test_hybrid hybrid_main
	@printf "===TESTS for Hybrid===\n"
	@printf "Running binary tests for hybrid\n"
	./test_hybrid
	@printf "\n"
	@printf "Running tests in Valgrind\n"
	$(VALGRIND) ./test_hybrid
	@printf "\n"
	@printf "Running shell tests for hybrid_main\n"
	./test_thermo_main.sh hybrid_main


# hybrid test program
test_hybrid : test_thermo_update.o thermo_sim.o thermo_update_asm.o thermo_update.o
	$(CC) -o $@ $^
