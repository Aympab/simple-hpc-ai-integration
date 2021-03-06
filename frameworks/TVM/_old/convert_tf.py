#For debugging we need to append path when running the file + module load
import sys
sys.path.insert(0,'/net/jabba/home0/am611608/source/tvm/python')

# tvm, relay
import tvm
from tvm import te
from tvm import relay

# os and numpy
import numpy as np
import os.path

# Tensorflow imports
import tensorflow as tf 

try:
    tf_compat_v1 = tf.compat.v1
except ImportError:
    tf_compat_v1 = tf

# Tensorflow utility functions
import tvm.relay.testing.tf as tf_testing

# Base location for model related files.
repo_base = "/net/jabba/home0/am611608/source/SAMPO/cas_test/nn/mutation_toy_case_best"

model_name = "saved_model.pb"
model_url = os.path.join(repo_base, model_name)


# Target settings
# Use these commented settings to build for cuda.
# target = tvm.target.Target("cuda", host="llvm")
# layout = "NCHW"
# dev = tvm.cuda(0)
target = tvm.target.Target("llvm", host="llvm")
layout = None
dev = tvm.cpu(0)

model_path = model_url


######################################################################
# Import model
# ------------
# Creates tensorflow graph definition from protobuf file.
model = tf.keras.models.load_model(repo_base, compile=False)
# tf.saved_model.load(repo_base)
with tf_compat_v1.gfile.GFile(model_path, "rb") as f:
    graph_def = tf_compat_v1.GraphDef()
    graph_def.ParseFromString(f.read())
    graph = tf.import_graph_def(graph_def, name="")
    # Call the utility to import the graph definition into default graph.
    graph_def = tf_testing.ProcessGraphDefParam(graph_def)
    # Add shapes to the graph.
    # with tf_compat_v1.Session() as sess:
        # graph_def = tf_testing.AddShapesToGraphDef(sess, "softmax")



######################################################################
# Import the graph to Relay
# -------------------------
# Import tensorflow graph definition to relay frontend.
#
# Results:
#   sym: relay expr for given tensorflow protobuf.
#   params: params converted from tensorflow params (tensor protobuf).
# shape_dict = {"DecodeJpeg/contents": x.shape}
# dtype_dict = {"DecodeJpeg/contents": "uint8"}
# mod, params = relay.frontend.from_tensorflow(graph_def, layout=layout)#, shape=shape_dict)

# print("Tensorflow protobuf imported to relay frontend.")
# ######################################################################
# # Relay Build
# # -----------
# # Compile the graph to llvm target with given input specification.
# #
# # Results:
# #   graph: Final graph after compilation.
# #   params: final params after compilation.
# #   lib: target library which can be deployed on target with TVM runtime.

# with tvm.transform.PassContext(opt_level=3):
#     lib = relay.build(mod, target, params=params)

# ######################################################################
# # Execute the portable graph on TVM
# # ---------------------------------
# # Now we can try deploying the compiled model on target.

# from tvm.contrib import graph_executor

# dtype = "uint8"
# m = graph_executor.GraphModule(lib["default"](dev))
# # set inputs
# m.set_input("DecodeJpeg/contents", tvm.nd.array(x.astype(dtype)))
# # execute
# m.run()
# # get outputs
# tvm_output = m.get_output(0, tvm.nd.empty(((1, 1008)), "float32"))

# ######################################################################
# # Process the output
# # ------------------
# # Process the model output to human readable text for InceptionV1.
# predictions = tvm_output.numpy()
# predictions = np.squeeze(predictions)

# # Creates node ID --> English string lookup.
# node_lookup = tf_testing.NodeLookup(label_lookup_path=map_proto_path, uid_lookup_path=label_path)

# # Print top 5 predictions from TVM output.
# top_k = predictions.argsort()[-5:][::-1]
# for node_id in top_k:
#     human_string = node_lookup.id_to_string(node_id)
#     score = predictions[node_id]
#     print("%s (score = %.5f)" % (human_string, score))

# ######################################################################
# # Inference on tensorflow
# # -----------------------
# # Run the corresponding model on tensorflow


# def create_graph():
#     """Creates a graph from saved GraphDef file and returns a saver."""
#     # Creates graph from saved graph_def.pb.
#     with tf_compat_v1.gfile.GFile(model_path, "rb") as f:
#         graph_def = tf_compat_v1.GraphDef()
#         graph_def.ParseFromString(f.read())
#         graph = tf.import_graph_def(graph_def, name="")
#         # Call the utility to import the graph definition into default graph.
#         graph_def = tf_testing.ProcessGraphDefParam(graph_def)


# def run_inference_on_image(image):
#     """Runs inference on an image.

#     Parameters
#     ----------
#     image: String
#         Image file name.

#     Returns
#     -------
#         Nothing
#     """
#     if not tf_compat_v1.gfile.Exists(image):
#         tf.logging.fatal("File does not exist %s", image)
#     image_data = tf_compat_v1.gfile.GFile(image, "rb").read()

#     # Creates graph from saved GraphDef.
#     create_graph()

#     with tf_compat_v1.Session() as sess:
#         softmax_tensor = sess.graph.get_tensor_by_name("softmax:0")
#         predictions = sess.run(softmax_tensor, {"DecodeJpeg/contents:0": image_data})

#         predictions = np.squeeze(predictions)

#         # Creates node ID --> English string lookup.
#         node_lookup = tf_testing.NodeLookup(
#             label_lookup_path=map_proto_path, uid_lookup_path=label_path
#         )

#         # Print top 5 predictions from tensorflow.
#         top_k = predictions.argsort()[-5:][::-1]
#         print("===== TENSORFLOW RESULTS =======")
#         for node_id in top_k:
#             human_string = node_lookup.id_to_string(node_id)
#             score = predictions[node_id]
#             print("%s (score = %.5f)" % (human_string, score))


# run_inference_on_image(img_path)
print("Done !")