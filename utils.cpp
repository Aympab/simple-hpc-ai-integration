#include <onnxruntime_cxx_api.h>

extern const OrtApi* g_ort;

int run_inference(OrtSession* session){//, const ORTCHAR_T* input_file, const ORTCHAR_T* output_file) {
    
    size_t model_input_ele_count;

    OrtMemoryInfo* memory_info;
    ORT_ABORT_ON_ERR(g_ort->CreateCpuMemoryInfo(OrtArenaAllocator, OrtMemTypeDefault, &memory_info));
    const int64_t input_shape[] = {1, 1, 28, 28};
    const size_t input_shape_len = sizeof(input_shape) / sizeof(input_shape[0]);
    const size_t model_input_len = model_input_ele_count * sizeof(float);

    // OrtValue* input_tensor = NULL;
    // ORT_ABORT_ON_ERROR(g_ort->CreateTensorWithDataAsOrtValue(memory_info, model_input, model_input_len, input_shape,
    //                                                         input_shape_len, ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
    //                                                         &input_tensor));
    // assert(input_tensor != NULL);
    // int is_tensor;
    // ORT_ABORT_ON_ERROR(g_ort->IsTensor(input_tensor, &is_tensor));
    // assert(is_tensor);
    // g_ort->ReleaseMemoryInfo(memory_info);
    // const char* input_names[] = {"Input3"};
    // const char* output_names[] = {"Plus214_Output_0"};

    // std::array<float, 10> results_{};
    // int result_{0};
    // const int64_t output_shape[] = {1, 10};
    // const size_t output_shape_len = sizeof(output_shape) / sizeof(output_shape[0]);

    // OrtValue* output_tensor = NULL;
    // ORT_ABORT_ON_ERROR(g_ort->CreateTensorWithDataAsOrtValue(memory_info, results_.data(), results_.size()*sizeof(float), output_shape, output_shape_len, ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT, &output_tensor));

    // ORT_ABORT_ON_ERROR(
    //     g_ort->Run(session, NULL, input_names, (const OrtValue* const*)&input_tensor, 1, output_names, 1, &output_tensor));
    // assert(output_tensor != NULL);
    // ORT_ABORT_ON_ERROR(g_ort->IsTensor(output_tensor, &is_tensor));
    // assert(is_tensor);





//   size_t input_height;
//   size_t input_width;
//   float* model_input;
//   size_t model_input_ele_count;

//   const char* output_file_p = output_file;
//   const char* input_file_p = input_file;

//   if (read_png_file(input_file_p, &input_height, &input_width, &model_input, &model_input_ele_count) != 0) {
//     return -1;
//   }
//   if (input_height != 720 || input_width != 720) {
//     std::cout << "please resize to image to 720x720" << std::endl;
//     free(model_input);
//     return -1;
//   }

//   OrtMemoryInfo* memory_info;
//   ORT_ABORT_ON_ERROR(g_ort->CreateCpuMemoryInfo(OrtArenaAllocator, OrtMemTypeDefault, &memory_info));
//   const int64_t input_shape[] = {1, 3, 720, 720};
//   const size_t input_shape_len = sizeof(input_shape) / sizeof(input_shape[0]);
//   const size_t model_input_len = model_input_ele_count * sizeof(float);

//   OrtValue* input_tensor = NULL;
//   ORT_ABORT_ON_ERROR(g_ort->CreateTensorWithDataAsOrtValue(memory_info, model_input, model_input_len, input_shape,
//                                                            input_shape_len, ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
//                                                            &input_tensor));
//   assert(input_tensor != NULL);
//   int is_tensor;
//   ORT_ABORT_ON_ERROR(g_ort->IsTensor(input_tensor, &is_tensor));
//   assert(is_tensor);
//   g_ort->ReleaseMemoryInfo(memory_info);
//   const char* input_names[] = {"inputImage"};
//   const char* output_names[] = {"outputImage"};
//   OrtValue* output_tensor = NULL;
//   ORT_ABORT_ON_ERROR(
//       g_ort->Run(session, NULL, input_names, (const OrtValue* const*)&input_tensor, 1, output_names, 1, &output_tensor));
//   assert(output_tensor != NULL);
//   ORT_ABORT_ON_ERROR(g_ort->IsTensor(output_tensor, &is_tensor));
//   assert(is_tensor);
//   int ret = 0;
//   if (write_tensor_to_png_file(output_tensor, output_file_p) != 0) {
//     ret = -1;
//   }
//   g_ort->ReleaseValue(output_tensor);
//   g_ort->ReleaseValue(input_tensor);
//   free(model_input);
  return 3;
}
