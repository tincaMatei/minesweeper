#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <string>

// Map limits
const int MINESWEEPER_HEIGHT_LIMIT = 30;
const int MINESWEEPER_WIDTH_LIMIT = 50;

class Cell {
public:
  bool bomb, pressed, marked;
  Cell();
};

class Minesweeper {
private:
  // The sides of the board
  int width, height;

  // The states of the board itself
  Cell **board;

  // Message returned after the generation of the map
  std::string message;

  // Places the bombs on the table
  void placeBombs(int, int);

  // Bomb-related states
  int nrbombs, pressed;
public:
  // Constructor and destructor
  Minesweeper();
  ~Minesweeper();

  // Map generation
  // Returns true if the generation was successful
  bool generateTable(int, int, int);

  // Returns the number of bomb neighbours
  int getNeighbours(int, int);

  // Returns the sides of the table
  int getWidth();
  int getHeight();

  // Returns the cell at position (lin, col)
  Cell getCell(int, int);

  // Returns error messages
  std::string getError();

  // Press the cell at position (line, column)
  // Returns true if the cell kills you
  bool pressCell(int, int);

  // Toggles the flag on position (line, column)
  void toggleFlag(int, int);

  // Returns true if the win-condition is respected
  bool winCondition();
};

#endif