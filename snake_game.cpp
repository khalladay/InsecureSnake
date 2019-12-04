#include "snake_game.h"
#include <unistd.h>

SnakeGame::SnakeGame(int32_t w, int32_t h, int32_t randomSeed)
{
    srand(randomSeed);
    worldSize = {w, h};
    pendingInput = NONE;
    currentMode = PLAYING;
    velocity = {1,0};
	targetPos = {rand() % (w-1), rand() % (h-1)};
	score = 0;
	snakeSegments.push_back({w/2, h/2});
}

void SnakeGame::tick()
{
    if (currentMode == PLAYING)
    {
        inputMutex.lock();
        Point newHead = {snakeSegments.front().x + velocity.x, snakeSegments.front().y + velocity.y};
        inputMutex.unlock();

        if (newHead.x == targetPos.x && newHead.y == targetPos.y)
        {
            score+=3;
            
            bool valid = false;
            targetPos = {rand() % worldSize.x, rand() % worldSize.y};
            while(!valid)
            {
                valid = true;

                //this is dumb and likely will take forever near the end of the game 
                for (auto i = snakeSegments.begin(); i != snakeSegments.end(); ++i)
                {
                    if (targetPos.x == i->x && targetPos.y == i->y)
                    {
                        targetPos = {rand() % worldSize.x, rand() % worldSize.y};
                        valid = false;
                        break;
                    }
                }

            }

            snakeSegments.push_back(snakeSegments.back());
            snakeSegments.push_back(snakeSegments.back());
            snakeSegments.push_back(snakeSegments.back());
        }

        if (newHead.x < 0 || newHead.y < 0 || newHead.x > worldSize.x || newHead.y > worldSize.y)
        {
            currentMode = GAMEOVER;
        }
        else
        {
            for (auto i = snakeSegments.begin(); i != snakeSegments.end(); ++i)
            {
                if (newHead.x == i->x && newHead.y == i->y) currentMode = GAMEOVER;
            }
        }

        snakeSegments.pop_back();
        snakeSegments.push_front(newHead);
        usleep(velocity.y == 0 ? 32000 : 42000);
    }
}

void SnakeGame::handleInput(char c)
{
    inputMutex.lock();
    switch(c)
    {
        case 'w' : if (velocity.y != 1) velocity = {0,-1}; break;
        case 'a' : if (velocity.x != 1) velocity = {-1,0}; break;
        case 's' : if (velocity.y != -1) velocity = {0,1}; break;
        case 'd' : if (velocity.x != -1) velocity = {1,0}; break;
    }
    inputMutex.unlock();

}