/*
 * scan.h
 *
 *  Created on: Apr 24, 2021
 *      Author: eks144
 */

#ifndef SCAN_H_
#define SCAN_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int angle; //degrees
    float distance; //cm
    float width; //cm
} object;

//int getObjects(objects getObjects[], int size_of_input);

int getObject(object obj[], int num_of_objects);


#endif /* SCAN_H_ */
