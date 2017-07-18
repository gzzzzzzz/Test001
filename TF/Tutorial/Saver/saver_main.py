import tensorflow as tf
import numpy as np
import os.path
save_dir = "./log_save/"
if not tf.gfile.Exists(save_dir):
    tf.gfile.MakeDirs(save_dir)

x_data = np.float32(np.random.rand(2, 47))
y_data = np.dot([0.1, 0.2], x_data) + 0.3

b = tf.Variable(tf.zeros([1]))
W = tf.Variable(tf.random_uniform([1, 2], -1.0, 1.0))
y = tf.matmul(W, x_data) + b

loss = tf.reduce_mean(tf.square(y - y_data))
optimizer = tf.train.GradientDescentOptimizer(0.5)
train = optimizer.minimize(loss)
init = tf.global_variables_initializer()

sess = tf.Session()
sess.run(init)
saver = tf.train.Saver()
def train_step():
    for step in range(0, 50):
        sess.run(train)
        if step % 20 == 0:
            checkpoint_file = os.path.join(save_dir, 'model.ckpt')
            saver.save(sess, checkpoint_file, global_step=step)
            print("training.... ", step, sess.run(W), sess.run(b))
def load_saver():
    ckpt = tf.train.get_checkpoint_state(save_dir)
    if ckpt and ckpt.model_checkpoint_path:
            saver.restore(sess, ckpt.model_checkpoint_path)
    else:
        print("not find chekpoint")
        pass
    print("loaded W:", sess.run(W))
    print("loaded b:", sess.run(b))
load_saver()
train_step()
#W: [[0.100  0.200]], b: [0.300