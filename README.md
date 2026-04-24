# Line Follower Robot

An ESP32-based autonomous line follower robot with 10 IR sensors and dual motor control system.

## Hardware Configuration

### Sensor Pins (10 IR Sensors)
```
pins[0-9]: 34, 35, 32, 33, 25, 26, 27, 14, 12, 13
```
- Configured as INPUT
- Detects black line when pin reads LOW
- Weight-based position calculation for line tracking

### Motor Control Pins

**Left Motor (Motor A):**
- ENA (PWM): GPIO 5
- IN1 (Direction): GPIO 18
- IN2 (Direction): GPIO 19

**Right Motor (Motor B):**
- ENB (PWM): GPIO 17
- IN3 (Direction): GPIO 16
- IN4 (Direction): GPIO 4

**Motor Configuration:**
- PWM Frequency: 1000 Hz
- PWM Resolution: 8-bit (0-255)

### Control Buttons

- **START Button**: GPIO 21 (INPUT_PULLUP) - Starts the robot
- **STOP Button**: GPIO 22 (INPUT_PULLUP) - Emergency stop/kill switch

## Software Features

### Motor Functions

**moveForward(leftSpeed, rightSpeed)**
- Drives both motors forward
- Parameters: speed values 0-255
- Differential speed enables turning

**stopMotors()**
- Stops both motors
- Sets PWM to 0

### Line Position Detection

**readLinePosition()**
- Returns weighted position (-4 to +4)
- Negative values: line is on the left
- Positive values: line is on the right
- Special values:
  - `999`: T-strip detected (all 10 sensors see black)
  - `888`: Line lost (no sensors detect black)

**Weight Array:** `[-4, -3, -2, -1, 0, 0, 1, 2, 3, 4]`
- Sensors 4 & 5 (center): weight 0
- Outer sensors have higher weights for better turning

### Control Logic

**Base Speed:** 150 (0-255)

**Turning Algorithm:**
- If line is left (position < 0):
  - Reduce left motor speed
  - Formula: `leftSpeed = 150 - (|position| × 25)`
- If line is right (position > 0):
  - Reduce right motor speed
  - Formula: `rightSpeed = 150 - (|position| × 25)`

**Special Behaviors:**
- **T-Strip Detection**: Automatically stops when all sensors detect black
- **Line Lost**: Moves forward at reduced speed (80, 80) to find the line
- **Emergency Stop**: STOP button immediately halts and disables robot

## Operation

1. Power on the ESP32
2. Press START button (GPIO 21) to begin
3. Robot follows the black line automatically
4. Press STOP button (GPIO 22) to emergency stop
5. Robot stops automatically at T-strip intersection

## Technical Specifications

- **Microcontroller**: ESP32
- **Baud Rate**: 115200
- **Sensors**: 10× Infrared (IR) Line Sensors
- **Motors**: 2× DC Motors with PWM control
- **Power Management**: Button-controlled operation

## Algorithm Flow

```
Setup → Wait for START → Read 10 sensors
  ↓
  Calculate line position (weighted average)
  ↓
  If T-strip detected → STOP
  If line lost → Move forward slowly
  Otherwise → Adjust motor speeds based on position
  ↓
  Loop (until STOP pressed)
```
