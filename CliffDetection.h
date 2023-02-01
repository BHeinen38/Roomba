/*
 * CliffDetection.h
 *
 *  Created on: Apr 27, 2021
 *      Author: bjheinen
 */

#ifndef CLIFFDETECTION_H_
#define CLIFFDETECTION_H_

int CliffForward(oi_t *sensor_data);

int CliffLeft(oi_t *sensor_data);
int Cliff45Left(oi_t *sensor_data);

int CliffRight(oi_t *sensor_data);
int Cliff45Right(oi_t *sensor_data);

int WallForward(oi_t *sensor_data);

int WallRight(oi_t *sensor_data);
int Wall45Right(oi_t *sensor_data);

int WallLeft(oi_t *sensor_data);
int Wall45Left(oi_t *sensor_data);

int WallCorner(oi_t *sensor_data);




#endif /* CLIFFDETECTION_H_ */
