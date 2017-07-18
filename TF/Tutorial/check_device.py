
def is_gpu_available(cuda_only=True):
  from tensorflow.python.client import device_lib as _device_lib
  if cuda_only:
    return any((x.device_type == 'GPU') for x in _device_lib.list_local_devices())
  else:
    return any((x.device_type == 'GPU' or x.device_type == 'SYCL') for x in _device_lib.list_local_devices())

def get_available_gpus():
  from tensorflow.python.client import device_lib as _device_lib
  local_device_protos = _device_lib.list_local_devices()
  return [x.name for x in local_device_protos if x.device_type == 'GPU']
get_available_gpus()

#Creates a graph.
import tensorflow as tf
a = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[2, 3], name='a')
b = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[3, 2], name='b')
c = tf.matmul(a, b)
sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))
print(sess.run(c))