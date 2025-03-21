# Hot_n_Squishy Architecture

## High Level Description

<ul>
<li>Pressure and heat controller with touchscreen interface  
</ul>

<ul>
<li>Main CPU is STM32L475VGT6 (subject to change based on required features)
<li>CPU will be running Zephyr RTOS
<li>Program functionality will be structured around Zephyr's State Machine Framework
</ul>

<ul>
<li>Pressure sense performed by analog sensor through ADC on I2C bus
<li>Pressure control performed by solenoid valves triggered by relays on GPIO bus
<li>Heat control is performed by communication with Watlow PID controllers via ModBus protocol on UART over RS-485
</ul>

<ul>
<li>Touchscreen control will be on I2C bus using the LVGL handler; the handler is polling, so other I2C peripherals will be need to be on a separate bus
<li>LCD display will be communicated with via SPI
</ul>

<ul>
<li>Interface will have three menus: Manual Control, Run Profile, and Set Profile
<ul>
<li>Manual control will have input for Top Heat, Bottom Heat, and Pressure
<li>Run Profile will monitor profile status and accept input for Run and Cancel
<li>Set Profile will accept input for profile parameters
</ul>
</ul>

## Hardware Modules

### Pressure Control

- 12V Solenoid Valve (model #?)
- 5V GPIO-triggered relay (part #?) (support circuitry?) (3.3V tolerant?)
- Relays on GPIOA, GPIOB, or GPIOC bus (whichever accessible with touchscreen installed)

### Pressure Sense

- Analog pressure sensor (range?) (model?) (power?)
- Texas Instruments ADS1X15 (TBD 12- or 16-bit ) Analog to Digital Converter
- Input to ADC will be differential to cut down on noise
- ADC on I2C2 bus
- ADC power requirement?
- Stable voltage reference could be provided by LM4040 (TBD)

### Power Supply

- External 12V power supply for solenoid valves and main system input
- 5V LDO (peripherals) TI LD1117S50TR (from dev board)
- 3.3V LDO (MCU power) TI LD1117S33TR (from dev board)
- TBD: can 5V LDO be eliminated if all peripherals are 3.3V tolerant?

### Touchscreen 

- Touchscreen controller on I2C1 bus
- Display on SPI1 bus
- Combined adafruit package (model?) (power?)

### Debug / Flash

- Single Wire Debug (SWD) to accomodate either STM or JLlink programmers
- Any resistors needed for SWD lines?
- Break out:

1. SWCLK
2. SWDIO
3. SWO
4. NRST (needs 100nF cap to GND)
5. GND
6. 3.3V

### Heat Control

- Communication over RS-485 will need level shifters (spec TBD)
- Communication will take place on UART2 bus (Dev board probably needs some jumpers on PMOD)

### Comm Bus Summary

GPIOA/B/C (whichever accessible thru arduino connectors)
GPIO? (ADC Drdy)
SPI1
I2C1
UART1
UART2

## Software Modules

### Pressure Control

### Heat Control

### Menus

- Button: Next Menu
- Button: Previous Menu

#### Manual Menu

- Indicator: Top Heat Actual (from Watlow)
- Indicator: Top Heat Setpoint (from Watlow)
- Indicator: Bottom Heat Actual (from Watlow)
- Indicator: Bottom Heat Setpoint (from Watlow)
- Button: Top Heat Setpoint Up (to Watlow)
- Button: Top Heat Setpoint Down (to Watlow)
- Button: Bottom Heat Setpoint Up (to Watlow)
- Button: Bottom Heat Setpoint Down (to Watlow)
- Indicator: Pressure actual (From ADC)
- Indicator: Pressure setpoint (from pressure object)
- Button: Pressure up (to pressure object)
- Button: Pressure down (to pressure object)

#### Run Profile Menu

- Indicator: Top Heat Setpoint (from Watlow)
- Indicator: Bottom Heat Setpoint (from Watlow)
- Indicator: Top Heat Actual (from Watlow)
- Indicator: Bottom Heat Actual (from Watlow)
- Indicator: Pressure Setpoint (from pressure object)
- Indicator: Pressure Actual (from ADC)
- Button: Run (smf event)
- Button: Cancel (smf event)

#### Set Profile Menu
- Indicator: Top Heat Dwell Temp (from profile object)
- Indicator: Top Heat Cool Temp (from profile object)
- Indicator: Bottom Heat Dwell Temp (from profile object)
- Indicator: Bottom Heat Cool Temp (from profile object)
- Indicator: Pressure Setpoint (from profile object)
- Indicator: Hold Time (from profile object)
- Indicator: Dwell Time (from profile object)
- Button: Top Heat Dwell Temp up (to profile object)
- Button: Top Heat Dwell Temp down (to profile object)
- Button: Bottom Heat Dwell Temp up (to profile object)
- Button: Bottom Heat Dwell Temp down (to profile object)
- Button: Top Heat Cool Temp up (to profile object)
- Button: Top Heat Cool Temp down (to profile object)
- Button: Bottom Heat Cool Temp up (to profile object)
- Button: Bottom Heat Cool Temp down (to profile object)
- Button: Pressure Setpoint up (to profile object)
- Button: Pressure Setpoint down (to profile object)
- Button: Hold Time up (to profile object)
- Button: Hold Time down (to profile object)
- Button: Dwell Time up (to profile object)
- Button: Dwell Time down (to profile object)


