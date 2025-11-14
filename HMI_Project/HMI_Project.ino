/**
*********************************************************************************
* @file              : HMI_Project.ino
* @author            : Ziad Khalil, Ali Akram Ali, Mahmoud Ahmed El- Adgham
* @brief             : Main application logic for the HMI (Human-Machine Interface)
* : Reads a potentiometer, checks it against user-defined
* : limits, and displays the status on an LCD.
*********************************************************************************
*/


/* ****************** Include Section Start ******************** */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h> 
#include <string.h> 
#include "Hardware_Defs.h" 
#include "Adc.h"           
#include "Lcd.h"           
#include "Keypad.h"        
#include "Utils.h"         
/* ****************** Include Section End   ******************** */


/* ****************** Global Variables Section Start *********** */
int High_Limit = 500;   // Default upper threshold for the potentiometer
int Low_Limit = 100;    // Default lower threshold for the potentiometer
char G_Buffer[17];      // Global buffer for integer-to-string conversions
/* ****************** Global Variables Section End ************* */


/* ****************** Main Program Section Start *************** */

/**
* @brief Main entry point and super-loop of the application.
* Initializes peripherals and continuously reads inputs,
* processes logic, and updates the LCD.
*/

int main(void) {
    
    // --- 1. Initialization ---
    LED_DDR |= (1 << LED_PIN);     // Set the LED pin as an output
    LED_PORT &= ~(1 << LED_PIN);  // Start with the LED turned off
    
    Adc_Init(); // Initialize the Analog-to-Digital Converter
    Lcd_Init(); // Initialize the 16x2 LCD Display

    int Last_Key = KEY_NONE; // Stores the previous key press to detect new presses
    
    // --- 2. Main Loop ---
    while (1) {
        
        // --- A. Read Inputs ---
        int Pot_Value = Adc_Read(POT_ADC_CHANNEL); // Read the 10-bit value from the potentiometer
        int Key_Press = Get_Keypad_Press();      // Read the current state of the analog keypad

        // --- B. Process Button Logic ---
        // Check for a *new* button press (simple de-bounce)
        if (Key_Press != KEY_NONE && Key_Press != Last_Key) {
            
            if (Key_Press == KEY_UP) {
                High_Limit += 50;
                if (High_Limit > 1050) { // Wrap around
                    High_Limit = 0;
                }
            } 
            else if (Key_Press == KEY_DOWN) {
                Low_Limit += 50;
                if (Low_Limit > 1050) { // Wrap around
                    Low_Limit = 0;
                }
            }
            _delay_ms(150); // Debounce delay to prevent rapid-fire increments
        }
        Last_Key = Key_Press; // Save the current press for the next loop's comparison

        // --- C. Process Alarm Logic ---
        // Determine if the POT value is outside the set limits
        int Out_Of_Limit = (Pot_Value > High_Limit || Pot_Value < Low_Limit);
        
        if (Out_Of_Limit) {
            LED_PORT |= (1 << LED_PIN);  // If out of limits, turn on the alarm LED
        } else {
            LED_PORT &= ~(1 << LED_PIN); // Otherwise, keep it off
        }

        // --- D. Update Display ---
        
        // --- Row 0: POT Value and Status ---
        Lcd_Set_Cursor(0, 0);
        Lcd_Send_String("POT: "); 
        Simple_Itoa(Pot_Value, G_Buffer); // Convert integer value to a string
        Lcd_Send_String(G_Buffer); 
        
        // Add padding spaces to clear old digits and prevent artifacts
        if (Pot_Value < 1000) Lcd_Send_String(" "); 
        if (Pot_Value < 100)  Lcd_Send_String(" ");
        if (Pot_Value < 10)   Lcd_Send_String(" "); 

        Lcd_Send_String("  "); 
        
        // Move to status position (e.g., "OK" / "NOK")
        Lcd_Set_Cursor(0, 12);
        if (Out_Of_Limit) {
            Lcd_Send_String("NOK "); // "Not OK"
        } else {
            Lcd_Send_String("OK  "); // "OK"
        }

        // --- Row 1: Low and High Limits ---
        Lcd_Set_Cursor(1, 0);
        Lcd_Send_String("L:"); 
        Simple_Itoa(Low_Limit, G_Buffer);
        Lcd_Send_String(G_Buffer); 
        
        // Add padding spaces
        if (Low_Limit < 1000) Lcd_Send_String(" "); 
        if (Low_Limit < 100)  Lcd_Send_String(" ");
        if (Low_Limit < 10)   Lcd_Send_String(" "); 

        Lcd_Set_Cursor(1, 8); 
        Lcd_Send_String("H:"); 
        Simple_Itoa(High_Limit, G_Buffer);
        Lcd_Send_String(G_Buffer); 
        Lcd_Send_String("    "); // Padding

        _delay_ms(100); // Delay to make LCD readable and slow loop
    }

    return 0; // Should never be reached
}

/* ****************** Main Program Section End ***************** */