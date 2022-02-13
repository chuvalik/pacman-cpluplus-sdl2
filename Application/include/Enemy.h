#ifndef GHOST_H
#define GHOST_H

#include <SDL.h>
#include <array>
#include "Global.h"
#include "SDL_Handler.h"
#include "Collision.h"
#include "Hero.h"

class Enemy
{
private:
    // �������� �������/������ �� �����
    bool Mode;
    // �������� �����
    bool openDoor;

    // ������� �����������
    unsigned char MoveDir;
    // ������������� ��������� (0, 1, 2, 3);
    unsigned char GhostID;
    // ��������� ��������� (������� �� ������� ����� �������)
    unsigned char FearMode;
    // ������������ ��������� (������� �� ������� ����� �������)
    unsigned char FearModeTimer;
    // ������ ��� ����������� ��������
    unsigned char animationTimer;

    // ���� ���������, ����� ��� ���� ������ �������� ��� ������
    Vector2 Exit;
    // ���������� ������ �� �����
    Vector2 ExitHome;
    // ������� ������� ��������
    Vector2 Ghost;
    // ������� ���� ��������
    Vector2 Target;

    SDL_Handler handler;

    // ����������� � ������������� ���������
    SDL_Surface *fear_ghost;
    SDL_Surface *yellow_ghost;
    SDL_Surface *red_ghost;
    SDL_Surface *blue_ghost;
    SDL_Surface *pink_ghost;

public:
    // ����������� � ���������� �������������� ��������
    Enemy(unsigned char ghost_id);

    // ��������� ������� ��������
    void SetPosition(short m_x, short m_y);

    // �������� �� �������� � ��������
    bool PacmanCollision(const Vector2& Pacman);

    // ���������� ���� ���������
    void UpdateTarget(unsigned char MoveDir, const Vector2& m_Ghost, const Vector2& Pacman);

    // ���������� ������� ���������
    void Update(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Enemy& m_Ghost, Hero& Pacman);

    // �������� ������� ����������� ���������
    float GetDirection(unsigned char MoveDir);

    // �������� ��������
    void Reset(const Vector2 &m_Exit, const Vector2 &m_HomeExit);

    // �������� ��������� �������� (�������� ������/������ �� �����)
    void SwitchMode();

    // ��������� ��������
    void Draw(bool isVanishing);

    // �������� ������� ��������
    Vector2 GetPosition();

    // ������� ��� ����������� � ����������
    void CleanUp();
};

#endif // GHOST_H
