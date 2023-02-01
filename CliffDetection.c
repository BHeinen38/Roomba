/*
 * CliffDetection.c
 *
 *  Created on: Apr 27, 2021
 *      Author: bjheinen
 */
#include "lcd.h"
#include "open_interface.h"
#include "timer.h"

//Using the various cliff detectors provided in open_interface. They can tell the direction and type of obstruction underneath the cybot.
int CliffForward(oi_t *sensor_data)
{
    if ((sensor_data->cliffFrontLeftSignal == 0
            && sensor_data->cliffFrontRightSignal == 0)
            && (sensor_data->cliffFrontLeft > 0
                    && sensor_data->cliffFrontRight > 0))
    {
        return 1;
    }
    return 0;
}

int CliffLeft(oi_t *sensor_data){
    if(sensor_data->cliffLeftSignal == 0 && sensor_data->cliffLeft > 0){
        return 1;
    }
    return 0;
}

int Cliff45Left(oi_t *sensor_data){
    if(sensor_data->cliffFrontLeftSignal == 0 && sensor_data->cliffFrontLeft > 0){
            return 1;
        }
        return 0;
}


int CliffRight(oi_t *sensor_data){
    if(sensor_data->cliffRightSignal == 0 && sensor_data->cliffRight > 0){
                return 1;
            }
            return 0;
}

int Cliff45Right(oi_t *sensor_data){
    if(sensor_data->cliffFrontRightSignal == 0 && sensor_data->cliffFrontRight > 0){
                    return 1;
                }
                return 0;
}
int WallForward(oi_t *sensor_data){
    if(sensor_data->cliffFrontLeftSignal > 3000 && sensor_data->cliffFrontRightSignal > 3000){
            return 1;
           }
    return 0;
}
int WallRight(oi_t *sensor_data){
    if(sensor_data->cliffRightSignal > 3000 && sensor_data->cliffRight == 0){
        return 1;
    }
    return 0;
}
int Wall45Right(oi_t *sensor_data){
    if( sensor_data->cliffFrontRightSignal > 3000 && sensor_data->cliffFrontRight == 0){
        return 1;
    }
    return 0;
}

int WallLeft(oi_t *sensor_data){
    if(sensor_data->cliffLeftSignal > 3000 && sensor_data->cliffLeft == 0){
        return 1;
    }
    return 0;
}
int Wall45Left(oi_t *sensor_data){
    if(sensor_data->cliffFrontLeftSignal > 3000 && sensor_data->cliffFrontLeft == 0){
        return 1;
    }
    return 0;
}
int WallCorner(oi_t *sensor_data){
    if(sensor_data->cliffFrontLeftSignal > 3000 && sensor_data->cliffFrontRightSignal > 3000){
        return 1;
    }
    return 0;
}



