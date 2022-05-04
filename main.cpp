#include <iostream>
#include <mpi.h>
#include <Eigen/Dense>
// #include <onnxruntime_cxx_api.h>
#include <assert.h>
#include <random>
#include <algorithm>
#include "Timer.h"

// #include <stdint.h>
// #include <limits.h>
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
   #error "SIZE_MAX doesn't correspond to any MAX"
#endif

using Eigen::MatrixXf;
using Eigen::VectorXf;
using Eigen::Map;
using Eigen::Matrix;
using Eigen::Vector;


#define __DEBUG 1

int main (int argc, char* argv[]){
  /* Initialization */
  int myid, nb_procs;
  size_t N;
  double savedNorm = -1.0;
  double finalNorm = -1.5;
  Timer initTimer("Initialization");
  Timer computeTimer("Computation");
  Timer mpiTimer("MPI communications");

  omp_set_num_threads(4);
  Eigen::setNbThreads(4);

  /* Initialize MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_procs);

  if(myid == 0){
    /* Parsing command line arguments */
    if (argc > 1){
      N = atoi(argv[1]);
      if(N <= 0){
        // MPI_Finalize(); TODO CANNOT DO THIS BECAUSE EVERY ONE NEEDS TO CALL MPI FINALIZE : HOW TO THROW ERROR WELL ??
        throw std::invalid_argument("N must be an integer > 0.");
      }
      std::cout << "Running with matrix size N = " << N << std::endl;
    }
    else{
      N = 5;
      std::cout << "Running with default matrix size N = " << N << std::endl;
    }
  } // end if(myid == 0)


  //Sending Matrix size to everyone
  MPI_Bcast(&N, 1, my_MPI_SIZE_T, 0, MPI_COMM_WORLD);

  /* Split & Send matrix between the procs */
  int localNbRows = N / nb_procs;
 { //appending localNbRows if there is any extra rows
    int extra = N % nb_procs;
    if(extra != 0){
      for(int i = 0; i < extra; i++){
        if(myid == i){
          localNbRows++;
        }
      }
    }// end if(extra != 0)
  }

  //Settings arrays to use to split matrix with MPI
  int counts[nb_procs];
  int displacements[nb_procs];
  int counts_final[nb_procs];
  int displacements_final[nb_procs];

  displacements[0] = 0;
  displacements_final[0] = 0;
  if(myid == 0)
  {
     Timer::Sentry sentry(initTimer, "SetupCountsDisplacements");
    //Filling the counts and offset arrays for MPI_ScatterV and GatherV
      int plocal_size = N/nb_procs;
      int pextra = N - plocal_size*nb_procs;

      int plower;
      int pupper;
      int sum_count = 0;
      int sum_count_final = 0;

      for(int p = 0; p < nb_procs; p++){
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
          }
          sum_count += counts[p];
          sum_count_final += counts_final[p];
      } // end for p
  } // end if(myid == 0)

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
    std::uniform_real_distribution<double> uniform(-1.0, 1.0);

    Matrix<float, -1, -1, Eigen::RowMajor> M;
    {
      Timer::Sentry sentry(initTimer,"InitMatrix");
      M = MatrixXf::NullaryExpr(N, N,[&](){return uniform(gen)*50;});
    }

    Vector<float, -1> v;
    {
      Timer::Sentry sentry(initTimer,"InitVector");
      v = VectorXf::NullaryExpr(N,[&](){return uniform(gen);});
    }

    //We use tmpMv so we can time without the norm computation
    VectorXf tmpMv;
    {
      Timer::Sentry sentry(computeTimer,"GlobalMv");
      tmpMv = (M*v).eval();
    }
    savedNorm = tmpMv.norm();

    if(__DEBUG) std::cout << "True norm : " << savedNorm << std::endl;

    /* Send matrix to other proc */
    {
      Timer::Sentry sentry(mpiTimer,"MPIScatterV");
      MPI_Scatterv(
          M.data(),         //buffer_send
          counts,           //number of elements to send to each proc
          displacements,    //displacement to each process
          MPI_FLOAT,        //type send
          &valuesBuff,      //buffer to receive valuesBuff
          localNbRows*N,    //number of elements on the reveive buffer
          MPI_FLOAT,        //datatype receive
          0,                //root MPI ID
          MPI_COMM_WORLD);
    }

    /* Setup to send the whole vector, going to use this in Bcast */
    // vecBuff = v.data(); //TODO : NO IDEA WHY JUST THIS DOESN'T WORK ????
    auto tmp = v.data();
    for(size_t i =0; i<N;i++){
      vecBuff[i] = tmp[i];
    }
  } // if(myid == 0)
  else{
    MPI_Scatterv(NULL, NULL, NULL, MPI_FLOAT,
        &valuesBuff,
        localNbRows*N,
        MPI_FLOAT,        //Datatype receive
        0,                //root MPI ID
        MPI_COMM_WORLD);
  } // end if(myid == 0)


  {
    Timer::Sentry sentry(mpiTimer, "BcastVector");
    //Bcast the whole vector into vecBuffer
    MPI_Bcast(&vecBuff, N, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }

  MatrixXf localMat;
  {
    Timer::Sentry sentry(initTimer,"InitLocalMatrix");
    /* Receiving objects */
    //Build Eigen local matrix based on valuesBuff
    localMat = Eigen::Map<Matrix<float, -1, -1, Eigen::RowMajor>>(
                                                          valuesBuff,
                                                          localNbRows,
                                                          N);
  }

  VectorXf localVec;
  {
    Timer::Sentry sentry(initTimer,"InitLocalVector");
    //Build Eigen vector based on vecBuffer
    localVec = Eigen::Map<VectorXf>(vecBuff, N);
  }

  //Compute partial Mv product
  VectorXf localRes; 
  {
    Timer::Sentry sentry(computeTimer,"LocalMv");
    localRes = (localMat * localVec).eval();
  }
  //TODO : Add some computation (maybe compute local SVD with eigen ?)
  //TODO HERE : CALL NEURAL NETWORK (and time it)

  //Eigen::JacobiSVD<MatrixXf, Eigen::ComputeThinU | Eigen::ComputeThinV> svd(localMat);
  //svd.singularValues();
  //svd.matrixU();
  //svd.matrixV();

  //std::cout << "Its singular values are:" << std::endl << svd.singularValues() << std::endl;

  if(myid == 0){
    float finalBuff[N];
    {
      Timer::Sentry sentry(mpiTimer,"MPIGatherV");
      MPI_Gatherv(localRes.data(),      //const void* buffer_send,
                  localNbRows,          //int count_send,
                  MPI_FLOAT,            //MPI_Datatype datatype_send,
                  &finalBuff,           //void* buffer_recv,
                  counts_final,         //const int* counts_recv,
                  displacements_final,  //const int* displacements,
                  MPI_FLOAT,            //MPI_Datatype datatype_recv,
                  0,                    //int root,
                  MPI_COMM_WORLD);      //MPI_Comm communicator);
    }

    VectorXf finalResult = Eigen::Map<VectorXf>(finalBuff, N);
    finalNorm = finalResult.norm();
    if(__DEBUG) std::cout << "FINAL NORM = "<< finalNorm << std::endl;

    //not working 100% of times with that much precision
    // assert(abs(finalNorm - savedNorm) <= std::numeric_limits<float>::epsilon());
    assert(abs(finalNorm - savedNorm) <= 0.1);
  }
  else{
    MPI_Gatherv(localRes.data(),  //buffer_send,
                localNbRows,      //count_send,
                MPI_FLOAT,        //datatype_send,
                NULL,             //buffer_recv,
                NULL,             //counts_recv,
                NULL,             //displacements,
                MPI_FLOAT,        //datatype_recv,
                0,                //root,
                MPI_COMM_WORLD);  //communicator
  }

  if(myid==0){
    initTimer.printInfo();
    mpiTimer.printInfo();
    computeTimer.printInfo();

    std::cout << "\nSEQUENTIAL COMPUTATION : " \
              << computeTimer.get_perf("GlobalMv")
              << std::endl;

    double mpiTime = computeTimer.get_perf("LocalMv")
                    // + mpiTimer.get_perf("BcastVector")
                    + mpiTimer.get_perf("MPIScatterV")
                    + mpiTimer.get_perf("MPIGatherV");

    std::cout << "MPI COMPUTATION : " \
              << mpiTime
              << std::endl;
  }

  /* Finalize MPI */
  MPI_Finalize();
  return 0;
}
