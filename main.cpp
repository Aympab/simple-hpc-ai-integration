#include <iostream>
#include <mpi.h>

int main (int argc, char* argv[]){
    int myid, num_procs;

    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);


    //Do stuff
    std::cout << "Hello World!" << std::endl;


    /* Finalize MPI */
    MPI_Finalize();
    return 0;
}


// mpicxx -g -Wall -Wno-deprecated -Ibin_hypre/hypre-master/src/hypre/include -I.. -DHAVE_CONFIG_H -DHYPRE_TIMING    -c -o tp.o tp.cpp
// mpicxx -g -Wall -Wno-deprecated -DHAVE_CONFIG_H -c -o tp.o main.cpp
// mpicxx -o tp tp.o -g -Wall -Lbin_hypre/hypre-master/src/hypre/lib -lHYPRE -lm -lstdc++