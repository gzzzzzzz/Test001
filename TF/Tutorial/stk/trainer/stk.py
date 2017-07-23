from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import argparse
import os.path
import sys
import time
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
import numpy as np
#from trainer import input_data #TODO
import input_data
FLAGS = None
debug_input = True#True False

def run_training():
  stk_data = input_data.read_data_sets(FLAGS.input_data_dir, one_hot=True)
  if debug_input:
    print("train.images.shape ", stk_data.train.images.shape)
    print("train.labels.shape ", stk_data.train.labels.shape)
    print("test.images.shape ", stk_data.test.images.shape)
    print("test.labels.shape ", stk_data.test.labels.shape)
    img = stk_data.train.images[stk_data.train.images.shape[0]-1]
    lbl = stk_data.train.labels[stk_data.train.labels.shape[0]-1]
    print("last img:", img[0], img[1], img[2], img[3], img[4], img[5])
    print("last lbl:", lbl)
    img = stk_data.test.images[stk_data.test.images.shape[0]-1]
    lbl = stk_data.test.labels[stk_data.test.labels.shape[0]-1]
    print("last test img:", img[0], img[1], img[2], img[3], img[4], img[5])
    print("last test lbl:", lbl)
  sess = tf.InteractiveSession()
  sess.run(tf.global_variables_initializer())

def main(_):
  if tf.gfile.Exists(FLAGS.log_dir):
    tf.gfile.DeleteRecursively(FLAGS.log_dir)
  tf.gfile.MakeDirs(FLAGS.log_dir)
  run_training()

if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument(
      '--input_data_dir',
      type=str,
      default='../data/input_data',
      help='Directory to put the input data.'
  )
  parser.add_argument(
      '--log_dir',
      type=str,
      default='../data/log_data',
      help='Directory to put the log data.'
  )
  parser.add_argument(
      '--ckp_dir',
      type=str,
      default='../data/ckp_data',
      help='Directory to put the check point data.'
  )
  FLAGS, unparsed = parser.parse_known_args()
  tf.app.run(main=main, argv=[sys.argv[0]] + unparsed)