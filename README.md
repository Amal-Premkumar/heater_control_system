# Heater Control System Using DHT Sensor (Bare Metal Implementation)

This project is a heater control system that reads temperature data from a DHT sensor and controls a heater to maintain a target temperature. The system is implemented in bare metal C++ for Arduino, without using any external libraries, to provide better understanding and direct access to the sensor communication protocol and heater control logic.

***

## Overview

The program reads temperature data from a DHT sensor connected to pin 2 and controls two LEDs connected to pins 4 and 6 which indicate heater status and overheat warning respectively. It maintains the temperature at a targeted level by turning the heater on or off and provides overheat protection with visual indication.

***

## Key Features

- **Bare Metal Sensor Communication:** The DHT sensor data collection is done by bit-banging the data line without using any external library, making the protocol and data timing transparent and easier to understand.
- **Heater Control Logic:** Implements a finite state machine with states including idle, heating, target heat reached, overheated, and stabilizing.
- **Overheat Protection:** When temperature exceeds a safety threshold, the heater turns off and an overheat LED blinks.
- **Temperature Targeting with Stabilization Offset:** The heater maintains temperature within a stable window below the target before turning off.
- **Serial Output:** Real-time status and temperature are output on serial for debugging and monitoring.

***

## Hardware Connections

| Component       | Microcontroller Pin | Description                     |
|-----------------|---------------------|--------------------------------|
| DHT sensor data | Digital pin 2       | Temperature sensor data line   |
| Heater LED      | Digital pin 4       | Indicates heater state         |
| Overheat LED    | Digital pin 6       | Indicates overheat condition   |

***

## State Machine Description

| State          | Description                                            | Heater LED | Overheat LED |
|----------------|--------------------------------------------------------|------------|--------------|
| idle           | System is inactive                                     | OFF        | OFF          |
| heating        | Temperature below target minus stabilization offset   | ON         | OFF          |
| stabilizing    | Temperature near target but below it                   | ON         | OFF          |
| target_heat    | Temperature within target range, heater off            | OFF        | OFF          |
| overheated     | Temperature exceeding overheat threshold, heater off  | OFF        | Blinking     |

***

## Block Diagram

```
 +-----------------+       +-----------------+       +-----------------+
 |                 |       |                 |       |                 |
 |  DHT Sensor     |-----> |   Microcontroller|-----> | Heater + LEDs   |
 |  (Pin 2)        |       | (Bare metal code)|       | Heater LED (Pin 4)|
 |                 |       |                 |       | Overheat LED (Pin 6)|
 +-----------------+       +-----------------+       +-----------------+
          |                       |                           |
          |--- Temperature ------>|--- Control Signal ------->|
          |                       |                           |
```

***

## Code Highlights

- **No libraries used:** The entire DHT sensor protocol is manually implemented through GPIO manipulation and timing delays.
- **Temperature Reading:** Uses raw 40-bit transmission from DHT sensor, verifies with checksum.
- **PWM Logic:** Simple on/off logic based on temperature thresholds to control heater and LEDs.
- **Safety:** Overheat LED blinks to alert when temperature exceeds threshold.

***

## Usage

1. Connect the DHT sensor data pin to digital pin 2.
2. Connect heater indicator LED to pin 4 and overheat indicator LED to pin 6.
3. Upload the code to your Arduino or compatible microcontroller.
4. Open the Serial Monitor at 115200 baud to view real-time temperature and system state.
5. Adjust `targeted_heat`, `overheat`, and `stablize` parameters in code to tune temperature control behavior as desired.

***

## Parameters (Configurable)

| Parameter       | Default Value | Description                              |
|-----------------|---------------|------------------------------------------|
| targeted_heat   | 32.0 °C       | Desired temperature to be maintained     |
| overheat       | 35.0 °C       | Safety overheat threshold                 |
| stablize       | 2.0 °C        | Offset below target temperature for stabilization |

***




