import onnx
import numpy as np
import tvm
from tvm import te
import tvm.relay as relay
from tvm.contrib.download import download_testdata
import os

root_path="/net/jabba/home0/am611608/source/simple-hpc/frameworks/ONNX/models"
shapes=[str(i) for i in [60, 200, 3000, 3536]]

# target = "llvm -mcpu=skylake-avx512 --system-lib -num-cores=32"
target = 'llvm -mcpu=skylake-avx512'
opt='-num-cores=32 -opt-level=3'
input_name = "input_1:0"



# for bs in shapes:
bs="60"

x = np.ones((int(bs),1,2))
shape_dict = {input_name: x.shape}
model_url = root_path + "/model_bs" + bs +".onnx"

onnx_model = onnx.load(model_url)


mod, params = relay.frontend.from_onnx(onnx_model,
                                       shape_dict,
                                       dtype='float32')


with relay.build_config(opt_level=3) :
    # executor = relay.build_module.create_executor(
    #     "graph", mod, tvm.cpu(0), target, params
    # ).evaluate()
    fadd_dylib = relay.build(mod,
                             target,
                             params=params)

    dylib_path = "test_dll.so"
    fadd_dylib.export_library(dylib_path)

    # export it as a shared library
    # If you are running cross compilation, you can also consider export
    # to tar and invoke host compiler later.
    # compiled_lib = relay.build(mod, target, params=params)
    # dylib_path = "test_relay.so"
    # compiled_lib.export_library(dylib_path)


# n = te.var("n")
# A = te.placeholder((n,), name="A")
# B = te.compute(A.shape, lambda *i: A(*i) + 1.0, name="B")
# s = te.create_schedule(B.op)
# # Compile library as dynamic library
# fadd_dylib = tvm.build(s, [A, B], "llvm", name="addone")
# dylib_path = os.path.join(base_path, "test_addone_dll.so")
# fadd_dylib.export_library(dylib_path)


