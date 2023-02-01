/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum
{
    LOW, HIGH, DONE
} STATE = LOW; // State of ping echo pulse

void ping_init(void)
{

    // YOUR CODE HERE
    SYSCTL_RCGCGPIO_R |= 0x2;
    while ((SYSCTL_PRGPIO_R & 0x02) != 0x02)
    {
    };

    GPIO_PORTB_DEN_R |= 0x8;
    GPIO_PORTB_AFSEL_R |= 0x8;
    GPIO_PORTB_PCTL_R |= 0x7000;
    GPIO_PORTB_DIR_R |= 0x8;
    SYSCTL_RCGCTIMER_R |= 0x8;
    while ((SYSCTL_PRTIMER_R & 0x8) != 0x8)
    {
    };
    TIMER3_CTL_R &= ~0x100;
    TIMER3_CTL_R |= 0xC00;
    TIMER3_CFG_R = 0x4;
    TIMER3_TBMR_R = 0b00111; //0x07;
    TIMER3_TBILR_R = 0xFFFF;
    TIMER3_TBPR_R = 0xFF;

    TIMER3_ICR_R |= 0x400; // clear interrupts
    TIMER3_IMR_R |= 0x400; //enable capture interrupt.

    NVIC_PRI9_R |= 0b00100000;
    NVIC_EN1_R |= 0x10; //enable bit 4

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    TIMER3_CTL_R |= 0x100;

    STATE = LOW;
}

void ping_trigger(void)
{
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0x400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x8;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DATA_R &= ~0x8;
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x8;

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400; // CBCINT = bit 10, 0b100 0000 0000
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    //GPIO_PORTB_PCTL_R = 0x7000;
    TIMER3_IMR_R |= 0x400;    //CBIM = bit 10, 0b100 0000 0000
    TIMER3_CTL_R |= 0x100;
}

void TIMER3B_Handler(void)
{

    // YOUR CODE HERE
    if (STATE == LOW)
    {
        STATE = HIGH;
        START_TIME = TIMER3_TBR_R;

    }

    else if (STATE == HIGH)
    {

        END_TIME = TIMER3_TBR_R;
        STATE = DONE;

    }

    TIMER3_ICR_R |= 0x400;

}

float ping_getDistance(void)
{
    ping_trigger();
    unsigned long overflow;
    unsigned long lastTime = 0;

    while (STATE != DONE)
    {
        unsigned long totalTime1 = START_TIME - END_TIME;
        overflow = (lastTime > totalTime1);

    }

    unsigned long totalTime = START_TIME - END_TIME;

    float dist = (float) 34300 * totalTime / 32000000;

    if(overflow){
        dist = -1;
    }

    return dist;

}
