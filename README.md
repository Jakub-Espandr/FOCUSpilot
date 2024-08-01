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
| Camera Trigger | 10 | Relay trigger |

---

## 🚀 Installation

1. **Install Libraries**: Install the required Arduino libraries through the Library Manager
2. **Connect Hardware**: Wire the components according to the pin diagram above
3. **Upload Code**: Upload the `fccFS-PRO_1.2.0.ino` file to your Arduino board
4. **Power On**: Connect power and initialize the system

---

## 🎯 **NEW IN VERSION 1.2.0**

### **Revolutionary Objective Preset System**
- **15 Pre-configured Objectives** across 3 carousels:
  - **Carousel 1 (small)**: 2.5x, 3.2x, 6.3x, 10x, 16x
  - **Carousel 2 (apo)**: 6.3x, 16x, 40x, 63x, 100x  
  - **Carousel 3 (GF)**: 10x, 20x, 40x, 63x, 100x
- **One-Click Loading** - Instant preset selection with pre-configured DOF values
- **Individual EEPROM Storage** - Each objective has dedicated memory location

### **Advanced Photo-Based Stacking**
- **Photo Count Mode** - Set number of photos instead of distance
- **Dynamic Step Calculation** - Automatically calculates optimal step size
- **Enhanced Progress Display** - Real-time photo count and time remaining

### **Enhanced Testing Capabilities**
- **Dedicated Test Menu** - Comprehensive testing options
- **Motor Movement Testing** - Verify forward/backward operation
- **Camera Trigger Testing** - Test shutter release functionality

### **Product Specialization**
- **Microscope-Only Platform** - Optimized specifically for microscope workflows
- **Streamlined Interface** - Reduced complexity for specialized use cases

---

## 📋 Features

### **Core Functionality**
- **Automated Focus Stacking** - Precise motor control for focus stacking sequences
- **Dual DOF System** - Small and big increment options for different workflows
- **Auto Return** - Automatic return to start position after stacking
- **Mirror Lock** - Camera mirror lock functionality for vibration reduction
- **Auto Save** - Automatic EEPROM saving of settings

### **Advanced Controls**
- **Rotary Encoder** - Intuitive menu navigation and value adjustment
- **Keypad Input** - Quick access to functions and numeric input
- **LCD Display** - Real-time status and menu information
- **Stepper Motor Control** - Precise microstep positioning

### **Configuration Options**
- **Motor Speed** - Adjustable from 5-1000ms for optimal performance
- **Microstep Settings** - Configurable MS1, MS2, MS3 for precision
- **DOF Ranges** - Extended ranges for both small and big increments
- **Objective Presets** - 15 pre-configured microscope objectives

---

## 🎛️ Menu System

### **Main Menu Structure**
- **testSHOT**: Trigger camera shutter manually
- **RUN**: Automated focus stacking sequences
  - **RUN Forward**: Execute forward focus stacking
    - **um/step small**: Set precise depth of field for detailed work
    - **um/step big**: Set extended depth of field for faster stacking
    - **Photos**: Set number of photos for photo-based stacking
    - **RUN by Photos**: Execute stacking based on photo count
    - **RUN dist**: Execute stacking based on distance
  - **RUN Backward**: Execute backward focus stacking
    - **um/step small**: Set precise depth of field
    - **um/step big**: Set extended depth of field
    - **RUN**: Execute backward stacking
- **PREFERENCES**: System configuration
  - **SET Objectives**: Configure objective presets
    - **Carousel 1 (small)**: 2.5x, 3.2x, 6.3x, 10x, 16x
    - **Carousel 2 (apo)**: 6.3x, 16x, 40x, 63x, 100x
    - **Carousel 3 (GF)**: 10x, 20x, 40x, 63x, 100x
  - **Presets**: Quick objective selection
  - **Motor Speed**: Adjust motor timing (5-1000ms)
  - **Microsteps**: Configure MS1, MS2, MS3 settings
  - **Auto Return**: Enable/disable automatic return
  - **Mirror Lock**: Enable/disable mirror lock
  - **Auto Save**: Enable/disable automatic saving
- **TEST**: System testing and validation
  - **testStep**: Test motor movement
  - **testSHOT**: Test camera trigger

---

## 🔧 Configuration Parameters

### **Motor Settings**
- **Speed Range**: 5-1000ms (extended from previous versions)
- **Microstep Configuration**: MS1, MS2, MS3 for precision control
- **Direction Control**: Forward/backward movement

### **DOF Ranges (Small Increment)**
- **Microscope Normal**: 10-1800nm
- **Microscope HiRes**: 5-1000nm

### **DOF Ranges (Big Increment)**
- **Microscope Normal**: 10-3000nm
- **Microscope HiRes**: 5-2000nm

### **Objective Presets**
- **15 Pre-configured objectives** with optimized DOF values
- **Individual EEPROM storage** for each objective
- **Quick selection** from 3 carousels

---

## 📊 System Information

- **VERSION**: System information
  - model: fccFS2 PRO
  - by FlyCamCzech
  - version 1.2.0
  - 01. Aug. 2024

---

## 📁 Project Structure

```
FOCUSpilot/
├── fccFS-PRO_1.2.0/
│   └── fccFS-PRO_1.2.0.ino    # Main Arduino sketch (current version)
├── fccFS-PRO_1.2.0.cpp        # Implementation file
├── fccFS-PRO_1.2.0.h          # Header file
├── libraries/                  # Required Arduino libraries
│   ├── EEPROM/                # EEPROM management
│   ├── EEPROMTyped-1.0.0/     # Typed EEPROM operations
│   ├── Keypad/                # Keypad input handling
│   ├── LiquidCrystal_I2C_Menu-master/  # LCD menu system
│   └── Wire/                  # I2C communication
├── CHANGELOG.md               # Version history and changes
└── README.md                  # This documentation
```

---

## 🔄 Workflow

### **Basic Focus Stacking**
1. **Select Objective**: Choose from 15 pre-configured objectives
2. **Set Parameters**: Configure DOF, speed, and microstep settings
3. **Position**: Move to start position using manual controls
4. **Execute**: Run automated focus stacking sequence
5. **Return**: Automatic return to start position (if enabled)

### **Photo-Based Stacking**
1. **Set Photo Count**: Specify number of photos desired
2. **Calculate Steps**: System automatically determines optimal step size
3. **Execute**: Run stacking based on photo count
4. **Monitor Progress**: Real-time display of photos taken and remaining

### **Objective Configuration**
1. **Access SET Objectives**: Navigate to objective configuration menu
2. **Select Carousel**: Choose from 3 carousels (small, apo, GF)
3. **Configure DOF**: Set optimal depth of field for each objective
4. **Save Settings**: Automatic EEPROM storage of configurations

---

## 🛠️ Technical Specifications

### **Hardware Requirements**
- **Arduino Board**: Compatible with Arduino Mega or similar
- **Display**: I2C LCD display (16x2 or 20x4)
- **Motor**: Stepper motor with driver
- **Input**: Rotary encoder and 4x4 keypad
- **Trigger**: Relay for camera shutter control

### **Software Requirements**
- **Arduino IDE**: Version 1.8.x or newer
- **Libraries**: EEPROM, Keypad, LiquidCrystal_I2C_Menu, Wire
- **Memory**: ~60KB program space, ~2KB RAM
- **EEPROM**: 60+ addresses for comprehensive storage

### **Performance**
- **Motor Speed**: 5-1000ms step timing
- **Precision**: Microstep control (MS1, MS2, MS3)
- **Storage**: 15 objective presets with individual EEPROM storage
- **Menu System**: 80+ menu options with hierarchical navigation

---

## 🔐 License

This project is licensed under the **Non-Commercial Public License (NCPL v1.0)**  
© 2025 Jakub Ešpandr - Born4FLight, FlyCamCzech

See the [LICENSE](https://github.com/Jakub-Espandr/FOCUSpilot/raw/main/LICENSE) file for full terms.

---

## 🙏 Acknowledgments

- Built with ❤️ using Arduino and open-source libraries
- For support and updates, visit: [WWW.FlyCamCzech.EU/FOCUSpilot](https://www.flycamczech.eu/en/focuspilot/)