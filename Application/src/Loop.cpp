#include <SDL.h>
#include "Loop.h"
#include "SDL_Handler.h"
#include "Game.h"
#include "Timer.h"

void Loop::Run()
{
    bool quit = false;

    SDL_Handler handler;

    // �������� ����� � ������� ����
    Game game;

    Timer fps;

    int frame = 0;

    bool cap = true;

    Vector2 mousePosition;

    handler.Update();

    while (!quit)
    {
        // ������� ������
        fps.Start();

        while (SDL_PollEvent(&handler.event))
        {
            switch (handler.event.type)
            {
                case SDL_QUIT:
                {
                    quit = true;
                    break;
                }

                case SDL_MOUSEBUTTONDOWN:
                {
                    // ������������ ������� ���� ��� �������������� � ����
                    mousePosition = { handler.event.button.x, handler.event.button.y };

                    game.SetMousePosition(mousePosition.x, mousePosition.y);

                    game.HandleMouseButtonKey();
                    break;
                }

                case SDL_KEYDOWN:
                {
                    // ������������ ������� �� ����������
                    switch (handler.event.key.keysym.sym)
                    {
                        case SDLK_p: game.PauseGame(); break; // ����� �� ����� ����
                        case SDLK_UP: game.HandleMoveUpKey(); break;
                        case SDLK_DOWN: game.HandleMoveDownKey(); break;
                        case SDLK_LEFT: game.HandleMoveLeftKey(); break;
                        case SDLK_RIGHT: game.HandleMoveRightKey(); break;
                        case SDLK_ESCAPE: game.HandleEscapeKey(); break; // ����� � ������� ����
                        default: break;
                    }
                    break;

                }
                default: break;
            }
        }

        frame++;

        // ������ ���������� ������ � ������� ������
        if (game.State == InGame)
        {
            game.Render();
        }

        // ������������� ������ �� ���� ����� ������� �� ������ Exit � ����.
        else if (game.State == Exit)
        {
            quit = true;
        }

        // ���������� ������
        handler.Update();

        if ( (cap == true) && (fps.getTicks() < 1000 / FPS) )
        {
            SDL_Delay( (1000 / FPS) - fps.getTicks());
        }

    }

    handler.cleanUp();

}

