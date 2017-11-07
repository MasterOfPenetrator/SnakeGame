#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "CSFMLMenu.h"
#include "CSFMLGameMain.h"

int main (void)
{
    // Set First Game Off
    GameMain.Game_Is_Init = false;

    // Init all things
    bool InitError = false;
    InitError |= !CSFMLInit();
    InitError |= !CSFMLMenuInit();

    while (!InitError && sfRenderWindow_isOpen(screen))
    {
        while (sfRenderWindow_pollEvent(screen, &EventLoop))
        {
            // Proceed Close
            if(EventLoop.type == sfEvtClosed)
                sfRenderWindow_close(screen);

            // Proceed Mouse Button
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

            // Proceed Keys
            if(EventLoop.type == sfEvtKeyReleased && GameMain.Game_Is_Init)
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

            // Proceed User Enter
            if(EventLoop.type == sfEvtTextEntered && mstate.Allow_UserEnter)
            {
                // Just ASCII
                if(EventLoop.text.unicode < 128 && mstate.Text_Field_Clicked)
                {
                    // Read Input
                    if(mstate.UserName_Counter < 19)
                    {
                        if(((char)EventLoop.text.unicode) == '\b')
                        {
                            if(mstate.UserName_Counter > 0)
                            {
                                mstate.Level_Username[mstate.UserName_Counter-1] = '\0';
                                mstate.UserName_Counter--;
                            }
                        }
                        else
                        {
                            mstate.Level_Username[mstate.UserName_Counter] = (char) EventLoop.text.unicode;
                            mstate.UserName_Counter++;
                        }

                    }
                    // Close Input
                    if(mstate.UserName_Counter == 19)
                        mstate.Level_Username[19] = '\0';
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
        if(!mstate.startgame)
        {
            MenuProcessButtons();
            MenuProcessSliders();
        }

        // Show FPS
        CSFMLShowFPS();

        // Process Game
        if(mstate.startgame)
        {
            if(!GameMain.Game_Is_Init)
            {
                CSFMLGameInit(mstate.Level_ID);
            }
            if(GameMain.Game_Is_Init)
            {
                CSFMLGameUpdate();
            }
        }

        // Display all Things!
        sfRenderWindow_display(screen);

    }

    // Clear Up things
    if(GameMain.Game_Is_Init)
        CSFMLGameQuit();
    CSFMLMenuQuit();
    CSFMLQuit();
    return 0;
}
