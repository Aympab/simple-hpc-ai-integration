#include <iostream>
#include <mpi.h>
#include <Eigen/Dense>
// #include <onnxruntime_cxx_api.h>
#include <assert.h>
#include <random>


using Eigen::MatrixXf;
using Eigen::VectorXf;

#define __DEBUG 1

int main (int argc, char* argv[]){
  int myid, num_procs;
  int N;

  if (argc > 1){
    N = atoi(argv[1]);
    if(N <= 0){
      throw std::invalid_argument("N must be an integer > 0.");
    }
    std::cout << "Running with matrix size N = " << N << std::endl;
  }
  else{
    std::cout << "Running with default matrix size N = 5" << std::endl;
    N = 5;
  }

  /*
  Random initializations 
    - Matrix M size NxN
    - Vector v size N
  
  See :
  https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
  std::random_devices sets the seed by default
  */
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(-1.0, 1.0);

  MatrixXf M = MatrixXf::NullaryExpr(N, N,[&](){return dis(gen);});
  VectorXf V = VectorXf::NullaryExpr(N,[&](){return dis(gen);});

  if (__DEBUG){
    std::cout << "M =" << std::endl << M << std::endl;
    std::cout << "V =" << std::endl << V << std::endl;
  }

  /* Initialize MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if(num_procs % N == 0){
    std::cout << "num_procs % N == 0" << std::endl;
  }
  else{
    std::cout << "num_procs % N != 0" << std::endl;
  }

  //Split & Send matrix between the procs
  //

  //Do stuff
  if (myid > 0){
      std::cout << "Hello World!" << std::endl;
  }

  /* Finalize MPI */
  MPI_Finalize();
  return 0;
}
