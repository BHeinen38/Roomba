/*
 * movement.h
 *
 *  Created on: Feb 9, 2021
 *      Author: eks144
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "movement.h"
#include "open_interface.h"
#include "lcd.h"

void move_forward(oi_t *sensor_data, double distance_mm);

int move_forwardFinal(oi_t *sensor_data, double distance_mm);

void move_backwards(oi_t *sensor_data, double distance_mm);

void move_right(oi_t *sensor_data, double degrees);

void move_left(oi_t *sensor_data, double degrees);

void ping_turn_left(oi_t *sensor_data);
void ping_turn_right(oi_t *sensor_data);

void ping_moveWhite(oi_t *sensor_data);
#endif /* MOVEMENT_H_ */
