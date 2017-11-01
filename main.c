#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "CSFMLMenu.h"
#include "CSFMLGameMain.h"

int main (void)
{
    // Init all things
    bool InitError = false;
    InitError |= !CSFMLInit();
    InitError |= !CSFMLMenuInit();
    InitError |= CSFMLGameInit();

    while (!InitError && sfRenderWindow_isOpen(screen))
    {
        while (sfRenderWindow_pollEvent(screen, &EventLoop))
        {
            if(EventLoop.type == sfEvtClosed)
                sfRenderWindow_close(screen);
            if(EventLoop.type == sfEvtMouseButtonPressed)
            {
                if(EventLoop.mouseButton.button == sfMouseLeft)
                    mstate.isPress = true;
            }
            if(EventLoop.type == sfEvtMouseButtonReleased)
            {
                if(EventLoop.mouseButton.button == sfMouseLeft)
                    mstate.isPress = false;
            }
            if(EventLoop.type == sfEvtKeyReleased)
            {
                if(EventLoop.key.code == sfKeyUp)
                {
                    GameSnake.S_Actual_Direction = UP;
                }
                if(EventLoop.key.code == sfKeyDown)
                {
                    GameSnake.S_Actual_Direction = DOWN;
                }
                if(EventLoop.key.code == sfKeyRight)
                {
                    GameSnake.S_Actual_Direction = RIGHT;
                }
                if(EventLoop.key.code == sfKeyLeft)
                {
                    GameSnake.S_Actual_Direction = LEFT;
                }
            }
        }

        // Draw Menu
        MenuPlace();

        // Get Mouse Position
        mstate.mouse_pos = sfMouse_getPosition(screen);

        // Get Mouse Position normalizued
        mstate.n_mouse_pos.x = (float) mstate.mouse_pos.x / sfRenderWindow_getSize(screen).x;
        mstate.n_mouse_pos.y = (float) mstate.mouse_pos.y / sfRenderWindow_getSize(screen).y;

        // Process Menu Things
        MenuProcessButtons();
        MenuProcessSliders();

        // Show FPS
        CSFMLShowFPS();

        // Process Game
        if(mstate.startgame)
        {
            CSFMLGameUpdate();
        }

        // Display all Things!
        sfRenderWindow_display(screen);

    }

    // Clear Up things
    CSFMLGameQuit();
    CSFMLMenuQuit();
    CSFMLQuit();
    return 0;
}
