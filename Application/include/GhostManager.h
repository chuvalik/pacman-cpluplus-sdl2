#ifndef GHOSTMANAGER_H
#define GHOSTMANAGER_H
#include "Enemy.h"
#include "Global.h"
#include <SDL.h>
#include "Hero.h"
#include <array>


class GhostManager // ��������������� ����� � Enemy
{
private:

    // �������� �������� �������
    unsigned char current_wave;
    // ������ ��� ������������ ������� ����� �������
    unsigned short wave_timer;

    // ������ � ����� Enemy, ���������� 4 ��������
    std::array<Enemy, 4> Ghosts;

public:

    GhostManager();

    void CleanUp();
    // ��������� ���� ���������
    void Draw(bool isVanishing);
    // ���������� ���� ���������
    void Reset(unsigned char level, const std::array<Vector2, 4>& m_GhostPosition);
    // ��������� ��������� ��������� ( ������������/������� )
    void Update(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Hero& Pacman);
};

#endif // GHOSTMANAGER_H
