from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import math
import tensorflow as tf
NUM_CLASSES = 10
IMAGE_SIZE = 28
IMAGE_PIXELS = IMAGE_SIZE * IMAGE_SIZE
def initWeightAndBiase(dim1, dim2):
	stdd = 1.0 / math.sqrt(float(dim1))
	w = tf.Variable(tf.truncated_normal([dim1, dim2], stddev=stdd), name='weights')
	b = tf.Variable(tf.zeros([dim2]), name='biases')
	return w,b

def inference(images, hidden1_units, hidden2_units):
	with tf.name_scope('hidden1'):
		weights, biases = initWeightAndBiase(IMAGE_PIXELS, hidden1_units)
		hidden1 = tf.nn.relu(tf.matmul(images, weights) + biases)
	with tf.name_scope('hidden2'):
		weights, biases = initWeightAndBiase(hidden1_units, hidden2_units)
		hidden2 = tf.nn.relu(tf.matmul(hidden1, weights) + biases)
	with tf.name_scope('softmax_linear'):
		weights, biases = initWeightAndBiase(hidden2_units, NUM_CLASSES)
		logits = tf.matmul(hidden2, weights) + biases
	return logits

def loss(logits, labels):
	labels = tf.to_int64(labels)
	cross_entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(labels=labels, logits=logits, name='xentropy')
	return tf.reduce_mean(cross_entropy, name='xentropy_mean')

def training(loss, learning_rate):
	tf.summary.scalar('loss', loss)
	optimizer = tf.train.GradientDescentOptimizer(learning_rate)
	global_step = tf.Variable(0, name='global_step', trainable=False)
	train_op = optimizer.minimize(loss, global_step=global_step)
	return train_op

def evaluation(logits, labels):
	correct = tf.nn.in_top_k(logits, labels, 1)
	return tf.reduce_sum(tf.cast(correct, tf.int32))
