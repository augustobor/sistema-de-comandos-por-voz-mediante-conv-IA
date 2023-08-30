/* EmbedIA model definition file*/
#ifndef EJEMPLO_MODEL_H
#define EJEMPLO_MODEL_H

/*

+-------------------+-----------------+------------+-------------+-------+------------+
| Layer(activation) | Name            | #Param(NT) |    Shape    |  MACs | Size (KiB) |
+-------------------+-----------------+------------+-------------+-------+------------+
| Conv2D(relu)      | conv2d_5        |     80(80) | (21, 30, 8) | 45360 |     0.375  |
| MaxPooling2D      | max_pooling2d_4 |          0 | (10, 15, 8) |     0 |     0.000  |
| Conv2D(relu)      | conv2d_6        |        528 | (9, 14, 16) | 64512 |     2.188  |
| Flatten           | flatten_2       |          0 |   (2016,)   |     0 |     0.000  |
| Dense(relu)       | dense_4         |      32272 |    (16,)    | 32256 |   126.125  |
| Dense(softmax)    | dense_5         |         51 |     (3,)    |    48 |     0.211  |
+-------------------+-----------------+------------+-------------+-------+------------+
Total params (NT)....: 32931(80)
Total size in KiB....: 128.898
Total MACs operations: 142176

*/

#include "embedia.h"

#define INPUT_CHANNELS 1
#define INPUT_WIDTH 32
#define INPUT_HEIGHT 23

#define INPUT_SIZE 736


void model_init();

void model_predict(data3d_t input, data1d_t * output);

int model_predict_class(data3d_t input, data1d_t * results);

#endif
