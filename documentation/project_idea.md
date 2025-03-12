# Final Project Idea
## MIDI Keyboard with Touch Features
### Features
- 63 note keyboard
- Touchscreen drum pad
- Menu options on touch screen (MIDI effect, pitch control, etc.)
### Implementation
#### Software
- Hierarchical state machine
- Interrupt handling for all sensors
- Expandable GUI (multiple pages)
#### Hardware
- Adafruit audio shield for MIDI codec and audio amp
- Adafruit capacitive touch shield for touchscreen
- PCF8574 IO Expanders (I2C) for buttons and interrupts
- Keyboard chassis and speakers from Goodwill 
### Risks
- That's a lot of hardware
- I'm going to have to learn what "Hierarchical" means
- GUI development strikes fear into my heart
### Notes
- This will be an expansion of my project from 320, which was a keyboard with a touchscreen drum pad and 16 notes
- I was proud of the functional outcome of my project, but embarrassed at how haphazardly the software was put together and would like to take better advantage of the capabilities offered by a RTOS
- I have a simple C++ driver built already for the PCF8574, but I am assuming I will have to update it to replace HAL calls with the Zephyr I2C and GPIO drivers
- The PCF8574 has an interrupt that triggers a change in any of its inputs, so my tentative plan is to use an extra IO expander to handle interrupts from its PCF siblings
- I'm somewhat familiar with state machines from the 'Embedded Systems Shape the World' free online course; however, I will need to read ahead to understand the hierarchical state machine architecture