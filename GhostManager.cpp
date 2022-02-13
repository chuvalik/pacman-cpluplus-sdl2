#include "GhostManager.h"

GhostManager::GhostManager(SDL_Renderer* ren) :
                ghosts({Ghost(0, ren),
                Ghost(1, ren),
                Ghost(2, ren),
                Ghost(3, ren)})
{
    renderer = ren;
    currentWave = 0;
    waveTimer = SECOND_SCATTER_TIME;
}

GhostManager::~GhostManager()
{

}


void GhostManager::render(bool isVanishing)
{
    // ������ ������� ��������
    for (Ghost& ghost : ghosts)
    {
        ghost.render(isVanishing);
    }
}

void GhostManager::reset(unsigned char level, const std::array<Vector2, 4> &m_GhostPosition)
{
    currentWave = 0;

    // ���������� �������� ��������� � ��������� ������������
    waveTimer = static_cast<unsigned short> (SECOND_SCATTER_TIME / pow(2, level));

    // ���������� �������������� �������
    for (unsigned char i = 0; i < 4; i++)
    {
        ghosts[i].setPosition(m_GhostPosition[i].x, m_GhostPosition[i].y);
    }

    // �������� ���� ��������� �� ��������������� ���������
    for (Ghost& ghost : ghosts)
    {
        ghost.reset(ghosts[2].getPosition(), ghosts[0].getPosition());
    }
}

void GhostManager::update(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Pacman &pacman)
{
    // ���� ������ �� ��������� ��� �����
    if (pacman.getPowerTimer() == 0)
    {
        if (waveTimer == 0)
        {
            if (currentWave < 7)
            {
                currentWave++;

                // ������ ������� �������� ��� ���������
                for (Ghost& ghost : ghosts)
                {
                    ghost.switchMode();
                }
            }
            // ������ ������ ���� � ����������� ������� �� �������
            if (currentWave % 2 == 1)
            {
                waveTimer = CHASE_TIME;
            }

            // ���� ��� ������ �����
            else if (currentWave == 2)
            {
                // ������� � ������ ����� ������� ����� ����������� ���������
                waveTimer = static_cast<unsigned short>(SECOND_SCATTER_TIME / pow(2, level));
            }

            else
            {
                // � ������ ������ ������� ����� ��������� ������ ����������� ���������
                waveTimer = static_cast<unsigned short>(FIRST_SCATTER_TIME / pow(2, level));
            }
        }
        else
        {
            // ��������� ������ �� ��� ���, ���� �� ����� �� ����� ����� 0
            waveTimer--;
        }
    }


    // ��������� ���� ���������
    for (Ghost& ghost: ghosts)
    {
        ghost.update(level, i_map, ghosts[0], pacman);
    }

}
