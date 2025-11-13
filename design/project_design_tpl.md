# Detailed Design Document

| **Author**              | `Ali Akram - Mahmoud Adgham - Ziad Khalil`                                       |
|:------------------------|:-----------------------------------------------------|
| **Status**              | `Draft`                          |
| **Version**             | `1.0`                                                |
| **Date**                | `13/11/2025`                                         |

## Introduction


This document provides the detailed software design for the "Human Machine Interface" project, which is Element 2 of the KH5023FTE Embedded System Design & Development module coursework. The system is developed for the AVR ATmega328P microcontroller.

### Purpose

This project aims to build a reliable electronic system that uses the UART communication link to send and receive data between its main chip (the microcontroller) and an outside device like a computer. The system has three main goals:

Read and Show Data: The system will constantly take an analog reading (like voltage) using the ADC and show the result on the LCD screen.

Allow User Control: The system will watch the KeyPad for button presses, specifically to let the user press a button to send the current reading to the computer.

Send Data Out: The system will use UART to dependably send the prepared data to the computer when the user triggers it.

### Scope



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

### Assumptions & Constraints
Indicate constraints of the low level design and any assumptions appropriate for the design.

## Functional Description
The following chapter describes the software functionality.  The following is a list of suggested sections to be considered for inclusion.

## Implementation of the Module
This chapter discusses the detailed design of the module.

## Integration and Configuration
### Static Files
Typically a module consists of C and H files but other file types may exist. List all files that form this module

| File name | Contents                             |
|-----------|--------------------------------------|
| abc_xxx.c | Source code file, add detail         |
| abc.h     | Export Interface file                |
| abci.h    | Import and Module Configuration file |

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
Any required hardware/software or system configuration that can be adjusted a header file shall be detailed in this section. This can be placed in the table below.
| Name | Value range | Description |
|------|-------------|-------------|
|      |             |             |
