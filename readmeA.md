# Paintbots - Part A
## Configuration and Board Square Classes

This is Part A of the Paintbots game project, focusing on setting up the basic simulation infrastructure including board square classes and configuration file parsing.

---

## Usage

### Building the Project

To compile all files:
```bash
make
```

To build and run the configuration tests:
```bash
make testconfig
./testconfig
```

To clean build artifacts:
```bash
make clean
```

### Configuration Files

Create a configuration file (e.g., `game.config`) with the following format:

```
# Configuration file for Paintbots
# Lines starting with # are comments
# Blank lines are ignored

HIT_DURATION = 28
PAINTBLOB_LIMIT = 30
ROCK_LOWER_BOUND = 10
ROCK_UPPER_BOUND = 12
FOG_LOWER_BOUND = 2
FOG_UPPER_BOUND = 8
LONG_RANGE_LIMIT = 10
```

**Valid Configuration Keywords:**
- `HIT_DURATION` - Number of moves a robot paints opponent's color after being hit (default: 20)
- `PAINTBLOB_LIMIT` - Maximum paint blobs a robot can fire (default: 30)
- `ROCK_LOWER_BOUND` - Minimum rocks on board (default: 10)
- `ROCK_UPPER_BOUND` - Maximum rocks on board (default: 20)
- `FOG_LOWER_BOUND` - Minimum fog squares (default: 5)
- `FOG_UPPER_BOUND` - Maximum fog squares (default: 10)
- `LONG_RANGE_LIMIT` - Number of long-range scans available (default: 30)

**Notes:**
- Keywords are case-insensitive (`HIT_DURATION` = `hit_duration`)
- Order doesn't matter
- Missing parameters use default values
- Invalid files will throw an error

### Using the Config Class

```cpp
#include "Config.h"

// Use default configuration
Config defaultConfig;
int hitDuration = defaultConfig.getHitDuration();

// Load from file
Config customConfig("path/to/game.config");
int paintLimit = customConfig.getPaintBlobLimit();
int rockMin = customConfig.getRockLowerBound();
int rockMax = customConfig.getRockUpperBound();
```

### Testing

Run the configuration unit tests to verify your implementation:
```bash
./testconfig
```

Expected output format:
```
Config class unit tests
Checking default constructor: PASS
Checking bad file name: PASS
Checking valid config file, all parameters: PASS
Checking valid config file, missing parameters: PASS
...
```

---

## Project Structure

```
coms3270P2/
├── BoardSquare.h
├── BoardSquare.cpp
├── InternalBoardSquare.h
├── InternalBoardSquare.cpp
├── ExternalBoardSquare.h
├── ExternalBoardSquare.cpp
├── SquareInfo.h
├── RobotInfo.h
├── Config.h
├── Config.cpp
├── ConfigTest.cpp
├── Makefile
└── README.md
```

## Component Overview

### BoardSquare Hierarchy

**BoardSquare** (Abstract Base Class)
- Pure virtual methods for getting square and robot information
- Base class for both internal and external board representations

**InternalBoardSquare**
- Used by the simulation engine
- Contains full game state information
- May include extra data not exposed to robot players

**ExternalBoardSquare**
- Given to robot agents for decision-making
- Contains only information robots are allowed to see
- Constructor accepts `InternalBoardSquare` and copies safe data

### Information Structures

**SquareInfo**
- `PBSquareContent content` - EMPTY, ROCK, FOG, ROCKFOG, or WALL
- `PBColor color` - RED, BLUE, or WHITE
- `RobotInfo robot` - Information about robot (if present)

**RobotInfo**
- `bool robotPresent` - Whether a robot occupies the square
- `PBColor color` - The robot's color
- `PBDirection direction` - NORTH, EAST, SOUTH, or WEST

### Config Class

Parses and stores game configuration parameters. Supports both default values and file-based configuration.

**Key Methods:**
- `Config()` - Default constructor with standard values
- `Config(string filepath)` - Load from configuration file
- `getHitDuration()` - Get hit effect duration
- `getPaintBlobLimit()` - Get maximum paint blobs
- `getRockLowerBound()` / `getRockUpperBound()` - Rock count bounds
- `getFogLowerBound()` / `getFogUpperBound()` - Fog count bounds
- `getLongRangeLimit()` - Get long-range scan limit

---

## Testing Requirements

The `ConfigTest.cpp` file must include **at least 10 unique tests** covering:

1. Default constructor functionality
2. Invalid file paths
3. Valid configuration files with all parameters
4. Valid configuration files with missing parameters
5. Invalid file formats (missing equals signs, invalid syntax)
6. Case-insensitive keyword handling
7. Comment line handling
8. Blank line handling
9. Out-of-order parameters
10. Boundary value testing

Each test should output PASS or FAIL with descriptive messages.

---

## Design Considerations

### Information Hiding
The `ExternalBoardSquare` should **never** expose information that could give a robot agent an unfair advantage. Internal simulation data (like opponent's exact strategy or hidden state) should remain in `InternalBoardSquare`.

### Protected Members
Consider using protected member variables in the `BoardSquare` base class to avoid code duplication between `InternalBoardSquare` and `ExternalBoardSquare`.

### Error Handling
The `Config` constructor should throw appropriate exceptions for:
- Non-existent files
- Malformed configuration syntax
- Invalid parameter values

---

## Git Repository

This project uses Git for version control:

**Repository:** `git.las.iastate.edu/coms3270P2`

### Tagging Part A Completion
```bash
git add .
git commit -m "Complete Part A implementation"
git tag partacomplete
git push origin main --tags
```

**Important:** Consider creating a branch for Part A before starting Part B, as Part B involves significant refactoring:
```bash
git checkout -b part-a
git push origin part-a
```

---

## Game Rules Summary (Context)

Paintbots is played on a 15×15 grid where two robots compete to paint more squares their color. Key mechanics:
- Robots move simultaneously each turn
- Moving leaves a painted square behind
- Robots can fire paint blobs to temporarily change opponent's paint color
- Game lasts 300 moves
- Obstacles include rocks (impassable) and fog (limits vision)
- Collision with walls, rocks, or opponent ends the game

Part A builds the foundation for representing board state and game configuration.

---

## Next Steps

After completing Part A:
1. Tag your repository with `partacomplete`
2. Ensure all tests pass
3. Verify documentation is complete
4. Create a Part A branch before starting Part B
5. Part B will involve significant refactoring with design patterns

---

## Compilation Notes

- Use C++11 or later
- Ensure all virtual methods are properly overridden
- Test with various configuration files
- Verify memory management (no leaks)
- All const methods should be marked appropriately

---

## Support

For questions about the assignment, refer to:
- Project specification document
- Course materials on class design and inheritance
- C++ documentation for string parsing and file I/O

---

## AI Usage:

```
1. ChatGPT 
prompt:
    attachment - Documentation/Project2.pdf
    "assuming i have completed part A, make me a boilerplate Readme.md file for part A"
