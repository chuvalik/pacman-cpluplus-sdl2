#include "Enemy.h"

Enemy::Enemy(unsigned char ghost_id) :
    GhostID(ghost_id)
{
}

void Enemy::Draw(bool isVanishing)
{
    // ������� ���� ��������
    unsigned char bodyFrame = static_cast<unsigned char>(floor(animationTimer / static_cast<float>(GHOST_ANIMATION_SPEED)));

    // � ��������� �������� ������������� �������� ������ � ����
    SDL_Rect face;
    SDL_Rect body;

    // ��������� ����� �������� � ����������� �� �������� �����
    body.x = CELL_SIZE * bodyFrame;
    body.y = 0;
    body.h = CELL_SIZE;
    body.w = CELL_SIZE;

    // ���� ������� � ������� ���������
    if (FearMode == 0)
    {

        face.x = CELL_SIZE * MoveDir;
        face.y = CELL_SIZE;
        face.h = CELL_SIZE;
        face.w = CELL_SIZE;

        // ��������� ���� ���������
        switch (GhostID)
        {
        case 0:
            {
                handler.applySurface(Ghost.x, Ghost.y, yellow_ghost, handler.screen, &body);
                handler.applySurface(Ghost.x, Ghost.y, yellow_ghost, handler.screen, &face);
                break;
            }
        case 1:
            {
                handler.applySurface(Ghost.x, Ghost.y, red_ghost, handler.screen, &body);
                handler.applySurface(Ghost.x, Ghost.y, red_ghost, handler.screen, &face);
                break;
            }
        case 2:
            {
                handler.applySurface(Ghost.x, Ghost.y, blue_ghost, handler.screen, &body);
                handler.applySurface(Ghost.x, Ghost.y, blue_ghost, handler.screen, &face);
                break;
            }
        case 3:
            {
                handler.applySurface(Ghost.x, Ghost.y, pink_ghost, handler.screen, &body);
                handler.applySurface(Ghost.x, Ghost.y, pink_ghost, handler.screen, &face);
                break;
            }
        }
    }

    // ���� ������ �������� ����
    else if (FearMode == 1)
    {
        face.x = CELL_SIZE * 4;
        face.y = CELL_SIZE;
        face.h = CELL_SIZE;
        face.w = CELL_SIZE;


        // ����� ����� ������ ����� ����� �������� ������ �������
        if (isVanishing == 1 && bodyFrame % 4)
        {
            handler.applySurface(Ghost.x, Ghost.y, NULL, handler.screen, &body);

        }
        // � ���� ��������� ��� ������ ������� ���������� ��������
        else
        {
            handler.applySurface(Ghost.x, Ghost.y, fear_ghost, handler.screen, &body);
            handler.applySurface(Ghost.x, Ghost.y, fear_ghost, handler.screen, &face);
        }
    }

    // ���� ������ ����� �������� ��� ������
    else
    {
        // ���������� ������ ����
        face.x = CELL_SIZE * MoveDir;
        face.y = 2 * CELL_SIZE;
        face.h = CELL_SIZE;
        face.w = CELL_SIZE;
        handler.applySurface(Ghost.x, Ghost.y, yellow_ghost, handler.screen, &face);
    }

    animationTimer = (1 + animationTimer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}

void Enemy::Reset(const Vector2& m_Exit, const Vector2& m_ExitHome)
{
    // �������� ������� ���������
    fear_ghost = handler.loadImage("ghost_fear.png");
    yellow_ghost = handler.loadImage("ghost1.png");
    red_ghost = handler.loadImage("ghost2.png");
    blue_ghost = handler.loadImage("ghost3.png");
    pink_ghost = handler.loadImage("ghost4.png");


    // ���������� ����������
    Mode = 0;
    MoveDir = 0;
    FearMode = 0;
    animationTimer = 0;
    FearModeTimer = 0;
    openDoor = 0 < GhostID; // ���, ����� ������� ��������, ���������� �������

    Exit = m_Exit;
    ExitHome = m_ExitHome;
    Target = m_ExitHome;
}

// ������� ��������� ��������
void Enemy::SwitchMode()
{
    Mode = 1 - Mode;
}

// �������� �� �������� � ��������
bool Enemy::PacmanCollision(const Vector2& m_Pacman)
{
    if (Ghost.x > m_Pacman.x - CELL_SIZE && Ghost.x < CELL_SIZE + m_Pacman.x)
    {
        if (Ghost.y > m_Pacman.y - CELL_SIZE && Ghost.y < CELL_SIZE + m_Pacman.y)
        {
            return 1;
        }
    }

    return 0;
}

// ��������� ����� �������
void Enemy::SetPosition(short m_x, short m_y)
{
    Ghost.x = m_x;
    Ghost.y = m_y;
}

// ���������� ���� ��������
void Enemy::UpdateTarget(unsigned char MoveDir, const Vector2& m_Ghost, const Vector2& m_Pacman)
{
    // ���� ������� ����� ������������ �����
    if (openDoor == 1)
    {
        if (Ghost.x == Target.x && Ghost.y == Target.y)
        {
            // ���� ������ ������ ������
            if (Target.x == ExitHome.x && Target.y == ExitHome.y)
            {
                // �� ������ �� ����� ������������ �����
                openDoor = 0;
            }
            // ���� ������ ������ ����
            else if (Exit.x == Target.x && Exit.y == Target.y)
            {
                FearMode = 0; // �������� ��������� �������� �� �������
                Target.x = ExitHome.x; // �������� ���� �������� ����� �� �����
                Target.y = ExitHome.y;
            }
        }
    }
    else
    {
        // ���� ������� � ��������� ������������
        if (Mode == 0)
        {
            // ������ ������� �������� ����������� � ���� ���� �����
            switch (GhostID)
            {
                case 0: Target = {CELL_SIZE * (MAP_WIDTH -1), 0}; break;
                case 1: Target = {0, 0}; break;
                case 2: Target = {CELL_SIZE * (MAP_WIDTH -1 ), CELL_SIZE * (MAP_HEIGHT - 1)}; break;
                case 3: Target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
            }
        }

        // ���� ������� � ��������� ������ �� ��������
        else
        {
            // ������ ������� ��������� �������������
            switch (GhostID)
            {
                case 0: Target = m_Pacman; break; // ������� 0 ���������� �������
                case 1: // ������� 1 ���������� 4 ������ ����� ��������
                {
                    Target.x = m_Pacman.x;
                    Target.y = m_Pacman.y;
                    switch (MoveDir)
                    {
                        case 0: Target.x += CELL_SIZE * GHOST_1_CHASE; break;
                        case 1: Target.y -= CELL_SIZE * GHOST_1_CHASE; break;
                        case 2: Target.x -= CELL_SIZE * GHOST_1_CHASE; break;
                        case 3: Target.y += CELL_SIZE * GHOST_1_CHASE;
                    }
                    break;
                }
                case 2: // ������� 2 ���������� ������ ������ ����� ��������
                {
                    Target.x = m_Pacman.x;
                    Target.y = m_Pacman.y;
                    switch (MoveDir)
                    {
                        case 0: Target.x += CELL_SIZE * GHOST_2_CHASE; break;
                        case 1: Target.y -= CELL_SIZE * GHOST_2_CHASE; break;
                        case 2: Target.x -= CELL_SIZE * GHOST_2_CHASE; break;
                        case 3: Target.y += CELL_SIZE * GHOST_2_CHASE;
                    }

                    Target.x += Target.x - m_Ghost.x;
                    Target.y += Target.y - m_Ghost.y;
                    break;
                }
                case 3: // ������� 3 ���������� ������� �� ��� ���, ���� �� ����� ������ � ����.
                {
                    if (CELL_SIZE * GHOST_3_CHASE <= sqrt(pow(Ghost.x - m_Pacman.x, 2) + pow(Ghost.y - m_Pacman.y, 2)))
                    {
                        Target.x = m_Pacman.x;
                        Target.y = m_Pacman.y;
                    }
                    else
                    {
                        Target.x = 0;
                        Target.y = CELL_SIZE * (MAP_HEIGHT - 1);
                    }

                }
            }
        }
    }
}

void Enemy::Update(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Enemy& m_Ghost, Hero& Pacman)
{
    bool canMove = 0;

    // ���� ��������� ����� ������ 1, �� ��� ��������, ��� ������� ������ �����������
    // �������� �� ����� ��������� �����
    unsigned char availableWays = 0;
    unsigned char speed = GHOST_SPEED;

    std::array<bool, 4> walls {};

    // ���� ������ �������� ����, �� �������� ����� ��������� ��������
    if (FearMode == 0 && Pacman.GetPowerTimer() == BUFF_TIME / pow(2, level))
    {
        FearModeTimer = FEAR_SPEED;
        FearMode = 1;
    }

    // ���� � ������� ���������� ����, �� ����� ��������� ��������
    else if (Pacman.GetPowerTimer() == 0 && FearMode == 1)
    {
        FearMode = 0;
    }

    // ���� ������ ����� �������� ��� ������, �� ����� �������� ����� ��������
    if (FearMode == 2 && Ghost.x % FEAR_SPEED == 0 && Ghost.y % FEAR_SPEED == 0)
    {
        speed = FEAR_SPEED;
    }

    // �������� ���� ��������
    UpdateTarget(Pacman.GetDirection(), m_Ghost.GetPosition(), Pacman.GetPosition());

    // ��������� �� �������� ��� ������ �������
	walls[0] = checkCollision(0, 0, openDoor, speed + Ghost.x, Ghost.y, i_map);
	walls[1] = checkCollision(0, 0, openDoor, Ghost.x, Ghost.y - speed, i_map);
	walls[2] = checkCollision(0, 0, openDoor, Ghost.x - speed, Ghost.y, i_map);
	walls[3] = checkCollision(0, 0, openDoor, Ghost.x, speed + Ghost.y, i_map);

	// ���� ������� ��������� �� � ��������� ������������ (������ �������� ����, �� �� ������� ��������)
	if (FearMode != 1)
    {
        unsigned char OptimMoveDir = 4;

        // ��������� ��������� ��������
        canMove = 1;


        // � ����� ���������� ����� �������� ����������� ��� ��������
        for (unsigned char i = 0; i < 4; i++)
        {
            // ������� �� ����� ������������ �����
            if (i == (2 + MoveDir) % 4)
            {
                continue;
            }

            // �������� �� �������� � ����������� �� �����������
            else if (walls[i] == 0)
            {
                if (OptimMoveDir == 4)
                {
                    OptimMoveDir = i;
                }

                availableWays++;

                if (GetDirection(i) < GetDirection(OptimMoveDir))
                {
                    // ���������� ����� �������� �����������
                    OptimMoveDir = i;
                }
            }
        }

        // ���� ������� ������ �����������
        if (availableWays > 1)
        {
            // �� ������� ����� �������� �����������
            MoveDir = OptimMoveDir;
        }

        else
        {
            if (OptimMoveDir == 4)
            {
                // ������� ������ ����� � ������ ���������� ������� ������
                MoveDir = (2 + MoveDir) % 4;
            }
            else
            {
                MoveDir = OptimMoveDir;
            }
        }
    }

    // ��� ��������� ������������ �������� ��������� � ��������� �����������
    else
    {
        // ������ ��������� �����������
        unsigned char RandomDir = rand() % 4;

        if (FearModeTimer == 0)
        {
            canMove = 1;
            FearModeTimer = FEAR_SPEED;

            for (unsigned char i = 0; i < 4; i++)
            {
                if (i == (2 + MoveDir) % 4)
                {
                    // ��������� ����� ������
                    continue;
                }
                // ���� �� �������� ������� � ����� ��� �������� �� ������
                else if (walls[i] == 0)
                {
                    // �������� ��������� ����
                    availableWays++;
                }
            }

            if (availableWays > 0)
            {
                // �� ��� ��� ���� ��������� ����������� ������ ���� ����� ��� ���� � ����������� ���� �����
                while (walls[RandomDir] == 1 || RandomDir == (2 + MoveDir) % 4)
                {
                    // �������� ����� ��������� �����������
                    RandomDir = rand() % 4;
                }

                MoveDir = RandomDir;
            }

            else
            {
                // ���� ��� ����, ���������� �������� �����
                MoveDir = (2 + MoveDir) % 4;
            }
        }

        else
        {
            FearModeTimer--;
        }
    }

    // ������� ��������
    if (canMove == 1)
    {
        switch (MoveDir)
        {
            case 0: Ghost.x += speed; break;
            case 1: Ghost.y -= speed; break;
            case 2: Ghost.x -= speed; break;
            case 3: Ghost.y += speed;
        }

        // ����� ������� �������� �����, ������ ��� ���������� ������ �������
        if (-CELL_SIZE >= Ghost.x)
        {
            Ghost.x = CELL_SIZE * MAP_WIDTH - speed;
        }
        else if (Ghost.x >= CELL_SIZE * MAP_WIDTH)
        {
            Ghost.x = speed - CELL_SIZE;
        }
    }

    // ���� ������� ����� �������
    if (PacmanCollision(Pacman.GetPosition()) == 1)
    {
        // ���� ������� � ���������� ��������� - ��������� ����
        if (FearMode == 0)
        {
            Pacman.SetDead(1);
        }
        // ����� ��������� �������� �����
        else
        {
            openDoor = 1;
            FearMode = 2;
            Target = Exit;
        }
    }
}


float Enemy::GetDirection(unsigned char m_MoveDir)
{
    short x = Ghost.x;
    short y = Ghost.y;

    // ����� ����������� ��������, ����� �������� ���������� �� ����
    switch (m_MoveDir)
    {
        case 0: x += GHOST_SPEED; break;
        case 1: y -= GHOST_SPEED; break;
        case 2: x -= GHOST_SPEED; break;
        case 3: y += GHOST_SPEED;
    }

    // �� ������� �������� ������� ��������� �� ����
    return static_cast<float>(sqrt(pow(x - Target.x, 2) + pow(y - Target.y, 2)));
}

Vector2 Enemy::GetPosition()
{
    return Ghost;
}

void Enemy::CleanUp()
{
    SDL_FreeSurface(fear_ghost);
    SDL_FreeSurface(yellow_ghost);
    SDL_FreeSurface(red_ghost);
    SDL_FreeSurface(blue_ghost);
    SDL_FreeSurface(pink_ghost);
}




