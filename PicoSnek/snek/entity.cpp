#include "entity.h"

void entity::Draw()
{
    owner.DrawCell(x, y, BLACK);
};

bool entity::TryMove(int shiftX, int shiftY)
{
    if (x + shiftX < 0 || x + shiftX >= owner.Width() ||
        y + shiftY < 0 || y + shiftY >= owner.Height())
    {
        return false;
    }

    owner.DrawCell(x, y, WHITE);
    x += shiftX;
    y += shiftY;
    Draw();
    owner.Display();
    return true;
};