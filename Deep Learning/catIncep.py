from __future__ import print_function
from keras.applications.inception_v3 import InceptionV3
from keras.preprocessing import image
from keras.models import Model, load_model
from keras.layers import Dense, GlobalAveragePooling2D, GaussianNoise, Dropout, GaussianDropout
from keras import regularizers
from keras.callbacks import ModelCheckpoint, EarlyStopping
from keras.preprocessing.image import ImageDataGenerator
from keras.optimizers import SGD
import os.path
import numpy as np

MODEL_FILE = "cat10086.hd5"


def create_Mocdel(num_hidden, num_classes):
    base_model = InceptionV3(include_top=False, weights='imagenet')
    x = base_model.output
    x = GaussianNoise(0.5)(x)
    x = GlobalAveragePooling2D()(x)
    #    x=GaussianDropout(0.3)(x)
    x = Dense(num_hidden, activation='relu', kernel_regularizer=regularizers.l1(0.002))(x)
    x = Dropout(0.2)(x)
    predictions = Dense(num_classes, activation='softmax')(x)
    for layer in base_model.layers:
        layer.trainable = False
    model = Model(inputs=base_model.input, outputs=predictions)
    return model


def load_existing(model_file):
    model = load_model(model_file)
    numlayers = len(model.layers)
    for layer in model.layers[:numlayers - 3]:
        layer.trainable = False
    for layer in model.layers[numlayers - 3:]:
        layer.trainable = True
    return model


def train(model_file, train_path, validation_path, num_hidden=2000, num_classes=5, steps=32, num_epochs=20,
          save_period=1):
    if os.path.exists(model_file):
        print("exist")
        model = load_existing(model_file)
    else:
        print("Creating")
        model = create_Mocdel(num_hidden, num_classes)
    model.compile(optimizer='rmsprop', loss='categorical_crossentropy', metrics=['accuracy'])

    checkpoint = ModelCheckpoint(model_file, period=save_period)
    stopmodel = EarlyStopping(monitor='val_loss', min_delta=0.001, patience=50)

    train_datagen = ImageDataGenerator(rescale=1. / 255, shear_range=0.2, zoom_range=0.2, horizontal_flip=True,
                                       vertical_flip=True, channel_shift_range=0.2)
    test_datagen = ImageDataGenerator(rescale=1. / 255)
    train_generator = train_datagen.flow_from_directory(train_path, target_size=(249, 249), batch_size=32,
                                                        class_mode="categorical")
    validation_generator = test_datagen.flow_from_directory(validation_path, target_size=(249, 249), batch_size=32,
                                                            class_mode="categorical")

    model.fit_generator(train_generator, steps_per_epoch=steps, epochs=num_epochs, callbacks=[checkpoint, stopmodel],
                        validation_data=validation_generator, validation_steps=50)

    for layer in model.layers[:247]:
        layer.trainable = False

    for layer in model.layers[247:]:
        layer.trainable = True

    model.compile(optimizer='rmsprop', loss='categorical_crossentropy', metrics=['accuracy'])

    model.fit_generator(train_generator, steps_per_epoch=steps, epochs=num_epochs, callbacks=[checkpoint, stopmodel],
                        validation_data=validation_generator, validation_steps=50)


def main():
    train(MODEL_FILE, train_path="signtrain", validation_path="signtest", steps=120, num_epochs=20)


if __name__ == '__main__':
    main()
