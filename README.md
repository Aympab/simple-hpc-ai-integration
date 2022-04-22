# Toy HPC simulation calling a Neural Network

## Create a python virtual env
- Creation : `python3 -m venv my-venv`
- Activation : `source my-env/bin/activate`
- Package installation : `python3 -m pip install ipykernel`


## Compilation
### Export ONNX Library
Append the `LD_LIBRARY_PATH` variable with the lib of onnxruntine :

`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/source/simple-hpc/external/onnxruntime/lib/`

### Build
`make`


mpicxx -o main -g -Wall -Wno-deprecated -Iexternal/onnxruntime/include -Lexternal/onnxruntime/lib -lonnxruntime -lm -std=c++11     main.cpp
# TODO : CHANGER L'ORDRED'APPEL