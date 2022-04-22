#include <iostream>
#include <mpi.h>
// #include "external/onnxruntime/include/onnxruntime_cxx_api.h"
#include <onnxruntime_cxx_api.h>

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
