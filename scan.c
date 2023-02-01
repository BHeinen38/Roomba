/*
 * scan.c
 *
 *  Created on: Apr 24, 2021
 *      Author: eks144
 */

#include "adc.h"
#include "scan.h"
#include "lcd.h"
#include "uart-interrupt.h"
#include "timer.h"
#include "ping.h"
#include "servo.h"
#include <math.h>
#define PI 3.14159265358979323846

//  IR Sensor:      10 - 80 cm  Is pin point accurate; has poor range; needs calibration
//  Ping Sensor:    5 - 300 cm  No directional accuracy > +/- 4degrees, has large range and does not require calibration.
//                              Has decent accuracy for singular objects or range values.

/**
 * Function takes in an array of custom objects. will return the number of valid small tall objects (width < 7 )
 * moves servo arm, takes a IR read value, converts it to a distance.
 * using the angle when the distance becomes within the sensors threshold for accuracy and the last angle before the IR distance value jumps, we can determine
 * the arc angle of the object.
 */
int getObjects(object objects[], int number_of_objects)
{
    int smallObj = 0;   // counts the number of small objects
    int obj = 0;        // used as a boolean for obstacle detection
    int i = 0;          // used for counting the angle of the current scan  also a loop iterator
    int j = 0;
    int d;
    int init_angle;
    int final_angle;
    for (i = 0; i <= 180; i++)
    {
        if(command_byte == 2)
        {
            break;
        }
        servo_move(i);
        int IRval = adc_read();

        //Calibrated Line of Best Fit Equation:
        d = (96248 * pow(IRval, -1.14));

        if (d < 80.0 && obj == 0)
        {
            obj = 1;
            init_angle = i;
            objects[j].distance = d;

        }

        if (d > 80.0 && obj == 1)
        {
            final_angle = i;
            double theta = (final_angle - init_angle);

            double dist = objects[j].distance;

            double width = (2.0 * PI * dist * (theta / 360));

            objects[j].angle = (final_angle + init_angle) / 2;

            objects[j].width = width;

            //calibration for the IRvalues. This is because the servo arm and the distance given by the 288 sim GUI have differnt origin angles.
            if (i < 55)
                objects[j].distance -= 7;
            if (i < 15)
                objects[j].distance -= 2.5;
            if (i > 120)
                objects[j].distance -= 3;

            //if the object has a width within these bounds then it is a valid small object. Incorrect distance and angle measurements are handled in main
            if(objects[j].width < 7 && objects[j].width > 4) smallObj++;

            //Prints valid objects for the scan.
            printf("Angle: %d\tDistance: %lf\tWidth: %lf\n", objects[j].angle,
                   objects[j].distance, objects[j].width);
            obj = 0;
            j++;
        }
        //timer for consistency sake
        timer_waitMillis(25);
    }
    return smallObj;
}

