/**
*********************************************************************************
* @file              : Hardware_Defs.h
* @author            : Ziad Khalil, Ali Akram Ali, Mahmoud Ahmed El- Adgham
* @brief             : Central file for all hardware pin definitions,
* : ports, and constant thresholds.
*********************************************************************************
*/

#ifndef HARDWARE_DEFS_H
#define HARDWARE_DEFS_H


/* ****************** Macro Section Start ********************** */

// --- LCD Pin Definitions (4-Bit Mode) ---
#define LCD_RS_PORT PORTB // Port for LCD Register Select
#define LCD_RS_DDR  DDRB
#define LCD_RS_PIN  0     // PB0

#define LCD_E_PORT  PORTB // Port for LCD Enable
#define LCD_E_DDR   DDRB
#define LCD_E_PIN   1     // PB1

#define LCD_DATA_PORT PORTD // Port for LCD Data Lines
#define LCD_DATA_DDR  DDRD
#define LCD_D4_PIN 4      // PD4
#define LCD_D5_PIN 5      // PD5
#define LCD_D6_PIN 6      // PD6
#define LCD_D7_PIN 7      // PD7

// --- LED Pin Definition ---
#define LED_PORT PORTD // Port for the Alarm LED
#define LED_DDR  DDRD
#define LED_PIN  3     // PD3

// --- ADC Channel Definitions ---
#define KEYPAD_ADC_CHANNEL 0 // ADC0 is used for the analog keypad
#define POT_ADC_CHANNEL    2 // ADC2 is used for the potentiometer

// --- Keypad ADC Value Definitions (Thresholds) ---
// These values depend on the resistor network used
#define KEY_ADC_RIGHT  50   // ADC value for RIGHT key (0-49)
#define KEY_ADC_UP     195  // ADC value for UP key (50-194)
#define KEY_ADC_DOWN   380  // ADC value for DOWN key (195-379)
#define KEY_ADC_LEFT   555  // ADC value for LEFT key (380-554)
#define KEY_ADC_SELECT 790  // ADC value for SELECT key (555-789)
                            // Anything above 790 is KEY_NONE


/* ****************** Macro Section End ************************ */


#endif // HARDWARE_DEFS_H