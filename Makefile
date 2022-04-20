main.o: main.cpp
	mpicxx -o $@ $^ $(FLAGS)