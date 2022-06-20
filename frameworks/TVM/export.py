# import tensorflow as tf
# from tensorflow.python.compiler.tensorrt import trt_convert as trt

# dir="/net/jabba/home0/am611608/source/simple-hpc/models/sampo-model/cas_test/nn/mutation_toy_case_best"
# converter = trt.TrtGraphConverterV2(input_saved_model_dir=dir)
# converter.convert()
# converter.save("./test")

import onnx
import numpy as np
import tvm
from tvm import te
import tvm.relay as relay
from tvm.contrib.download import download_testdata

model_url = "/net/jabba/home0/am611608/source/simple-hpc/frameworks/TVM/modelBS1.onnx"
# model_path = download_testdata(model_url, "super_resolution.onnx", module="onnx")
# now you have super_resolution.onnx on disk
onnx_model = onnx.load(model_url)


target = "llvm"

input_name = "input_1:0"
x = np.ones((1,1,2))


shape_dict = {input_name: x.shape}
mod, params = relay.frontend.from_onnx(onnx_model, shape_dict)

with tvm.transform.PassContext(opt_level=1):
    executor = relay.build_module.create_executor(
        "graph", mod, tvm.cpu(0), target, params
    ).evaluate()

dtype = "float32"
tvm_output = executor(tvm.nd.array(x.astype(dtype))).numpy()
print(tvm_output)