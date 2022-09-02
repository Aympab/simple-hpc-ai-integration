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

#TODO : add input parameter with configuration name and archive all compiledNN

root_path="/net/jabba/home0/am611608/source/simple-hpc/frameworks/ONNX/models"
shapes=[str(i) for i in [60, 200, 3000, 3536]]

# target = "llvm -mcpu=skylake-avx512 --system-lib -num-cores=32"
target = 'llvm -mcpu=skylake-avx512'
# opt=' --system-lib -num-cores=32 -opt-level=3'
# opt='-num-cores=32 -opt-level=3'
input_name = "input_1:0"



for bs in shapes :
    print('Compiling with batch size ' + bs + "...")

    model_url = root_path + "/model_bs" + bs +".onnx"
    onnx_model = onnx.load(model_url)

    x = np.ones((int(bs),1,2))


    shape_dict = {input_name: x.shape}
    mod, params = relay.frontend.from_onnx(onnx_model,
                                           shape=shape_dict,
                                           dtype='float32')

    # mod2, params2 = relay.optimize(mod, target=target, params=params)

    with tvm.transform.PassContext(opt_level=3) :
        compiled_lib = relay.build(mod,
                                   target=tvm.target.Target(target),
                                   params=params)

        compiled_lib.export_library("compiledNN-bs" + bs + ".so")

#TODO check option LLVM online, chekc O3

opt-level,
cl-opt,
fast-math-arcp,
fast-math-contract,
fast-math-nsz,
libs,
device,
host,
num-cores,
fast-math,
tag,
from_device,
fast-math-nnan,
mattr,
keys,
model,
mcpu,
fast-math-reassoc,
mtriple,
fast-math-ninf,
mfloat-abi,
mabi.
# fast-math-reassoc,
# fast-math-contract,
# device,
# libs,
# opt-level,
# host,
# tag,
# fast-math,
# from_device,
# runtime,
# mattr,
# mcpu,
# unpacked-api,
# model,
# keys,
# mfloat-abi,
# mabi,
# system-lib,
# num-cores,
# link-params,
# mtriple,
# interface-api,
# fast-math-ninf,
# fast-math-nnan,
# fast-math-nsz,
# fast-math-arcp