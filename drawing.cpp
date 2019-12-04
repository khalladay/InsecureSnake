#include <curses.h>
#include <stdint.h>
#include "drawing.h"
#include "snake_game.h"

WindowSize initializeWindow()
{
    initscr();
	cbreak();
	noecho();
	clear();
    curs_set(false);
    WindowSize outSize;    
    getmaxyx(stdscr, outSize.h, outSize.w);
    scrollok(stdscr, TRUE);
    nodelay(stdscr, true);

    return outSize;
}

void drawGame(class SnakeGame* game)
{
    if (game->currentMode == PLAYING)
    {
        start_color();			/* Start color 			*/
        init_color(COLOR_BLACK, 0, 0, 0);

        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        attron(COLOR_PAIR(1));

        //draw game board
        for (int32_t x = 0; x < game->worldSize.x; ++x)
        {
            for (int32_t y = 0; y < game->worldSize.y; ++y)
            {
                mvprintw(y, x, " "); // Print our "ball" at the current xy position
            }
        }
        
        attroff(COLOR_PAIR(1));

        //draw snake
        for (auto i = game->snakeSegments.begin(); i != game->snakeSegments.end(); ++i)
        {
            mvprintw(i->y, i->x, "O");
        }
        
        //draw target
        mvprintw(game->targetPos.y, game->targetPos.x, "X");
    }
    else
    {
        clear();
        mvprintw(game->worldSize.y/2, game->worldSize.x/2 - 10, "Game Over");

        char scoreString[64];
        sprintf(scoreString, "Score: %i", game->score);
        mvprintw(game->worldSize.y/2 + 1, game->worldSize.x/2 - 10, &scoreString[0]);

    }
    refresh();

}

void shutdownDrawing()
{
    echo();
    nocbreak();
    curs_set(true);
    endwin();
}