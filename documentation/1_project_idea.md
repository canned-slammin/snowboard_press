# Final Project Idea
## Snowboard Press Heat and Pressure Controller
"Hot 'n' Squishy"
### Features
- Pressure sensing
- Pressure inlet control
- Pressure exhaust control
- Temperature sensing
- Temperature control (heating only)
- Touch screen control
- LCD display
- Manual mode
- Automatic (profile) mode
- Manual overrides
### Implementation
#### Hardware
- Watlow PID controllers for heat blanket control
- 12V solenoids for pressure control
- 3.3V - 5V Mechanical relays for solenoid control
- LCD capacitive touchscreen as Human Machine Interface (HMI)
- 12V power input
- 3.3V LDO (LD1117S33TR)
- 5V LDO (LD1117S50TR)
- SWD broken out for JLink debug/flashing
- RS-485 level shifting
- 16 bit ADC
#### Software
- Zephyr RTOS
- Zephyr State Machine Framework
- Modbus communication to Watlow controllers
- I2C communication to ADC
- GPIO control of solenoid relays
- SPI communication to display
- I2C communication to captouch controller
### Risks
- PCB design is hard. Mitigate by copying dev board design as much as possible and making intermediary prototypes (e.g. a breakout board to power and flash the microcontroller)
- Touchscreen control with manual overrides introduces complexity and potential for race conditions. Mitigate risk by designing system as a hierarchical state machine
- Modbus communication is a new challenge. Mitigate by using Zephyr modules and understanding examples before implementing.
### Notes