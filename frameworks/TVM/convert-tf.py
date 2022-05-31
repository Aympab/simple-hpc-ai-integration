# tvm, relay
import tvm
from tvm import te
from tvm import relay

# os and numpy
import numpy as np
import os.path

# Tensorflow imports
import tensorflow as tf


# # Ask tensorflow to limit its GPU memory to what's actually needed
# # instead of gobbling everything that's available.
# # https://www.tensorflow.org/guide/gpu#limiting_gpu_memory_growth
# # This way this tutorial is a little more friendly to sphinx-gallery.
# gpus = tf.config.list_physical_devices("GPU")
# if gpus:
#     try:
#         for gpu in gpus:
#             tf.config.experimental.set_memory_growth(gpu, True)
#         print("tensorflow will use experimental.set_memory_growth(True)")
#     except RuntimeError as e:
#         print("experimental.set_memory_growth option is not available: {}".format(e))


try:
    tf_compat_v1 = tf.compat.v1
except ImportError:
    tf_compat_v1 = tf

# Tensorflow utility functions
# import tvm.relay.testing.tf as tf_testing


model_path ="/home/cea/am611608/source/simple-hpc/models/sampo-model/cas_test/nn/mutation_toy_case_best/saved_model.pb"


target = tvm.target.Target("llvm", host="llvm")
layout = None
dev = tvm.cpu(0)

with tf_compat_v1.gfile.GFile(model_path, "rb") as f:
    graph_def = tf_compat_v1.GraphDef()
    graph_def.ParseFromString(f.read())
    graph = tf.import_graph_def(graph_def, name="")
    # Call the utility to import the graph definition into default graph.
    # graph_def = tf_testing.ProcessGraphDefParam(graph_def)
    # Add shapes to the graph.
    # with tf_compat_v1.Session() as sess:
        # graph_def = tf_testing.AddShapesToGraphDef(sess, "softmax")