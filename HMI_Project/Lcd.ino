/**
*********************************************************************************
* @file              : Lcd.ino
* @author            : Ziad Khalil, Ali Akram Ali, Mahmoud Ahmed El- Adgham
* @brief             : Implementation file for the 16x2 LCD driver.
* : Wraps the "Doctor's Code" in a standard interface.
*********************************************************************************
*/

/* ****************** Include Section Start ******************** */
#define F_CPU 16000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h" 
/* ****************** Include Section End   ******************** */


/* ****************** Sup-Program Section Start **************** */

// --- START: Doctor's Code (Private implementation) ---
#define LCD_Dir  DDRD
#define LCD_Port PORTD
#define RS_EN_Dir  DDRB
#define RS_EN_Port PORTB
#define RS PB0
#define EN PB1


/**
* @brief Internal function: Sends a command byte (in 4-bit mode).
*/
static void LCD_Command_Internal( unsigned char cmnd ) {
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); // Send upper nibble
	RS_EN_Port &= ~ (1<<RS); // RS=0 (command)
	RS_EN_Port |= (1<<EN);   // Pulse Enable
	_delay_us(1);
	RS_EN_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  // Send lower nibble
	RS_EN_Port |= (1<<EN);   // Pulse Enable
	_delay_us(1);
	RS_EN_Port &= ~ (1<<EN);
	_delay_ms(2);
}


/**
* @brief Internal function: Sends a data/character byte (in 4-bit mode).
*/
static void LCD_Char_Internal( unsigned char data ) {
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0); // Send upper nibble
	RS_EN_Port |= (1<<RS);   // RS=1 (data)
	RS_EN_Port|= (1<<EN);    // Pulse Enable
	_delay_us(1);
	RS_EN_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (data << 4);  // Send lower nibble
	RS_EN_Port |= (1<<EN);   // Pulse Enable
	_delay_us(1);
	RS_EN_Port &= ~ (1<<EN);
	_delay_ms(2);
}


/**
* @brief Internal function: Sends a null-terminated string.
*/
static void LCD_String_Internal (char *str) {
	int i;
	for(i=0;str[i]!=0;i++) // Loop until null terminator
	{
		LCD_Char_Internal(str[i]);
	}
}
// --- END: Doctor's Code ---


// --- START: "Adapter" Layer (Public functions) ---


/**
* @brief Public function: Initializes the LCD (4-bit mode sequence).
*/
void Lcd_Init(void) {
	LCD_Dir = 0xFF; // Set LCD data port as output
	RS_EN_Dir |= (1 << EN) | (1 << RS); // Set control pins as output
	_delay_ms(20); // Wait for LCD to power on
	
	LCD_Command_Internal(0x02); // Return home (part of 4-bit init)
	LCD_Command_Internal(0x28); // 4-bit mode, 2 lines, 5x8 font
	LCD_Command_Internal(0x0c); // Display ON, Cursor OFF
	LCD_Command_Internal(0x06); // Entry mode: increment cursor, no shift
	LCD_Command_Internal(0x01); // Clear Display
	_delay_ms(2);
}


/**
* @brief Public function: Sends a command to the LCD.
*/
void Lcd_Send_Command(uint8_t cmd) {
    LCD_Command_Internal(cmd);
}


/**
* @brief Public function: Sends data (a character) to the LCD.
*/
void Lcd_Send_Data(uint8_t data) {
    LCD_Char_Internal(data);
}


/**
* @brief Public function: Sends a string to the LCD.
*/
void Lcd_Send_String(const char* str) {
    LCD_String_Internal((char*)str); // Cast const away for the internal function
}


/**
* @brief Public function: Clears the LCD display.
*/
void Lcd_Clear(void) {
    LCD_Command_Internal(0x01); // Send "Clear Display" command
    _delay_ms(2); // This command takes longer to execute
}


/**
* @brief Public function: Sets the cursor to a specific row and column.
*/
void Lcd_Set_Cursor(uint8_t row, uint8_t col) {
    uint8_t address;
    if (row == 0)
	  address = (col & 0x0F) | 0x80; // DDRAM address for row 0
    else // row == 1
	  address = (col & 0x0F) | 0xC0; // DDRAM address for row 1
    
    LCD_Command_Internal(address); // Send the "Set DDRAM Address" command
}


/* ****************** Sup-Program Section End ****************** */