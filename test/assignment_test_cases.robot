*** Settings ***
Documentation     Test Suite for the HMI Alarm System (KH5023FTE Element 2).
...               These tests verify the core logic:
...               1. Reading and displaying the Potentiometer value.
...               2. Updating High/Low limits via the keypad.
...               3. Correctly triggering the LED alarm and "OK/NOK" status.
Library           HMIHardwareLibrary.py    # A hypothetical library to control the hardware

*** Keywords ***
Given The System Is In Default State
    [Documentation]    Powers on the device and waits for init.
    Power On Device
    Wait For Milliseconds    500
    Then The LED State Should Be    OFF
    And The LCD Line 1 Should Display    "L:100 H:500"

When The User Sets Potentiometer Value To
    [Arguments]    ${value}
    [Documentation]    Simulates setting the potentiometer to a specific ADC value.
    Set Potentiometer    ${value}
    Wait For Milliseconds    250    # Wait for the main loop to refresh

When The User Presses The Button
    [Arguments]    ${button_name}
    [Documentation]    Simulates a single, debounced press of a keypad button.
    Press Button    ${button_name}
    Wait For Milliseconds    250    # Wait for the button logic and debounce

Then The LED State Should Be
    [Arguments]    ${expected_state}
    [Documentation]    Checks if the Alarm LED is ON or OFF.
    ${actual_state}=    Read LED State
    Should Be Equal As Strings    ${actual_state}    ${expected_state}

And The LCD Line 0 Should Display
    [Arguments]    ${expected_text}
    [Documentation]    Verifies the full text of the first line of the LCD.
    ${actual_text}=    Read LCD Line    0
    Should Be Equal As Strings    ${actual_text}    ${expected_text}

And The LCD Line 1 Should Display
    [Arguments]    ${expected_text}
    [Documentation]    Verifies the full text of the second line of the LCD.
    ${actual_text}=    Read LCD Line    1
    Should Be Equal As Strings    ${actual_text}    ${expected_text}

*** Test Cases ***
TC-01: System Power On (Initial State)
    [Documentation]    Verifies the device boots to the correct default state.
    [Tags]    Sanity    Boot
    Given The System Is In Default State
    Then The LED State Should Be    OFF
    And The LCD Line 1 Should Display    "L:100 H:500"
    When The User Sets Potentiometer Value To    300
    Then The LCD Line 0 Should Display    "POT: 300  OK  "

TC-02: Alarm Logic (Potentiometer Above High Limit)
    [Documentation]    Tests that the alarm triggers when POT > High Limit.
    [Tags]    Functional    Alarm
    Given The System Is In Default State
    When The User Sets Potentiometer Value To    600
    Then The LED State Should Be    ON
    And The LCD Line 0 Should Display    "POT: 600  NOK "

TC-03: Alarm Logic (Potentiometer Below Low Limit)
    [Documentation]    Tests that the alarm triggers when POT < Low Limit.
    [Tags]    Functional    Alarm
    Given The System Is In Default State
    When The User Sets Potentiometer Value To    50
    Then The LED State Should Be    ON
    And The LCD Line 0 Should Display    "POT: 50   NOK "

TC-04: Alarm Logic (Potentiometer Within Limits - OK)
    [Documentation]    Tests that the alarm is OFF when POT is within limits.
    [Tags]    Functional    Alarm
    Given The System Is In Default State
    When The User Sets Potentiometer Value To    300
    Then The LED State Should Be    OFF
    And The LCD Line 0 Should Display    "POT: 300  OK  "

TC-05: Alarm Logic (Boundary Check - At High Limit)
    [Documentation]    Tests that the alarm is OFF when POT is *equal* to the High Limit.
    [Tags]    Functional    Alarm    Boundary
    Given The System Is In Default State
    When The User Sets Potentiometer Value To    500
    Then The LED State Should Be    OFF
    And The LCD Line 0 Should Display    "POT: 500  OK  "

TC-06: Alarm Logic (Boundary Check - At Low Limit)
    [Documentation]    Tests that the alarm is OFF when POT is *equal* to the Low Limit.
    [Tags]    Functional    Alarm    Boundary
    Given The System Is In Default State
    When The User Sets Potentiometer Value To    100
    Then The LED State Should Be    OFF
    And The LCD Line 0 Should Display    "POT: 100  OK  "

TC-07: Keypad Logic (Increment High Limit)
    [Documentation]    Tests that the UP button increases the high_limit by 50.
    [Tags]    Functional    Keypad
    Given The System Is In Default State
    When The User Presses The Button    KEY_UP
    Then The LCD Line 1 Should Display    "L:100 H:550"
    When The User Presses The Button    KEY_UP
    Then The LCD Line 1 Should Display    "L:100 H:600"

TC-08: Keypad Logic (Increment Low Limit)
    [Documentation]    Tests that the DOWN button increases the low_limit by 50.
    [Tags]    Functional    Keypad
    Given The System Is In Default State
    When The User Presses The Button    KEY_DOWN
    Then The LCD Line 1 Should Display    "L:150 H:500"
    When The User Presses The Button    KEY_DOWN
    Then The LCD Line 1 Should Display    "L:200 H:500"

TC-09: Keypad Logic (High Limit Wrap-Around)
    [Documentation]    Tests that high_limit wraps to 0 after 1050.
    [Tags]    Functional    Keypad    Wrap
    Given The System Is In Default State
    ...    # Manually press UP 11 times to get to 1050
    FOR    ${i}    IN RANGE    11
        When The User Presses The Button    KEY_UP
    END
    Then The LCD Line 1 Should Display    "L:100 H:1050"
    When The User Presses The Button    KEY_UP
    Then The LCD Line 1 Should Display    "L:100 H:0"

TC-10: Keypad Logic (Low Limit Wrap-Around)
    [Documentation]    Tests that low_limit wraps to 0 after 1050.
    [Tags]    Functional    Keypad    Wrap
    Given The System Is In Default State
    ...    # Manually press DOWN 19 times to get to 1050
    FOR    ${i}    IN RANGE    19
        When The User Presses The Button    KEY_DOWN
    END
    Then The LCD Line 1 Should Display    "L:1050 H:500"
    When The User Presses The Button    KEY_DOWN
    Then The LCD Line 1 Should Display    "L:0 H:500"

TC-11: Integration Test (Change Limit and Verify Alarm)
    [Documentation]    Tests that the alarm logic uses the *new* limits set by the user.
    [Tags]    Integration
    Given The System Is In Default State
    When The User Sets Potentiometer Value To    550
    Then The LED State Should Be    ON        # Alarm is ON (550 > 500)
    
    When The User Presses The Button    KEY_UP    # New High Limit is 550
    Then The LCD Line 1 Should Display    "L:100 H:550"
    And The LED State Should Be    OFF       # Alarm is now OFF (550 is not > 550)
    
    When The User Presses The Button    KEY_UP    # New High Limit is 600
    Then The LCD Line 1 Should Display    "L:100 H:600"
    And The LED State Should Be    OFF       # Alarm is still OFF (550 < 600)
    
    When The User Sets Potentiometer Value To    650
    Then The LED State Should Be    ON        # Alarm is ON again (650 > 600)