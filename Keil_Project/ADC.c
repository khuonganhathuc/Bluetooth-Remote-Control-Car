/**
 * @file ADC.c
 *
 * @brief Source code for the ADC driver.
 *
 * This file contains the function definitions for the ADC driver.
 *
 * ADC Module 0 is used to sample the potentiometer and the analog
 * light sensor that are connected on the EduBase board. ADC Module 0
 * is configured to sample the potentiometer first and the light sensor after.
 *
 * After the light sensor is sampled, an interrupt signal is set to 
 * indicate that the sampling sequence has ended. After the conversion
 * results have been read from the corresponding FIFO, the interrupt is cleared.
 *
 * The following pins are used:
 *  - Potentiometer   <-->  Tiva LaunchPad Pin PE2 (Channel 1)
 *  - Light Sensor    <-->  Tiva LaunchPad Pin PE1 (Channel 2)
 *
 * @author
 *
 */

#include "ADC.h"

void ADC_Init(void)
{
	
	SYSCTL->RCGCADC |= 0x01;
	SysTick_Delay1ms(1);
	SYSCTL->RCGCGPIO |= 0x10;
	GPIOE->DIR &= ~0x04;	
	GPIOE->DEN &= ~0x04;
	GPIOE->AMSEL |= 0x04;
	GPIOE->AFSEL |= 0x04;
	ADC0->ACTSS &= ~0x1;
	ADC0->EMUX &= ~0x000F;
	ADC0->SSMUX0 &= ~0xFFFFFFFF;
	ADC0->SSMUX0 |= 0x00000001;
	ADC0->SSCTL0 |= 0x00000002;
	ADC0->SSCTL0 |= 0x00000004;
	ADC0->ACTSS |= 0x1;
	/*T1:
		// 1. Enable the clock to ADC Module 0
    SYSCTL->RCGCADC |= 0x01;
    SysTick_Delay1ms(1);

    // 2. Enable the clock to Port E
    SYSCTL->RCGCGPIO |= 0x10;

    // 3. Configure PE2 (Potentiometer) and PE1 (Light Sensor) as inputs
    // Clearing bits 1 and 2
    GPIOE->DIR &= ~0x06;

    // 4. Disable digital functionality for PE2 and PE1
    GPIOE->DEN &= ~0x06;

    // 5. Enable analog functionality for PE2 and PE1
    GPIOE->AMSEL |= 0x06;

    // 6. Select alternate function for PE2 and PE1
    GPIOE->AFSEL |= 0x06;

    // 7. Disable Sample Sequencer 0 (SS0) before configuration
    ADC0->ACTSS &= ~0x01;

    // 8. Configure trigger event for SS0 to be the processor (default 0)
    ADC0->EMUX &= ~0x000F;

    // 9. Clear SS0 input multiplexer select register
    ADC0->SSMUX0 &= ~0xFFFFFFFF;

    // 10. Configure the sampling sequence:
    // 1st Sample (MUX0): Channel 1 (PE2/Potentiometer) -> Write 0x1 to bits 3:0
    // 2nd Sample (MUX1): Channel 2 (PE1/Light Sensor)  -> Write 0x2 to bits 7:4
    ADC0->SSMUX0 |= 0x00000021;

    // 11. Configure sample control bits (ADCSSCTL0):
    // 1st Sample (Bit 0-3): No interrupt, not end of sequence. (All 0s)
    // 2nd Sample (Bit 4-7): Enable Interrupt (IE1 at bit 6), End of Sequence (END1 at bit 5)
    // Binary: 0110 0000 = 0x60
    ADC0->SSCTL0 = 0x00000060;

    // 13. Enable Sample Sequencer 0
    ADC0->ACTSS |= 0x01;
		*/
}

void ADC_Sample(double analog_value_buffer[])
{
	ADC0->PSSI |= 0x01;
	while((ADC0->RIS & 0x01) == 0);
	// First read is the Potentiometer (1st sample in sequence)
  uint32_t potentiometer_sample_result = ADC0->SSFIFO0;
  //Second read is the Light Sensor (2nd sample in sequence)
  //T1: uint32_t light_sensor_sample_result = ADC0->SSFIFO0;
	ADC0->ISC |= 0x01;
	// Index 0: Potentiometer
	analog_value_buffer[0] = (potentiometer_sample_result * 3.3) / 4096.0;
	// Index 1: Light Sensor
  //analog_value_buffer[1] = (light_sensor_sample_result * 3.3) / 4096.0;
}
