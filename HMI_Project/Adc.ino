/**
*********************************************************************************
* @file              : Adc.ino
* @author            : Ziad Khalil, Ali Akram Ali, Mahmoud Ahmed El- Adgham
* @brief             : Implementation file for the ADC driver.
* : Wraps the "Doctor's Code" in a standard interface.
*********************************************************************************
*/

/* ****************** Include Section Start ******************** */
#include <avr/io.h>
#include "Adc.h" 
/* ****************** Include Section End   ******************** */


/* ****************** Sup-Program Section Start **************** */

// --- START: Doctor's Code (Private implementation) ---

/**
* @brief Internal function to configure ADC registers.
*/

static void Adc_Init_Internal(void) {
    ADMUX = (1<<REFS0); // Use AVcc as reference voltage
    // Enable ADC, Set Prescaler to 128 (16MHz/128 = 125kHz)
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

/**
* @brief Internal function to select channel and perform one conversion.
*/

static unsigned short Adc_ReadChannel_Internal(unsigned char ch) {

  ch &= 0b00000111;           // Ensure channel is 0-7
  ADMUX = (ADMUX & 0xF8)|ch;  // Select the channel
  ADCSRA |= (1<<ADSC);        // Start a single conversion
  while(ADCSRA & (1<<ADSC));  // Wait for the conversion to complete
  return (ADC);               // Return the 10-bit result (ADCL/ADCH)

}
// --- END: Doctor's Code ---


// --- START: "Adapter" Layer (Public functions) ---

/**
* @brief Public function: Initializes the ADC module.
*/

void Adc_Init(void) {
    Adc_Init_Internal(); // Call the private implementation
}


/**
* @brief Public function: Reads a 10-bit value from the specified channel.
*/
uint16_t Adc_Read(uint8_t Channel) {
    return Adc_ReadChannel_Internal(Channel); // Call the private implementation
}

/* ****************** Sup-Program Section End ****************** */