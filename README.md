 # Sudoku Solver

Logical solver and generator for 9x9 sudoku puzzles.

## Overview

This project implements a bunch of sudoku techniques in order to estimate the puzzle's difficulty level in a very accurate way, and it can also generate puzzles based on the difficulty level or the used techniques.
The solver may provide detailed logs of the used techniques, containing the found pattern and applied values, what can be used for analysis purposes as well as providing useful hints to the user.  

### Solving techniques implemented so far
* Naked (Single, Pair, Triple, Quadruple)
* Hidden (Single, Pair, Triple, Quadruple)
* Locked Candidates (Pointing, Claiming)
* Basic Fish (X-Wings, Swordfish, Jellyfish)
* Wings (XY-Wing, W-Wing)
* Single Digit Patterns (Skyscraper, 2-String Kite)
* Guessing
* Brute Force

### Generator
* Generate puzzles based on the level of difficulty estimated from the solving techniques used
* Can generate puzzles that contain a pattern for a provided technique
* Can generate symmetrical and not symmetrical puzzles

## Build

__Minimum requirements__
* C++ 17
* CMake 3.10

Building the project via command line:
```
mkdir build
cd build
cmake ../
make
```

## Run
The project is supposed to be used as a library by other projects, but it includes a simple command line application for testing.

Solve a puzzle:
```
app/sudoku_app -p 8.......5.16...79..9.4.1.3...25.96......3......18.79...4.7.8.1..68...37.9.......8
```

Load puzzle from file and solve it:
```
app/sudoku_app -f puzzle.txt
```

Generate a puzzle level 2 (Hard)
```
app/sudoku_app -g 2
```

Type `--help` or `-h` to see more options and details.
