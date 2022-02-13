#include "Game.h"

Game::Game() :
    level(0),
    isVictory(0),
    State(MainMenu),
    playerScores({0}),
    isPaused(0),
    winscreen(NULL),
    gameover(NULL),
    texture(NULL),
    menu(NULL),
    font(NULL),
    pauseMessage(NULL),
    textColor({255, 255, 255})
{
    rect_menu.x = 300;
    rect_menu.y = 0;
    rect_menu.h = MENU_SCREEN_HEIGHT;
    rect_menu.w = MENU_SCREEN_WIDTH;

    font = TTF_OpenFont("VT323-Regular.ttf", 28);

    levelPath = "level0.txt";

    menu = handler.loadImage("menu_full.bmp");

    handler.applySurface(0, 0, menu, handler.screen, &rect_menu);
}


void Game::Render()
{
    // ���� ���� �� � �����
    if (!isPaused)
    {
        // ��������� ���� �����, ����� �� ���������� ������ �� ������������
        SDL_FillRect(handler.screen, NULL, SDL_MapRGB(handler.screen->format, 0, 0, 0));

        // ���� ���� �� ���������
        if (isVictory == 0 && pacman.GetDead() == 0)
        {
            isVictory = 1;

            // ������ �����
            DrawMap(worldMap);

            // ��������� �������
            pacman.Update(level, worldMap);

            // ��������� ���������
            manager.Update(level, worldMap, pacman);

            // ���������� ������� ���������� �����
            PrintScore();

            // ������ ���������
            manager.Draw(GHOST_VANISH_START >= pacman.GetPowerTimer());

            // ���� ���� ���� ���� ������ � ����, ������ �� ���������
            for (const std::array<Cell, MAP_HEIGHT>& column : worldMap)
            {

                for (const Cell& cell : column)
                {
                    if (Cell::Food == cell)
                    {
                        isVictory = 0;

                        break;
                    }
                }

                if (isVictory == 0)
                {
                    break;
                }
            }
        }

        // ������ �������
        pacman.Draw(isVictory);

        // �������� ������� ��� ������/���������
        if (pacman.GetAnimationOver() == 1)
        {
            // �������� ���� �� ����� ���� � ������
            AddScore(pacman.GetTotalScore());
            // ��������� ������ � ����
            SaveScoreFile();

            // ���� ���� ����������� �������
            if (isVictory == 1)
            {
                // �������� ��������� ���� �� Victory
                State = Victory;
                // ���������� �������� ��������
                handler.applySurface(0, 0, winscreen, handler.screen, NULL);
            }
            // ���� ���� ����������� ����������
            else
            {
                // �������� ��������� ���� �� GameOver
                State = GameOver;
                // ���������� �������� ���������
                handler.applySurface(0, 0, gameover, handler.screen, NULL);
            }
        }
    }
}

void Game::AddScore(unsigned short score)
{
    playerScores[10] = score;

    // ���������� �������
    for (unsigned char i = 1; i < 11; i++)
    {
        for (unsigned char j = 10; j >= i; j--)
        {
            if (playerScores[j - 1] < playerScores[j])
            {
                score = playerScores[j - 1];
                playerScores[j - 1] = playerScores[j];
                playerScores[j] = score;
            }
        }
    }
}

void Game::SaveScoreFile()
{
    // ��������� ���� � ���������
    std::ofstream file;
    file.open("bestscore.txt");
    if (!file.is_open())
    {
        std::cout << "SaveScoreFile error";
    }
    else
    {
        for (unsigned char i = 0; i < 10; i++)
        {
            file << playerScores[i] << std::endl;
        }
    }
    file.close();
}

void Game::OpenScoreFile()
{
    // ��������� ���� � ���������
    std::ifstream file;
    file.open("bestscore.txt");
    if (!file.is_open())
    {
        std::cout << "OpenScoreFile error";
    }
    else
    {
        for (unsigned char i = 0; i < 10; i++)
        {
            file >> playerScores[i];
        }
    }
    file.close();
}

void Game::PauseGame()
{
    // ���� ��������� � ������� ���������
    if(State == InGame)
    {
        // ����������/������ �����
        isPaused = 1 - isPaused;
        if (isPaused == 1)
        {
            // ������������ � ����� ������� ���� ������� Pause
            SDL_Rect boundBox;
            boundBox.x = 0;
            boundBox.y = 0;
            boundBox.h = 32;
            boundBox.w = 60;

            SDL_FillRect(handler.screen, &boundBox, SDL_MapRGB(handler.screen->format, 0, 0, 0));

            pauseMessage = TTF_RenderText_Solid(font, "PAUSE", textColor);

            handler.applySurface(3, 2, pauseMessage, handler.screen, NULL);
        }
        else
        {
            SDL_FreeSurface(pauseMessage);
        }
    }
}

void Game::PrintScore()
{
    // �������������� �� unsigned short � stringstream
    std::stringstream ss;
    ss << "Total Score: " << pacman.GetTotalScore();
    // �������������� �� stringstream � string
    std::string str_score = ss.str();
    // �������������� �� string � const char*
    const char *score = str_score.c_str();

    // ������� ������� � ������� �����������
    scoreMessage = TTF_RenderText_Solid(font, score, textColor);

    handler.applySurface(10, 340, scoreMessage, handler.screen, NULL);
}

// �������� ����
void Game::LoadGame()
{
    isVictory = 0;

    isPaused = 0;

    // ��������� ����
    State = InGame;

    SDL_FreeSurface(menu);

    // �������� ���� ������
    std::ifstream file(levelPath);

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        file >> mapSketch[i];
    }

    OpenScoreFile();

    gameover = handler.loadImage("gameover.jpg");

    winscreen = handler.loadImage("victory.jpg");

    texture = handler.loadImage("map.png");

    worldMap = PopulateBlocks(mapSketch);

    handler.ChangeVideoMode(1);

    manager.Reset(level, ghostPosition);

    pacman.Reset();

}

void Game::SelectLevel()
{
    if (level == 0)
    {
        levelPath = "level0.txt";

        State = MainMenu;
        LoadMenu();
    }
    else
    {
        levelPath = "level1.txt";

        State = MainMenu;
        LoadMenu();
    }
}

void Game::LoadMenu()
{
    rect_menu.x = 300;
    rect_menu.y = 0;
    rect_menu.h = MENU_SCREEN_HEIGHT;
    rect_menu.w = MENU_SCREEN_WIDTH;

    SDL_FillRect(handler.screen, NULL, SDL_MapRGB(handler.screen->format, 0, 0, 0));

    menu = handler.loadImage("menu_full.bmp");

    handler.applySurface(0, 0, menu, handler.screen, &rect_menu);
}

void Game::HandleEscapeKey()
{
    switch (State)
    {
        // ����� �� �������� ������
        case InGame:
        {
            pacman.CleanUp();
            manager.CleanUp();

            handler.ChangeVideoMode(0);

            State = MainMenu;
            LoadMenu();
            break;
        }
        // ���� ��� � ������� ���� - ������ �� ������
        case MainMenu:
        {
            break;
        }
        // ���� ��������� � ����� �� ������� ���� - ��������� � �������
        default:
        {
            handler.ChangeVideoMode(0);

            State = MainMenu;
            LoadMenu();
            break;
        }
    }
}

void Game::LoadLeaderboard()
{
    SDL_Rect scoreRect;
    std::stringstream ss;
    for (unsigned char i = 0; i < 10; i++)
    {
        // �� unsigned short � const char*
        ss << playerScores[i];

        std::string str_score = ss.str();

        const char *score = str_score.c_str();

        scoreMessage = TTF_RenderText_Solid(font, score, textColor);

        // �����-�� �������������� ������.
        switch (strlen(score))
        {
            case 1: scoreRect.x = 195; break;
            case 3: scoreRect.x = 185; break;
            case 4: scoreRect.x = 180; break;
            case 5: scoreRect.x = 175; break;
        }

        scoreRect.y = 28 + i * 46;

        // ��������� ������� (���� �����, ���� ������)
        if (i % 2 == 0)
        {
            handler.applySurface(scoreRect.x, scoreRect.y, scoreMessage, handler.screen, NULL);
        }
        else
        {
            handler.applySurface(scoreRect.x - 100, scoreRect.y, scoreMessage, handler.screen, NULL);
        }

        ss.str("");
        SDL_FreeSurface(scoreMessage);
    }
}


void Game::HandleMouseButtonKey()
{
    switch (State)
    {
        // ���� ��������� � ������� ����
        case MainMenu:
        {
            // ������ ������ Play
            if (mousePos.x > 50 && mousePos.x < 250 && mousePos.y > 60 && mousePos.y < 130)
            {
                // ��������� ������� �����
                LoadGame();
                break;
            }
            // ������ ������ Level
            else if (mousePos.x > 50 && mousePos.x < 250 && mousePos.y > 162 && mousePos.y < 232)
            {
                // ������� ����� ������
                State = ChangeLevel;

                rect_menu.x = 600;

                handler.applySurface(0, 0, menu, handler.screen, &rect_menu);

                break;
            }
            // ������ ������ Score
            else if (mousePos.x > 50 && mousePos.x < 250 && mousePos.y > 263 && mousePos.y < 336)
            {
                // ������� ������� ��������
                State = Leaderboard;

                rect_menu.x = 900;

                handler.applySurface(0, 0, menu, handler.screen, &rect_menu);

                OpenScoreFile();

                LoadLeaderboard();

                break;
            }
            // ������ ������ Help
            else if (mousePos.x > 50 && mousePos.x < 250 && mousePos.y > 367 && mousePos.y < 437)
            {
                // ������� ������� ���� � ����������
                State = Help;

                rect_menu.x = 1200;

                handler.applySurface(0, 0, menu, handler.screen, &rect_menu);

                break;
            }
            // ������ ������ Exit
            else if (mousePos.x > 50 && mousePos.x < 250 && mousePos.y > 470 && mousePos.y < 540)
            {
                // ����� �� ����
                State = Exit;
            }
            break;
        }
        // ���� ��������� � ���� ������ ������
        case ChangeLevel:
        {
            // ������� Canon Level
            if (mousePos.x > 50 && mousePos.x < 250 && mousePos.y > 60 && mousePos.y < 130)
            {
                // �������� �������
                level = 0;
            }
            // ������� Level
            else if (mousePos.x > 50 && mousePos.x < 250 && mousePos.y > 162 && mousePos.y < 232)
            {
                // �������� �������
                level = 1;
            }
            // ������� Back
            else if (mousePos.x > 50 && mousePos.x < 250 && mousePos.y > 470 && mousePos.y < 540)
            {
                // ��������� � ������� ����
                State = MainMenu;
                LoadMenu();
                break;
            }

            // ����� ������
            SelectLevel();

            break;
        }
        // ���� ��������� � ������� ��������
        case Leaderboard:
        {
            // ������ ������ Back
            if (mousePos.x > 37 && mousePos.x < 170 && mousePos.y > 530 && mousePos.y < 570)
            {
                // ��������� � ����
                State = MainMenu;
                LoadMenu();
                break;
            }
            // ������ ������ Clear
            else if (mousePos.x > 130 && mousePos.x < 275 && mousePos.y > 485 && mousePos.y < 527)
            {
                // �������� ������� ��������
                for (unsigned char i = 0; i < 11; i++)
                {
                    playerScores[i] = 0;
                }

                SaveScoreFile();

                SDL_FillRect(handler.screen, NULL, SDL_MapRGB(handler.screen->format, 0, 0, 0));

                handler.applySurface(0, 0, menu, handler.screen, &rect_menu);

                LoadLeaderboard();
            }
            break;
        }
        default:
        {
            // �������� ��� �����������
            pacman.CleanUp();
            manager.CleanUp();

            SDL_FreeSurface(winscreen);
            SDL_FreeSurface(gameover);

            // ��������� � ����
            handler.ChangeVideoMode(0);

            State = MainMenu;
            LoadMenu();
            break;
        }
    }
}



std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> Game::PopulateBlocks(const std::array<std::string, MAP_HEIGHT>& m_mapSketch)
{
    // �������� "������" ����� (����������� ��������)
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> outputMap {};

    for (unsigned char i = 0; i < MAP_HEIGHT; i++)
    {
        for (unsigned char j = 0; j < MAP_WIDTH; j++)
        {
            outputMap[j][i] = Cell::Empty;

            // � ����������� �� ������� ������������� ������������ ��� ������
            switch (m_mapSketch[i][j])
            {
                case '#': // �����
                    {
                        outputMap[j][i] = Cell::Wall;
                        break;
                    }
                case '=': // �����
                    {
                        outputMap[j][i] = Cell::Door;
                        break;
                    }
                case '.': // ���
                    {
                        outputMap[j][i] = Cell::Food;
                        break;
                    }
                case 'o': // ����
                    {
                        outputMap[j][i] = Cell::Buff;
                        break;
                    }
                case 'P': // ������
                    {
                        pacman.SetPosition(CELL_SIZE * j, CELL_SIZE * i);
                        break;
                    }
                case '0': // ��������
                    {
                        ghostPosition[0].x = CELL_SIZE * j;
                        ghostPosition[0].y = CELL_SIZE * i;
                        break;
                    }
                case '1':
                    {
                        ghostPosition[1].x = CELL_SIZE * j;
                        ghostPosition[1].y = CELL_SIZE * i;
                        break;
                    }
                case '2':
                    {
                        ghostPosition[2].x = CELL_SIZE * j;
                        ghostPosition[2].y = CELL_SIZE * i;
                        break;
                    }
                case '3':
                    {
                        ghostPosition[3].x = CELL_SIZE * j;
                        ghostPosition[3].y = CELL_SIZE * i;
                        break;
                    }
            }
        }
    }

    return outputMap;
}

void Game::DrawMap(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& m_map)
{
    SDL_Rect rect; // ������� ��� ������������ �����������
    SDL_Rect clip; // ����� ������������ ��������� �����������

    for (unsigned char i = 0; i < MAP_WIDTH; i++)
    {
        for (unsigned char j = 0; j < MAP_HEIGHT; j++)
        {
            clip.x = CELL_SIZE;
            clip.y = CELL_SIZE;
            clip.w = CELL_SIZE;
            clip.h = CELL_SIZE;

            rect.x = i*CELL_SIZE;
            rect.y = j*CELL_SIZE;
            switch (m_map[i][j])
            {
                case Cell::Food:
                {
                    clip.x = 0;
                    handler.applySurface(rect.x, rect.y, texture, handler.screen, &clip);
                    break;
                }
                case Cell::Door:
                {
                    clip.x = 2 * CELL_SIZE;
                    handler.applySurface(rect.x, rect.y, texture, handler.screen, &clip);
                    break;
                }
                case Cell::Buff:
                {
                    handler.applySurface(rect.x, rect.y, texture, handler.screen, &clip);
                    break;
                }
                case Cell::Wall:
                {
                    // ������ �������� ��� ����
                    bool down = 0;
                    bool left = 0;
                    bool right = 0;
                    bool up = 0;

                    if (j < MAP_HEIGHT - 1)
                    {
                        if (Cell::Wall == m_map[i][j + 1])
                        {
                            down = 1;
                        }
                    }

                    if (i > 0)
                    {
                        if (Cell::Wall == m_map[i - 1][j])
                        {
                            left = 1;
                        }
                    }
                    else
                    {
                        left = 1;
                    }

                    if (i < MAP_WIDTH - 1)
                    {
                        if (Cell::Wall == m_map[i + 1][j])
                        {
                            right = 1;
                        }
                    }
                    else
                    {
                        right = 1;
                    }

                    if (j > 0)
                    {
                        if (Cell::Wall == m_map[i][j - 1])
                        {
                            up = 1;
                        }
                    }

                    clip.x = CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up)));
                    clip.y = 0;

                    handler.applySurface(rect.x, rect.y, texture, handler.screen, &clip);
                    break;
                }
            }
        }
    }
}

void Game::HandleMoveUpKey()
{
    if (State == InGame)
    {
        pacman.MoveUp();
    }
}

void Game::HandleMoveDownKey()
{
    if (State == InGame)
    {
        pacman.MoveDown();
    }

}

void Game::HandleMoveLeftKey()
{
    if (State == InGame)
    {
        pacman.MoveLeft();
    }
}

void Game::HandleMoveRightKey()
{
    if (State == InGame)
    {
        pacman.MoveRight();
    };
}

void Game::SetMousePosition(short i_x, short i_y)
{
    mousePos = {i_x, i_y};
}
