CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS = \
	thermo_main \
	puzzlebox \

all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o


# thermometer problem
thermo_main : thermo_main.o thermo_update.o thermo_sim.o
	$(CC) -o $@ $^

thermo_main.o : thermo_main.c thermo.h
	$(CC) -c $<

thermo_sim.o : thermo_sim.c thermo.h
	$(CC) -c $<

thermo_update.o : thermo_update.c thermo.h
	$(CC) -c $<


# debugging problem
puzzlebox.o : puzzlebox.c
	$(CC) -c $<

puzzlebox : puzzlebox.o
	$(CC) -o $@ $^



# Testing Targets
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

test: test-p1 test-p2

test-p1 : test_thermo_update thermo_main
	@printf "===TESTS for P1===\n"
	@printf "Running functions tests for thermo_update.c\n"
	./test_thermo_update
	@printf "\n"
	@printf "Running tests in Valgrind\n"
	$(VALGRIND) ./test_thermo_update
	@printf "\n"
	@printf "Running shell tests for lcd_main\n"
	./test_thermo_main.sh

test_thermo_update : test_thermo_update.o thermo_sim.o thermo_update.o
	$(CC) -o $@ $^

test_thermo_update.o : test_thermo_update.c
	$(CC) -c $<


test-p2 : puzzlebox
	./puzzlebox input.txt


clean-tests : clean
	rm -f test-data/*.tmp test_thermo_update

