#ifndef HERO_H
#define HERO_H
#include "Global.h"
#include <SDL.h>
#include "SDL_Handler.h"
#include "Collision.h"
#include <iostream>
#include <array>
#include <list>

class Hero
{
private:

    // ����� ����
    bool isDead;

    // ����� ��������
    bool animationOver;

    // ����������� ���������
    unsigned char moveDir;

    // ������� ����� ��������
    unsigned short animationTimer;

    // ������������ ����� ���������
    unsigned short powerTimer;

    // ���������� �����
    unsigned short totalScore;

    // ������, ���������� ��������� ����������� ���������
    std::array<bool, 4> walls {};
    // ������, ���������� ����������� ���������
    std::list<unsigned char> directions;

    // ������� ���������
    Vector2 mainHero;

    SDL_Handler handler;

    // ���������� ��� ����������� ��������� �� ������� �����������
    SDL_Surface *texture;
    SDL_Surface *death_texture;
    SDL_Rect clip;

public:

    Hero();

    // ���������� ��������� ���������
    void Update(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);

    // ����������� �������� ���������
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    // ��������� ���������
    void Draw(bool isVictory);

    // ���������� ���������
    void Reset();

    // ��������� ��������� ���������
    void SetDead(bool m_isDead);
    // ��������� ������� ���������
    void SetPosition(short x, short y);
    // ��������� ������� ��������
    void SetAnimationTimer(unsigned short m_animationTimer);

    // ���������� ������ �� ����� �����
    unsigned short GetPowerTimer();
    // �������� ������� ���������� �����
    unsigned short GetTotalScore();
    // ���������� ����������� ���������
    unsigned short GetDirection();
    // �������� ���������� ���������
    Vector2 GetPosition();
    // �������� ��������� ��������
    bool GetAnimationOver();
    // �������� ��������� ���������
    bool GetDead();

    // �������� ���� ������� (������������) ���������
    void CleanUp();


};

#endif
