/*
 * @file main.c
 *
 * @brief Main source code for the ADC program.
 *
 * This file contains the main entry point and function definitions for the ADC program.
 *
 * It interfaces with the following:
 *  - EduBase Board Potentiometer (PE2)
 *  - EduBase Board Analog Light Sensor (PE1)
 *
 * @author
 */
#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"

#include "GPIO.h"
#include "PWM.h"

void PLL_Init(void) {
    // 1. Configure to use RCC2
    SYSCTL->RCC2 |= 0x80000000;
    // 2. Bypass PLL while initializing
    SYSCTL->RCC2 |= 0x00000800;
    // 3. Select Crystal (16MHz) and Main Oscillator
    SYSCTL->RCC = (SYSCTL->RCC & ~0x000007C0) + 0x00000540;
    SYSCTL->RCC2 &= ~0x00000070;
    // 4. Activate PLL
    SYSCTL->RCC2 &= ~0x00002000;
    // 5. Set System Divisor for 50 MHz
    // 400 MHz / 8 = 50 MHz. SYSDIV2 = 7.
    SYSCTL->RCC2 |= 0x40000000; 
    SYSCTL->RCC2 = (SYSCTL->RCC2 & ~0x1FC00000) + (7 << 22);
    // 6. Wait for lock
    while((SYSCTL->RIS & 0x00000040) == 0);
    // 7. Enable PLL
    SYSCTL->RCC2 &= ~0x00000800;
}

int main(void)
{
	SysTick_Delay_Init();
	PLL_Init();
	PWM_Init();
	
    
			//Servo_Set_Angle_Value(SERVO_CENTER_VAL);
			//SysTick_Delay1ms(1800);

    // 4. Disable Output (Stop sending)
    //PWM0->ENABLE &= ~0x02;

    while(1){
			Servo_Set_Angle_Value(SERVO_CENTER_VAL);
			SysTick_Delay1ms(3000);
			Servo_Set_Angle_Value(SERVO_LEFT_SAFE);
			SysTick_Delay1ms(3000);
			
			Servo_Set_Angle_Value(SERVO_CENTER_VAL);
			SysTick_Delay1ms(3000);
			Servo_Set_Angle_Value(SERVO_RIGHT_SAFE);
			SysTick_Delay1ms(3000);
			//Servo_Set_Angle_Value(SERVO_RIGHT_SAFE);
			//SysTick_Delay1ms(5000);
       /* 
        // Stop (1.5ms)
        Servo_Set_Angle_Value(SERVO_CENTER_VAL);
        SysTick_Delay1ms(3000);
			Servo_Set_Angle_Value(SERVO_LEFT_VAL);
        SysTick_Delay1ms(3000);
			Servo_Set_Angle_Value(SERVO_RIGHT_VAL);
        SysTick_Delay1ms(3000);
			*/
    }
}
