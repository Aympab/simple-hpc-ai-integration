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
using Eigen::Map;
using Eigen::Matrix;
using Eigen::Vector;


#define __DEBUG 1


void inline print_buffer(std::string infos, float* buffer, size_t N){
  std::cout << "\n\nBUFFER " << infos << "=\n";
  for (size_t i = 0; i<N; ++i){
    std::cout << buffer[i];
    if(i < N-1)
      std::cout << ", ";
    else
      std::cout << std::endl;
  }
}

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
  } // end if(myid == 0)


  //Sending Matrix size to everyone
  MPI_Bcast(&N, 1, my_MPI_SIZE_T, 0, MPI_COMM_WORLD);

  /* Split & Send matrix between the procs */
  int localNbRows = N / num_procs;

  //Settings arrays to use to split matrix with MPI
  int counts[num_procs];
  int displacements[num_procs];
  int counts_final[num_procs];
  int displacements_final[num_procs];

  displacements[0] = 0;
  displacements_final[0] = 0;
  //Filling the counts and offset arrays for MPI_ScatterV
  if(myid == 0)
  {
      int plocal_size = N/num_procs;
      int pextra = N - plocal_size*num_procs;

      int plower;
      int pupper;
      int sum_count = 0;
      int sum_count_final = 0;

      for(int p = 0; p < num_procs; p++){
          plower=0;
          pupper=0;

          plower = plocal_size*p;
          plower += std::min(p, pextra);

          pupper = plocal_size*(p+1);
          pupper += std::min(p+1, pextra);

          counts[p] = (pupper - plower) * N;
          counts_final[p] = (pupper - plower);

          if(p > 0){
            displacements[p] = sum_count;
            displacements_final[p] = sum_count_final;
            // std::cout << displacements_final[p] <<
          }
          sum_count += counts[p];
          sum_count_final += counts_final[p];
      } // end for p

  } // end if(myid == 0)

  if(N % num_procs != 0){
    int extra = N % num_procs;
    for(int i = 0; i < extra; i++){
      if(myid == i){
        localNbRows++;
      }
    }
  }// end if(N % num_procs != 0)

  float valuesBuff[localNbRows*N];
  // float* vecBuff = (float*)malloc(N*sizeof(float));
  float vecBuff[N];

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

    Matrix<float, -1, -1, Eigen::RowMajor> M = MatrixXf::
                                                NullaryExpr(
                                                  N, N,
                                                  [&](){return uniform(gen);}
                                                );
    Vector<float, -1> v = VectorXf::NullaryExpr(N,[&](){return uniform(gen);});

    // std::cout << "REAL VEC =\n" << v << std::endl;

    /* Send matrix to other proc */
    MPI_Scatterv(
        M.data(),         //buffer_send
        counts,           //number of elements to send to each proc
        displacements,    //displacement to each process
        MPI_FLOAT,        //type send
        &valuesBuff,          //buffer to receive valuesBuff
        localNbRows*N,    //number of elements on the reveive buffer
        MPI_FLOAT,        //datatype receive
        0,                //root MPI ID
        MPI_COMM_WORLD);

    /* Setup to send the whole vector, going to use this in Bcast */
    // vecBuff = v.data(); //NO IDEA WHY JUST THIS DOESN'T WORK ????
    auto tmp = v.data();
    for(size_t i =0; i<N;i++){
      vecBuff[i] = tmp[i];
    }
  }
  else{
    MPI_Scatterv(NULL, NULL, NULL, MPI_FLOAT,
        &valuesBuff,
        localNbRows*N,
        MPI_FLOAT,        //Datatype receive
        0,                //root MPI ID
        MPI_COMM_WORLD);
  } // end if(myid == 0)

  // if(myid == 0) print_buffer(std::string("BEFORE BCAST"), vecBuff, N);

  /* Receiving objects */
  //Build local matrix
  auto localMat = Eigen::Map<MatrixXf>(valuesBuff, localNbRows, N);

  //Bcast the whole vector into vecBuffer
  MPI_Bcast(&vecBuff, N, MPI_FLOAT, 0, MPI_COMM_WORLD);
  
  //Create Eigen vector based on vecBuffer
  auto localVec = Eigen::Map<VectorXf>(vecBuff, N);

  //Compute partial Mv product
  VectorXf localRes = localMat * localVec;
  // if(myid == 0 || myid == 1) std::cout << "\n\nLOCAL RES=\n"<< localRes << std::endl;

  if(myid == 0){
    float finalBuff[N];
    MPI_Gatherv(localRes.data(),  //const void* buffer_send,
                localNbRows,      //int count_send,
                MPI_FLOAT,        //MPI_Datatype datatype_send,
                &finalBuff,        //void* buffer_recv,
                counts_final,           //const int* counts_recv,
                displacements_final,    //const int* displacements,
                MPI_FLOAT,        //MPI_Datatype datatype_recv,
                0,                //int root,
                MPI_COMM_WORLD);  //MPI_Comm communicator);


  VectorXf finalResult = Eigen::Map<VectorXf>(finalBuff, N);
  std::cout << "\n\nFINAL RES =\n"<< finalResult << std::endl;

  }
  else{
    MPI_Gatherv(localRes.data(),  //const void* buffer_send,
                localNbRows,      //int count_send,
                MPI_FLOAT,        //MPI_Datatype datatype_send,
                NULL,             //void* buffer_recv,
                NULL,             //const int* counts_recv,
                NULL,             //const int* displacements,
                MPI_FLOAT,        //MPI_Datatype datatype_recv,
                0,                //int root,
                MPI_COMM_WORLD);  //MPI_Comm communicator);
  }

  /* Finalize MPI */
  MPI_Finalize();
  return 0;
}

