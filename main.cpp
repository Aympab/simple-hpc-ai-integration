#include <iostream>
#include <mpi.h>
// #include "utils.cpp"
#include <onnxruntime_cxx_api.h>
#include <assert.h>

#define ORT_ABORT_ON_ERR(expr)                               \
  do {                                                       \
    OrtStatus* onnx_status = (expr);                         \
    if (onnx_status != NULL) {                               \
      const char* msg = g_ort->GetErrorMessage(onnx_status); \
      fprintf(stderr, "%s\n", msg);                          \
      g_ort->ReleaseStatus(onnx_status);                     \
      abort();                                               \
    }                                                        \
  } while (0);

const OrtApi* g_ort = NULL;

int run_inference(OrtSession* session){//, const ORTCHAR_T* input_file, const ORTCHAR_T* output_file) {
    size_t model_input_ele_count;
    float model_input[] = {1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2};
    const size_t model_input_len = 8;
    const int64_t input_shape[] = {1};
    const size_t input_shape_len = 1;

    OrtMemoryInfo* memory_info;
    ORT_ABORT_ON_ERR(g_ort->CreateCpuMemoryInfo(OrtArenaAllocator, OrtMemTypeDefault, &memory_info));
    // const int64_t input_shape[] = {1, 1, 1, 8};
    // const size_t input_shape_len = sizeof(input_shape) / sizeof(input_shape[0]);
    // const size_t model_input_len = model_input_ele_count * sizeof(float);

    OrtValue* input_tensor = NULL;
    ORT_ABORT_ON_ERR(g_ort->CreateTensorWithDataAsOrtValue(
        memory_info,
        model_input,
        model_input_len,
        input_shape,
        input_shape_len,
        ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
        &input_tensor));

    assert(input_tensor != NULL);
    int is_tensor;
    ORT_ABORT_ON_ERR(g_ort->IsTensor(input_tensor, &is_tensor));
    assert(is_tensor);
    g_ort->ReleaseMemoryInfo(memory_info);
    // const char* input_names[] = {"Input3"};
    // const char* output_names[] = {"Plus214_Output_0"};

    // std::array<float, 1> results_{};
    // int result_{0};
    // const int64_t output_shape[] = {1};
    // const size_t output_shape_len = sizeof(output_shape) / sizeof(output_shape[0]);

    const int64_t output_shape[] = {1};

    OrtValue* output_tensor = NULL;
    ORT_ABORT_ON_ERR(g_ort->CreateTensorWithDataAsOrtValue(
        memory_info,
        NULL,
        sizeof(float),
        output_shape,
        1,
        ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
        &output_tensor
    ));

    // ORT_ABORT_ON_ERR(
        // g_ort->Run(session, NULL, input_names, (const OrtValue* const*)&input_tensor, 1, output_names, 1, &output_tensor));
    
    ORT_ABORT_ON_ERR(g_ort->Run(
        session,
        NULL,
        NULL,
        (const OrtValue* const*)&input_tensor,
        (size_t)8,
        NULL,
        1,
        &output_tensor
    ));
    // assert(output_tensor != NULL);
    // ORT_ABORT_ON_ERROR(g_ort->IsTensor(output_tensor, &is_tensor));
    // assert(is_tensor);



//   OrtSession* session;
//   ORT_ABORT_ON_ERROR(g_ort->CreateSession(env, model_path, session_options, &session));
//   verify_input_output_count(session);
//   int ret = run_inference(session, input_file);
//   g_ort->ReleaseSessionOptions(session_options);
//   g_ort->ReleaseSession(session);
//   g_ort->ReleaseEnv(env);
//   if (ret != 0) {
//     std::cout << "fail." << std::endl;
//   }
  return 0;
}



int main (int argc, char* argv[]){
    int myid, num_procs;

    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);


    //Do stuff
    if (myid > 0){
        std::cout << "Hello World!" << std::endl;
    }

    //Call Neural Network
    g_ort = OrtGetApiBase()->GetApi(ORT_API_VERSION);

    // ORTCHAR_T* model_path = argv[1];
    // ORTCHAR_T* input_file = argv[2];
    // ORTCHAR_T* output_file = argv[3];
    // ORTCHAR_T* execution_provider = (argc >= 5) ? argv[4] : NULL;
    // OrtEnv* env;
    // ORT_ABORT_ON_ERROR(g_ort->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "test", &env));
    // OrtSessionOptions* session_options;
    // ORT_ABORT_ON_ERROR(g_ort->CreateSessionOptions(&session_options));

    
    OrtEnv* env = NULL;
    ORT_ABORT_ON_ERR(g_ort->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "test", &env));
    
    OrtSessionOptions* session_options = NULL;
    ORT_ABORT_ON_ERR(g_ort->CreateSessionOptions(&session_options));
    
    OrtSession* session = NULL;
    ORT_ABORT_ON_ERR(g_ort->CreateSession(env,
                                         "toy-model.onnx",
                                         session_options,
                                         &session));

    // verify_input_output_count(session);
    // int ret = run_inference(session, input_file, output_file);
    int ret = run_inference(session);



    g_ort->ReleaseSessionOptions(session_options);
    g_ort->ReleaseSession(session);
    g_ort->ReleaseEnv(env);

    /* Finalize MPI */
    MPI_Finalize();
    return 0;
}
