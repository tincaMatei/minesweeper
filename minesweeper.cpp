#include <cstdio>
#include <cstdlib>
#include <vector>
#include <utility>
#include <algorithm>
#include "minesweeper.h"

// TODO: Remaining bombs counter and flags counter and win function
// TODO: Make the first press safe

Cell::Cell() {
  this->bomb = this->marked = this->pressed = false;
}

Minesweeper::Minesweeper() {
  // Init table
  this->width = this->height = 0;
  this->pressed = this->nrbombs = 0;
  this->board = NULL;
}

Minesweeper::~Minesweeper() {
  // If the matrix is allocated, we deallocate it
  if (this->board != NULL) {
    for (int i = 0; i < this->width; ++i)
      delete [] this->board[i];
    delete [] this->board;
  }
}

bool Minesweeper::generateTable(int w, int h, int bombs) {
  // If the board sides are too big
  if (w > MINESWEEPER_WIDTH_LIMIT || h > MINESWEEPER_HEIGHT_LIMIT) {
    this->message = "Sides are too big";
    return false;
  }

  if (w <= 0 || h <= 0 || bombs <= 0) {
    this->message = "Are you retarded?";
    return false;
  }

  // If there are too many bombs
  if (bombs > w * h / 2) {
    this->message = "Too many bombs";
    return false;
  }

  // Delete previous board
  if (this->board != NULL) {
    for (int i = 0; i < this->height; ++i)
      delete [] this->board[i];
    delete [] this->board;
  }

  // Map generation successful
  this->message = "Board created succesfully";
  this->width = w;
  this->height = h;
  this->nrbombs = bombs;
  this->pressed = 0;

  // Init board
  this->board = new Cell*[this->height];
  for (int i = 0; i < this->height; ++i)
    this->board[i] = new Cell[this->width];

  return true;
}

// Place the bombs on the board without killing at the first press
void Minesweeper::placeBombs(int lin, int col) {
  // A vector containg all the positions on the board
  // The coordinates are 0-indexed
  std::vector<std::pair<int, int> > positions;

  // Create the position vector
  for (int l = 0; l < this->height; ++l)
    for (int c = 0; c < this->width; ++c)
      if (!(l == lin && c == col))
      positions.push_back(std::make_pair(l, c));

  // Shuffle the vector
  std::random_shuffle(positions.begin(), positions.end());

  // Take the first bombs and place them
  for (int i = 0; i < this->nrbombs; ++i)
    this->board[positions[i].first][positions[i].second].bomb = true;
}

// Return the number of neighbouring bombs
int Minesweeper::getNeighbours(int lin, int col) {
  // Direction vectors; every pair of elements is a neighbour of the cell
  int dlin[] = {1, 1, 1, 0, -1, -1, -1, 0};
  int dcol[] = {-1,0, 1, 1,  1,  0, -1,-1};

  int neighbours = 0;

  // Check how many neighbouring bombs are there
  for (int i = 0; i < 8; ++i) {
    // Compute the new coordinates
    int newl = lin + dlin[i];
    int newc = col + dcol[i];

    // Check if the cell is valid and if there's a bomb
    if (0 <= newl && newl < this->height && 0 <= newc && newc < this->width && this->board[newl][newc].bomb)
      ++neighbours;
  }

  return neighbours;
}

// Returns the sides of the table
int Minesweeper::getWidth() {
  return this->width;
}

int Minesweeper::getHeight() {
  return this->height;
}

// Press the cell
bool Minesweeper::pressCell(int lin, int col) {
  // Direction vectors; every pair of elements is a neighbour of the cell
  int dlin[] = {1, 1, 1, 0, -1, -1, -1, 0};
  int dcol[] = {-1,0, 1, 1,  1,  0, -1,-1};

  if (this->pressed == 0)
    this->placeBombs(lin, col);

  // If the button was not pressed, not marked and there's no bomb
  if (!this->board[lin][col].pressed && !this->board[lin][col].marked && !this->board[lin][col].bomb) {
    // Contorize the number of pressed cells
    this->pressed++;

    // Mark cell as pressed
    this->board[lin][col].pressed = true;

    // End the game if there's a bomb
    if (this->board[lin][col].bomb)
      return true;


    // If the cell doesn't have any neighbouring bombs, we press its neighbours
    if (this->getNeighbours(lin, col) == 0)
      for (int i = 0; i < 8; ++i) {
        // Compute the new coordinates
        int newl = lin + dlin[i];
        int newc = col + dcol[i];

        // Check if the new cell is on the board
        if (0 <= newl && newl < this->height && 0 <= newc && newc < this->width)
          this->pressCell(lin + dlin[i], col + dcol[i]);
      }

    // The button didn't kill us
    return false;

  // The button kills us
  } else if (!this->board[lin][col].marked && this->board[lin][col].bomb)
    return true;

  // In all other cases, nothing happens
  return false;
}

// Toggle flag
void Minesweeper::toggleFlag(int lin, int col) {
  // Flag the cell if it wasn't pressed nor marked
  if (!this->board[lin][col].pressed && !this->board[lin][col].marked) {
    this->board[lin][col].marked = true;
  } else if (this->board[lin][col].marked) {
    this->board[lin][col].marked = false;
  }
}

// Returns the cell
Cell Minesweeper::getCell(int lin, int col) {
  return this->board[lin][col];
}

// Get error message
std::string Minesweeper::getError() {
  return this->message;
}

// Returns true if the win condition is met
bool Minesweeper::winCondition() {
  return this->width * this->height - nrbombs == pressed;
}
