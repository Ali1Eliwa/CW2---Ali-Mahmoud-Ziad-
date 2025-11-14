/**
*********************************************************************************
* @file              : Keypad.ino
* @author            : Ziad Khalil, Ali Akram Ali, Mahmoud Ahmed El- Adgham
* @brief             : Implementation file for the 5-button analog keypad.
*********************************************************************************
*/
 

/* ****************** Include Section Start ******************** */
#include "Keypad.h"        
#include "Hardware_Defs.h" // For ADC channel and thresholds
#include "Adc.h"           // For Adc_Read()
/* ****************** Include Section End   ******************** */


/* ****************** Sup-Program Section Start **************** */
/**
* @brief Reads the analog keypad channel and returns the pressed key.
*/
int Get_Keypad_Press(void) {
    // Read the raw ADC value from the keypad channel
    uint16_t Adc_Val = Adc_Read(KEYPAD_ADC_CHANNEL);

    // Compare the value against the thresholds defined in Hardware_Defs.h
    // The order (lowest to highest) is important.
    if (Adc_Val < KEY_ADC_RIGHT)  return KEY_RIGHT;
    if (Adc_Val < KEY_ADC_UP)    return KEY_UP;
    if (Adc_Val < KEY_ADC_DOWN)  return KEY_DOWN;
    if (Adc_Val < KEY_ADC_LEFT)  return KEY_LEFT;
    if (Adc_Val < KEY_ADC_SELECT) return KEY_SELECT;
    
    // If no other button is pressed (ADC value is high)
    return KEY_NONE;
}


/* ****************** Sup-Program Section End ****************** */