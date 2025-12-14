#include "PWM.h"

#include "PWM.h"

#include "PWM.h"

void PWM_Init(void)
{
    // 1. Enable Clocks
    SYSCTL->RCGCPWM |= 0x01;       // Enable PWM Module 0
    SYSCTL->RCGCGPIO |= 0x02;      // Enable Port B
    
    // Delay for clock stabilization

    // 2. Configure PWM Clock Divider to /64
    // System is 80MHz. Max PWM counter speed is usually best around 1-2MHz.
    // 80MHz / 64 = 1.25MHz. This is perfect.
    // Bit 20 = 1 (Enable Div), Bits 19:17 = 0x7 (/64)
    SYSCTL->RCC |= 0x001E0000;

    // 3. Configure PB6 and PB7 Pins
    GPIOB->AFSEL |= 0xC0;          // Enable Alt Function (Pins 6,7)
    GPIOB->PCTL &= ~0xFF000000;    // Clear PCTL
    GPIOB->PCTL |= 0x44000000;     // Set M0PWM0 and M0PWM1
    GPIOB->DEN |= 0xC0;            // Enable Digital

    // 4. Configure Generator 0 (Controls PB6 & PB7)
    PWM0->_0_CTL &= ~0x01;         // Disable Generator 0 first

    // Configure Count-Down Mode:
    // Drive High on Load, Drive Low on Compare Match
    PWM0->_0_GENA = 0x0000008C;    // For PB6 (Motor)
    PWM0->_0_GENB = 0x0000080C;    // For PB7 (Servo)
    
    // 5. Set Period and Initial Positions (Using 100Hz values)
    PWM0->_0_LOAD = SERVO_LOAD_VAL;    // 10ms Period
    
    // Set both to Neutral/Stop initially
    PWM0->_0_CMPA = SERVO_CENTER_VAL;
    PWM0->_0_CMPB = SERVO_CENTER_VAL;

    // 6. Enable Generator and Outputs
    PWM0->_0_CTL |= 0x01;          // Enable Generator 0
    PWM0->ENABLE |= 0x03;          // Enable Output 0 (PB6) and 1 (PB7)
}

void ESC_Set_Speed(uint32_t value)
{
    // Write new match value to Comparator A (Datasheet p. 1278)
    PWM0->_0_CMPA = value;
}

// Helper function to update just the Servo (PB7 / CMPB)
void Servo_Set_Angle_Value(uint32_t value)
{
    // Write new match value to Comparator B (Datasheet p. 1279)
    PWM0->_0_CMPB = value;
}