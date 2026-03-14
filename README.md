# EPUMP_Arduino_Code

Arduino-based test controller for the Epump. Functions: temperature monitoring, adjustable motor speed via potentiometer, and safety interlocks for over-temperature and manual emergency shutoff.

## Hardware

| Component | Description | Pin |
|-----------|-------------|-----|
| MAX6675 module | K-type thermocouple interface | CLK → D13, CS → D10, DO → D12 |
| ESC / Motor | Brushless motor via ESC (PWM) | Signal → D9 |
| Potentiometer | Speed control knob | Wiper → A0, ends → 5V / GND |
| Push button | Emergency stop | One leg → D2, other leg → GND |

> The button uses the Arduino's internal pull-up resistor (`INPUT_PULLUP`), so no external resistor is needed. Pressing the button pulls D2 to LOW.

## Configuration

These constants at the top of the sketch:

| Constant | Default | Description |
|----------|---------|-------------|
| `TEMP_THRESHOLD` | 80.0 °C | Temperature at which the motor is automatically disabled |
| `MAX_FORWARD` | 2000 µs | ESC full-forward pulse width |
| `NEUTRAL` | 1500 µs | ESC neutral pulse width |
| `MAX_REVERSE` | 1000 µs | ESC full-reverse pulse width |

## Dependencies

- Adafruit MAX6675 Library
- Servo Library

## Motor Cutoff Conditions
 
The motor is permanently disabled (sent to neutral) when any of the following occurs:
 
1. **Over-temperature** — Thermocouple reading meets or exceeds `TEMP_THRESHOLD` (80.0 °C).
2. **Manual kill switch** — The physical button on D2 is pressed.
3. **Thermocouple fault** — The MAX6675 returns `NaN`, indicating the thermocouple is disconnected or damaged.
 
All three conditions are one-way: once triggered, the `motorEnabled` flag is set to `false` and the ESC receives a neutral signal (1500 µs) for every subsequent loop iteration. A full board reset or power cycle is required to restore motor control.
