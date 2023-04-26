# Changelog

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