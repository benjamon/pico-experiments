#include "snek.h"

void grid::Init()
{
    Paint_NewImage(image, cellSize*gridWidth, cellSize*gridHeight, 0, WHITE);
    Paint_SelectImage(image);
    Paint_Clear(WHITE);
}

void grid::DrawBox()
{
    Paint_DrawRectangle(1, 1, PixelWidth(), PixelHeight(), BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // Paint_DrawRectangle(1, cellSize, PixelWidth(), PixelHeight()-cellSize, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // Paint_DrawRectangle(cellSize, 1, PixelWidth()-cellSize, PixelHeight(), BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // for (int i = 1; i < gridWidth; i++)
    // {
    //     int x = cellSize * i;
    //     Paint_DrawLine(x, 1, x, Height(), BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // }
    // for (int i = 1; i < gridHeight; i++)
    // {
    //     int y = cellSize * i;
    //     Paint_DrawLine(1, y, Width(), y, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // }
}
 
void grid::DrawCell(int cx, int cy, int fill)
{
    int left = 2 + cx * cellSize;
    int top = 2 + cy * cellSize;
    Paint_DrawRectangle(left, top, left+cellSize-4, top+cellSize-4, fill, DOT_PIXEL_1X1, DRAW_FILL_FULL);
}

void grid::SetCell(int id, int fill)
{
    cells[id] = fill;
    DrawCell(id % gridWidth, id / gridWidth, fill);
}

void grid::Display()
{
    EPD_4IN2_V2_PartialDisplay(image, screenX, screenY, Right(), Bottom());
}

bool snake::Step(int x_input, int y_input)
{
    //don't chnage dir if input is zero or exactly reversed 
    if (!(x_input == 0 && y_input == 0) && x_input != -x_dir && y_input != -y_dir)
    {
        x_dir = x_input;
        y_dir = y_input;
    }
    
    int nx = x_pos + x_dir;
    int ny = y_pos + y_dir;
    
    int newPos = owner.GetCellIndex(nx, ny);
    if (newPos != -1)
    {
        int existing = owner.GetCellType(newPos);
        if (existing != WHITE)
        {
            return false;
        }

        tail.push(newPos);

        if (tail.size() > Length)
        {
            int lastId = tail.front();
            tail.pop();
            owner.SetCell(lastId, WHITE);
        }
        
        x_pos = nx;
        y_pos = ny;
        owner.SetCell(newPos, BLACK);
        return true;
    }
    else
    {
        return false;
    }
}