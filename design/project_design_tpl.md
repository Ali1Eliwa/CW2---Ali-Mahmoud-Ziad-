# Human Machine Interface (HMI) Project Design

| **Author**              | `Ali Akram - Mahmoud Adgham - Ziad Khalil`                                       |
|:------------------------|:-----------------------------------------------------|
| **Status**              | `Draft`                          |
| **Version**             | `1.0`                                                |
| **Date**                | `13/11/2025`                                         |

## Introduction


This document provides the detailed software design for the "Human Machine Interface" project, which is Element 2 of the KH5023FTE Embedded System Design & Development module coursework. The system is developed for the AVR ATmega328P microcontroller.

### Purpose

This project aims to build a reliable electronic system that uses the UART communication link to send and receive data between its main chip (the microcontroller) and an outside device like a computer. The system has three main goals:

1. Read and Show Data: The system will constantly take an analog reading (like voltage) using the ADC and show the result on the LCD screen.

2. Allow User Control: The system will watch the KeyPad for button presses, specifically to let the user press a button to send the current reading to the computer.

3. Send Data Out: The system will use UART to dependably send the prepared data to the computer when the user triggers it.

### Scope

According to the project brief and the architectural requirements, this detailed design's scope is limited to two primary use cases that cover data acquisition and user interaction:

`Use Case #1`: Display and Monitoring
The system will continuously acquire and process analog data using the ADC module. It will take the digitized value, format it (e.g., convert to temperature or LDR intensity), and immediately display the result on the LCD 16x2 screen for real-time monitoring by the user. An LED or Buzzer alert will be triggered if the monitored value falls outside the configured High or Low limits.

`Use Case #2`: User Configuration and Transmission
The system will monitor the KeyPad for user input. It must allow the user to configure the High Limit (LH) and the Low Limit (LL) for the monitored sensor data using dedicated control buttons. Furthermore, upon a specific key press (e.g., an 'Enter' key), the system must transmit the current sensor reading to the external host via the UART interface.

Native C programming must be used to complete the project; no external libraries or Arduino-based code may be used. 


## Architectural Overview

This section describes where this module resides in the context of the software architecture
```plantuml

@startuml
title HMI Project - Software Component Diagram
!theme spacelab

component [Main App (HMI_Project.ino)] as Main
component [LCD Driver (Lcd.ino)] as LCD
component [ADC Driver (Adc.ino)] as ADC
component [Keypad Driver (Keypad.ino)] as Keypad
component [Utilities (Utils.ino)] as Utils
database [Hardware_Defs.h] as HW_Defs

Main --> LCD : Uses
Main --> ADC : Uses (for POT)
Main --> Keypad : Uses
Main --> Utils : Uses
Main ..> HW_Defs : Includes

Keypad --> ADC : Uses
Keypad ..> HW_Defs : Includes
@enduml

```

## Assumptions & Constraints
### Assumptions
* **Microcontroller:** The code is written for an **AVR ATmega328P** microcontroller.
* **System Clock:** The system clock frequency (`F_CPU`) is **16MHz**. This is critical for `_delay_ms()` timing and the ADC prescaler settings.
* **Hardware Connections:** All components (LCD, LED, Keypad, Potentiometer) are connected to the exact ports and pins defined in `Hardware_Defs.h`.
* **ADC Configuration:** The ADC is a 10-bit converter (0-1023) using AVcc as its reference voltage.
* **Keypad Thresholds:** The resistor network for the analog keypad produces ADC values that reliably fall within the thresholds defined in `Hardware_Defs.h`.
* **Design:** A simple polling "super-loop" design with blocking delays (like `_delay_ms()`) is acceptable for this application's responsiveness needs.

### Constraints
* **Native C:** The application must be written in **Native C**, without using any Arduino-based functions or libraries.
* **Direct Register Access:** All peripherals (Ports, ADC) must be controlled via direct register manipulation (e.g., `PORTD`, `ADMUX`, `ADCSRA`).
* **AI Use:** Using AI generative models to write the code is **not permitted**.
* **Hardware Interface:** The LCD must be driven in **4-bit mode**, and the display is a 16x2 character LCD.
* **Input Logic:** The high and low limits are controlled using the `KEY_UP` and `KEY_DOWN` buttons, respectively.

## Functional Description
The system runs on one continuous, loop that manages all its core activities. This loop has three primary jobs:
1. Check Sensor & Update Screen: The system constantly reads the sensor data using the ADC. It quickly shows this new value on the LCD screen.
2. Look for Button Presses: It continually checks the KeyPad to see if the user pressed a button, especially the buttons used to set the High and Low limits for the sensor.
3. Send Data if Triggered: If the user presses the special send button on the KeyPad, the system uses UART to immediately send the current sensor reading to the computer.

#### This section provides a more detailed illustration of the application's logic.
### 1. One-Time Setup
* Initialize the LCD, ADC, and LED.
* Set the default **High Limit** (500) and **Low Limit** (100).

### 2. The Main Loop (Repeats Forever)
* **Read Inputs:** Get the current sensor value (from the potentiometer) and check which button (if any) is pressed.
* **Process Commands:**
    * If the **UP** button is pressed, increase the **High Limit** by 50 (wrapping to 0 after 1050).
    * If the **DOWN** button is pressed, increase the **Low Limit** by 50 (wrapping to 0 after 1050).
* **Check Alarm:** Compare the sensor value to the limits. If the value is **outside** the `Low Limit` to `High Limit` range, set an alarm.
* **Show Status:**
    * Turn the **LED ON** if the alarm is set, otherwise turn it **OFF**.
    * Update the **LCD** to show the current sensor value, the (OK/NOK) status, and the current high/low limits.

#### Application Logic Flow
```plantuml
@startuml
title HMI Application Main Loop Logic

start
:Initialize LED Pin (Output);
:Initialize ADC (adc_init);
:Initialize LCD (lcd_init);
:Store last_key = KEY_NONE;

while (true) is (Main Loop)
    :Read Potentiometer Value
    (pot_value = adc_read(POT_ADC_CHANNEL));
    :Read Keypad Value
    (key_press = get_keypad_press());
    
    if (key_press != KEY_NONE AND 
        key_press != last_key) then (yes)
      :Handle New Key Press;
      if (key_press == KEY_UP) then (yes)
        :high_limit += 50;
        if (high_limit > 1050) then (yes)
            :high_limit = 0;
        endif
      else if (key_press == KEY_DOWN) then (yes)
        :low_limit += 50;
        if (low_limit > 1050) then (yes)
            :low_limit = 0;
        endif
      endif
      :Debounce delay (150ms);
      :Update last_key = key_press;
    else (no)
      :Update last_key = key_press;
    endif
    
    :Process Alarm Logic;
    if (pot_value > high_limit OR
        pot_value < low_limit) then (yes)
        :Turn LED ON;
        :Set status to "NOK";
    else (no)
        :Turn LED OFF;
        :Set status to "OK";
    endif
    
    :Update LCD Display;
    note right
        - Row 0: "POT: [value] [status]"
        - Row 1: "L: [low_limit] H: [high_limit]"
    end note
    
    :Short delay (100ms);
endwhile

stop
@enduml
```

## Implementation of the Module

The project is broken into several key modules:

* **`HMain_Project.ino`**: Contains the `main()` function, global variables for `high_limit` and `low_limit`, and all high-level application logic.
* **`adc.ino`**: Implements the ADC driver.
    * `adc_init()`: Configures the ADC by setting the reference voltage to AVcc and setting the prescaler to 128 for a 125kHz ADC clock.
    * `adc_read()`: Selects the specified channel, starts a conversion, polls the `ADSC` bit until the conversion is complete, and returns the 10-bit result.
* **`lcd.ino`**: Implements the 16x2 LCD driver in 4-bit mode.
    * `lcd_init()`: Performs the required 4-bit initialization sequence.
    * `lcd_send_nibble()` / `lcd_send_byte()`: Internal helper functions to send 4-bit and 8-bit data, respectively, by manipulating the data and control pins.
    * `lcd_send_string()`: Iterates through a character pointer and sends each character to the LCD.
* **`keypad.ino`**: Implements the analog keypad driver.
    * `get_keypad_press()`: Reads the raw 10-bit ADC value from the keypad channel. It then compares this value against the thresholds defined in `hardware_defs.h` to identify and return the pressed key.
* **`utils.ino`**: Provides a `simple_itoa()` utility function to convert an integer into a string, which is necessary for displaying numerical values on the LCD.

## Integration and Configuration
### Static Files
All source and header files that comprise the project are listed below.
| File name | Contents |
| :--- | :--- |
| **HMI\_Project.ino** | Main application logic (`setup()` / `loop()`), input processing, and display control. |
| **Hardware\_Defs.h** | Central definitions for all hardware pins, ports, and ADC thresholds. |
| **Adc.h** | Exported interface for the ADC driver. |
| **Adc.ino** | Implementation of the ADC driver (init and read functions). |
| **Lcd.h** | Exported interface for the 16x2 LCD driver. |
| **Lcd.ino** | Implementation of the 4-bit LCD driver. |
| **Keypad.h** | Exported interface for the analog keypad driver. |
| **Keypad.ino** | Implementation of the keypad driver (maps ADC values to key presses). |
| **Utils.h** | Exported interface for utility functions. |
| **Utils.ino** | Implementation of `Simple_Itoa` utility function. |

### Include Structure

If there is a complex file structure e.g. more than one c-file or more than two h-files use a diagram to explain the relationship between the source and dependent include files.

```plantuml

@startuml
title File Include Structure

package "Application" {
    component [HMI_Project.ino]
}

package "Drivers" {
    component [Keypad.ino]
    component [Lcd.ino]
    component [Adc.ino]
    component [Utils.ino]
}

package "Interfaces" {
    header [Keypad.h]
    header [Lcd.h]
    header [Adc.h]
    header [Utils.h]
}

package "Configuration" {
    header [Hardware_Defs.h]
}

' --- Relationships ---
[HMI_Project.ino] ..> [Hardware_Defs.h] : includes
[HMI_Project.ino] ..> [Adc.h] : includes
[HMI_Project.ino] ..> [Lcd.h] : includes
[HMI_Project.ino] ..> [Keypad.h] : includes
[HMI_Project.ino] ..> [Utils.h] : includes

[Keypad.ino] ..> [Keypad.h] : includes
[Keypad.ino] ..> [Hardware_Defs.h] : includes
[Keypad.ino] ..> [Adc.h] : includes

[Lcd.ino] ..> [Lcd.h] : includes
[Adc.ino] ..> [Adc.h] : includes
[Utils.ino] ..> [Utils.h] : includes
@enduml

```

### Configuration
All hardware and application-level constants are defined in Hardware_Defs.h for easy configuration.
| Name | Value | Description |
|:---|:---|:---|
| *`LCD Pins`* | | |
| LCD_RS_PORT | PORTB | Port for LCD Register Select |
| LCD_RS_PIN | 0 (PB0) | Pin for LCD Register Select |
| LCD_E_PORT | PORTB | Port for LCD Enable |
| LCD_E_PIN | 1 (PB1) | Pin for LCD Enable |
| LCD_DATA_PORT| PORTD | Port for LCD Data Lines (D4-D7) |
| LCD_D4_PIN | 4 (PD4) | Data Line 4 |
| LCD_D5_PIN | 5 (PD5) | Data Line 5 |
| LCD_D6_PIN | 6 (PD6) | Data Line 6 |
| LCD_D7_PIN | 7 (PD7) | Data Line 7 |
| *`LED Pin`* | | |
| LED_PORT | PORTD | Port for the Alarm LED |
| LED_PIN | 3 (PD3) | Pin for the Alarm LED |
| *`ADC Channels`* | | |
| KEYPAD_ADC_CHANNEL | 0 | ADC channel for the analog keypad |
| POT_ADC_CHANNEL | 2 | ADC channel for the potentiometer |
| *`Keypad Thresholds`*| | ADC values for key detection |
| KEY_ADC_RIGHT | 50 | Threshold for RIGHT key (0-49) |
| KEY_ADC_UP | 195 | Threshold for UP key (50-194) |
| KEY_ADC_DOWN | 380 | Threshold for DOWN key (195-379) |
| KEY_ADC_LEFT | 555 | Threshold for LEFT key (380-554) |
| KEY_ADC_SELECT| 790 | Threshold for SELECT key (555-789) |
