#ifndef GAME_H
#define GAME_H

#include <array>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <SDL_ttf.h>
#include "Collision.h"
#include "GhostManager.h"
#include "Global.h"
#include "SDL_Handler.h"
#include "Hero.h"
#include "Enemy.h"


class Game
{
public:

    Game();

    // �������� ���� ����
    void LoadMenu();

    // ������ �������� ������
    void Render();

    // ����� ����
    void PauseGame();

    // ��������� ����������� �����c� ��� �������� ������ ��� ������� �� ������ Play � ����
    void LoadGame();

    // ����� �������� ������ � ����
    void SelectLevel();

    // ��������� �����
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> PopulateBlocks(const std::array<std::string, MAP_HEIGHT>& m_mapSketch);

    // ��������� ������� �����
    void DrawMap(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& m_map);

    // �������� ������� ������� � ����
    void LoadLeaderboard();

    // ������������ ���������
    void HandleMoveUpKey();
    void HandleMoveDownKey();
    void HandleMoveLeftKey();
    void HandleMoveRightKey();

    // ��������� ������ ����� ����
    void HandleMouseButtonKey();

    // ����� � ������� ����
    void HandleEscapeKey();

    // ����������� ���������� ����� �� ����� ����
    void PrintScore();

    // ���������  � ��������� ���� �� ��������� �������� ������
    void AddScore(unsigned short score);

    // ��������� ������� ���� � ����
    void SaveScoreFile();

    // ��������� ���� � �������� ������
    void OpenScoreFile();

    // ������������� ������� ����
    void SetMousePosition(short i_x, short i_y);

    GameState State;

private:

    SDL_Handler handler;

    // ����������� ��� ����������� ����
    SDL_Surface *menu;
    // ����������� ��� ����������� ������� �����
    SDL_Surface *texture;
    // ����������� ��� ����������� ������ ���������
    SDL_Surface *gameover;
    // ����������� ��� ����������� ������ ������
    SDL_Surface *winscreen;

    // ����������� ��� ����������� ��������� � ����� ����
    SDL_Surface *pauseMessage;
    // ����������� ��� ����������� ��������� � ���������� ������� �����
    SDL_Surface *scoreMessage;

    TTF_Font *font;
    SDL_Color textColor;

    SDL_Rect rect_menu;

    Vector2 mousePos;

    Hero pacman;
    GhostManager manager;

    // ���� � ������ � �������� ��������
    std::string levelPath;

    // �������� ������� �����
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> worldMap {};

    // ���������� ��� ���������� ������� ����� �������� (enum Cell)
    std::array<std::string, MAP_HEIGHT> mapSketch {};

    std::array<Vector2, 4> ghostPosition;

    std::array<unsigned short, 11> playerScores;


    bool level;
    bool isVictory;
    bool isPaused;

};

#endif
