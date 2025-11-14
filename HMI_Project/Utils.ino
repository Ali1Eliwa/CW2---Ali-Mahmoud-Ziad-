/**
*********************************************************************************
* @file              : Utils.ino
* @author            : Ziad Khalil, Ali Akram Ali, Mahmoud Ahmed El- Adgham
* @brief             : Implementation file for utility functions.
*********************************************************************************
*/


/* ****************** Include Section Start ******************** */
#include "Utils.h" 
/* ****************** Include Section End   ******************** */


/* ****************** Sup-Program Section Start **************** */
/**
* @brief A simple integer-to-string conversion function.
* (Handles positive integers and zero).
*/
void Simple_Itoa(int Value, char* Buffer) {
    char Temp_Buffer[7]; // Max 5 digits (0-1023) + sign + null
    int i = 0;

    // Handle the special case of 0
    if (Value == 0) {
        Buffer[0] = '0';
        Buffer[1] = '\0';
        return;
    }

    // Handle negative numbers (though not needed for this project)
    if (Value < 0) {
        Value = -Value;
        // You would add a '-' to the buffer here if needed
    }

    // Convert digits to characters in reverse order
    while (Value > 0) {
        Temp_Buffer[i++] = (Value % 10) + '0'; // Get last digit
        Value /= 10;                             // Remove last digit
    }

    // Reverse the temporary buffer into the final buffer
    int j = 0;
    while (i > 0) {
        Buffer[j++] = Temp_Buffer[--i];
    }
    Buffer[j] = '\0'; // Add null terminator to make it a valid string
}

/* ****************** Sup-Program Section End ****************** */