/*
 * movement.c
 *
 *  Created on: Feb 9, 2021
 *      Author: eks144
 */

#include "movement.h"
#include "servo.h"
#include "ping.h"
#include <string.h>
#include "lcd.h"
#include "open_interface.h"
#include "timer.h"
#include "CliffDetection.h"

//double move_left(oi_t *sensor_data, double degrees);
//double move_right(oi_t *sensor_data, double degrees);
//double move_backwards(oi_t *sensor_data, double distance_mm);
//double move_forward(oi_t *sensor_data, double distance_mm);

volatile int found = 0;
volatile int initialDistance = 0;
volatile int done = 0;
volatile int foundBlack = 0;
volatile int angle = 0;
volatile int bumped = 0;

//this method moves the robot forward. It handles all un-seeable obstacle avoidance. the avoidance handlers have been designed to prevent infinite looping, however they are not perfect.
void move_forward(oi_t *sensor_data, double distance_mm)
{
    double sum = 0;
    oi_setWheels(100, 100);
    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        if (WallForward(sensor_data))
        {
            move_backwards(sensor_data, 50);
            move_left(sensor_data, 180);
            break;
        }
        if (Wall45Right(sensor_data))
        {
            move_left(sensor_data, 90);
            break;
        }

        if (Wall45Left(sensor_data))
        {
            move_right(sensor_data, 90);
            break;
        }

        if (CliffForward(sensor_data))
        {
            move_backwards(sensor_data, 50);
            move_left(sensor_data, 90);

            break;
        }

        if (Cliff45Left(sensor_data))
        {
            move_right(sensor_data, 90);

            break;
        }
        if (Cliff45Right(sensor_data))
        {
            move_left(sensor_data, 90);

            break;
        }

        if (sensor_data->bumpLeft)
        {
            move_backwards(sensor_data, 75);
            move_right(sensor_data, 50);
            move_forward(sensor_data, 250);
            move_left(sensor_data, 50);
            move_forward(sensor_data, 50);
            break;

        }

        if (sensor_data->bumpRight)
        {
            move_backwards(sensor_data, 75);
            move_left(sensor_data, 50);
            move_forward(sensor_data, 225);
            move_right(sensor_data, 45);
            move_forward(sensor_data, 15);
            break;
        }
        sum += sensor_data->distance;
    }
    oi_setWheels(0, 0);
    printf("Distance Travelled: %lf\n", sum);
}

//move forward, but called in the later half of the program. this is used when we don't wish to lose track of the parking location.
//returns 1 if an un-seeable obstacle interrupted the method.
int move_forwardFinal(oi_t *sensor_data, double distance_mm)
{
    double sum = 0;
       oi_setWheels(100, 100);
       while (sum < distance_mm)
       {
           oi_update(sensor_data);
           if (WallForward(sensor_data) || Wall45Right(sensor_data)
                   || Wall45Left(sensor_data) || CliffForward(sensor_data)
                   || Cliff45Left(sensor_data) || Cliff45Right(sensor_data)
                   || sensor_data->bumpLeft || sensor_data->bumpRight)
           {
               move_backwards(sensor_data, 150);
               move_right(sensor_data, 100);


               return 1;
               break;
           }
           sum += sensor_data->distance;
       }
       oi_setWheels(0, 0);
       printf("Distance Travelled: %lf\n", sum);
    return 0;
}
void move_backwards(oi_t *sensor_data, double distance_mm)
{

    double sum = 0;
    oi_setWheels(-200, -200);
    while (sum < distance_mm)
    {

        oi_update(sensor_data);

        sum -= sensor_data->distance;
    }
    oi_setWheels(0, 0);
}

void move_right(oi_t *sensor_data, double degrees)
{
    double sum = 0;
    degrees = 1.0 * degrees;
    oi_setWheels(150, -150);
    while (sum < fabs(degrees))
    {
        oi_update(sensor_data);
        // if (degrees + sum <= 5){
        //       turnright(sensor_data, sum);
        //}
        sum += sensor_data->angle;
    }
    oi_setWheels(0, 0);
    return 0;

}

void move_left(oi_t *sensor_data, double degrees)
{
    double sum = 0;

    degrees = -1.0 * degrees;
    oi_setWheels(-50, 50);

    while (sum > degrees)
    {
        oi_update(sensor_data);
        sum += sensor_data->angle;
    }
    oi_setWheels(0, 0);

}

/**
 * used to detect any object within range of the cybot. will turn left, and move to the closest object (counter-clockwise)
 */
void ping_turn_left(oi_t *sensor_data)
{
    int degrees = 0;
    servo_move(90);
    oi_setWheels(-25, 25);
    while (1)
    {
        oi_update(sensor_data);
        double dist = ping_getDistance();
        if(degrees >= 360)
        {
            return;
        }

        if (dist < 300)
        {
            printf("Ping Dist: %lf\n", dist);
            timer_waitMillis(1000);
            oi_setWheels(0, 0);
            break;
        }
        degrees += sensor_data->angle;

    }
    double data = (ping_getDistance() * 10) * 0.7;
    printf("%lf\n", data);
    move_forward(sensor_data, (data));
}

/**
 * essentially the opposite of ping_turn_left. Never used in this program
 */
void ping_turn_right(oi_t *sensor_data)
{
    servo_move(90);
    oi_setWheels(25, -25);
    while (1)
    {
        oi_update(sensor_data);
        double dist = ping_getDistance();
        if (dist < 100)
        {
            printf("Ping Dist: %lf\n", dist);
            timer_waitMillis(25);
            oi_setWheels(0, 0);
            break;
        }
    }
}

void ping_moveWhite(oi_t *sensor_data)
{
    servo_move(90);
    double dist = ping_getDistance();

    move_right(sensor_data, 45);
    move_forward(sensor_data, dist );
    move_left(sensor_data, 45);
    move_forward(sensor_data, 150);
}


