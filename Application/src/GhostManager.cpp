#include "GhostManager.h"

GhostManager::GhostManager() :
    current_wave(0),
    wave_timer(SECOND_SCATTER_TIME),
    Ghosts({Enemy(0), Enemy(1), Enemy(2), Enemy(3)})
{
}


void GhostManager::Draw(bool isVanishing)
{
    // ������ ������� ��������
    for (Enemy& enemy : Ghosts)
    {
        enemy.Draw(isVanishing);
    }
}


void GhostManager::Reset(unsigned char level, const std::array<Vector2, 4> &m_GhostPosition)
{
    current_wave = 0;

    // ���������� �������� ��������� � ��������� ������������
    wave_timer = static_cast<unsigned short> (SECOND_SCATTER_TIME / pow(2, level));

    // ���������� �������������� �������
    for (unsigned char i = 0; i < 4; i++)
    {
        Ghosts[i].SetPosition(m_GhostPosition[i].x, m_GhostPosition[i].y);
    }

    // �������� ���� ��������� �� ��������������� ���������
    for (Enemy& enemy : Ghosts)
    {
        enemy.Reset(Ghosts[2].GetPosition(), Ghosts[0].GetPosition());
    }
}

void GhostManager::Update(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Hero &Pacman)
{
    // ���� ������ �� ��������� ��� �����
    if (Pacman.GetPowerTimer() == 0)
    {
        if (wave_timer == 0)
        {
            if (current_wave < 7)
            {
                current_wave++;

                // ������ ������� �������� ��� ���������
                for (Enemy& enemy : Ghosts)
                {
                    enemy.SwitchMode();
                }
            }
            // ������ ������ ���� � ����������� ������� �� �������
            if (current_wave % 2 == 1)
            {
                wave_timer = CHASE_TIME;
            }

            // ���� ��� ������ �����
            else if (current_wave == 2)
            {
                // ������� � ������ ����� ������� ����� ����������� ���������
                wave_timer = static_cast<unsigned short>(SECOND_SCATTER_TIME / pow(2, level));
            }

            else
            {
                // � ������ ������ ������� ����� ��������� ������ ����������� ���������
                wave_timer = static_cast<unsigned short>(FIRST_SCATTER_TIME / pow(2, level));
            }
        }
        else
        {
            // ��������� ������ �� ��� ���, ���� �� ����� �� ����� ����� 0
            wave_timer--;
        }
    }


    // ��������� ���� ���������
    for (Enemy& enemy: Ghosts)
    {
        enemy.Update(level, i_map, Ghosts[0], Pacman);
    }

}

void GhostManager::CleanUp()
{
    for (Enemy& enemy : Ghosts)
    {
        enemy.CleanUp();
    }
}


