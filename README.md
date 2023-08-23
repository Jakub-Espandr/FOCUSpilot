<p align="center">
  <a href="https://ibb.co/YBk6h9gZ">
    <img src="https://i.ibb.co/LdtM1wK5/fcc-FS-PRO.jpg" alt="fcc-FS-PRO" width="300"/>
  </a>
</p>

<h1 align="center">FlyCamCzech FOCUSpilot</h1>
<p align="center"><em>(Born4Flight | FlyCamCzech | Jakub Ešpandr)</em></p>

## Overview
The FlyCamCzech FOCUSpilot is an Arduino-based system designed for automated focus stacking photography. This device controls a stepper motor to move a camera lens through different focus positions, automatically triggering the camera shutter at each position to capture a series of images for focus stacking. Perfect for macro and close-up photography where extended depth of field is required.

---

## ✨ Features

- **Automated Focus Stacking**: Automatically moves through focus positions and triggers camera capture
- **Professional Preset System**: Dedicated modes for Macro Rail and Microscope applications
- **Enhanced Algorithms**: Mode-specific focus stacking algorithms optimized for different applications
- **AutoSave Functionality**: Automatic saving of settings when enabled for seamless operation
- **Mirror Lock Support**: Vibration-free photography with configurable mirror lock timing
- **Auto Return Function**: Automatic return to start position after stacking completion
- **Manual Control**: Manual forward/backward movement with position memory
- **Dynamic Motor Control**: Automatic speed adjustment based on depth of field and stepping mode
- **Configurable Parameters**: Adjustable depth of field (DoF), step size, timing, and trigger settings
- **LCD Interface**: 16x2 LCD display with rotary encoder navigation
- **Keypad Control**: 4-button keypad for quick manual operations
- **EEPROM Storage**: Settings are saved to non-volatile memory with separate preset storage
- **Bidirectional Operation**: Support for both forward and backward focus stacking sequences
- **Emergency Stop**: Quick stop functionality using encoder button
- **Progress Monitoring**: Real-time display with remaining time estimation
- **HiRes Mode**: High-resolution stepping (1/32 microstepping) for precise positioning
- **Direct Preset Selection**: Quick access to Microscope and Rail presets
- **Smart Unit Display**: Automatic unit display (nm/μm) based on selected mode
- **Relay Test**: Automatic camera trigger relay verification on startup

---

## 📦 Requirements

### Hardware Components
- Arduino board (compatible with Arduino IDE)
- 16x2 LCD display with I2C interface (address 0x27)
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
3. **Upload Code**: Upload the `fccFS-PRO_1.1.6a.ino` file to your Arduino board
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
- **testSHOT**: Trigger camera shutter manually
- **RUN**: Automated focus stacking sequences
  - RUN Forward: Execute forward focus stacking with settings
  - RUN Backward: Execute backward focus stacking with settings
  - Delete TRACE: Clear saved positions
- **GO position**: Manual movement commands
  - BACK pos.: Move to start position
  - FRONT pos.: Move to end position
  - Delete TRACE: Clear saved positions
- **PREFERENCES**: Configure basic parameters
  - HiRes.: Set high-resolution stepping (0=OFF/1=ON)
  - um/step: Set depth of field per step
  - time shutter: Set shutter timing
  - time vibr.: Set vibration delay
  - mirrorLock: Enable/disable mirror lock function
  - time (mirror): Set mirror lock timing
  - SAVE: Save current settings
- **SETTINGS**: Advanced configuration
  - AutoReturn: Enable/disable auto return after stacking
  - time (trigg): Set trigger timing
  - motor speed: Set motor speed
  - added length: Set length compensation
  - BOOT screen: Enable/disable boot screen
  - PRESET a.LOAD: Choose autoload preset
    - Microscope: Load microscope preset
    - Rail: Load rail preset
  - AutoSAVE: Enable/disable automatic settings saving
  - SAVE: Save current settings
- **Rail/Microscope**: Mode selection
  - Rail: Switch to macro rail mode
  - Microscope: Switch to microscope mode
  - PRESET a.LOAD: Choose autoload preset
- **SAVE/RESTORE**: Preset management
  - SAVE PRESET: Save current configuration
  - LOAD DEFAULTS: Restore factory settings
    - Rail: Load rail default settings
    - Microscope: Load microscope default settings
- **VERSION**: System information
  - model: fccFS2 PRO
  - by FlyCamCzech
  - version 1.1.6a
  - 23. Aug. 2023

### Configuration Parameters

#### Step Size (Microstepping)
- 0 = Normal mode (1/16 step)
- 1 = HiRes mode (1/32 step) for high precision

#### Depth of Field (DoF)
- **Rail Mode**: 20-1500μm/step (normal), 10-652μm/step (HiRes)
- **Microscope Mode**: 10-1800nm/step (normal), 5-1000nm/step (HiRes)

#### Timing Settings
- **Shutter timing**: 1-20 seconds delay between captures
- **Vibration delay**: 1-20 seconds delay between movement and capture
- **Trigger timing**: 250-2000μs trigger pulse length
- **Mirror timing**: 250-5000μs mirror lock delay

#### Motor Settings
- **Motor speed**: 100-1000ms per step (increased range)
- **Added length**: 10-100μm compensation for backlash

#### System Settings
- **Mirror Lock**: Enable/disable for vibration-free photography
- **Auto Return**: Enable/disable automatic return to start position
- **Boot screen**: Enable/disable startup screen
- **Preset autoload**: Choose default mode (Microscope/Rail)
- **AutoSave**: Enable/disable automatic settings saving

---

## 📁 Project Structure

```
FOCUSpilot/
├── fccFS-PRO_1.1.6a/
│   └── fccFS-PRO_1.1.6a.ino    # Main Arduino sketch (current version)
├── fccFS-PRO_1.1.6a.cpp        # Implementation file
├── fccFS-PRO_1.1.6a.h          # Header file
├── libraries/                  # Required Arduino libraries
│   ├── EEPROM/
│   ├── EEPROMTyped-1.0.0/
│   ├── Keypad/
│   ├── LiquidCrystal_I2C_Menu-master/
│   └── Wire/
├── README.md                   # This file
└── CHANGELOG.md               # Version history
```

---

## 🔧 Focus Stacking Workflow

1. **Mode Selection**: Choose between Rail or Microscope mode based on your application
2. **Setup**: Configure step size, DoF, timing, and trigger settings in PREFERENCES/SETTINGS
3. **AutoSave Configuration**: Enable AutoSave for automatic settings persistence
4. **Positioning**: Use manual controls to set start and end positions
   - Move to desired start position and press button 2
   - Move to desired end position and press button 4
5. **Execution**: Choose RUN Forward or RUN Backward from the menu
6. **Monitoring**: The LCD displays progress with remaining time estimation
7. **Completion**: System returns to start position if Auto Return is enabled

---

## ⚙️ Default Settings

### Rail Mode (Macro Photography)
- Step Size: Normal (1/16)
- DoF: 250μm/step
- Shutter timing: 2s
- Vibration delay: 2s
- Trigger timing: 500μs
- Mirror timing: 750μs
- Mirror Lock: OFF
- Auto Return: ON
- Motor speed: 500ms
- Added length: 30μm
- AutoSave: OFF

### Microscope Mode
- Step Size: HiRes (1/32) - enabled by default
- DoF: 20nm/step
- Shutter timing: 3s (increased from 2s)
- Vibration delay: 2s
- Trigger timing: 500μs
- Mirror timing: 750μs
- Mirror Lock: OFF
- Auto Return: ON
- Motor speed: 500ms
- Added length: 50μm (increased from 30μm)
- AutoSave: OFF

---

## 🔐 License

This project is licensed under the **Non-Commercial Public License (NCPL v1.0)**  
© 2025 Jakub Ešpandr - Born4FLight, FlyCamCzech

See the [LICENSE](https://github.com/Jakub-Espandr/FOCUSpilot/raw/main/LICENSE) file for full terms.

---

## 🙏 Acknowledgments

- Built with ❤️ using Arduino and open-source libraries
- For support and updates, visit: [WWW.FlyCamCzech.EU/FOCUSpilot](https://www.flycamczech.eu/en/focuspilot/)