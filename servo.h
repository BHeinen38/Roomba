/*
 * servo.h
 *
 *  Created on: Apr 6, 2021
 *      Author: bjheinen
 */

#ifndef SERVO_H_
#define SERVO_H_
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>


void servo_init(void);

int servo_move(float degrees);


#endif /* SERVO_H_ */
