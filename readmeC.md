# Paintbots - Part C

## Complete Game Simulation with AI Agents

Part C completes the Paintbots game by implementing the full game logic and creating AI robot agents that compete against each other. This is where everything comes together into a playable game!

---

## Usage

### Building the Project

To compile all files:

```bash
make
```

To clean build artifacts:

```bash
make clean
```

### Running a Game

The `paintbots` executable requires two or three command-line arguments:

**Basic usage (default configuration):**

```bash
./paintbots RedRobotName BlueRobotName
```

**With custom configuration file:**

```bash
./paintbots RedRobotName BlueRobotName config.txt
```

**Arguments:**

1. **First argument:** Name of the red robot agent
2. **Second argument:** Name of the blue robot agent
3. **Third argument (optional):** Path to configuration file

### Example Commands

```bash
# Run LazyAgent (red) vs RandomAgent (blue) with defaults
./paintbots LazyAgent RandomAgent

# Run your custom agents
./paintbots BeatRandom AntiRandom

# Use a custom configuration
./paintbots RandomAgent LazyAgent my_config.txt

# Run a tournament matchup
./paintbots MyAwesomeBot BeatRandom tournament.config
```

### Understanding the Output

The game will display the board after each turn (both robots move). Each turn shows:

**Board Display Format (4 characters per square):**

```
Format: XYZD

X = Square color: W (white), B (blue), R (red)
Y = Obstruction: R (rock), F (fog), X (rock+fog), - (none)
Z = Robot: B (blue robot), R (red robot), - (none)
D = Direction: n (north), e (east), s (south), w (west), - (no robot)
```

**Example Output:**

```
Turn 1:
W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W---
W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W---
W--- WR-- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W--- W---
W--- W--- W--- W-Bn W--- W--- W--- W--- R--- R--- R--- W--- W--- W--- W---
W--- W--- W--- B--- W--- W--- W--- W--- R--- W--- W--- W--- W--- W--- W---
W--- W--- W--- B--- W--- WR-- W--- W--- W-Re W--- W--- W--- W--- W--- W---
...

Turn 2:
...

Turn 300:
...

GAME OVER
Winner: Blue
Final Score - Blue: 156, Red: 87
```

### Example Runs

**LazyAgent vs RandomAgent:**

```bash
./paintbots LazyAgent RandomAgent
```

- LazyAgent stays in place and fires randomly each turn
- RandomAgent moves randomly but never fires
- RandomAgent typically wins by painting more squares

**Your Custom Agent vs RandomAgent:**

```bash
./paintbots BeatRandom RandomAgent
```

- BeatRandom should win at least 8 out of 10 times
- Tests your AI's effectiveness against random movement

**Trail-Following Agent:**

```bash
./paintbots FollowerBot RandomAgent
```

- Your agent follows the opponent's paint trail
- Uses long-range scans strategically
- Fires paint blobs when appropriate

---

## Available Robot Agents

Your implementation must include these agents in the `RobotAgentRoster`:

### LazyAgent

- **Strategy:** Never moves from starting square
- **Behavior:** Fires paint blob in random direction each turn
- **Strengths:** Conserves paint blobs early, controls center if placed well
- **Weaknesses:** Paints very few squares, easy target

### RandomAgent

- **Strategy:** Moves randomly to legal squares
- **Behavior:** Never fires paint blobs
- **Strengths:** Covers lots of territory, unpredictable movement
- **Weaknesses:** No offensive capability, inefficient movement

### BeatRandom (AntiRandom class)

- **Strategy:** Designed to counter RandomAgent
- **Behavior:** Your custom implementation
- **Requirement:** Must beat RandomAgent at least 8/10 times
- **Goal:** Exploit RandomAgent's weaknesses

### FollowerBot (Your Custom Agent)

- **Strategy:** Follows opponent's paint trail
- **Behavior:**
  - Tracks and follows painted squares
  - Uses long-range scans strategically
  - Fires paint blobs at opportune moments
- **Goal:** Efficient territory control and opponent disruption

---

## Game Rules Summary

### Objective

Have more squares painted your color than your opponent after 300 turns.

### Each Turn (Simultaneous)

1. Both robots perform movement (turn left/right, move forward, or stay)
2. Both robots may fire a paint blob
3. Both robots receive scan information for next turn

### Key Mechanics

**Painting:**

- Moving out of a square paints it your color
- Hit by opponent's paint blob → paint their color for 20 moves (default)

**Resources:**

- Limited paint blobs: 30 (default)
- Limited long-range scans: 30 (default)
- Unlimited short-range (5×5) scans

**Obstacles:**

- **Rocks:** Impassable, running into them = instant loss
- **Fog:** Obscures vision in short-range scans, but can move through
- **Walls:** Board boundaries, running into them = instant loss

**Win Conditions:**

- After 300 turns: Most painted squares wins
- Collision with wall/rock: Opponent wins immediately
- Robot collision: Most painted squares at that moment wins

### Strategic Considerations

**Short-range scans (5×5):**

- Always available
- Centered on your robot
- "Up" is always your facing direction
- Obscured completely if you're in fog

**Long-range scans (15×15):**

- Limited quantity (default: 30)
- Shows entire board
- Always oriented with north up
- Use strategically for planning

**Paint blobs:**

- Limited quantity (default: 30)
- Fires in direction robot is facing
- Blocked by rocks (cannot shoot through)
- Can shoot through fog
- Makes opponent paint YOUR color temporarily

---

## Configuration File

Same format as Parts A and B:

```
# Game Configuration
HIT_DURATION = 20          # Turns opponent paints your color after hit
PAINTBLOB_LIMIT = 30       # Total paint blobs available
ROCK_LOWER_BOUND = 10      # Minimum rocks on board
ROCK_UPPER_BOUND = 20      # Maximum rocks on board
FOG_LOWER_BOUND = 5        # Minimum fog squares
FOG_UPPER_BOUND = 10       # Maximum fog squares
LONG_RANGE_LIMIT = 30      # Total long-range scans available
```

Adjust these to create different game scenarios:

- **Aggressive:** More paint blobs, longer hit duration
- **Defensive:** More rocks/fog, fewer scans
- **Open:** Fewer obstacles, more scans
- **Chaotic:** High fog, low scans

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
├── RobotAgent.h
├── RobotAgentRoster.h
├── RobotAgentRoster.cpp
├── TestAgents.h
├── TestAgents.cpp
├── GameLogic.h
├── GameLogic.cpp
├── PaintBot.cpp
├── [Your custom agents...]
├── Makefile
└── README.md
```

---

## Creating Your Own Robot Agents

### Required Interface

Your robot agent must implement the `RobotAgent` interface:

```cpp
class MyCustomBot : public RobotAgent
{
public:
    virtual string getAgentName();         // Return agent's name
    virtual string getAgentCreator();      // Return your name
    virtual void getAgentMove(...);        // Decide next move
    virtual void setRobotColor(Color c);   // Set your color
    virtual RobotAgent* clone();           // Create deep copy
};
```

### Decision Making

Your `getAgentMove()` function receives:

- **Short-range scan:** 5×5 grid around your robot
- **Long-range scan:** 15×15 full board (if you request it)
- Must set: `RobotMoveRequest` with move and fire decision

**Available moves:**

- `TurnLeft` - Rotate 90° counterclockwise
- `TurnRight` - Rotate 90° clockwise
- `MoveForward` - Move one square in facing direction
- `NoMove` - Stay in current square

**Fire decision:**

- `fire = true` - Fire paint blob this turn (uses one blob)
- `fire = false` - Don't fire

### Adding Your Agent to the Roster

In `RobotAgentRoster` constructor, add your agent:

```cpp
RobotAgentRoster::RobotAgentRoster()
{
    // Default agents
    add(new LazyAgent());
    add(new RandomAgent());
    add(new AntiRandom());
    add(new FollowerBot());

    // Add your custom agents here
    add(new MyAwesomeBot());
}
```

---

## Testing Your Agents

### Basic Testing

1. **Test against LazyAgent:**

   ```bash
   ./paintbots MyBot LazyAgent
   ```

   Should win easily - LazyAgent doesn't move

2. **Test against RandomAgent:**

   ```bash
   ./paintbots MyBot RandomAgent
   ```

   Good baseline - can you consistently win?

3. **Test AntiRandom requirement:**

./paintbots <RobotAgent1> <RandomAgent2> <congif_file>

### For BeatRandom (Anti-Random)

RandomAgent's weaknesses:

- Moves randomly (inefficient pathing)
- Never fires (can't disrupt you)
- No strategic positioning

Exploit by:

- Efficient coverage patterns
- Painting over opponent's squares
- Using obstacles to your advantage
- Strategic long-range scanning

### For FollowerBot (Trail-Following)

Key strategies:

- Detect opponent's paint color in scans
- Move toward painted squares
- Use long-range scans to track opponent
- Fire when opponent is in line of sight
- Repaint opponent's squares

### General Strategy Advice

**Resource management:**

- Don't waste all paint blobs early
- Save long-range scans for critical decisions
- Balance offense (paint blobs) with territory control

**Positioning:**

- Avoid corners and edges
- Stay near center for more options
- Watch for rocks in your path
- Use fog strategically (hide from opponent)

**Opponent interaction:**

- Hit opponent with paint → they help you paint
- Follow but don't collide
- Cut off opponent's best paths
- Force opponent into obstacles

---

## Common Issues and Solutions

### "Agent not found" error

- Check agent name spelling matches exactly
- Ensure agent is added to `RobotAgentRoster`
- Verify agent's `getAgentName()` returns correct string

### Game ends immediately (collision)

- Check movement logic doesn't go out of bounds
- Verify obstacle detection in short-range scan
- Test boundary conditions (edges of board)

### Segmentation fault during game

- Check array bounds for scans
- Verify null pointer checks
- Ensure proper memory management in `clone()`

### Agent doesn't move/fire as expected

- Debug `getAgentMove()` logic
- Print scan data to verify what agent sees
- Check `RobotMoveRequest` is set correctly

### Infinite loop or very slow execution

- Check for infinite loops in decision logic
- Optimize scan processing
- Verify game termination conditions work

---

## Tournament Preparation

If your class runs a tournament:

### Submission Requirements

- Working robot agent(s)
- Compiles without errors
- Follows RobotAgent interface exactly
- No hard-coded opponent names
- No cheating (accessing internal data)

### Tournament Format

Typically:

- Round-robin (everyone vs everyone)
- Multiple games per matchup
- Both color assignments (red and blue)
- Default or specified configuration

### Winning Strategies

- **Consistency:** Win reliably, don't just beat one agent
- **Adaptability:** Handle different board layouts
- **Resource management:** Don't run out of blobs/scans
- **Robustness:** Handle edge cases without crashing

---

## Debugging Tips

### Print Board Each Turn

The game already does this - watch for patterns:

- Is your agent moving efficiently?
- Are you painting over opponent squares?
- Are resources being used well?

### Add Debug Output

In your agent:

```cpp
// Add temporary debug output
std::cout << "MyBot: Detected obstacle ahead, turning right" << std::endl;
```

### Test Specific Scenarios

Modify `TestGameBoard.cpp` to:

- Place robots in specific positions
- Add specific obstacle patterns
- Test particular game states

### Use Small Configurations

For faster testing:

```
ROCK_LOWER_BOUND = 2
ROCK_UPPER_BOUND = 3
FOG_LOWER_BOUND = 1
FOG_UPPER_BOUND = 2
LONG_RANGE_LIMIT = 50
PAINTBLOB_LIMIT = 50
```

---

## Git Repository

### Tagging Part C Completion

```bash
git add .
git commit -m "Complete Part C implementation"
git tag partccomplete
git push origin main --tags
```

### Final Repository Structure

Should have tags:

- `partacomplete` - On part-a branch
- `partbcomplete` - On main
- `partccomplete` - On main (final)

---

## Grading Considerations

Your submission will likely be evaluated on:

**Functionality:**

- Game runs without crashes
- All required agents implemented
- Game rules correctly enforced
- Win conditions work properly

**Agent Requirements:**

- LazyAgent and RandomAgent work as specified
- AntiRandom beats RandomAgent ≥80% of time
- FollowerBot demonstrates trail-following behavior
- Custom agents use scans and paint blobs appropriately

**Code Quality:**

- Proper design pattern implementation (Singleton)
- Clean interface implementation (RobotAgent)
- Good memory management (no leaks)
- Appropriate documentation

**Testing:**

- Code compiles cleanly
- Executable runs with correct arguments
- Configuration files work properly
- Edge cases handled

---

## Submission Checklist

- [ ] All files compile without errors or warnings
- [ ] `paintbots` executable runs with 2-3 arguments
- [ ] LazyAgent implemented and working
- [ ] RandomAgent implemented and working
- [ ] AntiRandom beats RandomAgent at least 8/10 times
- [ ] FollowerBot demonstrates trail-following
- [ ] Game enforces all rules correctly
- [ ] Winner determination works
- [ ] Configuration files parse correctly
- [ ] Board displays properly each turn
- [ ] Code is documented
- [ ] Makefile updated
- [ ] Git tagged as `partccomplete`
- [ ] All custom agents added to roster
- [ ] No memory leaks (test with valgrind)

---

## Beyond the Assignment

### Improve Your Agents

Ideas for stronger strategies:

- **Territory control:** Divide board into regions
- **Pathfinding:** A\* or Dijkstra for efficient routes
- **Prediction:** Anticipate opponent moves
- **Defensive play:** Block opponent's best moves
- **Adaptive strategy:** Change tactics based on opponent

### Tournament Dominance

Advanced techniques:

- Machine learning for move selection
- Minimax for adversarial planning
- Pattern recognition in opponent behavior
- Statistical analysis of successful strategies
- Ensemble methods (multiple sub-strategies)

### Extend the Game

Possible enhancements:

- More than 2 robots
- Power-ups on the board
- Dynamic obstacles that move
- Different robot types with abilities
- Replay system to review games
- GUI interface instead of text

---

## Support Resources

- Part C specification in project document
- Previous parts (A and B) for foundation
- C++ documentation for polymorphism
- Design pattern references (Strategy, Singleton)
- Algorithm resources for pathfinding
- Game AI tutorials and papers

Good luck in the tournament! 🤖🎨

## AI Usage:

```
1. ChatGPT
prompt:
    attachment - Documentation/Project2.pdf
    "assuming i have completed part C, make me a boilerplate Readme.md file for part C"
```
