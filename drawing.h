#include <stdint.h>

struct WindowSize
{
    int32_t w;
    int32_t h;
};

WindowSize initializeWindow();
void drawGame(class SnakeGame* game);
void shutdownDrawing();