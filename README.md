 # Sudoku Solver

Optimized logic solver for 9x9 sudoku puzzles.

## Overview

This project implements a bunch of sudoku techniques in order to estimate the puzzle's difficulty level in a very accurate way.  
The solver may provide detailed logs of the used techniques, containing the found pattern and applied values, what can be used for analysis purposes as well as providing useful hints to the user.  
All implemented algorithms are focused in performance.

### Implemented techniques so far
* Naked (Single, Pair, Triple, Quadruple)
* Hidden (Single, Pair, Triple, Quadruple)
* Locked Candidates (Pointing, Claiming)
* Basic Fish (X-Wings, Swordfish, Jellyfish)
* Wings (XY-Wing, W-Wing)
* Skyscraper
* Guessing
* Brute Force

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
app/sudoku_app 8.......5.16...79..9.4.1.3...25.96......3......18.79...4.7.8.1..68...37.9.......8
```

Load puzzle from file and solve it:
```
app/sudoku_app --file puzzle.txt
```

Type `--help` or `-h` to see more options and details.
