#include "Ghost.h"

Ghost::Ghost(unsigned char m_ghostId, SDL_Renderer *ren)
{
    ghostId = m_ghostId;
    renderer = ren;
}

Ghost::~Ghost()
{

}


void Ghost::reset(const Vector2 &m_Home, const Vector2 &m_HomeExit)
{
    debuffGhost = SpriteLoader::loadTexture("ghost_fear.png", renderer);
    yellowGhost = SpriteLoader::loadTexture("ghost1.png", renderer);
    redGhost = SpriteLoader::loadTexture("ghost2.png", renderer);
    blueGhost = SpriteLoader::loadTexture("ghost3.png", renderer);
    pinkGhost = SpriteLoader::loadTexture("ghost4.png", renderer);

    moveState = 0;
    moveDir = 0;
    debuffState = 0;
    debuffStateTimer = 0;
    openDoor = 0 < ghostId;
    srcRect = { 0, 0, 16, 16 };

    Home = m_Home;
    HomeExit = m_HomeExit;
    Target = m_HomeExit;
}

void Ghost::switchMode()
{
    moveState = 1 - moveState;
}

void Ghost::render(bool isVanishing)
{
    SpriteLoader::applyTexture(Position.x, Position.y, renderer, texture, &srcRect);
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
    if (debuffState == 0)
    {

        face.x = CELL_SIZE * moveDir;
        face.y = CELL_SIZE;
        face.h = CELL_SIZE;
        face.w = CELL_SIZE;

        // ��������� ���� ���������
        switch (ghostId)
        {
        case 0:
            {
                SpriteLoader::applyTexture(Position.x, Position.y, renderer, yellowGhost, &body);
                SpriteLoader::applyTexture(Position.x, Position.y, renderer, yellowGhost, &face);
                break;
            }
        case 1:
            {
                SpriteLoader::applyTexture(Position.x, Position.y, renderer, redGhost, &body);
                SpriteLoader::applyTexture(Position.x, Position.y, renderer, redGhost, &face);
                break;
            }
        case 2:
            {
                SpriteLoader::applyTexture(Position.x, Position.y, renderer, blueGhost, &body);
                SpriteLoader::applyTexture(Position.x, Position.y, renderer, blueGhost, &face);
                break;
            }
        case 3:
            {
                SpriteLoader::applyTexture(Position.x, Position.y, renderer, pinkGhost, &body);
                SpriteLoader::applyTexture(Position.x, Position.y, renderer, pinkGhost, &face);
                break;
            }
        }
    }

    // ���� ������ �������� ����
    else if (debuffState == 1)
    {
        face.x = CELL_SIZE * 4;
        face.y = CELL_SIZE;
        face.h = CELL_SIZE;
        face.w = CELL_SIZE;


        // ����� ����� ������ ����� ����� �������� ������ �������
        if (isVanishing == 1 && bodyFrame % 4)
        {
            SpriteLoader::applyTexture(Position.x, Position.y, renderer, NULL, &body);

        }
        // � ���� ��������� ��� ������ ������� ���������� ��������
        else
        {
            SpriteLoader::applyTexture(Position.x, Position.y, renderer, debuffGhost, &body);
            SpriteLoader::applyTexture(Position.x, Position.y, renderer, debuffGhost, &face);
        }
    }

    // ���� ������ ����� �������� ��� ������
    else
    {
        // ���������� ������ ����
        face.x = CELL_SIZE * moveDir;
        face.y = 2 * CELL_SIZE;
        face.h = CELL_SIZE;
        face.w = CELL_SIZE;
        SpriteLoader::applyTexture(Position.x, Position.y, renderer, debuffGhost, &face);
    }

    animationTimer = (1 + animationTimer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}

bool Ghost::pacmanCollision(const Vector2& Pacman)
{
    if (Position.x > Pacman.x - CELL_SIZE && Position.x < CELL_SIZE + Pacman.x)
    {
        if (Position.y > Pacman.y - CELL_SIZE && Position.y < CELL_SIZE + Pacman.y)
        {
            return 1;
        }
    }

    return 0;
}

float Ghost::getTargetDistance(unsigned char m_moveDir)
{
    short x = Position.x;
    short y = Position.y;

    // ����� ����������� ��������, ����� �������� ���������� �� ����
    switch (m_moveDir)
    {
        case 0: x += GHOST_SPEED; break;
        case 1: y -= GHOST_SPEED; break;
        case 2: x -= GHOST_SPEED; break;
        case 3: y += GHOST_SPEED; break;
    }

    // �� ������� �������� ������� ��������� �� ����
    return static_cast<float>(sqrt(pow(x - Target.x, 2) + pow(y - Target.y, 2)));
}

void Ghost::updateTarget(unsigned char moveDir, const Vector2 &m_Ghost, const Vector2 &Pacman)
{
    // ���� ������� ����� ������������ �����
    if (openDoor == 1)
    {
        if (Position.x == Target.x && Position.y == Target.y)
        {
            // ���� ������ ������ ������
            if (Target.x == HomeExit.x && Target.y == HomeExit.y)
            {
                // �� ������ �� ����� ������������ �����
                openDoor = 0;
            }
            // ���� ������ ������ ����
            else if (Home.x == Target.x && Home.y == Target.y)
            {
                debuffState = 0; // �������� ��������� �������� �� �������
                Target.x = HomeExit.x; // �������� ���� �������� ����� �� �����
                Target.y = HomeExit.y;
            }
        }
    }
    else
    {
        // ���� ������� � ��������� ������������
        if (moveState == 0)
        {
            // ������ ������� �������� ����������� � ���� ���� �����
            switch (ghostId)
            {
                case 0: Target = {CELL_SIZE * (MAP_WIDTH -1), 0}; break;
                case 1: Target = {0, 0}; break;
                case 2: Target = {CELL_SIZE * (MAP_WIDTH -1 ), CELL_SIZE * (MAP_HEIGHT - 1)}; break;
                case 3: Target = {0, CELL_SIZE * (MAP_HEIGHT - 1)}; break;
            }
        }

        // ���� ������� � ��������� ������ �� ��������
        else
        {
            // ������ ������� ��������� �������������
            switch (ghostId)
            {
                case 0: Target.x = Pacman.x; Target.y = Pacman.y; break; // ������� 0 ���������� �������
                case 1: // ������� 1 ���������� 4 ������ ����� ��������
                {
                    Target.x = Pacman.x;
                    Target.y = Pacman.y;
                    switch (moveDir)
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
                    Target.x = Pacman.x;
                    Target.y = Pacman.y;
                    switch (moveDir)
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
                    if (CELL_SIZE * GHOST_3_CHASE <= sqrt(pow(Position.x - Pacman.x, 2) + pow(Position.y - Pacman.y, 2)))
                    {
                        Target.x = Pacman.x;
                        Target.y = Pacman.y;
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

void Ghost::update(unsigned char level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Ghost& ghost, Pacman& pacman)
{
    bool canMove = 0;

    // ���� ��������� ����� ������ 1, �� ��� ��������, ��� ������� ������ �����������
    // �������� �� ����� ��������� �����
    unsigned char availableWays = 0;
    unsigned char speed = GHOST_SPEED;

    // ���� ������ �������� ����, �� �������� ����� ��������� ��������
    if (debuffState == 0 && pacman.getPowerTimer() == POWER_TIME / pow(2, level))
    {
        debuffStateTimer = DEBUFF_SPEED;
        debuffState = 1;
    }

    // ���� � ������� ���������� ����, �� ����� ��������� ��������
    else if (pacman.getPowerTimer() == 0 && debuffState == 1)
    {
        debuffState = 0;
    }

    // ���� ������ ����� �������� ��� ������, �� ����� �������� ����� ��������
    if (debuffState == 2 && Position.x % DEBUFF_SPEED == 0 && Position.y % DEBUFF_SPEED == 0)
    {
        speed = DEBUFF_SPEED;
    }

    // �������� ���� ��������
    updateTarget(pacman.getDirection(), ghost.getPosition(), pacman.getPosition());

    // ��������� �� �������� ��� ������ �������
	walls[0] = checkCollision(0, 0, openDoor, 1 + Position.x, Position.y, i_map);
	walls[1] = checkCollision(0, 0, openDoor, Position.x, Position.y - 1, i_map);
	walls[2] = checkCollision(0, 0, openDoor, Position.x - 1, Position.y, i_map);
	walls[3] = checkCollision(0, 0, openDoor, Position.x, 1 + Position.y, i_map);

	// ���� ������� ��������� �� � ��������� ������������ (������ �������� ����, �� �� ������� ��������)
	if (debuffState != 1)
    {
        unsigned char optimalMoveDir = 4;

        // ��������� ��������� ��������
        canMove = 1;


        // � ����� ���������� ����� �������� ����������� ��� ��������
        for (unsigned char i = 0; i < 4; i++)
        {
            // ������� �� ����� ������������ �����
            if (i == (2 + moveDir) % 4)
            {
                continue;
            }

            // �������� �� �������� � ����������� �� �����������
            else if (walls[i] == 0)
            {
                if (optimalMoveDir == 4)
                {
                    optimalMoveDir = i;
                }

                availableWays++;

                if (getTargetDistance(i) < getTargetDistance(optimalMoveDir))
                {
                    // ���������� ����� �������� �����������
                    optimalMoveDir = i;
                }
            }
        }

        // ���� ������� ������ �����������
        if (availableWays > 1)
        {
            // �� ������� ����� �������� �����������
            moveDir = optimalMoveDir;
        }

        else
        {
            if (optimalMoveDir == 4)
            {
                // ������� ������ ����� � ������ ���������� ������� ������
                moveDir = (2 + moveDir) % 4;
            }
            else
            {
                moveDir = optimalMoveDir;
            }
        }
    }

    // ��� ��������� ������������ �������� ��������� � ��������� �����������
    else
    {
        // ������ ��������� �����������
        unsigned char randomDir = rand() % 4;

        if (debuffStateTimer == 0)
        {
            canMove = 1;
            debuffStateTimer = DEBUFF_SPEED;

            for (unsigned char i = 0; i < 4; i++)
            {
                if (i == (2 + moveDir) % 4)
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
                while (walls[randomDir] == 1 || randomDir == (2 + moveDir) % 4)
                {
                    // �������� ����� ��������� �����������
                    randomDir = rand() % 4;
                }

                moveDir = randomDir;
            }

            else
            {
                // ���� ��� ����, ���������� �������� �����
                moveDir = (2 + moveDir) % 4;
            }
        }

        else
        {
            debuffStateTimer--;
        }
    }

    // ������� ��������
    if (canMove == 1)
    {
        switch (moveDir)
        {
            case 0: Position.x += speed; break;
            case 1: Position.y -= speed; break;
            case 2: Position.x -= speed; break;
            case 3: Position.y += speed; break;
        }

        // ����� ������� �������� �����, ������ ��� ���������� ������ �������
        if (-CELL_SIZE >= Position.x)
        {
            Position.x = CELL_SIZE * MAP_WIDTH - speed;
        }
        else if (Position.x >= CELL_SIZE * MAP_WIDTH)
        {
            Position.x = speed - CELL_SIZE;
        }
    }

    // ���� ������� ����� �������
    if (pacmanCollision(pacman.getPosition()) == 1)
    {
        // ���� ������� � ���������� ��������� - ��������� ����
        if (debuffState == 0)
        {
            pacman.setDead(1);
        }
        // ����� ��������� �������� �����
        else
        {
            openDoor = 1;
            debuffState = 2;
            Target = Home;
        }
    }
}
