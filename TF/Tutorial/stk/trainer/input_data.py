from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import gzip
import numpy
from six.moves import xrange  # pylint: disable=redefined-builtin
import base
#from trainer import base #TODO
from tensorflow.python.framework import dtypes

pix_per_img = 5 * 28
SOURCE_URL = 'ks3://ai-train-demo/tf-1.0/mnist/input_data/'

def _read32(bytestream):
  dt = numpy.dtype(numpy.uint32).newbyteorder('<')
  return numpy.frombuffer(bytestream.read(4), dtype=dt)[0]


def extract_images(f):
  print('Extracting', f.name)
  with gzip.GzipFile(fileobj=f) as bytestream:
    magic = _read32(bytestream)
    if magic != 2051:
      raise ValueError('Invalid magic number %d in image file: %s' % (magic, f.name))
    num_images = _read32(bytestream)
    print("extract img %s : num_images=%d" % (f.name, num_images))
    buf = bytestream.read(pix_per_img * num_images * 4)
    data = numpy.frombuffer(buf, dtype=numpy.float32)
    data = data.reshape(num_images, pix_per_img, 1)
    return data


def dense_to_one_hot(labels_dense, num_classes):
  num_labels = labels_dense.shape[0]
  index_offset = numpy.arange(num_labels) * num_classes
  labels_one_hot = numpy.zeros((num_labels, num_classes))
  labels_one_hot.flat[index_offset + labels_dense.ravel()] = 1
  return labels_one_hot


def extract_labels(f, one_hot=False, num_classes=11):
  """Extract the labels into a 1D uint8 numpy array [index].
  """
  print('Extracting', f.name)
  with gzip.GzipFile(fileobj=f) as bytestream:
    magic = _read32(bytestream)
    if magic != 2049:
      raise ValueError('Invalid magic number %d in MNIST label file: %s' % (magic, f.name))
    num_items = _read32(bytestream)
    print("extract lbl %s : num_lbl=%d" % (f.name, num_items))
    buf = bytestream.read(num_items)
    labels = numpy.frombuffer(buf, dtype=numpy.uint8)
    if one_hot:
      return dense_to_one_hot(labels, num_classes)
    return labels


class DataSet(object):

  def __init__(self,
               images,
               labels,
               fake_data=False,
               one_hot=False,
               dtype=dtypes.float32,
               reshape=True):
    dtype = dtypes.as_dtype(dtype).base_dtype
    if dtype not in (dtypes.uint8, dtypes.float32):
      raise TypeError('Invalid image dtype %r, expected uint8 or float32' % dtype)
    if fake_data:
      self._num_examples = 10000
      self.one_hot = one_hot
    else:
      assert images.shape[0] == labels.shape[0], ('images.shape: %s labels.shape: %s' % (images.shape, labels.shape))
      self._num_examples = images.shape[0]

    self._images = images
    self._labels = labels
    self._epochs_completed = 0
    self._index_in_epoch = 0

  @property
  def images(self):
    return self._images

  @property
  def labels(self):
    return self._labels

  @property
  def num_examples(self):
    return self._num_examples

  @property
  def epochs_completed(self):
    return self._epochs_completed

  def next_batch(self, batch_size, fake_data=False):
    """Return the next `batch_size` examples from this data set."""
    if fake_data:
      fake_image = [1] * 784
      if self.one_hot:
        fake_label = [1] + [0] * 9
      else:
        fake_label = 0
      return [fake_image for _ in xrange(batch_size)], [
          fake_label for _ in xrange(batch_size)
      ]
    start = self._index_in_epoch
    self._index_in_epoch += batch_size
    if self._index_in_epoch > self._num_examples:
      # Finished epoch
      self._epochs_completed += 1
      # Shuffle the data
      perm = numpy.arange(self._num_examples)
      numpy.random.shuffle(perm)
      self._images = self._images[perm]
      self._labels = self._labels[perm]
      # Start next epoch
      start = 0
      self._index_in_epoch = batch_size
      assert batch_size <= self._num_examples
    end = self._index_in_epoch
    return self._images[start:end], self._labels[start:end]


def read_data_sets(train_dir,
                   fake_data=False,
                   one_hot=False,
                   dtype=dtypes.float32,
                   reshape=True,
                   validation_size=5000):
  if fake_data:

    def fake():
      return DataSet([], [], fake_data=True, one_hot=one_hot, dtype=dtype)

    train = fake()
    validation = fake()
    test = fake()
    return base.Datasets(train=train, validation=validation, test=test)

  TRAIN_IMAGES = 'stk_train_img.bin.gz'
  TRAIN_LABELS = 'stk_train_lbl.bin.gz'
  TEST_IMAGES = 'stk_test_img.bin.gz'
  TEST_LABELS = 'stk_test_lbl.bin.gz'

  local_file = base.maybe_download(TRAIN_IMAGES, train_dir, SOURCE_URL + TRAIN_IMAGES)
  with open(local_file, 'rb') as f:
    train_images = extract_images(f)

  local_file = base.maybe_download(TRAIN_LABELS, train_dir, SOURCE_URL + TRAIN_LABELS)
  with open(local_file, 'rb') as f:
    train_labels = extract_labels(f, one_hot=one_hot)

  local_file = base.maybe_download(TEST_IMAGES, train_dir, SOURCE_URL + TEST_IMAGES)
  with open(local_file, 'rb') as f:
    test_images = extract_images(f)

  local_file = base.maybe_download(TEST_LABELS, train_dir, SOURCE_URL + TEST_LABELS)
  with open(local_file, 'rb') as f:
    test_labels = extract_labels(f, one_hot=one_hot)

  if not 0 <= validation_size <= len(train_images):
    raise ValueError(
        'Validation size should be between 0 and {}. Received: {}.'
        .format(len(train_images), validation_size))

  validation_images = train_images[:validation_size]
  validation_labels = train_labels[:validation_size]
  train_images = train_images[validation_size:]
  train_labels = train_labels[validation_size:]

  train = DataSet(train_images, train_labels, dtype=dtype, reshape=reshape)
  validation = DataSet(validation_images, validation_labels, dtype=dtype, reshape=reshape)
  test = DataSet(test_images, test_labels, dtype=dtype, reshape=reshape)
  return base.Datasets(train=train, validation=validation, test=test)


def load_stk(train_dir='../data/input_data/'):
  return read_data_sets(train_dir)
