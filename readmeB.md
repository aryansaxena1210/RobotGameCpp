# Paintbots - Part B

## GameBoard Implementation with Design Patterns

Part B involves significant refactoring to create a complete GameBoard object using the Strategy and Singleton design patterns. This part builds the core game state management system.

---

## Usage

### Building the Project

To compile all files:

```bash
make
```

To build the test executable:

```bash
make testgameboard
```

To clean build artifacts:

```bash
make clean
```

### Running the TestGameBoard Program

The `testgameboard` executable requires a configuration file as the first command-line argument:

```bash
./testgameboard config.txt
```

**What it does:**

1. Loads the configuration file you specify
2. Creates a GameBoard instance with that configuration
3. Places a blue robot and a red robot on random empty squares
4. Displays the board in text format

**Example Output:**

```
W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W--
W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W--
W-- WR- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W--
W-- W-- W-- W-B W-- W-- W-- W-- R-- R-- R-- W-- W-- W-- W--
W-- W-- W-- B-- W-- W-- W-- W-- R-- W-- W-- W-- W-- W-- W--
W-- W-- W-- B-- W-- WR- W-- W-- W-R W-- W-- W-- W-- W-- W--
W-- W-- W-- B-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W--
W-- W-- W-- B-- W-- W-- W-- W-- W-- W-- W-- WF- W-- W-- W--
W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W--
W-- W-- W-- W-- W-- W-- WR- W-- W-- W-- W-- W-- WX- W-- W--
W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W--
W-- W-- W-- W-- WF- W-- W-- W-- W-- WF- W-- W-- W-- W-- W--
W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W--
W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W--
W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W-- W--
```

### Understanding the Board Display

Each square is represented by **three characters**:

**Format: `XYZ`**

- **X** = Square color: `W` (white), `B` (blue), `R` (red)
- **Y** = Obstruction: `R` (rock), `F` (fog), `X` (rock+fog), `-` (none)
- **Z** = Robot: `B` (blue robot), `R` (red robot), `-` (none)

**Examples:**

- `W--` = White square, no obstruction, no robot
- `WR-` = White square with a rock
- `WF-` = White square with fog
- `WX-` = White square with both rock and fog
- `B-B` = Blue square with blue robot
- `R-R` = Red square with red robot
- `W-B` = White square with blue robot

### Configuration File

Use the same configuration file format from Part A:

```
# Paintbots Configuration
HIT_DURATION = 28
PAINTBLOB_LIMIT = 30
ROCK_LOWER_BOUND = 10
ROCK_UPPER_BOUND = 12
FOG_LOWER_BOUND = 2
FOG_UPPER_BOUND = 8
LONG_RANGE_LIMIT = 10
```

The configuration determines:

- Number of rocks placed on the board (random between lower and upper bound)
- Number of fog squares (random between lower and upper bound)
- Other game parameters (not visible in this test, but used in Part C)

---

## Project Structure

```
coms3270P2/
├── Config.h
├── Config.cpp
├── Util.h
├── Util.cpp
├── Content.h
├── Content.cpp
├── Square.h
├── Square.cpp
├── GameBoard.h
├── GameBoard.cpp
├── TestGameBoard.cpp
├── Makefile
└── README.md
```

**Note:** All files from Part A (except Config.h and Config.cpp) have been deleted and replaced with the new architecture.

---

## Architecture Overview

### Design Patterns Used

**Strategy Pattern** - Used for square contents (Rock, Fog, SquareColor, Robot, Wall)

- Different content types implement a common `Content` interface
- Allows flexible composition of square properties
- Squares can have multiple content types simultaneously

**Singleton Pattern** - Used for GameBoard

- Only one GameBoard instance exists during execution
- Prevents multiple game states from conflicting
- Accessed via `getInstance()` with password protection

### Component Hierarchy

**Util Classes:**

- `Location` - Represents a position on the 15×15 board
- `Direction` - Represents cardinal directions with rotation methods
- `Color` - Enumeration for Red, Blue, White, None

**Content Classes (Strategy Pattern):**

- `Content` - Abstract base class
- `NotPresent` - Represents absence of content
- `RockContent` - Impassable obstacle
- `FogContent` - Obscures vision
- `WallContent` - Board boundary
- `SquareColorContent` - The painted color of a square
- `RobotContent` - Robot with color, direction, and paint color

**Square Classes:**

- `Square` - Base class with pointers to various content types
- `ObscuredSquare` - Square hidden by fog (no info visible)
- `AgentSquare` - Square information visible to robot agents
- `InternalSquare` - Full square information for game simulation

**GameBoard (Singleton):**

- Manages the 15×15 game board
- Handles robot movement and paint mechanics
- Tracks scores and game state
- Provides scanning capabilities (short-range and long-range)

---

## Key Differences from Part A

### Major Refactoring

1. **Deleted Files:** All BoardSquare classes from Part A are gone
2. **New Architecture:** Strategy pattern replaces simple inheritance
3. **Location System:** New `Location` and `Direction` structs with operator overloading
4. **Singleton GameBoard:** Centralized game state management
5. **Content Composition:** Squares now composed of multiple content types

### Why the Refactor?

- **Extensibility:** Easy to add new content types without modifying existing code
- **Flexibility:** Squares can have multiple properties (e.g., fog + rock)
- **Centralization:** Single GameBoard manages all game state
- **Type Safety:** Better separation between internal and external representations

---

## Testing Your Implementation

### Manual Testing

1. Create various configuration files with different parameters
2. Run `testgameboard` with each configuration
3. Verify board displays correctly with:
   - Correct number of rocks (within bounds)
   - Correct number of fog squares (within bounds)
   - Two robots placed on empty squares
   - Proper symbol representation

### What to Check

- **Board boundaries:** All squares should be within 15×15
- **Rock placement:** Count should match config bounds
- **Fog placement:** Count should match config bounds
- **Robot placement:** Both robots on empty (non-rock, non-fog) squares
- **No overlaps:** Robots should not start on the same square
- **Display format:** Three characters per square in correct format

---

## Git Repository Management

### Before Starting Part B

```bash
# Create a branch for Part A (IMPORTANT!)
git checkout -b part-a
git push origin part-a

# Tag Part A completion on the branch
git tag partacomplete
git push origin partacomplete

# Return to main for Part B
git checkout main
```

### After Completing Part B

```bash
git add .
git commit -m "Complete Part B implementation"
git tag partbcomplete
git push origin main --tags
```

**Critical:** Part B deletes most Part A files. Create a Part A branch BEFORE starting Part B to preserve your Part A submission!

---

## Common Issues and Solutions

### "Invalid password" error

- Make sure you're using the correct password (`"xyzzy"`) when calling `getInstance()`

### Compilation errors with static members

- Ensure all static `Content` instances are initialized in the `.cpp` file
- Static `Direction` constants must also be initialized in `Util.cpp`

### Robots placed on rocks or fog

- Check your random placement logic ensures empty squares only
- Verify rock and fog are placed BEFORE robots

### Board not displaying correctly

- Verify three-character format: `color + obstruction + robot`
- Check for off-by-one errors in loop indices
- Ensure proper spacing between squares

### Segmentation faults

- Check for null pointer dereferences with `Content` pointers
- Verify array bounds for the 15×15 board
- Ensure proper initialization of all pointers

---

## Moving to Part C

Part C will build on this foundation to:

- Implement complete game logic
- Create robot agents (AI players)
- Run full simulations with scoring
- Add win/loss conditions

The GameBoard you build in Part B is the core that Part C will use to run actual games.

---

## Design Considerations

### Singleton Password

The password (`"xyzzy"`) prevents accidental creation of multiple GameBoard instances. Only code that knows the password can access the singleton.

### Content Pointers

Squares use pointers to `Content` objects. Many squares can share the same static content instances (like `NotPresentInstance`) for memory efficiency.

### Agent vs Internal Squares

- `AgentSquare` - Only info robots are allowed to see
- `InternalSquare` - Full info for simulation, including hidden state

This separation prevents robots from "cheating" by accessing information they shouldn't have.

---

## Support Resources

- Part B specification in project document
- C++ references for operator overloading
- Design pattern documentation (Strategy, Singleton)
- Casting in C++: `static_cast` and `dynamic_cast`
- Previous Part A implementation (on your branch!)

---

## Submission Checklist

- [ ] All Part A files deleted (except Config.h/cpp)
- [ ] All new files compile without errors
- [ ] `testgameboard` executable runs successfully
- [ ] Board displays in correct format
- [ ] Rocks and fog within configured bounds
- [ ] Robots placed on valid empty squares
- [ ] Code is documented
- [ ] Makefile updated
- [ ] Part A tagged on separate branch
- [ ] Part B tagged as `partbcomplete`

## AI Usage:

```
1. ChatGPT
prompt:
    attachment - Documentation/Project2.pdf
    "assuming i have completed part B, make me a boilerplate Readme.md file for part A"
```
