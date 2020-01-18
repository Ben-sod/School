CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS = \
	save_deltas \
	deltas_main \
	print_graph_demo \
	graph_file \
	tree_main \

all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o

# int delta problem
deltas_main.o : deltas_main.c deltas.h
	$(CC) -c $<

deltas_main : deltas_main.o read_deltas.o
	$(CC) -o $@ $^

read_deltas.o : read_deltas.c deltas.h
	$(CC) -c $<

save_deltas.o : save_deltas.c deltas.h
	$(CC) -c $<

save_deltas : save_deltas.o 
	$(CC) -o $@ $^

# print_graph
print_graph.o : print_graph.c
	$(CC) -c $<

print_graph_demo.o : print_graph_demo.c
	$(CC) -c $<

print_graph_demo : print_graph_demo.o print_graph.o
	$(CC) -o $@ $^

graph_file.o : graph_file.c
	$(CC) -c $<

graph_file : graph_file.o print_graph.o read_deltas.o
	$(CC) -o $@ $^

# tree problem
tree_main : tree_main.o tree_funcs.o
	$(CC) -o $@ $^

tree_main.o : tree_main.c tree.h
	$(CC) -c $<

tree_funcs.o : tree_funcs.c tree.h
	$(CC) -c $<


# Testing Targets
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

test : test-p1 test-p2 test-p3

test-p1 : test_read_deltas
	@printf "===TESTS for P1===\n"
	@printf "Running binary tests for read_deltas\n"
	./test_read_deltas
	@printf "\n"
	@printf "Running tests in Valgrind\n"
	$(VALGRIND) ./test_read_deltas
	@printf "\n"


test_read_deltas.o : test_read_deltas.c deltas.h
	$(CC) -c $<

test_read_deltas : test_read_deltas.o read_deltas.o
	$(CC) -o $@ $^

test-p2 : print_graph_demo graph_file save_deltas
	@printf "===TESTS for P2===\n"
	@printf "Testing print_graph via print_graph_demo\n"
	./test_print_graph.sh
	@printf "\n"
	@printf "Testing graph_file\n"
	./test_graph_file.sh

test-p3 : tree_main
	@printf "===TESTS for P3===\n"
	./test_tree_main.sh

clean-tests :
	rm -f test_read_deltas
	rm -f test-data/*.tree test-data/*.valgrind
	rm -f test-data/*.actual test-data/*.expect test-data/*.in
	rm -f test-data/*.empty test-data/*.txt
	rm -f test-data/*.int test-data/*.diff
