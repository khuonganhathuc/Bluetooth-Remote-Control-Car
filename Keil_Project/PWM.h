#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"

// --- Constants for 50MHz System Clock @ 50 Hz Output ---
// PWM Clock = 781,250 Hz
// Period = 20ms (50 Hz)
// Load Value = 15,625 - 1 = 15,624 ticks

#define SERVO_LOAD_VAL   7811   // Period (20ms)

// --- Standard RC Pulse Widths ---
// 1.5ms is Center. 1.0ms and 2.0ms are standard limits.
#define SERVO_LEFT_SAFE  7030     // 1.0 ms
#define SERVO_CENTER_VAL 6640    // 1.5 ms (Neutral)
#define SERVO_RIGHT_SAFE 6249    // 2.0 ms

// --- Extended Range (From Datasheet) ---
// Only use these if your steering mechanism allows 180 degrees
#define SERVO_MIN_MAX    391     // 0.5 ms
#define SERVO_MAX_MAX    13671    // 2.5 ms


//Main motor (50Hz)
//1.5ms (not moving)
//1ms (reverse)
//2ms (forward)

// --- Function Prototypes ---
void PWM_Init(void);
void Servo_Set_Angle_Value(uint32_t value);
void Motor_Set_Speed(uint32_t value);
