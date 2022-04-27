#include <iostream>
#include <mpi.h>
#include <Eigen/Dense>
// #include <onnxruntime_cxx_api.h>
#include <assert.h>


using Eigen::MatrixXd;
using Eigen::VectorXd;

int main (int argc, char* argv[]){
    int myid, num_procs;
    int N;

    if (argc > 1){
      N = atoi(argv[1]);
      if(N <= 0){
        throw std::invalid_argument("N must be an integer > 0.");
      }
    }
    else{
      N = 5;
    }

    printf("N = %d", N);

    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);


    //Initialize random matrix size NxN
    //Split & Send matrix between the procs
    //

    //Do stuff
    if (myid > 0){
        std::cout << "Hello World!" << std::endl;
    }

    MatrixXd m = MatrixXd::Random(N,3);
    m = (m + MatrixXd::Constant(N,3,1.2)) * 50;
    std::cout << "m =" << std::endl << m << std::endl;
    VectorXd v(3);
    v << 1, 2, 3;
    std::cout << "m * v =" << std::endl << m * v << std::endl;

    /* Finalize MPI */
    MPI_Finalize();
    return 0;
}
