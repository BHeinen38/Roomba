/*
 * servo.c
 *
 *  Created on: Apr 6, 2021
 *      Author: bjheinen
 */
#include "servo.h"

void servo_init(void){

    SYSCTL_RCGCGPIO_R |= 0b10;
    while((SYSCTL_PRGPIO_R & 0b10)==0){};
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x00700000;
    GPIO_PORTB_PCTL_R &= ~0x00800000;
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_DEN_R |= 0x20;

    SYSCTL_RCGCTIMER_R |= 0x02;
    while((SYSCTL_PRTIMER_R & 0x02)==0){};
    TIMER1_CTL_R &= ~0x100;
    TIMER1_CFG_R = 0x4;
    // TIMER3_TBMR_R |= ~0xF7;
    TIMER1_TBMR_R = 0b1010;
    TIMER1_CTL_R &= ~0x4000;
    TIMER1_TBPR_R = 0x4E200>>16;
    TIMER1_TBILR_R = 0x4E200 & 0xFFFF;
    TIMER1_TBMATCHR_R = 0x4BED8 & 0xFFFF;
    TIMER1_TBPMR_R = 0x4BED8>>16;
    TIMER1_CTL_R |= 0x100;
/*
    GPTMCTL &= 0x0000;  // clear TBEN
    GPTMCFG = 0x00000004    // set register configuration value to 0x00000004
    GPTMTBMR |= 0b00001111; // turn on TBAMS, TBCMR, TMBR
    GPTMCTL |= 0x4000;  // configure TBPWML
    GPTMTBPR |= 0x00FF; // set prescale value to GPTM Timer and Prescale Register
    GPTMCTL |= 0x0C00;  // configure interrupt condition in TBEVENT
    GPTMTBMR |= 0x0200; // enable interrupts in TBPWMIE
    GPTMTnILR &= 0xFFFFFFFF;    // load timer start value into GPTM Timer and Interval Load
    GPTMTnMATCHR &= 0xFFFFFFFF; // load GPTM Timer and Match
    GPTMCTL |= 0x0100;  // turn on TBEN
*/
}

int servo_move(float degrees)
{

    int match = 320000 - 9000 - ((22000 * degrees)/180);
    TIMER1_TBMATCHR_R = match & 0xFFFF;
    TIMER1_TBPMR_R = match >> 16;

}
//unsigned int pulse = 20*1000*16;
//
//void servo_init(void){
//    SYSCTL_RCGCGPIO_R |= 0x2;
//        while ((SYSCTL_PRGPIO_R & 0x02) != 0x02)
//        {
//        };
//
//        GPIO_PORTB_DEN_R |= 0x20; //Pb5
//        GPIO_PORTB_AFSEL_R |= 0x20;
//        GPIO_PORTB_PCTL_R |= 0x700000;
//        GPIO_PORTB_DIR_R |= 0x20; //output
//        SYSCTL_RCGCTIMER_R |= 0x2; //timer 1
//        while ((SYSCTL_PRTIMER_R & 0x2) != 0x2)
//           {};
//           TIMER1_CTL_R &= ~0x100; //disable
//
//           TIMER1_CFG_R = 0x4;
//
//           TIMER1_TBMR_R = 0xA;
//
//           TIMER1_TBILR_R = (pulse & 0xFFFF);
//           TIMER1_TBPR_R = (pulse >> 16) & 0xFF;
//
//           TIMER1_TBMATCHR_R = 0x92B4 & 0xFFFF; //0x6500 & 0xFFFF; //180 //0x92B4 & 0xFFFF; // 90  //0xBED8 & 0xFFFF; // 0
//           TIMER1_TBPMR_R = 0x04; //((pulse - 9000) >> 16);
//
//           TIMER1_CTL_R |= 0x100;
//}
//int servo_move(float degrees)
//{
//    TIMER1_CTL_R &= ~0x100; //disable
//    int temp = -128.39 * degrees + 311158; //figure out later
//    // y = -128.39x + 311158
//
//    TIMER1_TBMATCHR_R = (temp & 0xFFFF);
//    TIMER1_TBPMR_R = (temp >> 16) & 0xFF;
//    TIMER1_CTL_R |= 0x100; //enable
//   return temp;
//
//}
