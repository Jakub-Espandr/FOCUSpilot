# Changelog

## [1.1.5] - 2023-06-16

### Added
- **Enhanced focus stacking algorithms** with mode-specific optimizations
- **Direct preset selection** with dedicated Microscope and Rail menu options
- **Improved unit display** showing correct units (nm/μm) based on selected mode
- **Extended DOF ranges** for more precise control
  - Microscope Normal: 10-1800nm (increased from 10-1000nm)
  - Microscope HiRes: 5-1000nm (increased from 5-500nm)
- **First run detection** system for improved initialization

### Changed
- **Data type improvements** - changed bootVal and presetVal from bool to int for better stability
- **Menu reorganization** - moved mirrorLock setting from SETTINGS to PREFERENCES for easier access
- **Motor control optimization** - improved speed logic for different stepping modes
  - Normal mode: motorSpeed * 2
  - HiRes mode: motorSpeed
- **RUN Backward algorithm** completely rewritten with mode-specific optimizations
  - Microscope mode: 0.75x DOF multiplier for finer control
  - Rail mode: 0.8x DOF multiplier for optimal performance
- **RUN Forward algorithm** enhanced with preset-aware calculations
- **Default settings optimization** for Microscope mode
  - HiRes enabled by default (stepsI = 1)
  - Increased timing to 3s (from 2s)
  - Increased added length to 50μm (from 30μm)

### Technical Improvements
- **Mode-specific focus stacking** with different algorithms for Microscope and Rail applications
- **Improved EEPROM handling** with more robust data type management
- **Better motor speed control** with automatic adjustment based on stepping mode
- **Enhanced preset system** with direct selection options
- **Optimized memory usage** with more efficient variable management

### Bug Fixes
- **Fixed unit display** - now correctly shows nm for Microscope and μm for Rail mode
- **Improved preset loading** with better error handling
- **Enhanced motor positioning** with more accurate backlash compensation

---

## [1.1.3] - 2023-04-26

### Added
- **Professional preset system** with Microscope and Rail modes
- **Mirror Lock functionality** for vibration-free photography
- **Auto Return feature** - automatic return to start position after stacking
- **Dynamic motor speed control** with automatic adjustment based on DOF
- **Extended menu system** with PREFERENCES, SETTINGS, and SAVE/RESTORE sections
- **Boot screen configuration** - enable/disable startup screen
- **Enhanced EEPROM management** with 16-bit read/write functions
- **Progress display** with remaining time estimation during stacking
- **Emergency stop functionality** with encoder button
- **Added length compensation** for precise positioning
- **Trigger timing configuration** for different camera types
- **Mirror vibration delay settings** for optimal image quality
- **HiRes mode** for high-resolution stepping (1/32 microstepping)
- **Version display** in boot screen and info menu

### Changed
- **Motor control** now uses dynamic timing instead of fixed 1400μs delays
- **Capture function** enhanced with Mirror Lock support and configurable timing
- **Menu structure** completely redesigned with hierarchical navigation
- **Display information** expanded to show mode (M/R), Mirror Lock status (N/L), and HiRes status
- **Branding** updated to "fccFS2 PRO" with version information

### Technical Improvements
- **Enhanced EEPROM storage** with separate memory areas for different presets
- **Improved motor positioning** with automatic compensation for backlash
- **Better timing control** with separate delays for movement, trigger, and mirror lock
- **Robust error handling** with emergency stop and recovery mechanisms
- **Optimized memory usage** with better variable management

### Default Settings
- **Rail Mode**: 250μm/step, 2s timing, Mirror Lock off, Auto Return on
- **Microscope Mode**: 20nm/step, 2s timing, Mirror Lock off, Auto Return on

---

## [1.0.0] - 2022-04-16

### Added
- Initial release
- Core focus stacking functionality
- Manual and automated operation modes
- LCD menu system with rotary encoder navigation
- 4-button keypad control for manual positioning
- EEPROM settings storage and persistence
- Camera trigger relay control
- Bidirectional focus stacking (forward/backward)
- Configurable parameters (DoF, step size, timing)
- Position memory system (start/end points)
- Progress display during automated sequences
- Settings menu with save/restore functionality