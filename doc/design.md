# Design Document for Sudoku Project
by Jiaao He, 2016011279
## Introduction
#### Purpose
Write a QT gui and a Sudoku core to generate a sudoku game.
#### Scope
Programming training
## System overview
This project generates a Sudoku chess board and then let the players play this game.
## System Architectural Design
#### Architectural Design
This project is divided into two parts. One is the Sudoku board generator, and the other one is the GUI controller.
#### Decomposition Design
###### Sudokore class
This class generates a Sudoku board with a single solution. It uses _Depth First Searching_ algorithm to find the solutions of this board, and judge if it has only one solution. It firstly generate a random board filled with numbers. Then it removes some numbers in random order, while testing if there is still only one solution after deleting this number. If not, this number will be returned. 

With a batch of tests done, this algorithm can stably generate boards with around `25` numbers left. 
###### MainWnd class
This class is in charge of the displayed modules including the board and a timer.
###### NumGrid class
This class is in charge of a single grid. For the color in a grid can be complex, this single class is generated to deal with the color and display of a single grid.
## Data Design
#### Data Description
For multiple numbers may be stored in one grid, an advanced way of storing the board must be there. I use binary bits in a `int` variable to show which numbers are in this grid. A `vector` of length `81` is used to store the whole board. Then, an undo stack is used to implement the undo function.

The difficulty of a game is simply defined according to the empty grids of the board. Users can input the targeted number of empty grids as the difficulty before clicking `reset` button.
## Human Interface Design
#### Overview
All functional slots are connected to the `MainWnd` class itself, and then it operates on the separate grids, using their `display` function to update their outlook. Button pushing actions are connected to the slot functions. 
#### Screen Images
Skipped.
