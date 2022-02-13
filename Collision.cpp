#include "Collision.h"

bool checkCollision(bool isFood, bool isBuff, bool useDoor, int i_x, int i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map)
{
    bool output = 0;

    // ��������� ������ �������
    float cell_x = i_x / static_cast<float>(CELL_SIZE);
    float cell_y = i_y / static_cast<float>(CELL_SIZE);


    //
    for (unsigned char i = 0; i < 4; i++)
	{
		short x = 0;
		short y = 0;

		switch (i)
		{
			case 0: // ������� ����� ������
			{
				x = static_cast<short>(floor(cell_x));
				y = static_cast<short>(floor(cell_y));
				break;
			}
			case 1: // ������� ������ ������
			{
				x = static_cast<short>(ceil(cell_x));
				y = static_cast<short>(floor(cell_y));
				break;
			}
			case 2: // ������ ����� ������
			{
				x = static_cast<short>(floor(cell_x));
				y = static_cast<short>(ceil(cell_y));

				break;
			}
			case 3: // ������ ������ ������
			{
				x = static_cast<short>(ceil(cell_x));
				y = static_cast<short>(ceil(cell_y));
			}
		}

		// ���� ������� ���������� ������ �����
        if (0 <= x && 0 <= y && MAP_HEIGHT > y && MAP_WIDTH > x)
		{
			if (isFood == 0 && isBuff == 0) // �������� �� �������� ������ �� ������
			{
				if (Cell::Wall == map[x][y])
				{
					output = 1;
				}
				else if (useDoor == 0 && Cell::Door == map[x][y])
                {
                    output = 1;
                }
			}

			else if (isFood == 0 && isBuff == 1) // �������� �� �������� ������ �� ������
            {
                if (Cell::Buff == map[x][y])
                {
                    output = 1;
                    map[x][y] = Cell::Empty;
                }
            }
            else if (isFood == 1 && isBuff == 0) // �������� �� �������� ������ �� ����
            {
                if (Cell::Food == map[x][y])
                {
                    output = 1;
                    map[x][y] = Cell::Empty;
                }
            }
		}
	}

	return output;
}
