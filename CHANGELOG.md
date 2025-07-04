# Changelog

## [1.2.0] - 2024-08-01

### **MAJOR ARCHITECTURAL OVERHAUL**
- **Complete code restructuring** - Implemented C++ structures for better organization
  - `struct Settings`: Groups all configuration variables (15 variables)
  - `struct Status`: Groups all runtime state variables (6 variables)
  - `uint8_t stepSettings`: Bit-packed boolean variables for microstep settings
- **Memory optimization** - PROGMEM usage, structured data, reduced variable count
- **Hardware compatibility** - Relay trigger pin changed from 11 to 10
- **Keypad layout** - Changed from `{ '4', '3', '2', '1' }` to `{ '1', '2', '3', '4' }` (standard order)

### **REVOLUTIONARY OBJECTIVE PRESET SYSTEM**
- **3 Carousels with 5 objectives each** (15 total presets):
  - **Carousel 1 (small)**: 2.5x, 3.2x, 6.3x, 10x, 16x
  - **Carousel 2 (apo)**: 6.3x, 16x, 40x, 63x, 100x  
  - **Carousel 3 (GF)**: 10x, 20x, 40x, 63x, 100x
- **Individual EEPROM storage** - Each objective has dedicated EEPROM address
- **Quick preset selection** - Instant loading of pre-configured DOF values
- **Objective management menu** - Separate "SET Objectives" section for configuration

### **ADVANCED PHOTO-BASED STACKING**
- **`goRunByPhotoCount()` function** - Revolutionary stacking based on photo count
- **Dynamic step calculation** - `localDOF = motorState / numPhotos`
- **Photo count validation** - Prevents execution without setting photo count
- **Enhanced progress display** - Shows photo count and remaining time
- **New menu options** - "Photos" and "RUN by Photos" menu items

### **ENHANCED TESTING CAPABILITIES**
- **New Test menu** - Dedicated testing section
- **`testStep()` function** - Tests forward and backward movement
- **Step validation** - Verifies motor movement and microstep settings
- **testSHOT** - Camera trigger testing functionality

### **EEPROM MANAGEMENT OVERHAUL**
- **Organized address mapping** - Clear address definitions for all variables
- **Dedicated objective storage** - 30 EEPROM addresses for objective presets
- **Photo count storage** - New EEPROM address for photo-based stacking
- **Structured loading/saving** - Systematic EEPROM operations

### **MENU SYSTEM REDESIGN**
- **Hierarchical structure** - 3-level deep menu system
- **Objective management** - Separate "SET Objectives" and "Presets" sections
- **Photo-based options** - New menu items for photo count and execution
- **Improved navigation** - Better back navigation and menu organization

### **PERFORMANCE ENHANCEMENTS**
- **Optimized motor control** - Improved step timing and movement
- **Faster boot process** - Streamlined initialization
- **Better LCD updates** - More efficient display refresh
- **Reduced memory fragmentation** - Structured data prevents memory issues

### **PRODUCT SPECIALIZATION**
- **CRITICAL CHANGE: Rail mode completely removed**
  - No more Rail/Microscope switching functionality
  - Removed `SetModeFS()`, `SetPresetR()`, `SetDefaultsFS()` functions
  - Eliminated all rail-specific EEPROM addresses and menu items
  - `presetVal` hardcoded to 0 (microscope only)
  - **Transformed from universal focus stacking controller to specialized microscope automation platform**

### **CODE QUALITY IMPROVEMENTS**
- **Function prototypes** - All functions properly declared
- **Consistent naming** - Standardized function and variable names
- **Error handling** - Validation for photo count and objective selection
- **Documentation** - Comprehensive comments and code organization

### **TECHNICAL SPECIFICATIONS**
- **Version**: 1.2.0 (August 2024)
- **Code size**: Significantly reduced through optimization
- **Memory usage**: More efficient through structured programming
- **EEPROM usage**: 60+ addresses for comprehensive storage
- **Menu items**: 80+ menu options with hierarchical structure

### **USER EXPERIENCE IMPROVEMENTS**
- **Quick objective selection** - One-click preset loading
- **Photo-based workflow** - Intuitive photo count setting
- **Enhanced feedback** - Better progress and status display
- **Simplified operation** - Reduced menu complexity for common tasks
- **Specialized interface** - Optimized specifically for microscope workflows

---

## [1.1.8b] - 2024-03-06

### Changed
- **Hardware compatibility** - relay trigger pin reverted from 10 to 11
- **Version numbering** - updated from 1.1.7 to 1.1.8b
- **Release date** - updated to 06. Mar. 2024
- **Motor speed range** - extended from 50-1500ms to 5-1000ms for faster operation

### Removed
- **Boot screen functionality** - completely disabled boot screen system
  - Removed `bootVal` variable and all related functionality
  - Disabled boot screen display in setup()
  - Removed boot screen menu option and settings
- **Added length setting** - simplified menu structure by removing "added length" option
  - Removed `menuAdded` from enum and menu system
  - Commented out `SetAdded()` function

### Enhanced
- **Shot-based stacking** - improved variable naming and functionality
  - Renamed `shots` to `noShots` for better clarity
  - Enhanced `goRunF_shots()` and `goRunB_shots()` functions
  - Improved progress display showing "no. shots"
  - Extended shot range to 0-200 with default 10
- **Code optimization** - streamlined codebase and reduced memory usage
  - Removed unused variables and functions
  - Simplified EEPROM operations
  - Cleaner code structure with commented-out sections removed

### Technical Improvements
- **Faster startup** - reduced initialization time by removing boot screen
- **Simplified interface** - streamlined menu navigation and settings
- **Better performance** - optimized motor control with extended speed range
- **Memory optimization** - reduced RAM usage by removing unused features

---

## [1.1.7] - 2024-01-22

### Added
- **Forward Shots Mode** - new shot-based stacking for forward direction
  - **New function** `goRunF_shots()` for forward shot-based stacking
  - **Menu integration** - "RUN shots" option added to RUN Forward settings
  - **Shot-based workflow** - allows setting number of shots instead of distance for forward stacking
  - **Auto-return support** - includes automatic return to start position when enabled
  - **Progress tracking** - real-time display of shot count and remaining time

### Changed
- **Hardware compatibility** - relay trigger pin reverted from 11 to 10
- **Version numbering** - updated from 1.1.7b to 1.1.7
- **Release date** - updated to 22. Jan. 2024

### Enhanced Features
- **Dual stacking modes** - users can now choose between distance-based and shot-based stacking for forward direction
- **Consistent interface** - shot-based mode follows same pattern as existing backward shot mode
- **Menu structure** - enhanced RUN Forward menu with both distance and shot options

### Technical Improvements
- **Workflow flexibility** - supports both traditional distance-based and new shot-based workflows
- **Error handling** - includes emergency stop functionality in new shot mode
- **Progress display** - enhanced user feedback during shot-based stacking operations

---

## [1.1.7b] - 2024-01-03

### Added
- **Dual DOF Setting System** - separate "small" and "big" increment options for different workflow needs
  - **Small increment** (`SetDOF`): Original precision ranges for detailed work
  - **Big increment** (`SetDOF2`): Extended ranges for faster stacking operations
- **Enhanced menu structure** with `menuDOF2` enum and dual DOF options
- **Extended DOF ranges** for big increment mode:
  - Microscope Normal: 10-3000nm (increased from 10-1800nm)
  - Microscope HiRes: 5-2000nm (increased from 5-1000nm)
  - Rail Normal: 20-2000μm (increased from 20-1500μm)
  - Rail HiRes: 10-1500μm (increased from 10-652μm)

### Changed
- **Hardware compatibility** - relay trigger pin reverted from 10 to 11
- **Menu navigation** - added dual DOF options in multiple menu sections:
  - RUN Forward settings: "um/step small" and "um/step big"
  - RUN Backward settings: "um/step small" and "um/step big"
  - PREFERENCES: "um/step small" and "um/step big"

### Enhanced
- **User interface** - improved DOF setting screens with descriptive labels
  - Small increment: displays "small increment" on second line
  - Big increment: displays "big increment" on second line
- **Workflow flexibility** - users can now choose between precision and speed modes
- **User experience** - more intuitive menu navigation with clear options

### Technical Improvements
- **Flexible DOF configuration** - supports both detailed macro work and faster overview stacking
- **Better workflow options** - quick setup for different types of focus stacking projects
- **Enhanced user guidance** - clear distinction between precision and speed modes

### Technical Improvements
- **Flexible DOF configuration** - supports both detailed macro work and faster overview stacking
- **Better workflow options** - quick setup for different types of focus stacking projects
- **Enhanced user guidance** - clear distinction between precision and speed modes

---

## [1.1.6] - 2023-11-13

### Changed
- **Version numbering** - updated from 1.1.6b to 1.1.6
- **Release date** - updated to 13. Nov. 2023

### Reverted
- **Code optimization reverted** - restored unused variables that were removed in 1.1.6b:
  - Restored `localAdded` variable (was removed in 1.1.6b)
  - Restored `percentage` variable (was removed in 1.1.6b)
  - Restored `newStepsI` variable (was removed in 1.1.6b)
  - Restored `delayTimingTrigger` variable (was removed in 1.1.6b)

### Technical Changes
- **Increased memory usage** - restored variables consume additional RAM
- **Code maintainability** - reverted to less optimized code structure
- **No functional changes** - all features remain identical to 1.1.6b

---

## [1.1.6b] - 2023-09-07

### Changed
- **Hardware compatibility update** - relay trigger pin changed from 11 to 10
- **Code optimization** - removed unused variables for cleaner codebase
  - Removed `localAdded` variable (was declared but never used)
  - Removed `percentage` variable (was declared but never used)
  - Removed `newStepsI` variable (was declared but never used)
  - Removed `delayTimingTrigger` variable (was declared but never used)

### Technical Improvements
- **Memory optimization** - reduced RAM usage by removing unused variables
- **Code cleanup** - streamlined variable declarations for better maintainability
- **Hardware compatibility** - updated pin assignment for relay trigger

### Bug Fixes
- **Fixed hardware compatibility** - corrected relay trigger pin assignment
- **Improved code efficiency** - removed unused variables that were consuming memory

---

## [1.1.6a] - 2023-08-23

### Added
- **AutoSave functionality** - automatic saving of settings when enabled
- **Enhanced initialization display** - shows mode and AutoSave status on startup
- **Relay test on startup** - verifies camera trigger relay functionality
- **Unified focus stacking algorithms** - consistent logic for Forward and Backward operations
- **Improved user interface** - better status information display

### Changed
- **Boot screen timing** - increased delay from 300ms to 350ms for better readability
- **RUN Forward algorithm** - now uses same optimization factors as RUN Backward
  - Microscope mode: 0.75x DOF multiplier for both Normal and HiRes
  - Rail mode: 0.8x DOF multiplier for both Normal and HiRes
- **Settings management** - automatic EEPROM writing when AutoSave is enabled
- **Menu structure** - added AutoSave option in SETTINGS menu
- **EEPROM storage** - added autoSave setting storage at address 66

### Technical Improvements
- **Automatic settings persistence** - changes are saved immediately when AutoSave is ON
- **Enhanced startup sequence** - better initialization with status display
- **Improved algorithm consistency** - unified approach for both stacking directions
- **Better error prevention** - relay test ensures proper camera trigger functionality
- **Optimized memory usage** - more efficient EEPROM management

### Bug Fixes
- **Fixed algorithm inconsistency** - Forward and Backward now use same optimization factors
- **Improved startup reliability** - relay test prevents camera trigger issues
- **Enhanced user feedback** - clearer status information during initialization

---

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