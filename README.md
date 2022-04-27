# Toy HPC simulation calling a Neural Network
Run `$ source env` to load environment variable and aliases needed (explained in
this README) 

## Create a python virtual env
- Creation : `python3 -m venv my-venv`
- Activation : `source my-env/bin/activate`
- Package installation : `python3 -m pip install ipykernel`

## Build with `make`

# ML Integration test

## ONNX Library
Append the `LD_LIBRARY_PATH` variable with the lib of onnxruntine :

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/source/simple-hpc/external/onnxruntime/lib/
```

## TVM
Install the library and set everything up with [TVM's user tutorial](https://tvm.apache.org/docs/tutorial/index.html).

Export TVM_HOME
``` 
$ export TVM_HOME=/net/jabba/home0/am611608/source/tvm
$ export PYTHONPATH=$TVM_HOME/python:${PYTHONPATH}
```

To use `TVMC` with python, use the full command :
```
$ python -m tvm.driver.tvmc <subcommand> <options>
```

From [TVM official tutorial](https://tvm.apache.org/docs/tutorial/tvmc_command_line_driver.html#using-tvmc).

### [Compiling with TVM](https://tvm.apache.org/docs/tutorial/tvmc_command_line_driver.html#compiling-an-onnx-model-to-the-tvm-runtime) :

```
$ python -m tvm.driver.tvmc compile    \
         --target "llvm"               \
         --output toy-model-tvm.tar \
         toy-model.onnx
```
