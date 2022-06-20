#include <onnxruntime_cxx_api.h>

// extern const OrtApi* g_ort;
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


void inline print_buffer(std::string infos, int* buffer, size_t N){
  std::cout << "\nBUFFER " << infos << "=\n";
  for (size_t i = 0; i<N; ++i){
    std::cout << buffer[i];
    if(i < N-1)
      std::cout << ", ";
    else
      std::cout << std::endl;
  }
}

const OrtApi* g_ort = NULL;

int run_inference(OrtSession* session){//, const ORTCHAR_T* input_file, const ORTCHAR_T* output_file) {
    size_t model_input_ele_count;
    // float model_input[] = {1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2};
    float model_input[] = {1.2, 1.2};
    std::Array<float, 120> model_input;
    model_input::fill(1.2);

    const size_t model_input_len = 2;
    const int64_t input_shape[] = {60,1,2};
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