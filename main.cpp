#include <iostream>
#include <mpi.h>
#include <Eigen/Dense>
// #include <onnxruntime_cxx_api.h>
#include <assert.h>
#include <random>
#include <algorithm>

#include <stdint.h>
#include <limits.h>
#if SIZE_MAX == UCHAR_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED_CHAR
#elif SIZE_MAX == USHRT_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED_SHORT
#elif SIZE_MAX == UINT_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED
#elif SIZE_MAX == ULONG_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED_LONG
#elif SIZE_MAX == ULLONG_MAX
   #define my_MPI_SIZE_T MPI_UNSIGNED_LONG_LONG
#else
   #error "what is happening here?"
#endif

using Eigen::MatrixXf;
using Eigen::VectorXf;

#define __DEBUG 1

int main (int argc, char* argv[]){
  /* Variables initialization */
  int myid, num_procs;
  size_t N;

  /* Initialize MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  if(myid == 0){
    /* Parsing command line arguments */
    if (argc > 1){
      N = atoi(argv[1]);
      if(N <= 0){
        // MPI_Finalize(); CANNOT DO THIS BECAUSE EVERY ONE NEEDS TO CALL MPI FINALIZE : HOW TO THROW ERROR WELL ??
        throw std::invalid_argument("N must be an integer > 0.");
      }
      std::cout << "Running with matrix size N = " << N << std::endl;
    }
    else{
      std::cout << "Running with default matrix size N = 5" << std::endl;
      N = 5;
    }
    // if (__DEBUG){
    //   std::cout << "M =" << std::endl << M << std::endl;
    //   std::cout << "v =" << std::endl << v << std::endl;
    // }
  } // end if(myid == 0)


  //Sending M size to everyone
  MPI_Bcast(&N, 1, my_MPI_SIZE_T, 0, MPI_COMM_WORLD);

  /* Split & Send matrix between the procs */
  int localsize = N / num_procs;

  //Settings arrays to use to split matrix with MPI
  int counts[num_procs];
  int displacements[num_procs];

  displacements[0] = 0;
  //Filling the counts and offset arrays for MPI_ScatterV
  if(myid == 0)
  {
      int plocal_size = N/num_procs;
      int pextra = N - plocal_size*num_procs;

      int plower;
      int pupper;
      int sum_count = 0;

      for(int p = 0; p < num_procs; p++){
          plower=0;
          pupper=0;

          plower = plocal_size*p;
          plower += std::min(p, pextra);

          pupper = plocal_size*(p+1);
          pupper += std::min(p+1, pextra);
          pupper = pupper - 1;

          counts[p] = pupper - plower + 1;

          if(p > 0) displacements[p] = sum_count;
          sum_count += counts[p]-1;
      }
  } // end if(myid == 0)

  if(N % num_procs != 0){
    int extra = N % num_procs;
    for(int i = 0; i < extra; i++){
      if(myid == i){
        localsize++;
      }
    }
  }// end if(N % num_procs != 0)

  if(__DEBUG)
      std::cout << "#" << myid << " - local size = " << localsize << std::endl;

  float rows[localsize];

  if(myid == 0){
    /*
    Random initializations on proc 0, then we'll send it to the others :
      - Matrix M size NxN
      - Vector v size N
    
    std::random_devices sets the seed by default. See :
    https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> uniform(-1.0, 1.0);

    MatrixXf M = MatrixXf::NullaryExpr(N, N,[&](){return uniform(gen);});
    VectorXf v = VectorXf::NullaryExpr(N,[&](){return uniform(gen);});

  /* Send matrix to other proc */
    MPI_Scatterv(
        M.data(),
        counts,
        displacements,
        MPI_FLOAT,
        &rows,
        localsize+1,
        MPI_FLOAT,        //Datatype receive
        0,              //root MPI ID
        MPI_COMM_WORLD);
  }
  else{
    MPI_Scatterv(NULL, NULL, NULL,
        MPI_FLOAT,
        &rows,
        localsize,
        MPI_FLOAT,        //Datatype receive
        0,              //root MPI ID
        MPI_COMM_WORLD);
  } // end if(myid == 0)


  /* Finalize MPI */
  MPI_Finalize();
  return 0;
}
