import numpy as np
import tensorflow as tf

def cnn_model_fn(features, labels, mode):
    # Model function for CNN

    # Input layer.  Our image is [28x28x1], but
    # tf expects images in [batch size, 28, 28, 1] so
    # we reshape our input layer
    input_layer = tf.reshape(features['x'], [-1, 28, 28, 1])

    # Conv layer 1, connected to input. 
    # We will create 32 kernels in this layer.
    # Each kernel is 5x5 pixels. padding="same"
    # means that we will not expand the image
    # to accomodate the kernel, and will stay at
    # 28x28.  We will end up with an output with
    # dimensions [-1, 28, 28, 32], as we will still
    # have a 28x28 image, but now output from 32
    # kernels.

    conv1=tf.layers.conv2d(inputs=input_layer,\
    filters=32,\
    kernel_size=[5,5],\
    padding="same",\
    activation = tf.nn.relu)

    # Create a pooling layer to reduce from 28x28 to 14x14
    # Thus within the 28x28 image from each of the 32
    # kernels, we move a 2x2 window over and choose the
    # maximum value within the 2x2 window. This effectively
    # halves the image in width and height.
    # We then move right (or down) by 2 pixes (strides=2)
    # Since the maxpool is 2x2 and we move 2 pixels this
    # means that the sampling does not overlap.
    # The end result is a [-1, 14, 14, 32] output.

    pool1 = tf.layers.max_pooling2d(inputs=conv1,\
    pool_size=[2,2],\
    strides=2)

    # Create another convolutional layer. Here we
    # create 64 kernels.  The input now though is 14x14
    # because of maxpooling.  The output will be
    # [-1, 14, 14, 64] since we are making 64 kernels
    conv2 = tf.layers.conv2d(\
    inputs=pool1,\
    filters=64,\
    kernel_size=[5,5],\
    padding="same",\
    activation=tf.nn.relu)

    # Second pooling layer 
    # This again reduces the height and width to 7x7
    # End output is [-1, 7, 7, 64]
    pool2=tf.layers.max_pooling2d(inputs=conv2,\
    pool_size=[2,2],\
    strides=2)

    # Dense layer. We expect just a single 1D input
    # (Not counting batch)
    # So we collapse down to a [-1, 7x7x64] vector.
    pool2_flat = tf.reshape(pool2, [-1, 7*7*64])
    dense = tf.layers.dense(inputs=pool2_flat,\
    units=1024,\
    activation=tf.nn.relu)

    # Dropout. We will randomly drop out 40% of the data
    # during training to reduce overfitting

    dropout=tf.layers.dropout(inputs=dense,\
    rate=0.4,\
    training = mode == tf.estimator.ModeKeys.TRAIN)

    # Logits layer. This is our output layer.
    logits = tf.layers.dense(inputs=dropout, units=10)

    # Specifies what we produce when we estimate. We do two things:
    # We output the index of the class with the maximum logits value
    # and the probaility of that class.
    predictions = {\
    'classes':tf.argmax(input=logits, axis=1),\
    'probabilities':tf.nn.softmax(logits, name='softmax_tensor')}

    # In prediction mode we call the estimator
    if mode == tf.estimator.ModeKeys.PREDICT:
        return tf.estimator.EstimatorSpec(mode=mode, predictions=predictions)

    loss = tf.losses.sparse_softmax_cross_entropy(labels=labels, logits=logits)

    # In training mode we create the gradient
    # descent optimize and ask it to minimize
    # loss.

    if mode == tf.estimator.ModeKeys.TRAIN:
        optimizer=tf.train.GradientDescentOptimizer(\
        learning_rate=0.01)

        train_op = optimizer.minimize(\
        loss=loss,\
        global_step=tf.train.get_global_step())

        # EstimatorSpec creates a complete estimation model 
        # for use by Tensorflow. It tells TF about the
        # loss function, training operation etc.
        return tf.estimator.EstimatorSpec(\
        mode=mode, loss=loss,\
        train_op = train_op)

    eval_metric_ops={\
    "accuracy":tf.metrics.accuracy(\
    labels=labels, 
    predictions=predictions["classes"])}
    
    return tf.estimator.EstimatorSpec(\
    mode=mode,\
    loss=loss,\
    eval_metric_ops = eval_metric_ops)

def main():
    # Load the training and eval data
    mnist = tf.contrib.learn.datasets.load_dataset("mnist")

    # This is an array of images
    train_data = mnist.train.images

    # We need to turn our labels into a Numpy array that
    # TF understands
    train_labels=np.asarray(mnist.train.labels,\
    dtype=np.int32)

    eval_data = mnist.test.images

    eval_labels = np.asarray(mnist.test.labels, \
    dtype=np.int32)

    # Create the estimator using the EstimatorSpec
    # generated by cnn_model_fn
    mnist_classifier = tf.estimator.Estimator(\
    model_fn=cnn_model_fn, model_dir="./cnn_model")

    # Set up logging. This is optional but useful.
    tensors_to_log={"probabilities": "softmax_tensor"}
    logging_hook = tf.train.LoggingTensorHook(\
    tensors=tensors_to_log, every_n_iter=50)

    # Train the model. numpy_input_fn is a useful
    # function to feed data into the model. Here we
    # feed in batches of 1000 images.
    train_input_fn = tf.estimator.inputs.numpy_input_fn(\
    x={"x":train_data},\
    y=train_labels,\
    batch_size=1000,\
    num_epochs=None,\
    shuffle=True)

    # Call the training method in the Estimator class.
    # We will do 100 steps because each step takes SO LONG.
    # If you re-run this program it will continue the training.
    mnist_classifier.train(\
    input_fn = train_input_fn,\
    steps=100,\
    hooks=[logging_hook])

    # Create a Numpy feed to the estimator
    eval_input_fn = tf.estimator.inputs.numpy_input_fn(\
    x={"x":eval_data},\
    y=eval_labels,\
    num_epochs=1,\
    shuffle=False)

    # Call the evaluate method in the Estimator class to
    # evaluate the performace.
    eval_results = mnist_classifier.evaluate(\
    input_fn = eval_input_fn)

    print(eval_results)


