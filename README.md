# Bitboard Checkers – CS 3503 Project 1

This project is a playable checkers game written in C that uses **64-bit bitboards** to represent the board state. Each player’s regular pieces and kings are stored in separate bitboards, which makes move calculations fast and memory-efficient.  

The game runs entirely in the terminal and lets two players take turns moving their pieces until someone wins.

---

##  How It Works
- The board is represented with 64 bits, one for each square.
- Moves and captures are handled through bitwise operations.
- Pieces are automatically promoted to kings when they reach the opposite side.
- You can save and load your game progress with simple commands.

---

##  How to Play
1. **Build the program**  
   ```bash
   make
   ```
2. **Run it**  
   ```bash
   ./checkers
   ```
3. **Controls**
   - Enter moves like `a3-b4` or `a3 b4`.
   - Type `w` to save your game (`save.chk`).
   - Type `x` to load a saved game.
   - Type `q` to quit.

---

##  Game Rules
- Player 1 uses **r/R** (red) pieces and moves upward.  
- Player 2 uses **b/B** (black) pieces and moves downward.  
- Lowercase letters (`r`, `b`) are regular pieces; uppercase (`R`, `B`) are kings.  
- Pieces move diagonally on dark squares only.  
- Jumps are supported, and kings can move both directions.  

---

##  Files Included
- `main.c`, `ui.c`, `ui.h` — User interface and input handling  
- `bitboard.c`, `bitboard.h` — Core logic and board manipulation  
- `Makefile` — Simple build script  
- `sample_output.txt` — Example of gameplay output  

---

