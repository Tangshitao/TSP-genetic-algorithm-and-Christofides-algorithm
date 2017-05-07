OBJS=vertex.o graph.o solver.o brute_force_solver.o minimum_spanning_tree_solver.o genetic_solver.o TSP_Solver.o graph_reader.o
CC=g++
DEBUG=
CFLAGS=-Wall -c -std=c++11 -pthread $(DEBUG)
LFLAGS=-Wall -pthread $(DEBUG)

TSP_Solver:$(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o TSP_Solver

vertex.o:vertex.h vertex.cpp
	$(CC) $(CFLAGS) vertex.cpp

graph.o:graph.h graph.cpp vertex.h
	$(CC) $(CFLAGS) graph.cpp

solver.o:solver.h solver.cpp graph.h vertex.h
	$(CC) $(CFLAGS) solver.cpp

brute_force_solver.o:brute_force_solver.h brute_force_solver.cpp graph.h vertex.h solver.h
	$(CC) $(CFLAGS) brute_force_solver.cpp

minimum_spanning_tree_solver.o:minimum_spanning_tree_solver.h minimum_spanning_tree_solver.cpp graph.h vertex.h solver.h
	$(CC) $(CFLAGS) minimum_spanning_tree_solver.cpp

genetic_solver.o:genetic_solver.h genetic_solver.cpp graph.h vertex.h solver.h minimum_spanning_tree_solver.h brute_force_solver.h
	$(CC) $(CFLAGS) genetic_solver.cpp

TSP_Solver.o: TSP_Solver.cpp graph.h vertex.h solver.h genetic_solver.h
	$(CC) $(CFLAGS) TSP_Solver.cpp

graph_reader.o:graph_reader.h graph_reader.cpp graph.h vertex.h
	$(CC) $(CFLAGS) graph_reader.cpp

clean:
	\rm *.o TSP_Solver

