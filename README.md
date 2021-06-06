 # L2SG - Logical Sudoku Solver and Generator

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
app/l2sg_app -p 2346.............4.......5.....2.4.8..5....6274...9...86.9.5.3.3...1...6........9
```

Load puzzle from file and solve it:
```
app/l2sg_app -f puzzle.txt
```

Generate a puzzle level 2 (Hard)
```
app/l2sg_app -g 2
```

Type `--help` or `-h` to see more options and details.
