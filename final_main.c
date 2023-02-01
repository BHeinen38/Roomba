/**
 * @file lab8_template.c
 * @author
 * Template file for CprE 288 Lab 8
 */
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"
#include "servo.h"
#include "resetSimulation.h"
#include "scan.h"
#include "ping.h"
#include "uart-interrupt.h"
#include "math.h"
#include "adc.h"
#include "CliffDetection.h"
#include "movement.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * a more rigorous test for object values
 */
int testObject(object givenObject);

int main(void)
{
    //Initialize all peripherals and helper functions
    timer_init();
    lcd_init();
    servo_init();
    adc_init();
    ping_init();
    char send[100];
    uart_interrupt_init();

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    //wait for start command 'g'
    while (command_flag != 3)
        ;

    //optional method that can either quickly lead a person to the first tall object detected, or completely ruin a run.
    //ping_turn_left(sensor_data);

    int inBox = 0;
    while (inBox == 0)
    {
        if (command_flag == 2)
        {
            command_flag = 1;
            break;
        }
        //THIS loop represents the first half of main. Its purpose is getting the robot to the specified zone.
        while (1)
        {

            object obj[10];
            int t;

            //set all initial object values to -1 for screening later.
            for (t = 0; t < 10; t++)
            {
                obj[t].angle = -1;
                obj[t].distance = -1;
                obj[t].width = -1;

            }

            //implicitly called because we don't need to know how many small objects are present yet.
            getObjects(obj, 10);

            //used as a boolean. shows if a red object was found in the last scan
            int redFound = 0;

            //used as a boolean. shows if a red object was found in the last scan
            int whiteFound = 0;

            //represents the first red object in a scan;
            object firstRed;
            //represents the first white object in  a scan;
            object firstWhite;

            firstRed.angle = 0;
            firstWhite.angle = 0;

            //determines what objects are red and which are white
            for (t = 0; t < 10; t++)
            {

                if (obj[t].angle > 180)
                    t++;
                if ((obj[t].width < 7 && obj[t].width > 0)
                        && obj[t].angle <= 180)
                {
                    redFound = 1;
                    firstRed = obj[t];
                    if (obj[t].distance < firstRed.distance
                            && testObject(obj[t]))
                    {
                        firstRed = obj[t]; //semi misleading, but will be over written for a red object that is closer to the robot.
                    }
                }
                else if ((obj[t].width > 8) && obj[t].angle <= 180)
                {
                    whiteFound = 1;
                    firstWhite = obj[t];
                    break;
                }
            }

            /**
             * Handles the last iteration of the first half of the search algorithm
             * moves 20cm in front of the red object, and then turns the program over to the last half.
             */
            if (redFound && testObject(firstRed))
            {
                int targetAngle = firstRed.angle + 2;
                sprintf(send, "Angle of Red Object: %d\n", firstRed.angle);
                lcd_printf(send);
                if (targetAngle < 90)
                {
                    targetAngle = 86 - firstRed.angle;
                    move_left(sensor_data, targetAngle);
                }
                if (targetAngle >= 90 && targetAngle < 130)
                {
                    targetAngle = 0.5 * (firstRed.angle - 90);
                    //printf("Turn Angle: %d\n", targetAngle);
                    move_right(sensor_data, targetAngle);
                }
                if (targetAngle >= 130)
                {
                    targetAngle = 0.75 * (firstRed.angle - 90);
                    //printf("Turn Angle: %d\n", targetAngle);
                    move_right(sensor_data, targetAngle);
                }

                move_forward(sensor_data, (firstRed.distance - 20) * 10);
                sprintf(send, "Parking!\r");

                move_forward(sensor_data, 50);

                break;
            }

            /**
             * handles white objects, which can be seen just like red objects.
             * handling these, on top of allowing the robot to find red objects (avoidance) is the main reason purpose of the first half of the program.
             */
            else if (whiteFound && testObject(firstWhite))
            {
                lcd_printf("Moving To Object Width: %lf\n", obj[0].width);
                int targetAngle = obj[0].angle + 2;
                if (targetAngle < 90)
                {
                    targetAngle = 86 - obj[0].angle;
                    move_left(sensor_data, targetAngle);
                }
                if (targetAngle >= 90 && targetAngle < 130)
                {
                    targetAngle = 0.5 * (obj[0].angle - 90);
                    move_right(sensor_data, targetAngle);
                }
                if (targetAngle >= 130)
                {
                    targetAngle = 0.75 * (obj[0].angle - 90);
                    move_right(sensor_data, targetAngle);
                }
                whiteFound = 0;
                move_forward(sensor_data, (obj[0].distance - 22) * 10);
                ping_moveWhite(sensor_data);
                if (obj[0].angle < 90)
                {
                    move_right(sensor_data, 60);
                    move_forward(sensor_data, 200);
                    move_left(sensor_data, 60);
                    move_forward(sensor_data, 20);
                }
                else
                {
                    move_left(sensor_data, 60);
                    move_forward(sensor_data, 200);
                    move_right(sensor_data, 60);
                    move_forward(sensor_data, 20);
                }
            }
            else
            {
                move_forward(sensor_data, 400);
            }

            oi_update(sensor_data);
        }


        /**
         * This loop represents the second half of main. Attempting; key word here. to get the robot into the specified zone.
         */
        while (1)
        {
            object obj[10];
            object smallest[1];

            getObjects(obj, 10);

            if (command_flag == 2)
            {
                return;
            }

            int i;
            int j = 0;
            for (i = 0; i < 10; i++)
            {
                if (j > 1)
                    break;

                if (obj[i].width < 7 && testObject(obj[i]))
                {
                    smallest[j] = obj[i];
                    j++;
                }
            }

            /**
             * the following are conditionals that determine the best course of action for the robot to take to get into the specified zone.
             */
            if (smallest[0].angle > 30 && smallest[0].angle < 60)
            {
                move_left(sensor_data, smallest[0].angle - 5);
                move_forward(sensor_data, smallest[0].distance / 0.2);
                move_right(sensor_data, 45);
                if (move_forwardFinal(sensor_data, 250))
                {
                    uart_sendStr("30-60\n");
                    sprintf(send, "Parked\r");
                    uart_sendStr(send);
                    lcd_printf(send);
                    break;
                }
                inBox = 1;
            }
            if (smallest[1].angle >= 90)
            {
                uart_sendStr(">= 90\n");
                move_right(sensor_data, smallest[1].angle - 90);
                move_forward(sensor_data, smallest[1].distance / 0.2);
                move_left(sensor_data, 90 - smallest[1].angle);
                move_forward(sensor_data, 300);
                if (move_forwardFinal(sensor_data, 250))
                {
                    sprintf(send, "Parked\r");
                    uart_sendStr(send);
                    lcd_printf(send);
                    break;
                }
                inBox = 1;
            }
            if (smallest[1].angle - smallest[0].angle > 85
                    && fabs(smallest[1].distance - smallest[0].distance) < 12.5)
            {
                uart_sendStr("Cosine\n");
                double theta = 0.5 * (smallest[1].angle - smallest[0].angle);
                double avgDist = ((smallest[0].distance + smallest[1].distance)
                        / 2.0);
                double l = 10 * (avgDist * cos(theta));
                if (move_forwardFinal(sensor_data, l))
                {
                    sprintf(send, "Parked\r");
                    uart_sendStr(send);
                    lcd_printf(send);
                    break;
                }

            }

            if (command_flag == 2)
            {
                command_flag == 1;
                uart_sendStr("Parked\r");
                break;
            }

        }

        /**
         * unimplemented portion of code (for the demo anyway) that can be used to control the robot after it is parked.
         */
        while (1)
        {
            if (command_flag == 4)
            {
                command_flag == 1;
                move_forward(sensor_data, 150);
            }
            if (command_flag == 5)
            {
                command_flag == 1;
                move_backwards(sensor_data, 150);
            }
            if (command_flag == 6)
            {
                command_flag == 1;
                move_left(sensor_data, 30);
            }
            if (command_flag == 7)
            {
                command_flag == 1;
                move_right(sensor_data, 30);
            }
            if (command_flag == 2)
            {
                return;
            }
        }
    }

}

int testObject(object givenObject)
{
    if (givenObject.angle <= 180 && givenObject.angle >= 0
            || givenObject.distance <= 80 && givenObject.distance >= 10
            || givenObject.width > 4 && givenObject.width < 10)
    {
        printf("Test Object: Pass\n");
        return 1;
    }
    printf("Test Object: Fail\n");
    return 0;
}

