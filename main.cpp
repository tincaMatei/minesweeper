#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "minesweeper.h"
#include "rlutil.h"

// TODO: Better graphics and controls

// Display the board on the screen. The upper-left corner is at console's coordinates (10, 10) 1-indexed
void displayMap(Minesweeper *game, int lcurs, int ccurs, bool loss = false) {
  // Clear screen
  rlutil::setBackgroundColor(rlutil::BLACK);
  rlutil::cls();

  // Draw border
  rlutil::setBackgroundColor(rlutil::BLACK);
  rlutil::setColor(rlutil::WHITE);
  for(int l = -1; l < game->getHeight() + 1; ++l) {
    rlutil::locate(9, l + 10);
    printf("#");
    rlutil::locate(game->getWidth() + 10, l + 10);
    printf("#");
  }
  for(int c = -1; c < game->getWidth() + 1; ++c) {
    rlutil::locate(c + 10, 9);
    printf("#");
    rlutil::locate(c + 10, game->getHeight() + 10);
    printf("#");
  }

  rlutil::locate(1, 1);
  printf("%d %d\n", game->getWidth(), game->getHeight());

  for (int l = 0; l < game->getHeight(); ++l)
    for (int c = 0; c < game->getWidth(); ++c) {
      // Put the cursor at the appropiate position
      rlutil::locate(c + 10, l + 10);

      // Set the cursor background color
      if (l == lcurs && c == ccurs)
        rlutil::setBackgroundColor(rlutil::GREY);
      else
        rlutil::setBackgroundColor(rlutil::BLACK);

      // If the cell is flagged, draw a flag
      if (game->getCell(l, c).marked) {
        rlutil::setColor(rlutil::RED);

        // Show the bad flags if the player lost
        if (loss && !game->getCell(l, c).bomb)
          rlutil::setChar('X');
        else
          rlutil::setChar('>');
      // If the cell was pressed, show its number
      } else if (game->getCell(l, c).pressed) {
        rlutil::setColor(rlutil::WHITE);

        int neighbours = game->getNeighbours(l, c);

        // Cells with no neighbours are not shown
        if (neighbours == 0)
          rlutil::setChar(' ');
        else
          rlutil::setChar('0' + neighbours);
      } else if (!loss) {
        rlutil::setColor(rlutil::WHITE);
        rlutil::setChar('.');
      } else if (game->getCell(l, c).bomb) {
        rlutil::setColor(rlutil::RED);
        rlutil::setChar('*');
      } else {
        rlutil::setColor(rlutil::WHITE);
        rlutil::setChar('.');
      }
    }

  // Empty the buffer
  fflush(stdout);
}

void init() {
  srand(time(NULL));
}

int main() {
  // Sides of the table
  int width, height;

  // Cursor coordinates
  int lcurs, ccurs;

  // Number of bombs
  int bombs;

  // Check if the board was generated
  bool generated;

  // The fucking game board
  Minesweeper *game = new Minesweeper();

  // Game states
  bool dead;

  init();

  printf("Please enter the width, the height and the number of bombs\n");

  // Try to generate the map until successful
  do {
    // Read the game parameters
    scanf("%d%d%d", &width, &height, &bombs);

    // Try to generate the board
    generated = game->generateTable(width, height, bombs);

    printf("%s\n", game->getError().c_str());
  } while (!generated);
  printf("Press any key to continue\n");
  while(kbhit() == 0);

  // Hide the cursor
  rlutil::hidecursor();

  dead = false;

  // Set the cursor on the middle of the table
  lcurs = height / 2;
  ccurs = width / 2;

  // Display the map at the beginning
  displayMap(game, lcurs, ccurs);

  // Run the game loop
  while (!dead && !game->winCondition()) {
    // Wait for a key to be pressed
    int pressedkey = rlutil::nb_getch();

    // Process key
    switch (pressedkey) {
      // Cell press and flag
      case 'f': {
        if(game->pressCell(lcurs, ccurs))
          dead = true;
        else
          displayMap(game, lcurs, ccurs);
        break;
      }
      case 'g': {
        game->toggleFlag(lcurs, ccurs);
        displayMap(game, lcurs, ccurs);
        break;
      }

      // WASD commands
      case 'w': {
        lcurs = (lcurs - 1 + height) % height;
        displayMap(game, lcurs, ccurs);
        break;
      }
      case 's': {
        lcurs = (lcurs + 1) % height;
        displayMap(game, lcurs, ccurs);
        break;
      }
      case 'd': {
        ccurs = (ccurs + 1) % width;
        displayMap(game, lcurs, ccurs);
        break;
      }
      case 'a': {
        ccurs = (ccurs - 1 + width) % width;
        displayMap(game, lcurs, ccurs);
        break;
      }
    }
  }

  // Display the death screen
  if(dead) {
    displayMap(game, lcurs, ccurs, true);
    rlutil::locate(1, 1);
    rlutil::setColor(rlutil::RED);
    rlutil::setString("YOU KILLED US ALL");
  // Display the win screen
  } else {
    rlutil::locate(1, 1);
    rlutil::setColor(rlutil::GREEN);
    rlutil::setString("CONGRATULATIONS! YOU SAVED US ALL!");
  }
  return 0;
}
