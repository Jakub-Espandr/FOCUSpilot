<p align="center">
  <a href="https://ibb.co/YBk6h9gZ">
    <img src="https://i.ibb.co/LdtM1wK5/fcc-FS-PRO.jpg" alt="fcc-FS-PRO" width="250"/>
  </a>
</p>

<h1 align="center">FlyCamCzech FOCUSpilot</h1>
<p align="center"><em>(Born4Flight | FlyCamCzech | Jakub Ešpandr)</em></p>

## Overview
The FlyCamCzech FOCUSpilot is an Arduino-based system designed for automated focus stacking photography. This device controls a stepper motor to move a camera lens through different focus positions, automatically triggering the camera shutter at each position to capture a series of images for focus stacking. Perfect for macro and close-up photography where extended depth of field is required.

---

## ✨ Features

- **Automated Focus Stacking**: Automatically moves through focus positions and triggers camera capture
- **Manual Control**: Manual forward/backward movement with position memory
- **Configurable Parameters**: Adjustable depth of field (DoF), step size, and timing
- **LCD Interface**: 16x2 LCD display with rotary encoder navigation
- **Keypad Control**: 4-button keypad for quick manual operations
- **EEPROM Storage**: Settings are saved to non-volatile memory
- **Bidirectional Operation**: Support for both forward and backward focus stacking sequences

---

## 📦 Requirements

### Hardware Components
- Arduino board (compatible with Arduino IDE)
- 16x2 LCD display with I2C interface (address 0x3F)
- Rotary encoder (CLK, DT, SW pins)
- 4-button keypad
- Stepper motor driver
- Relay module for camera trigger
- Stepper motor

### Software Dependencies
- [Wire.h](https://www.arduino.cc/reference/en/language/functions/communication/wire/) – I2C communication
- [LiquidCrystal_I2C_Menu.h](https://github.com/andriyadi/LiquidCrystal_I2C_Menu) – LCD menu system
- [EEPROM.h](https://www.arduino.cc/reference/en/libraries/eeprom/) – Non-volatile memory storage
- [Keypad.h](https://github.com/Chris--A/Keypad) – Keypad input handling
- [EEPROMTyped.h](https://github.com/xoseperez/eeprom-typed) – Typed EEPROM operations

---

## 🔌 Pin Connections

| Component | Pin | Description |
|-----------|-----|-------------|
| Rotary Encoder | 2 | CLK |
| Rotary Encoder | 4 | DT |
| Rotary Encoder | 3 | SW (Button) |
| Keypad Row | 5 | Row pin |
| Keypad Columns | 7, 6, 9, 8 | Column pins |
| Stepper Motor | 13 | Step pin |
| Stepper Motor | 12 | Direction pin |
| Stepper Motor | 17 | Enable pin |
| Microstep Pins | 16, 15, 14 | MS1, MS2, MS3 |
| Camera Trigger | 11 | Relay trigger |

---

## 🚀 Installation

1. **Install Libraries**: Install the required Arduino libraries through the Library Manager
2. **Connect Hardware**: Wire the components according to the pin diagram above
3. **Upload Code**: Upload the `fccFS-PRO_1.0.0.ino` file to your Arduino board
4. **Power On**: Connect power and initialize the system

---

## 🛠️ Usage

### Manual Control (Keypad)

| Button | Function |
|--------|----------|
| 1 | Move backward (hold to continue) |
| 2 | Save current position as start point |
| 3 | Move forward (hold to continue) |
| 4 | Save current position as end point |

### Menu Navigation (Rotary Encoder)

Press the encoder button to access the main menu:

#### Main Menu Options:
- **CAPTURE**: Trigger camera shutter manually
- **RUN**: Automated focus stacking sequences
  - RUN Front: Execute forward focus stacking
  - RUN Back: Execute backward focus stacking
  - Delete TRACE: Clear saved positions
- **GO**: Manual movement commands
  - GO Front: Move to end position
  - GO Back: Move to start position
  - Delete TRACE: Clear saved positions
- **SETTINGS**: Configure system parameters
  - Step: Set microstepping (1/1 to 1/32)
  - DoF: Set depth of field (1-16, affects step size)
  - Time: Set delay between captures (1-16 seconds)
- **SAVE**: Save current settings to EEPROM
- **RESTORE**: Restore default settings

### Configuration Parameters

#### Step Size (Microstepping)
- 1 = 1/1 step (coarsest)
- 2 = 1/2 step
- 3 = 1/4 step
- 4 = 1/8 step
- 5 = 1/16 step
- 6 = 1/32 step (finest)

#### Depth of Field (DoF)
- Range: 1-16
- 1 = 0.1mm step size
- 16 = 1mm step size
- Affects the distance between focus positions

#### Timing
- Range: 1-16 seconds
- Delay between camera captures during automated sequences

---

## 📁 Project Structure

```
FOCUSpilot/
├── fccFS-PRO_1.0.0/
│   └── fccFS-PRO_1.0.0.ino    # Main Arduino sketch
├── fccFS-PRO_1.0.0.cpp        # Implementation file
├── fccFS-PRO_1.0.0.h          # Header file
├── libraries/                  # Required Arduino libraries
│   ├── EEPROM/
│   ├── EEPROMTyped-1.0.0/
│   ├── Keypad/
│   ├── LiquidCrystal_I2C_Menu-master/
│   └── Wire/
└── README.md                   # This file
```

---

## 🔧 Focus Stacking Workflow

1. **Setup**: Configure step size, DoF, and timing in SETTINGS menu
2. **Positioning**: Use manual controls to set start and end positions
   - Move to desired start position and press button 2
   - Move to desired end position and press button 4
3. **Execution**: Choose RUN Front or RUN Back from the menu
4. **Monitoring**: The LCD displays progress (e.g., "5 from 100")
5. **Completion**: System returns to idle state

---

## ⚙️ Default Settings

- Step Size: 1/4 (3)
- DoF: 10
- Timing: 2 seconds

---

## 🔐 License

This project is licensed under the **Non-Commercial Public License (NCPL v1.0)**  
© 2025 Jakub Ešpandr - Born4FLight, FlyCamCzech

See the [LICENSE](https://github.com/Jakub-Espandr/FOCUSpilot/raw/main/LICENSE) file for full terms.

---

## 🙏 Acknowledgments

- Built with ❤️ using Arduino and open-source libraries
- For support and updates, visit: [WWW.FlyCamCzech.EU/FOCUSpilot](https://www.flycamczech.eu/en/focuspilot/)